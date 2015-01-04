#include <iostream> 
#include <iomanip> 
using namespace std; 
      
#define MATRIX_SIZE_MAX 5   // 迷宫大小 
#define STEP_CNT        4   // 四个走的方向 
      
int step;           // 记录迷宫走路的步骤 
int startX, startY; // 起始位置x, y 
int endX, endY;     // 终点位置x, y 
      
// 迷宫 
int maze[MATRIX_SIZE_MAX][MATRIX_SIZE_MAX] =  
{ 
    0, 1, 0, 0, 0, 
    0, 1, 0, 1, 0, 
    0, 0, 0, 0, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
}; 
      
void printMaze() 
{ 
    for (int row=0; row<MATRIX_SIZE_MAX; ++row) 
    { 
        for (int col=0; col<MATRIX_SIZE_MAX; ++col) 
        { 
            cout << setw(2) << maze[row][col] << " "; 
        } 
        cout << endl; 
    } 
    cout << endl; 
} 
      
bool checkStep(int row, int col) 
{ 
    if ( 
        row < MATRIX_SIZE_MAX &&  
        col < MATRIX_SIZE_MAX &&  
        row >=0 && col >=0 && 
        0 == maze[row][col] 
        ) 
    { 
        return true; 
    } 
    else
    { 
        return false; 
    } 
} 
      
void dfs(int curRow, int curCol) 
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
            // 标记走的迷宫 
            maze[newRow][newCol] = ++step; 
      
            // 如果走出迷宫，则打印 
            if (endX == newRow && endY == newCol) 
            { 
                maze[startX][startY] = 2;// 起始点设为2 
                printMaze(); 
                //return;   //如果只要找一种答案，找到就退出 
            } 
            else
            { 
                dfs(newRow, newCol); 
            } 
      
            // 还原现场 
            maze[newRow][newCol] = 0; 
            step--; 
        } 
    } 
} 
      
int main() 
{ 
    step = 2; 
    startX = 0;  
    startY = 0; 
    endX = 4; 
    endY = 4; 
      
    dfs(startX, startY); 
      
    return 0; 
}