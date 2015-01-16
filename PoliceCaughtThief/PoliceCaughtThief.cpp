/**
* @defgroup LGC_ChooseGift.c
* @{
* @code
Copyright (c) 2012, 广东小天才科技有限公司
All rights reserved.
  
文件名称： PoliceAndThief.c
文件标识： 警察抓小偷
文件摘要： 警察抓小偷
修改历史：
版本    日期        作者     修改内容
------  ----------  -------  ----------------------------
V1.0    2012.07.20  卢郴群    程序开始编写
* @endcode
*/
  
#include <windows.h>
  
#define SQUARE_SIDE 60      // 小正方形宽度
#define SRC_WID     (360+6) // 屏幕宽度
#define SRC_HGT     (420+38)// 屏幕高度
#define ROW_MAX     6       // 迷宫的行最大值
#define COL_MAX     6       // 迷宫的列最大值
#define POLICE_MAX  6       // 警察的最个数(283680)
  
#define SPACE        0      // 空白区域
#define BUILD        1      // 建筑物
#define THIEF        2      // 小偷
#define POLICE_HOLD  4      // 警察占领的标记
#define THIEF_ESCAPE 3      // 小偷可以逃跑的路线
#define THIEF_RUN    -1     // 标记小偷走过的迷宫
  
#define STEP_CNT     4      // 步长的四个方向
#define MATRIX_MAX   3      // 警察最大维度
  
#define TEMPMAZE_COPY_TO_MAZE   1   // 将临时数据复制到正式数据中
#define COMMOND_CATCH_THEIF     0   // “抓小偷”命令
#define COMMOND_RESET           1   // “重置”命令
  
#define arrLen(arr) sizeof(arr) / sizeof(arr[0])
  
typedef struct tagPOLICE
{
    int row;
    int col;
    int data[MATRIX_MAX * 2];
}POLICEROLE;
  
typedef struct tagPOLICTDATA
{
    const POLICEROLE *pr;
    int len;
}POLICTDATA;
  
typedef struct tagPOLICEINFO
{
    int pId;
    int index;
    int row;
    int col;
}POLICEINFO;
  
// 警察角色1
const POLICEROLE pr1[] =
{
    {2, 3, {0, 4, 0, 3, 3, 3}},
    {3, 2, {3, 0, 3, 4, 3, 0}},
    {2, 3, {3, 3, 3, 0, 4, 0}},
    {3, 2, {0, 3, 4, 3, 0, 3}},
};
// 警察角色2
const POLICEROLE pr2[] =
{
    {1, 3, {3, 5, 3}},
    {3, 1, {3, 5, 3}},
};
// 警察角色3
const POLICEROLE pr3[] =
{
    {2, 2, {3, 0, 6, 3}},
    {2, 2, {6, 3, 3, 0}},
    {2, 2, {3, 6, 0, 3}},
    {2, 2, {0, 3, 3, 6}},
};
// 警察角色4
const POLICEROLE pr4[] =
{
    {2, 3, {3, 0, 0, 7, 3, 3}},
    {3, 2, {7, 3, 3, 0, 3, 0}},
    {2, 3, {3, 3, 7, 0, 0, 3}},
    {3, 2, {0, 3, 0, 3, 3, 7}},
};
// 警察角色5
const POLICEROLE pr5[] =
{
    {2, 2, {3, 8, 3, 0}},
    {2, 2, {3, 3, 0, 8}},
    {2, 2, {0, 3, 8, 3}},
    {2, 2, {8, 0, 3, 3}},
};
// 警察角色6
const POLICEROLE pr6[] =
{
    {2, 3, {3, 3, 9, 3, 0, 0}},
    {3, 2, {3, 3, 0, 3, 0, 9}},
    {2, 3, {0, 0, 3, 9, 3, 3}},
    {3, 2, {9, 0, 3, 0, 3, 3}},
};
// 所有警察角色
const POLICTDATA pd[] =
{
    {pr1, arrLen(pr1)},
    {pr2, arrLen(pr2)},
    {pr3, arrLen(pr3)},
    {pr4, arrLen(pr4)},
    {pr5, arrLen(pr5)},
    {pr6, arrLen(pr6)},
};
  
// 全局变量
int maze[ROW_MAX][COL_MAX];     // 迷宫
int tmaze[ROW_MAX][COL_MAX];    // 迷宫数据存档
int startX, startY;
bool thiefFindPath;
bool policeWrapThief;
bool clickBd;
bool clickTh;
POLICEINFO policeInfo[POLICE_MAX], pi[POLICE_MAX];
  
// 函数声明
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
bool pointInRect(int x, int y, RECT rect);
bool checkCondition(void);
bool checkStep(int row, int col);
bool checePos(int row, int col);
bool theifSearchPath(int curRow, int curCol);
bool judgePolicePos(int pId, int index, int x, int y);
void setTheifPos(void);
void chgMazePolice(int pId, int index, int x, int y);
void recoverMazePolice(int pId, int index, int x, int y);
void setPolicePos(int deep);
void copyMaze(int flag);
void drawMaze(HWND hwnd);
void initGrobalVar(void);
void resetGrobalVar(void);
int getPoliceX(int pId, int index);
int getPoliceY(int pId, int index);
  
  
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("警察抓小偷") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;
  
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
  
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("注册窗口失败"), szAppName, MB_ICONERROR) ;
          return 0 ;
     }
       
     hwnd = CreateWindow (szAppName,                    // window class name
                          TEXT ("警察抓小偷"),           // window caption
                          WS_CAPTION | WS_SYSMENU,      // window style
                          CW_USEDEFAULT,                // initial x position
                          CW_USEDEFAULT,                // initial y position
                          SRC_WID,                      // initial x size
                          SRC_HGT,                      // initial y size
                          NULL,                         // parent window handle
                          NULL,                         // window menu handle
                          hInstance,                    // program instance handle
                          NULL) ;                       // creation parameters
       
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
       
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}
  
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  
     PAINTSTRUCT ps;
     HDC hdc;
     RECT rect;
     RECT rectBd = {0, ROW_MAX*SQUARE_SIDE+10, SQUARE_SIDE, (ROW_MAX+1)*SQUARE_SIDE+10};
     RECT rectTh = {SQUARE_SIDE*2, ROW_MAX*SQUARE_SIDE+10, SQUARE_SIDE+SQUARE_SIDE*2, (ROW_MAX+1)*SQUARE_SIDE+10};
     RECT rectMaze = {0, 0, ROW_MAX*SQUARE_SIDE, COL_MAX*SQUARE_SIDE};
  
     int row, col;
     int x, y;
  
     switch (message)
     {
     case WM_LBUTTONDOWN:
         x = LOWORD(lParam);
         y = HIWORD(lParam);
         // 选建筑物
         if (pointInRect(x, y, rectBd))
         {
             clickBd = true;
             clickTh = false;
         }
  
         // 选小偷
         if (pointInRect(x, y, rectTh))
         {
             clickBd = false;
             clickTh = true;
         }
  
         // 去掉小偷或警察
         if (pointInRect(x, y, rectMaze))
         {
             row = y / SQUARE_SIDE;
             col = x / SQUARE_SIDE;
               
             if (clickBd)
             {
                 maze[row][col] = (0 == maze[row][col]) ? (1) : (0);
             }
  
             if (clickTh)
             {
                 maze[row][col] = (0 == maze[row][col]) ? (2) : (0);
             }
         }
         InvalidateRect( hwnd, NULL, TRUE) ;
         return 0;
     case WM_MOUSEMOVE:
         return 0;
     case WM_CREATE:
         // 按钮“抓小偷”
         CreateWindow( TEXT("button"), TEXT("抓小偷"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                        SQUARE_SIDE*4-10, ROW_MAX*SQUARE_SIDE+10,
                        SQUARE_SIDE, SQUARE_SIDE,
                        hwnd, (HMENU)COMMOND_CATCH_THEIF,
                        ((LPCREATESTRUCT) lParam)->hInstance, NULL) ;
         // 按钮“重置”
         CreateWindow( TEXT("button"), TEXT("重置"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                        SQUARE_SIDE*5, ROW_MAX*SQUARE_SIDE+10,
                        SQUARE_SIDE, SQUARE_SIDE,
                        hwnd, (HMENU)COMMOND_RESET,
                        ((LPCREATESTRUCT) lParam)->hInstance, NULL) ;
        return 0 ;
     case WM_PAINT:
        HBRUSH hBrush;
        hdc = BeginPaint (hwnd, &ps) ;
  
        drawMaze(hwnd);
  
        //画矩形框
        for (int xx=0 ; xx<=ROW_MAX*SQUARE_SIDE && !policeWrapThief; xx+=SQUARE_SIDE)
        {
            MoveToEx (hdc, xx, 0, NULL);
            LineTo (hdc, xx, COL_MAX*SQUARE_SIDE);
        }
        for (int yy=0 ; yy<=COL_MAX*SQUARE_SIDE && !policeWrapThief; yy+=SQUARE_SIDE)
        {
            MoveToEx (hdc, 0, yy, NULL) ;
            LineTo (hdc, ROW_MAX*SQUARE_SIDE, yy) ;
        }
  
        // 建筑物
        SetRect (&rect, 0, ROW_MAX*SQUARE_SIDE+10, SQUARE_SIDE, (ROW_MAX+1)*SQUARE_SIDE+10);
        hBrush = CreateSolidBrush(RGB (0, 0, 0));
        hdc = GetDC (hwnd) ;
        FillRect (hdc, &rect, hBrush);
        TextOut (hdc, 7, 392, "建筑物", 6);
  
        // 小偷
        SetRect (&rect, SQUARE_SIDE*2, ROW_MAX*SQUARE_SIDE+10, SQUARE_SIDE+SQUARE_SIDE*2, (ROW_MAX+1)*SQUARE_SIDE+10);
        hBrush = CreateSolidBrush(RGB (255, 0, 0));
        hdc = GetDC (hwnd) ;
        FillRect (hdc, &rect, hBrush);
        TextOut (hdc, 135, 392, "小偷", 4);
  
        ReleaseDC (hwnd, hdc);
        DeleteObject (hBrush);
        EndPaint (hwnd, &ps);
        return 0 ;
     case WM_COMMAND:
        if (COMMOND_CATCH_THEIF == LOWORD(wParam))
        {
            if (checkCondition())
            {
                // 全局变量初始化
                initGrobalVar();
                setTheifPos();
                setPolicePos(0);
                copyMaze(TEMPMAZE_COPY_TO_MAZE);    // 将临时保存的数据复制到迷宫中
  
                if (!policeWrapThief)
                {
                    MessageBox (NULL, TEXT("亲，我无能为力！抓不到小偷！【请找完一个请按重置再找下一个答案】"), TEXT("提示"), MB_ICONERROR) ;
                    resetGrobalVar();
                }
            }
            else
            {
                resetGrobalVar();
            }
			InvalidateRect(hwnd, NULL, TRUE);
        }
  
        if(COMMOND_RESET == LOWORD(wParam))
        {
            resetGrobalVar();
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
     case WM_DESTROY:
          PostQuitMessage(0);
          return 0;
     case WM_KEYDOWN:
         return 0;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
  
bool pointInRect(int x, int y, RECT rect)
{
    if (x > rect.left && y > rect.top && x < rect.right && y < rect.bottom)
    {
        return true;
    }
    else
    {
        return false;
    }
}
  
// 获取小偷X位置
void setTheifPos()
{
    for (int row=0; row<ROW_MAX; ++row)
    {
        for (int col=0; col<COL_MAX; ++col)
        {
            if (THIEF == maze[row][col])
            {
                startX = row;
                startY = col;
                return;// 找到立即退出
            }
        }
    }
}
  
// 检查小偷是否可走该位置
bool checkStep(int row, int col)
{
    return (THIEF_ESCAPE == maze[row][col]) ? (true) : (false);
}
  
bool checePos(int row, int col)
{
    return (row<0 || row>=ROW_MAX || col<0 || col>=COL_MAX) ? (false) : (true);
}
  
// 搜索小偷逃跑路径
// false 不能出来
// true 能出来
bool theifSearchPath(int curRow, int curCol)
{
    int stepIndex;
    int newRow, newCol;
  
    int stepX[STEP_CNT] = {0, 0, -1, 1};
    int stepY[STEP_CNT] = {1, -1, 0, 0};
  
    for (stepIndex=0; stepIndex<STEP_CNT; ++stepIndex)
    {
        newRow = curRow + stepX[stepIndex];
        newCol = curCol + stepY[stepIndex];
  
        if (checkStep(newRow, newCol))
        {
            // 走出迷宫(深度优先搜索结束条件)
            if (newRow <= 0 || newCol <= 0 || newRow >= ROW_MAX-1 || newCol >= COL_MAX-1)
            {
                thiefFindPath = true;
            }
            else
            {
                maze[newRow][newCol] = THIEF_RUN;// 标记走的迷宫
                theifSearchPath(newRow, newCol);
                maze[newRow][newCol] = THIEF_ESCAPE;// 还原现场
            }
        }
    }
  
    return thiefFindPath;
}
  
/*
pId 警察的ID
index 警察的变形索引
x, y 警察放的位置
*/
bool judgePolicePos(int pId, int index, int x, int y)
{
    int arrRow = pd[pId].pr[index].row;
    int arrCol = pd[pId].pr[index].col;
    int px = getPoliceX(pId, index);
    int py = getPoliceY(pId, index);
  
    int row, col, mazeRow, mazeCol;
    int tempData;
  
    for (row=0; row<arrRow; ++row)
    {
        for (col=0; col<arrCol; ++col)
        {
            tempData = pd[pId].pr[index].data[row * arrCol + col];
  
            mazeRow = x + row - px;
            mazeCol = y + col - py;
  
            if (checePos(mazeRow, mazeCol))// 警察放到迷宫里面不能出迷宫
            {
                if (SPACE != tempData && SPACE != maze[mazeRow][mazeCol])
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
  
    return true;
}
  
void chgMazePolice(int pId, int index, int x, int y)
{
    int arrRow = pd[pId].pr[index].row;
    int arrCol = pd[pId].pr[index].col;
    int px = getPoliceX(pId, index);
    int py = getPoliceY(pId, index);
  
    int row, col, mazeRow, mazeCol;
    int tempData;
  
    for (row=0; row<arrRow; ++row)
    {
        for (col=0; col<arrCol; ++col)
        {
            tempData = pd[pId].pr[index].data[row * arrCol + col];
            if (SPACE != tempData)
            {
                mazeRow = x + row - px;
                mazeCol = y + col - py;
                maze[mazeRow][mazeCol] = tempData;
            }
        }
    }
}
  
void recoverMazePolice(int pId, int index, int x, int y)
{
    int arrRow = pd[pId].pr[index].row;
    int arrCol = pd[pId].pr[index].col;
    int px = getPoliceX(pId, index);
    int py = getPoliceY(pId, index);
  
    int row, col, mazeRow, mazeCol;
    int tempData;
  
    for (row=0; row<arrRow; ++row)
    {
        for (col=0; col<arrCol; ++col)
        {
            tempData = pd[pId].pr[index].data[row * arrCol + col];
            if (SPACE != tempData)
            {
                mazeRow = x + row - px;
                mazeCol = y + col - py;
                if (maze[mazeRow][mazeCol] >= THIEF_ESCAPE)
                {
                    maze[mazeRow][mazeCol] = SPACE;
                }
            }
        }
    }
}
  
// 放警察的位置
void setPolicePos(int deep)
{
    if (!policeWrapThief)
    {
        if(deep >= POLICE_MAX)
        {
            thiefFindPath = false;
            if (!theifSearchPath(startX, startY))
            {
                for (int pId=0; pId<POLICE_MAX; ++pId)
                {
                    pi[pId].pId = policeInfo[pId].pId;
                    pi[pId].index = policeInfo[pId].index;
                    pi[pId].row = policeInfo[pId].row;
                    pi[pId].col = policeInfo[pId].col;
                }
  
                policeWrapThief = true;
                copyMaze(0);    // 将迷宫数据保存一份
            }
            else
            {
                deep--;// 回溯
            }
        }
        else
        {
            for (int row=0; row<ROW_MAX; ++row)
            {
                for (int col=0; col<COL_MAX; ++col)
                {
                    if (SPACE == maze[row][col])
                    {
                        for (int index=0; index<pd[deep].len; ++index)
                        {
                            if (judgePolicePos(deep, index, row, col))
                            {
                                policeInfo[deep].pId = deep;
                                policeInfo[deep].index = index;
                                policeInfo[deep].row = row;
                                policeInfo[deep].col = col;
  
                                chgMazePolice(deep, index, row, col);
                                setPolicePos(deep+1);
                                recoverMazePolice(deep, index, row, col);
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void copyMaze(int flag)
{
    int row, col;
    if (TEMPMAZE_COPY_TO_MAZE == flag)
    {
        for (row=0; row<ROW_MAX; ++row)
            for (col=0; col<COL_MAX; ++col)
                maze[row][col] = tmaze[row][col];
  
    }
    else
    {
        for (row=0; row<ROW_MAX; ++row)
            for (col=0; col<COL_MAX; ++col)
                tmaze[row][col] = maze[row][col];
    }
}
  
void drawMaze(HWND hwnd)
{
    HBRUSH hBrush;
    HDC         hdc ;
    PAINTSTRUCT ps ;
    RECT        rect;
    int row, col;
  
    hdc = BeginPaint (hwnd, &ps) ;
    GetClientRect (hwnd, &rect) ;
  
    // 画警察
    for (int pId=0; pId<POLICE_MAX && policeWrapThief; ++pId)
    {
        int index = pi[pId].index;
        int x = pi[pId].row;
        int y = pi[pId].col;
      
        int arrRow = pd[pId].pr[index].row;
        int arrCol = pd[pId].pr[index].col;
        int px = getPoliceX(pId, index);
        int py = getPoliceY(pId, index);
  
        int row, col, mazeRow, mazeCol;
        int tempData;
  
        int rgb[POLICE_MAX][3] =
        {
            {0, 255, 0},
            {0, 0, 255},
            {255, 255, 0},
            {255, 0, 255},
            {0, 255, 255},
            {128, 0, 128}, 
        };
  
        hBrush = CreateSolidBrush(RGB (rgb[pId][0], rgb[pId][1], rgb[pId][2]));
        for (row=0; row<arrRow; ++row)
        {
            for (col=0; col<arrCol; ++col)
            {
                mazeRow = x + row - px;
                mazeCol = y + col - py;
                tempData = pd[pId].pr[index].data[row * arrCol + col];
  
                if (tempData != SPACE)
                {          
                    SetRect (&rect, mazeCol*SQUARE_SIDE, mazeRow*SQUARE_SIDE, (mazeCol+1)*SQUARE_SIDE, (mazeRow+1)*SQUARE_SIDE);
                    hdc = GetDC (hwnd) ;
                    FillRect (hdc, &rect, hBrush);
  
                    if (maze[mazeRow][mazeCol] >= POLICE_HOLD)
                    {
                        Ellipse(hdc,  mazeCol*SQUARE_SIDE+15, mazeRow*SQUARE_SIDE+15, (mazeCol+1)*SQUARE_SIDE-15, (mazeRow+1)*SQUARE_SIDE-15); // 画圆
                    }
                }
            }
        }
    }
  
    // 画小偷，空白区域，建筑物
    for (col=0; col<COL_MAX; ++col)
    {
        for(row=0; row<ROW_MAX; ++row)
        {
            SetRect (&rect, col*SQUARE_SIDE, row*SQUARE_SIDE, (col+1)*SQUARE_SIDE, (row+1)*SQUARE_SIDE);
            switch(maze[row][col])
            {
            case SPACE:
                hBrush = CreateSolidBrush(RGB (255, 255, 255));
                break;
            case BUILD:
                hBrush = CreateSolidBrush(RGB (0, 0, 0));
                break;
            case THIEF:
                hBrush = CreateSolidBrush(RGB (255, 0, 0));
                // Ellipse(hdc,  startY*SQUARE_SIDE+15, startX*SQUARE_SIDE+15, (startY+1)*SQUARE_SIDE-15, (startX+1)*SQUARE_SIDE-15); // 画圆
                break;
            default:
                break;
            }
  
            if (maze[row][col] <= THIEF)
            {
                hdc = GetDC (hwnd) ;
                FillRect (hdc, &rect, hBrush);
            }
        }
    }
}
  
void initGrobalVar(void)
{
    thiefFindPath = false;
    policeWrapThief = false;
}
  
void resetGrobalVar(void)
{
    for (int row=0; row<ROW_MAX; ++row)
    {
        for (int col=0; col<COL_MAX; ++col)
        {
            maze[row][col] = 0;
            tmaze[row][col] = 0;
        }
    }
  
    startX = startY = 0;
    thiefFindPath = false;
    policeWrapThief = false;
    clickBd = false;
    clickTh = false;
}
  
int getPoliceX(int pId, int index)
{
    int arrRow = pd[pId].pr[index].row;
    int arrCol = pd[pId].pr[index].col;
  
    for (int row=0; row<arrRow; ++row)
    {
        for (int col=0; col<arrCol; ++col)
        {
            if (pd[pId].pr[index].data[row * arrCol + col] >= POLICE_HOLD)
            {
                return row;
                break;
            }
        }
    }
  
    MessageBox (NULL, TEXT ("警察位置Y没找到"), TEXT ("警察位置Y没找到"), MB_ICONERROR) ;
  
    return 0;
}
  
int getPoliceY(int pId, int index)
{
    int arrRow = pd[pId].pr[index].row;
    int arrCol = pd[pId].pr[index].col;
  
    for (int row=0; row<arrRow; ++row)
    {
        for (int col=0; col<arrCol; ++col)
        {
            if (pd[pId].pr[index].data[row * arrCol + col] >= POLICE_HOLD)
            {
                return col;
                break;
            }
        }
    }
  
    MessageBox (NULL, TEXT("警察位置Y没找到"), TEXT("警察位置Y没找到"), MB_ICONERROR) ;
  
    return 0;
}
  
bool checkCondition(void)
{
    int row, col;
    int thiefCnt = 0;
    int buildCnt = 0;
    bool flag = true;
  
    for (row=0; row<ROW_MAX; ++row)
    {
        for (col=0; col<COL_MAX; ++col)
        {
            if (THIEF == maze[row][col])
            {
                thiefCnt++;
            }
  
            if (BUILD == maze[row][col])
            {
                buildCnt++;
            }
        }
    }
  
    if (thiefCnt != 1)
    {
        MessageBox (NULL, TEXT("要放1个且只能放1个【小偷】哦"), TEXT("错误"), MB_ICONERROR);
        flag = false;
    }
    if (buildCnt != 14)
    {
        MessageBox (NULL, TEXT("要放14个且只能放14个【建筑物】哦"), TEXT("错误"), MB_ICONERROR);
        flag = false;
    }
  
    return flag;
}