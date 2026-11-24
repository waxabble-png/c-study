#define NOMINMAX
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#endif

namespace {

    constexpr float kPi = 3.14159265358979323846f;
    constexpr float kTwoPi = 2.0f * kPi;
    constexpr float kTargetFps = 60.0f;
    constexpr float kPhase1Duration = 10.0f; // 第一阶段：圣诞树成形约 10 秒

    constexpr int kTreeParticleCount = 3000;   // 圣诞树金色光粒子数量
    constexpr int kOrnamentCount = 160;        // 树上彩色装饰光点数量
    constexpr int kSnowflakeCount = 220;       // 雪花数量
    constexpr int kFireworkPoolSize = 5000;    // 烟花粒子池大小
    constexpr int kMaxLaunchers = 16;          // 同时上升的烟花弹上限
    constexpr int kTrailLength = 6;            // 烟花粒子拖尾长度

    // ========================== 基础数据结构 ==========================

    struct ColorRGB {
        float r, g, b;
    };

    // 通用粒子结构体：位置、速度、颜色、生命周期、大小
    struct Particle {
        float x = 0.0f;
        float y = 0.0f;
        float vx = 0.0f;
        float vy = 0.0f;
        ColorRGB color{ 1.0f, 1.0f, 1.0f };
        float life = 0.0f;
        float maxLife = 1.0f;
        float size = 2.0f;
        bool active = false;

        // 拖尾历史坐标（用于烟花粒子拖尾）
        float trailX[kTrailLength]{};
        float trailY[kTrailLength]{};
        int trailCount = 0;
    };

    // 圣诞树粒子扩展数据（记录螺旋飞入的起点、终点和相位）
    struct TreeParticle {
        Particle base;
        float startX = 0.0f;
        float startY = 0.0f;
        float targetX = 0.0f;
        float targetY = 0.0f;
        float targetAngle = 0.0f;
        float treeRatio = 0.0f; // 0 在树顶，1 在树底
        float delay = 0.0f;     // 起飞延迟，营造错落飞入感
        float twinklePhase = 0.0f;
        float twinkleSpeed = 2.0f;
        bool isOrnament = false;
    };

    // 上升中的烟花弹
    struct FireworkRocket {
        float x = 0.0f;
        float y = 0.0f;
        float vx = 0.0f;
        float vy = 0.0f;
        float targetY = 0.0f;
        ColorRGB color{ 1.0f, 0.5f, 0.2f };
        bool active = false;
    };

    // 雪花粒子
    struct Snowflake {
        float x = 0.0f;
        float y = 0.0f;
        float fallSpeed = 0.0f;
        float swayAmp = 0.0f;
        float swayFreq = 0.0f;
        float swayPhase = 0.0f;
        float size = 2.0f;
        float alpha = 0.8f;
    };

    // 屏幕与双缓冲资源
    struct RenderContext {
        int width = 1920;
        int height = 1080;
        HDC memDC = nullptr;
        HBITMAP dibBitmap = nullptr;
        HBITMAP oldBitmap = nullptr;
        uint32_t* pixels = nullptr; // 32位 BGRA 像素缓冲
        std::vector<uint32_t> background; // 预渲染的夜空渐变背景
    };

    // 文字光晕与蒙版缓存（位于树下方区域）
    struct BlessingTextCache {
        int width = 0;
        int height = 0;
        int offsetY = 0;
        std::vector<float> coreMask; // 0.0 ~ 1.0 文字主体抗锯齿蒙版
        std::vector<float> glowMask; // 0.0 ~ 1.0 模糊后的光晕蒙版
    };

    // 全局状态
    RenderContext g_render;
    BlessingTextCache g_textCache;
    std::vector<TreeParticle> g_treeParticles;
    std::vector<Snowflake> g_snowflakes;
    std::vector<Particle> g_fireworkPool(kFireworkPoolSize);
    std::vector<FireworkRocket> g_rockets(kMaxLaunchers);

    int g_fireworkCursor = 0;
    float g_elapsedTime = 0.0f;
    float g_nextFireworkTimer = 0.4f;
    bool g_running = true;

    // ========================== 工具函数 ==========================

    inline float RandFloat(float minVal, float maxVal) {
        float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        return minVal + t * (maxVal - minVal);
    }

    inline int ClampInt(int v, int lo, int hi) {
        return (v < lo) ? lo : (v > hi ? hi : v);
    }

    inline float ClampFloat(float v, float lo, float hi) {
        return (v < lo) ? lo : (v > hi ? hi : v);
    }

    inline float EaseOutCubic(float t) {
        float u = 1.0f - ClampFloat(t, 0.0f, 1.0f);
        return 1.0f - u * u * u;
    }

    inline float EaseInOutQuad(float t) {
        t = ClampFloat(t, 0.0f, 1.0f);
        return (t < 0.5f) ? (2.0f * t * t) : (1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) * 0.5f);
    }

    // 像素加法混合（用于发光叠加）
    inline void AddPixel(int x, int y, uint8_t addR, uint8_t addG, uint8_t addB) {
        if (x < 0 || x >= g_render.width || y < 0 || y >= g_render.height) {
            return;
        }
        uint32_t& dst = g_render.pixels[y * g_render.width + x];
        uint32_t b = (dst & 0xFFu) + addB;
        uint32_t g = ((dst >> 8) & 0xFFu) + addG;
        uint32_t r = ((dst >> 16) & 0xFFu) + addR;
        if (b > 255u) b = 255u;
        if (g > 255u) g = 255u;
        if (r > 255u) r = 255u;
        dst = (r << 16) | (g << 8) | b;
    }

    // 绘制实心圆（加法混合），作为多层同心圆发光的基础图元
    void DrawAdditiveFilledCircle(float cx, float cy, float radius, const ColorRGB& color, float intensity) {
        if (radius <= 0.2f || intensity <= 0.005f) {
            return;
        }
        int rInt = static_cast<int>(std::ceil(radius));
        int ix = static_cast<int>(std::lround(cx));
        int iy = static_cast<int>(std::lround(cy));

        int minX = std::max(0, ix - rInt);
        int maxX = std::min(g_render.width - 1, ix + rInt);
        int minY = std::max(0, iy - rInt);
        int maxY = std::min(g_render.height - 1, iy + rInt);
        if (minX > maxX || minY > maxY) {
            return;
        }

        uint8_t addR = static_cast<uint8_t>(ClampInt(static_cast<int>(color.r * intensity * 255.0f), 0, 255));
        uint8_t addG = static_cast<uint8_t>(ClampInt(static_cast<int>(color.g * intensity * 255.0f), 0, 255));
        uint8_t addB = static_cast<uint8_t>(ClampInt(static_cast<int>(color.b * intensity * 255.0f), 0, 255));
        if (addR == 0 && addG == 0 && addB == 0) {
            return;
        }

        float rSq = radius * radius;
        for (int y = minY; y <= maxY; ++y) {
            float dy = static_cast<float>(y) - cy;
            float dySq = dy * dy;
            uint32_t* row = g_render.pixels + y * g_render.width;
            for (int x = minX; x <= maxX; ++x) {
                float dx = static_cast<float>(x) - cx;
                if (dx * dx + dySq <= rSq) {
                    uint32_t dst = row[x];
                    uint32_t b = (dst & 0xFFu) + addB;
                    uint32_t g = ((dst >> 8) & 0xFFu) + addG;
                    uint32_t r = ((dst >> 16) & 0xFFu) + addR;
                    if (b > 255u) b = 255u;
                    if (g > 255u) g = 255u;
                    if (r > 255u) r = 255u;
                    row[x] = (r << 16) | (g << 8) | b;
                }
            }
        }
    }

    // 发光效果：多层同心圆模拟（外层大圆暗、内层小圆亮，叠加出辉光感）
    void DrawGlowCircle(float cx, float cy, float baseRadius, const ColorRGB& color, float alpha) {
        if (alpha <= 0.01f) {
            return;
        }
        // 第1层：最外层大晕圈（暗）
        DrawAdditiveFilledCircle(cx, cy, baseRadius * 2.8f, color, alpha * 0.09f);
        // 第2层：中层光晕
        DrawAdditiveFilledCircle(cx, cy, baseRadius * 1.7f, color, alpha * 0.22f);
        // 第3层：内层高亮核心
        DrawAdditiveFilledCircle(cx, cy, baseRadius * 0.95f, color, alpha * 0.55f);
        // 第4层：中心炽亮点（微偏白）
        ColorRGB hotCore{
            ClampFloat(color.r * 0.6f + 0.4f, 0.0f, 1.0f),
            ClampFloat(color.g * 0.6f + 0.4f, 0.0f, 1.0f),
            ClampFloat(color.b * 0.6f + 0.4f, 0.0f, 1.0f)
        };
        DrawAdditiveFilledCircle(cx, cy, std::max(0.9f, baseRadius * 0.45f), hotCore, alpha * 0.85f);
    }

    // ========================== 渲染与双缓冲初始化 ==========================

    void InitRenderContext(HWND hwnd, int width, int height) {
        g_render.width = width;
        g_render.height = height;

        HDC screenDC = GetDC(hwnd);
        g_render.memDC = CreateCompatibleDC(screenDC);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // 自上而下（top-down）DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* bits = nullptr;
        g_render.dibBitmap = CreateDIBSection(screenDC, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
        g_render.pixels = static_cast<uint32_t*>(bits);
        g_render.oldBitmap = static_cast<HBITMAP>(SelectObject(g_render.memDC, g_render.dibBitmap));
        ReleaseDC(hwnd, screenDC);

        // 预生成深邃夜空渐变背景
        g_render.background.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
        for (int y = 0; y < height; ++y) {
            float t = static_cast<float>(y) / static_cast<float>(std::max(1, height - 1));
            uint8_t r = static_cast<uint8_t>(4.0f + t * 8.0f);
            uint8_t g = static_cast<uint8_t>(6.0f + t * 10.0f);
            uint8_t b = static_cast<uint8_t>(18.0f + t * 18.0f);
            uint32_t rowColor = (static_cast<uint32_t>(r) << 16) |
                (static_cast<uint32_t>(g) << 8) |
                static_cast<uint32_t>(b);
            uint32_t* dstRow = g_render.background.data() + static_cast<size_t>(y) * width;
            std::fill(dstRow, dstRow + width, rowColor);
        }
    }

    void CleanupRenderContext() {
        if (g_render.memDC) {
            if (g_render.oldBitmap) {
                SelectObject(g_render.memDC, g_render.oldBitmap);
            }
            if (g_render.dibBitmap) {
                DeleteObject(g_render.dibBitmap);
            }
            DeleteDC(g_render.memDC);
        }
        g_render.memDC = nullptr;
        g_render.dibBitmap = nullptr;
        g_render.oldBitmap = nullptr;
        g_render.pixels = nullptr;
    }

    // ========================== 粒子系统公共操作 ==========================

    // 从 5000 粒子池中回收复用一个空闲粒子
    Particle* AcquireFireworkParticle() {
        for (int i = 0; i < kFireworkPoolSize; ++i) {
            int idx = (g_fireworkCursor + i) % kFireworkPoolSize;
            if (!g_fireworkPool[idx].active) {
                g_fireworkCursor = (idx + 1) % kFireworkPoolSize;
                g_fireworkPool[idx].trailCount = 0;
                return &g_fireworkPool[idx];
            }
        }
        // 若池满则覆盖游标处最老的粒子
        Particle* p = &g_fireworkPool[g_fireworkCursor];
        g_fireworkCursor = (g_fireworkCursor + 1) % kFireworkPoolSize;
        p->trailCount = 0;
        return p;
    }

    // ========================== 圣诞树生成与更新 ==========================

    void InitChristmasTree() {
        g_treeParticles.clear();
        g_treeParticles.reserve(kTreeParticleCount + kOrnamentCount);

        float W = static_cast<float>(g_render.width);
        float H = static_cast<float>(g_render.height);

        // 树高占屏幕高度的 70%，居中显示，底部留出空间给祝福语
        float treeHeight = H * 0.70f;
        float treeTopY = H * 0.10f;
        float treeCenterX = W * 0.50f;
        float maxBaseRadius = treeHeight * 0.36f; // 锥形底部半径

        // 1. 生成约 3000 个金色树身光粒子
        for (int i = 0; i < kTreeParticleCount; ++i) {
            TreeParticle tp;
            // 使用 sqrt 分布让锥体表面密度均匀（底部更宽粒子更多）
            float u = static_cast<float>(i) / static_cast<float>(kTreeParticleCount);
            float hRatio = std::sqrt(RandFloat(0.01f, 1.0f));
            float angle = u * kTwoPi * 26.0f + RandFloat(-0.35f, 0.35f);

            // 稍微带一点层叠松枝的波浪感
            float layerWave = 1.0f + 0.08f * std::sin(hRatio * kTwoPi * 7.0f);
            float radius = maxBaseRadius * hRatio * layerWave * RandFloat(0.75f, 1.05f);

            tp.treeRatio = hRatio;
            tp.targetAngle = angle;
            tp.targetX = treeCenterX + std::cos(angle) * radius;
            tp.targetY = treeTopY + hRatio * treeHeight + std::sin(angle) * radius * 0.16f;

            // 从屏幕下方或四周飞入
            int edge = std::rand() % 3;
            if (edge == 0) {
                tp.startX = RandFloat(0.0f, W);
                tp.startY = H + RandFloat(10.0f, H * 0.15f);
            }
            else if (edge == 1) {
                tp.startX = -RandFloat(10.0f, W * 0.1f);
                tp.startY = RandFloat(H * 0.2f, H);
            }
            else {
                tp.startX = W + RandFloat(10.0f, W * 0.1f);
                tp.startY = RandFloat(H * 0.2f, H);
            }

            tp.base.x = tp.startX;
            tp.base.y = tp.startY;
            tp.base.size = H * RandFloat(0.0014f, 0.0024f);
            // 金色系变化（暖金到亮金）
            tp.base.color = {
                RandFloat(0.92f, 1.0f),
                RandFloat(0.68f, 0.86f),
                RandFloat(0.18f, 0.38f)
            };
            tp.base.active = true;
            tp.delay = RandFloat(0.0f, 2.2f);
            tp.twinklePhase = RandFloat(0.0f, kTwoPi);
            tp.twinkleSpeed = RandFloat(1.5f, 3.8f);
            tp.isOrnament = false;

            g_treeParticles.push_back(tp);
        }

        // 2. 生成红、绿、蓝、紫彩色装饰小光点
        const ColorRGB ornamentPalette[4] = {
            { 1.0f, 0.22f, 0.28f }, // 红
            { 0.25f, 0.95f, 0.42f }, // 绿
            { 0.28f, 0.62f, 1.0f },  // 蓝
            { 0.82f, 0.35f, 1.0f }   // 紫
        };

        for (int i = 0; i < kOrnamentCount; ++i) {
            TreeParticle tp;
            float hRatio = std::sqrt(RandFloat(0.06f, 0.98f));
            float angle = RandFloat(0.0f, kTwoPi);
            float radius = maxBaseRadius * hRatio * 0.96f;

            tp.treeRatio = hRatio;
            tp.targetAngle = angle;
            tp.targetX = treeCenterX + std::cos(angle) * radius;
            tp.targetY = treeTopY + hRatio * treeHeight + std::sin(angle) * radius * 0.16f;
            tp.startX = RandFloat(0.0f, W);
            tp.startY = H + RandFloat(20.0f, H * 0.15f);

            tp.base.x = tp.startX;
            tp.base.y = tp.startY;
            tp.base.size = H * RandFloat(0.0026f, 0.0038f);
            tp.base.color = ornamentPalette[i % 4];
            tp.base.active = true;
            tp.delay = RandFloat(0.5f, 2.2f);
            tp.twinklePhase = RandFloat(0.0f, kTwoPi);
            tp.twinkleSpeed = RandFloat(1.2f, 2.6f); // 缓慢闪烁
            tp.isOrnament = true;

            g_treeParticles.push_back(tp);
        }
    }

    void UpdateChristmasTree(float elapsed) {
        float W = static_cast<float>(g_render.width);
        float H = static_cast<float>(g_render.height);
        float centerX = W * 0.50f;
        float flyDuration = kPhase1Duration - 2.2f; // 留出错落起飞时间，确保第 10 秒全部就位

        for (auto& tp : g_treeParticles) {
            float localTime = elapsed - tp.delay;
            if (localTime <= 0.0f) {
                tp.base.x = tp.startX;
                tp.base.y = tp.startY;
                tp.base.life = 0.0f;
                continue;
            }

            if (localTime < flyDuration) {
                // 第一阶段：沿螺旋线飞入汇聚成圣诞树
                float progress = EaseInOutQuad(localTime / flyDuration);
                float baseX = tp.startX + (tp.targetX - tp.startX) * progress;
                float baseY = tp.startY + (tp.targetY - tp.startY) * progress;

                // 螺旋偏移：随汇聚过程逐渐收敛为 0
                float spiralRadius = (1.0f - progress) * W * 0.14f;
                float spiralAngle = tp.targetAngle + (1.0f - progress) * kTwoPi * 3.0f;
                tp.base.x = baseX + (centerX - baseX) * 0.08f * (1.0f - progress) + std::cos(spiralAngle) * spiralRadius;
                tp.base.y = baseY + std::sin(spiralAngle) * spiralRadius * 0.35f;
                tp.base.life = ClampFloat(progress * 1.2f, 0.15f, 1.0f);
            }
            else {
                // 到达目标位置后保持完整，伴随微弱呼吸与闪烁
                tp.base.x = tp.targetX;
                tp.base.y = tp.targetY;
                tp.base.life = 1.0f;
            }
            (void)H;
        }
    }

    // 绘制树顶明亮的五角星
    void DrawTreeTopStar(float elapsed) {
        float W = static_cast<float>(g_render.width);
        float H = static_cast<float>(g_render.height);
        float starX = W * 0.50f;
        float starY = H * 0.10f - H * 0.012f;

        // 星星在第一阶段后半段逐渐亮起，第二阶段保持闪烁
        float appear = ClampFloat((elapsed - kPhase1Duration * 0.45f) / (kPhase1Duration * 0.45f), 0.0f, 1.0f);
        if (appear <= 0.01f) {
            return;
        }

        float twinkle = 0.85f + 0.15f * std::sin(elapsed * 3.2f);
        float alpha = appear * twinkle;
        float starRadius = H * 0.026f;

        // 大光晕核心
        ColorRGB starGold{ 1.0f, 0.90f, 0.45f };
        DrawGlowCircle(starX, starY, starRadius * 0.72f, starGold, alpha * 0.95f);

        // 沿五角星轮廓叠加发光点，勾勒清晰星芒
        const int points = 5;
        for (int i = 0; i < points; ++i) {
            float outerAngle = -kPi * 0.5f + i * (kTwoPi / points);
            float innerAngle = outerAngle + kPi / points;
            float ox = starX + std::cos(outerAngle) * starRadius;
            float oy = starY + std::sin(outerAngle) * starRadius;
            float ix = starX + std::cos(innerAngle) * (starRadius * 0.42f);
            float iy = starY + std::sin(innerAngle) * (starRadius * 0.42f);

            // 从中心向星尖画光点射线
            for (int s = 1; s <= 5; ++s) {
                float t = static_cast<float>(s) / 5.0f;
                float px = starX + (ox - starX) * t;
                float py = starY + (oy - starY) * t;
                DrawGlowCircle(px, py, H * 0.0032f * (1.1f - 0.5f * t), starGold, alpha * 0.75f);
            }
            DrawGlowCircle(ix, iy, H * 0.0028f, starGold, alpha * 0.65f);
        }
    }

    void RenderChristmasTree(float elapsed) {
        for (const auto& tp : g_treeParticles) {
            if (tp.base.life <= 0.01f) {
                continue;
            }
            float twinkle = 0.65f + 0.35f * std::sin(elapsed * tp.twinkleSpeed + tp.twinklePhase);
            float alpha = tp.base.life * (tp.isOrnament ? (0.45f + 0.55f * twinkle) : (0.60f + 0.40f * twinkle));
            DrawGlowCircle(tp.base.x, tp.base.y, tp.base.size, tp.base.color, alpha);
        }
        DrawTreeTopStar(elapsed);
    }

    // ========================== 雪花系统 ==========================

    void InitSnowflakes() {
        g_snowflakes.clear();
        g_snowflakes.reserve(kSnowflakeCount);

        float W = static_cast<float>(g_render.width);
        float H = static_cast<float>(g_render.height);

        for (int i = 0; i < kSnowflakeCount; ++i) {
            Snowflake s;
            s.x = RandFloat(0.0f, W);
            s.y = RandFloat(0.0f, H);
            s.fallSpeed = H * RandFloat(0.035f, 0.095f); // 按屏幕高度比例计算下落速度
            s.swayAmp = W * RandFloat(0.006f, 0.018f);
            s.swayFreq = RandFloat(0.8f, 2.0f);
            s.swayPhase = RandFloat(0.0f, kTwoPi);
            s.size = H * RandFloat(0.0012f, 0.0026f);
            s.alpha = RandFloat(0.45f, 0.90f);
            g_snowflakes.push_back(s);
        }
    }

    void UpdateSnowflakes(float dt, float elapsed) {
        float W = static_cast<float>(g_render.width);
        float H = static_cast<float>(g_render.height);

        for (auto& s : g_snowflakes) {
            s.y += s.fallSpeed * dt;
            s.x += std::cos(elapsed * s.swayFreq + s.swayPhase) * s.swayAmp * dt;

            if (s.y > H + 10.0f) {
                s.y = -10.0f;
                s.x = RandFloat(0.0f, W);
            }
            if (s.x < -10.0f) s.x += W + 20.0f;
            if (s.x > W + 10.0f) s.x -= W + 20.0f;
        }
    }

    void RenderSnowflakes() {
        const ColorRGB white{ 0.92f, 0.96f, 1.0f };
        for (const auto& s : g_snowflakes) {
            // 雪花使用轻量级两层同心圆（微弱柔边 + 白色内核）
            DrawAdditiveFilledCircle(s.x, s.y, s.size * 1.8f, white, s.alpha * 0.22f);
            DrawAdditiveFilledCircle(s.x, s.y, s.size, white, s.alpha * 0.78f);
        }
    }

    // ========================== 烟花系统 ==========================

    void LaunchFirework() {
        float W = static_cast<float>(g_render.width);
        float H = static_cast<float>(g_render.height);

        for (auto& rocket : g_rockets) {
            if (!rocket.active) {
                rocket.active = true;
                // 随机水平位置，避开最边缘
                rocket.x = RandFloat(W * 0.10f, W * 0.90f);
                rocket.y = H + 5.0f;
                rocket.vx = RandFloat(-W * 0.03f, W * 0.03f);
                rocket.vy = -RandFloat(H * 0.55f, H * 0.78f);
                rocket.targetY = RandFloat(H * 0.12f, H * 0.48f);

                // 鲜艳烟花色系
                static const ColorRGB palette[] = {
                    { 1.0f, 0.28f, 0.32f }, // 宝石红
                    { 1.0f, 0.78f, 0.22f }, // 金黄
                    { 0.30f, 0.95f, 0.55f }, // 翡翠绿
                    { 0.32f, 0.72f, 1.0f },  // 天蓝
                    { 0.92f, 0.40f, 1.0f },  // 紫罗兰
                    { 1.0f, 0.52f, 0.18f }   // 橙红
                };
                rocket.color = palette[std::rand() % 6];
                return;
            }
        }
    }

    void ExplodeFirework(const FireworkRocket& rocket) {
        float H = static_cast<float>(g_render.height);
        int burstCount = 90 + (std::rand() % 50); // 每次爆炸约 90~140 颗粒子
        float baseSpeed = H * RandFloat(0.14f, 0.24f);

        for (int i = 0; i < burstCount; ++i) {
            Particle* p = AcquireFireworkParticle();
            float angle = RandFloat(0.0f, kTwoPi);
            float speedScale = RandFloat(0.25f, 1.05f);
            float speed = baseSpeed * speedScale;

            p->x = rocket.x;
            p->y = rocket.y;
            p->vx = std::cos(angle) * speed;
            p->vy = std::sin(angle) * speed;

            // 略微混合白色与色相微调，使烟花更有层次感
            p->color = {
                ClampFloat(rocket.color.r + RandFloat(-0.12f, 0.18f), 0.0f, 1.0f),
                ClampFloat(rocket.color.g + RandFloat(-0.12f, 0.18f), 0.0f, 1.0f),
                ClampFloat(rocket.color.b + RandFloat(-0.12f, 0.18f), 0.0f, 1.0f)
            };
            p->maxLife = RandFloat(1.1f, 2.1f);
            p->life = p->maxLife;
            p->size = H * RandFloat(0.0016f, 0.0028f);
            p->active = true;
            p->trailCount = 0;
        }
    }

    void UpdateFireworks(float dt, float elapsed) {
        // 仅在第二阶段持续放烟花
        if (elapsed >= kPhase1Duration) {
            g_nextFireworkTimer -= dt;
            if (g_nextFireworkTimer <= 0.0f) {
                LaunchFirework();
                g_nextFireworkTimer = RandFloat(0.5f, 1.5f); // 每隔 0.5~1.5 秒发射一枚
            }
        }

        float H = static_cast<float>(g_render.height);
        float gravity = H * 0.22f; // 重力加速度按屏幕高度比例设定

        // 更新上升中的烟花弹
        for (auto& rocket : g_rockets) {
            if (!rocket.active) {
                continue;
            }
            rocket.x += rocket.vx * dt;
            rocket.y += rocket.vy * dt;
            rocket.vy += gravity * 0.35f * dt;

            if (rocket.y <= rocket.targetY || rocket.vy >= 0.0f) {
                ExplodeFirework(rocket);
                rocket.active = false;
            }
        }

        // 更新烟花爆炸粒子（重力、空气阻力、拖尾记录、死亡回收）
        for (auto& p : g_fireworkPool) {
            if (!p.active) {
                continue;
            }

            // 记录拖尾历史位置
            for (int i = std::min(p.trailCount, kTrailLength - 1); i > 0; --i) {
                p.trailX[i] = p.trailX[i - 1];
                p.trailY[i] = p.trailY[i - 1];
            }
            p.trailX[0] = p.x;
            p.trailY[0] = p.y;
            if (p.trailCount < kTrailLength) {
                ++p.trailCount;
            }

            p.vx *= std::pow(0.28f, dt); // 空气阻力减速
            p.vy *= std::pow(0.28f, dt);
            p.vy += gravity * dt;        // 受重力下落

            p.x += p.vx * dt;
            p.y += p.vy * dt;
            p.life -= dt;

            if (p.life <= 0.0f) {
                p.active = false;
            }
        }
    }

    void RenderFireworks() {
        float H = static_cast<float>(g_render.height);

        // 渲染上升中的烟花弹
        for (const auto& rocket : g_rockets) {
            if (!rocket.active) {
                continue;
            }
            DrawGlowCircle(rocket.x, rocket.y, H * 0.0030f, rocket.color, 0.95f);
            DrawGlowCircle(rocket.x, rocket.y + H * 0.008f, H * 0.0020f, { 1.0f, 0.85f, 0.5f }, 0.55f);
        }

        // 渲染烟花爆炸粒子及其拖尾
        for (const auto& p : g_fireworkPool) {
            if (!p.active) {
                continue;
            }
            float lifeRatio = ClampFloat(p.life / p.maxLife, 0.0f, 1.0f);
            float alpha = lifeRatio * lifeRatio; // 逐渐熄灭

            // 绘制拖尾（从近到远逐渐变暗变小）
            for (int i = 0; i < p.trailCount; ++i) {
                float trailFade = (1.0f - static_cast<float>(i + 1) / static_cast<float>(kTrailLength + 1));
                DrawAdditiveFilledCircle(
                    p.trailX[i],
                    p.trailY[i],
                    p.size * (0.85f - 0.1f * i),
                    p.color,
                    alpha * trailFade * 0.42f
                );
            }

            // 绘制粒子发光主体
            DrawGlowCircle(p.x, p.y, p.size, p.color, alpha);
        }
    }

    // ========================== 祝福语文字生成与绘制 ==========================

    void InitBlessingTextCache() {
        int W = g_render.width;
        int H = g_render.height;

        // 祝福语位于树下方（树底约在 0.80H，文字区域放在 0.80H ~ 0.97H）
        int stripH = std::max(64, static_cast<int>(H * 0.18f));
        int offsetY = static_cast<int>(H * 0.80f);
        if (offsetY + stripH > H) {
            stripH = H - offsetY;
        }

        g_textCache.width = W;
        g_textCache.height = stripH;
        g_textCache.offsetY = offsetY;
        g_textCache.coreMask.assign(static_cast<size_t>(W) * stripH, 0.0f);
        g_textCache.glowMask.assign(static_cast<size_t>(W) * stripH, 0.0f);

        // 创建临时 GDI DIBSection 渲染高清晰度微软雅黑粗体文字
        HDC screenDC = GetDC(nullptr);
        HDC tempDC = CreateCompatibleDC(screenDC);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = W;
        bmi.bmiHeader.biHeight = -stripH;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* tempBits = nullptr;
        HBITMAP tempBmp = CreateDIBSection(screenDC, &bmi, DIB_RGB_COLORS, &tempBits, nullptr, 0);
        HBITMAP oldBmp = static_cast<HBITMAP>(SelectObject(tempDC, tempBmp));
        ReleaseDC(nullptr, screenDC);

        // 清空为纯黑
        uint32_t* raw = static_cast<uint32_t*>(tempBits);
        std::fill(raw, raw + static_cast<size_t>(W) * stripH, 0u);

        // 字高约为屏幕高度的 1/12，微软雅黑加粗
        int fontHeight = -std::max(16, static_cast<int>(std::lround(static_cast<float>(H) / 12.0f)));
        HFONT font = CreateFontW(
            fontHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑"
        );
        HFONT oldFont = static_cast<HFONT>(SelectObject(tempDC, font));

        SetBkMode(tempDC, TRANSPARENT);
        SetTextColor(tempDC, RGB(255, 255, 255));

        RECT rc{ 0, 0, W, stripH };
        DrawTextW(tempDC, L"祝宿悦天天开心", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
        GdiFlush();

        // 提取抗锯齿文字蒙版
        for (size_t i = 0; i < g_textCache.coreMask.size(); ++i) {
            uint8_t val = static_cast<uint8_t>((raw[i] >> 8) & 0xFFu);
            g_textCache.coreMask[i] = static_cast<float>(val) / 255.0f;
        }

        SelectObject(tempDC, oldFont);
        DeleteObject(font);
        SelectObject(tempDC, oldBmp);
        DeleteObject(tempBmp);
        DeleteDC(tempDC);

        // 对文字蒙版进行两遍可分离盒式模糊，生成淡淡的金色光晕蒙版
        int blurRadius = std::max(4, static_cast<int>(H * 0.006f));
        std::vector<float> tempBlur(g_textCache.coreMask.size(), 0.0f);

        // 水平模糊
        for (int y = 0; y < stripH; ++y) {
            float sum = 0.0f;
            int count = 0;
            for (int x = 0; x < std::min(W, blurRadius); ++x) {
                sum += g_textCache.coreMask[static_cast<size_t>(y) * W + x];
                ++count;
            }
            for (int x = 0; x < W; ++x) {
                int addIdx = x + blurRadius;
                if (addIdx < W) {
                    sum += g_textCache.coreMask[static_cast<size_t>(y) * W + addIdx];
                    ++count;
                }
                int remIdx = x - blurRadius - 1;
                if (remIdx >= 0) {
                    sum -= g_textCache.coreMask[static_cast<size_t>(y) * W + remIdx];
                    --count;
                }
                tempBlur[static_cast<size_t>(y) * W + x] = sum / static_cast<float>(std::max(1, count));
            }
        }

        // 垂直模糊
        for (int x = 0; x < W; ++x) {
            float sum = 0.0f;
            int count = 0;
            for (int y = 0; y < std::min(stripH, blurRadius); ++y) {
                sum += tempBlur[static_cast<size_t>(y) * W + x];
                ++count;
            }
            for (int y = 0; y < stripH; ++y) {
                int addIdx = y + blurRadius;
                if (addIdx < stripH) {
                    sum += tempBlur[static_cast<size_t>(addIdx) * W + x];
                    ++count;
                }
                int remIdx = y - blurRadius - 1;
                if (remIdx >= 0) {
                    sum -= tempBlur[static_cast<size_t>(remIdx) * W + x];
                    --count;
                }
                g_textCache.glowMask[static_cast<size_t>(y) * W + x] =
                    ClampFloat((sum / static_cast<float>(std::max(1, count))) * 1.8f, 0.0f, 1.0f);
            }
        }
    }

    void RenderBlessingText(float elapsed) {
        if (elapsed < kPhase1Duration) {
            return;
        }

        // 第二阶段开始后 1.5 秒内淡入，之后保持显示并轻微呼吸式明暗变化
        float phase2Time = elapsed - kPhase1Duration;
        float fadeIn = EaseOutCubic(phase2Time / 1.5f);
        float breath = 0.80f + 0.20f * std::sin(phase2Time * 2.2f);
        float intensity = fadeIn * breath;
        if (intensity <= 0.01f) {
            return;
        }

        const int W = g_textCache.width;
        const int stripH = g_textCache.height;
        const int offsetY = g_textCache.offsetY;

        // 淡金色光晕 + 亮金色文字主体
        const float glowR = 255.0f * intensity * 0.42f;
        const float glowG = 195.0f * intensity * 0.42f;
        const float glowB = 60.0f * intensity * 0.42f;

        const float coreR = 255.0f * intensity;
        const float coreG = 216.0f * intensity;
        const float coreB = 92.0f * intensity;

        for (int y = 0; y < stripH; ++y) {
            uint32_t* dstRow = g_render.pixels + static_cast<size_t>(offsetY + y) * W;
            const size_t maskRowOffset = static_cast<size_t>(y) * W;
            for (int x = 0; x < W; ++x) {
                float gMask = g_textCache.glowMask[maskRowOffset + x];
                float cMask = g_textCache.coreMask[maskRowOffset + x];
                if (gMask <= 0.002f && cMask <= 0.002f) {
                    continue;
                }

                uint32_t dst = dstRow[x];
                uint32_t db = dst & 0xFFu;
                uint32_t dg = (dst >> 8) & 0xFFu;
                uint32_t dr = (dst >> 16) & 0xFFu;

                // 先叠加光晕
                dr = std::min(255u, dr + static_cast<uint32_t>(glowR * gMask));
                dg = std::min(255u, dg + static_cast<uint32_t>(glowG * gMask));
                db = std::min(255u, db + static_cast<uint32_t>(glowB * gMask));

                // 再按文字蒙版混合主体金色
                if (cMask > 0.002f) {
                    float inv = 1.0f - cMask * intensity;
                    dr = static_cast<uint32_t>(ClampInt(static_cast<int>(dr * inv + coreR * cMask), 0, 255));
                    dg = static_cast<uint32_t>(ClampInt(static_cast<int>(dg * inv + coreG * cMask), 0, 255));
                    db = static_cast<uint32_t>(ClampInt(static_cast<int>(db * inv + coreB * cMask), 0, 255));
                }

                dstRow[x] = (dr << 16) | (dg << 8) | db;
            }
        }
    }

    // ========================== 帧更新与总渲染 ==========================

    void UpdateScene(float dt) {
        g_elapsedTime += dt;
        UpdateSnowflakes(dt, g_elapsedTime);
        UpdateChristmasTree(g_elapsedTime);
        UpdateFireworks(dt, g_elapsedTime);
    }

    void RenderScene(HWND hwnd) {
        // 1. 填充预渲染夜空背景
        std::copy(g_render.background.begin(), g_render.background.end(), g_render.pixels);

        // 2. 绘制雪花、圣诞树、烟花与祝福语
        RenderSnowflakes();
        RenderFireworks();
        RenderChristmasTree(g_elapsedTime);
        RenderBlessingText(g_elapsedTime);

        // 3. 内存 DC 双缓冲一次性拷贝到屏幕
        HDC windowDC = GetDC(hwnd);
        BitBlt(windowDC, 0, 0, g_render.width, g_render.height, g_render.memDC, 0, 0, SRCCOPY);
        ReleaseDC(hwnd, windowDC);
    }

    // ========================== 窗口过程 ==========================

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_ERASEBKGND:
            return 1; // 禁止背景擦除，彻底避免闪烁
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                g_running = false;
                DestroyWindow(hwnd);
                return 0;
            }
            break;
        case WM_CLOSE:
            g_running = false;
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            g_running = false;
            PostQuitMessage(0);
            return 0;
        default:
            break;
        }
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

} // namespace

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    // 开启高 DPI 感知，确保 2560x1600 等高分屏获取真实物理分辨率
    SetProcessDPIAware();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    const wchar_t* className = L"ChristmasParticleWindow";
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = className;

    if (!RegisterClassExW(&wc)) {
        return 1;
    }

    // 全屏无边框窗口（WS_POPUP），覆盖整个屏幕
    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        className,
        L"Christmas Particle Effect",
        WS_POPUP | WS_VISIBLE,
        0, 0, screenWidth, screenHeight,
        nullptr, nullptr, hInstance, nullptr
    );
    if (!hwnd) {
        return 1;
    }

    InitRenderContext(hwnd, screenWidth, screenHeight);
    InitChristmasTree();
    InitSnowflakes();
    InitBlessingTextCache();

    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    UpdateWindow(hwnd);

    LARGE_INTEGER freq{};
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER prevTime{};
    QueryPerformanceCounter(&prevTime);

    const double targetFrameSec = 1.0 / static_cast<double>(kTargetFps);

    MSG msg{};
    while (g_running) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        if (!g_running) {
            break;
        }

        LARGE_INTEGER now{};
        QueryPerformanceCounter(&now);
        double dt = static_cast<double>(now.QuadPart - prevTime.QuadPart) / static_cast<double>(freq.QuadPart);

        if (dt < targetFrameSec) {
            double remainMs = (targetFrameSec - dt) * 1000.0;
            if (remainMs > 2.0) {
                Sleep(static_cast<DWORD>(remainMs - 1.0));
            }
            continue;
        }

        prevTime = now;
        float clampedDt = static_cast<float>(std::min(dt, 0.05)); // 防止切屏造成过大步长

        UpdateScene(clampedDt);
        RenderScene(hwnd);
    }

    CleanupRenderContext();
    return 0;
}
