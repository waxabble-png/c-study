#define _CRT_SECURE_NO_WARNINGS
/*
 * ============================================================
 *  文字爱心动画  (Text Heart Animation)   —— 纯 C 语言
 * ------------------------------------------------------------
 *  平台：Windows 控制台
 *  编译：Visual Studio（新建"空项目" -> 添加 heart.c -> Ctrl+F5）
 *
 *  算法核心：心形隐函数
 *          f(x, y) = (x^2 + y^2 - 1)^3 - x^2 * y^3   <= 0 为心形内部
 *  在 COLS x ROWS 的字符网格上逐格判定，落在心形里的格子打印一张
 *  彩色小卡片（内容为随机情话），卡片文字随时间滚动/刷新，于是
 *  大量文字块堆叠出一个跳动的爱心。
 *
 *  可自行修改下面的  [ 可调参数 ]  与  [ 情话词库 ]  定制效果。
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>   /* _kbhit() */
#else
 /* 非 Windows 平台的占位定义，仅为方便跨平台编译检查；VS 下走上面分支 */
typedef int HANDLE;
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#endif

/* ===================== 可调参数 ===================== */
#define COLS       76       /* 横向格子数：越大爱心越宽（60~100）    */
#define ROWS       36       /* 纵向格子数：越大爱心越高（28~46）     */
#define CELL_W     2        /* 每格显示宽度（中文占 2 个字符宽）      */
#define TEXT_CHARS 2        /* 每格显示几个汉字：1~4，越大文字块越宽  */
#define FRAME_MS   45       /* 每帧毫秒数：越小越快                  */
#define GROW_STEP  2        /* 每帧生长推进的格数：越大出现越快      */

/* ===================== 情话词库 ===================== */
/* 每张卡片随机抽一句，可自由增删（注意是 UTF-8 中文即可） */
static const char* g_phrases[] = {
    "遇见你",   "别放弃",   "你可以的", "好好爱自己", "多想陪着你",
    "想陪着你", "记得微笑", "多喝热水", "阳光正好", "想和你一起",
    "把早安给你", "晚安好梦", "我喜欢你", "一直都在",  "乖乖听话",
    "愿你如愿", "每天开心", "抱一抱你", "一切顺利", "慢慢来",
    "记得吃饭", "天冷加衣", "照顾好自己", "你超棒的", "如约而至",
    "祝你好运", "别难过",   "有我在呢", "开心一点", "未来可期",
    "你是唯一", "心之所向", "一路顺风", "平安喜乐", "岁岁平安"
};
#define PHRASE_CNT ((int)(sizeof(g_phrases) / sizeof(g_phrases[0])))

/* 卡片配色：控制台 16 色编号，重复写以调整出现概率（12=亮红为主） */
static const int g_colors[] = {
    12, 12, 12, 12, 12,   /* 亮红  —— 主体      */
    13, 13,               /* 亮洋红            */
    14,                   /* 亮黄              */
    15,                   /* 亮白              */
     9,                   /* 亮蓝              */
    10,                   /* 亮绿              */
     6                    /* 暗黄              */
};
#define COLOR_CNT ((int)(sizeof(g_colors) / sizeof(g_colors[0])))

/* ===================== 运行期状态 ===================== */
static int g_inside[ROWS][COLS];   /* 1 = 该格在心形内部               */
static int g_phrase[ROWS][COLS];   /* 该格当前显示第几句情话           */
static int g_color[ROWS][COLS];   /* 该格当前颜色下标                 */
static int g_showRow[ROWS];         /* 第 r 行已生长到第几列（生长动画）*/
static int g_showCol[COLS];         /* 第 c 列已生长到第几行（生长动画）*/
static int g_shift = 0;               /* 全局滚动位移，驱动文字/颜色流动  */

/* ===================== 终端控制 ===================== */
static HANDLE g_hOut = INVALID_HANDLE_VALUE;

static void termInit(void) {
#ifdef _WIN32
    g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
#else
    g_hOut = (HANDLE)0;
#endif
}

static void hideCursor(void) {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO ci;
    if (GetConsoleCursorInfo(g_hOut, &ci)) {
        ci.bVisible = FALSE;
        SetConsoleCursorInfo(g_hOut, &ci);
    }
#endif
}

static void showCursor(void) {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO ci;
    if (GetConsoleCursorInfo(g_hOut, &ci)) {
        ci.bVisible = TRUE;
        SetConsoleCursorInfo(g_hOut, &ci);
    }
#endif
}

static void gotoXY(int x, int y) {   /* x=列, y=行, 均从 0 开始 */
#ifdef _WIN32
    COORD pos;
    pos.X = (SHORT)x;
    pos.Y = (SHORT)y;
    SetConsoleCursorPosition(g_hOut, pos);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

static void setColor(int color) {
#ifdef _WIN32
    SetConsoleTextAttribute(g_hOut, (WORD)color);
#else
    printf("\033[%dm", 30 + color);
#endif
}

/* 检测是否有按键按下（用于退出） */
static int keyPressed(void) {
#ifdef _WIN32
    return _kbhit();
#else
    return 0;
#endif
}

static void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

/* 取一个 UTF-8 字符串的前 n 个"字符"（一个汉字算 1 个字符）写入 out */
static void utf8Prefix(const char* src, int maxChars, char* out) {
    int i = 0, bi = 0, n = 0;
    while (src[i] && n < maxChars) {
        unsigned char ch = (unsigned char)src[i];
        int bytes = 1;
        if (ch >= 0xF0) bytes = 4;
        else if (ch >= 0xE0) bytes = 3;
        else if (ch >= 0xC0) bytes = 2;
        for (int k = 0; k < bytes && src[i + k]; k++) out[bi++] = src[i + k];
        i += bytes;
        n += 1;
    }
    out[bi] = '\0';
}

/* 计算 UTF-8 字符串的显示宽度（汉字按 2 计） */
static int utf8Width(const char* s) {
    int i = 0, w = 0;
    while (s[i]) {
        unsigned char ch = (unsigned char)s[i];
        int bytes = 1;
        if (ch >= 0xF0) bytes = 4;
        else if (ch >= 0xE0) bytes = 3;
        else if (ch >= 0xC0) bytes = 2;
        w += (bytes >= 3) ? 2 : 1;
        i += bytes;
    }
    return w;
}

/* ===================== 心形生成 ===================== */
static int inHeart(double x, double y) {
    double a = x * x + y * y - 1.0;
    return (a * a * a - x * x * y * y * y) <= 0.0;
}

static void buildHeart(void) {
    int r, c;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            /* 归一到 [-1,1]，再乘缩放系数控制爱心胖瘦高矮 */
            double x = ((double)c / (COLS - 1) - 0.5) * 2.0 * 1.55;
            double y = (0.5 - (double)r / (ROWS - 1)) * 2.0 * 1.28;
            g_inside[r][c] = inHeart(x, y);
            g_phrase[r][c] = rand() % PHRASE_CNT;
            g_color[r][c] = rand() % COLOR_CNT;
        }
    }
}

/* ===================== 绘制一帧 ===================== */
static void drawFrame(void) {
    int r, c;
    for (r = 0; r < ROWS; r++) {
        gotoXY(0, r);
        for (c = 0; c < COLS; c++) {
            int visible = g_inside[r][c] && c <= g_showRow[r] && r <= g_showCol[c];
            if (visible) {
                int pi = (g_phrase[r][c] + g_shift) % PHRASE_CNT;
                int ci = (g_color[r][c] + g_shift) % COLOR_CNT;
                char buf[24];
                utf8Prefix(g_phrases[pi], TEXT_CHARS, buf); /* 每格最多 TEXT_CHARS 个汉字 */
                setColor(g_colors[ci]);
                printf("%s", buf);
                int rest = CELL_W * TEXT_CHARS - utf8Width(buf); /* 空格补齐，保证对齐 */
                while (rest-- > 0) printf(" ");
            }
            else {
                int w = CELL_W * TEXT_CHARS;              /* 空白格：等宽空格 */
                while (w-- > 0) printf(" ");
            }
        }
    }
    setColor(7);
}

/* ===================== 生长动画 ===================== */
static void resetGrow(void) {
    int i;
    for (i = 0; i < ROWS; i++) g_showRow[i] = -1;
    for (i = 0; i < COLS; i++) g_showCol[i] = -1;
}

static int growStep(int frame) {
    int i, done;
    for (i = 0; i < ROWS; i++) {
        int t = frame - (ROWS - i) * GROW_STEP;
        g_showRow[i] = (t < 0) ? -1 : (t > COLS ? COLS : t);
    }
    for (i = 0; i < COLS; i++) {
        int t = frame - 4 * GROW_STEP - i * GROW_STEP / 2;
        g_showCol[i] = (t < 0) ? -1 : (t > ROWS ? ROWS : t);
    }
    done = 1;
    for (i = 0; i < ROWS; i++) if (g_showRow[i] < COLS) { done = 0; break; }
    return done;
}

/* ===================== 主函数 ===================== */
int main(void) {
    int frame;
    time_t lastRandom = 0;

#ifdef _WIN32
    SetConsoleOutputCP(65001);          /* 控制台按 UTF-8 解释输出 */
    SetConsoleCP(65001);
#endif

    termInit();
    srand((unsigned)time(NULL));

    /* 适配窗口：把控制台缓冲区/窗口调大一些，避免滚动 */
#ifdef _WIN32
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        COORD size;
        SMALL_RECT rect;
        if (GetConsoleScreenBufferInfo(g_hOut, &csbi)) {
            size.X = (SHORT)(COLS * CELL_W * TEXT_CHARS + 4);
            size.Y = (SHORT)(ROWS + 4);
            SetConsoleScreenBufferSize(g_hOut, size);
            rect.Left = 0; rect.Top = 0;
            rect.Right = (SHORT)(COLS * CELL_W * TEXT_CHARS + 3);
            rect.Bottom = (SHORT)(ROWS + 3);
            SetConsoleWindowInfo(g_hOut, TRUE, &rect);
        }
    }
#endif

    hideCursor();
    system("cls");

    buildHeart();
    resetGrow();

    /* ---------- 阶段一：心形从左上向右下"生长"出来 ---------- */
    for (frame = 0; ; frame++) {
        int done = growStep(frame);
        drawFrame();
        sleepMs(FRAME_MS);
        if (keyPressed()) { showCursor(); return 0; }   /* 任意键退出 */
        if (done) break;
    }

    /* ---------- 阶段二：文字持续滚动 / 随机刷新 ---------- */
    for (frame = 0; ; frame++) {
        time_t now = time(NULL);
        int k;

        if (keyPressed()) break;                     /* 任意键退出 */

        g_shift++;                                  /* 驱动整体文字与颜色流动 */
        if (g_shift >= 100000) g_shift = 0;

        /* 每 0.4 秒随机刷新若干格，让画面始终在变化 */
        if (now != lastRandom) {
            lastRandom = now;
            for (k = 0; k < 12; k++) {
                int rr = rand() % ROWS;
                int cc = rand() % COLS;
                if (g_inside[rr][cc]) {
                    g_phrase[rr][cc] = rand() % PHRASE_CNT;
                    g_color[rr][cc] = rand() % COLOR_CNT;
                }
            }
        }

        drawFrame();
        sleepMs(FRAME_MS);
    }

    showCursor();
    return 0;
}
