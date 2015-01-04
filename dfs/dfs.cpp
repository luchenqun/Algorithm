#include <iostream> 
#include <iomanip> 
using namespace std; 
      
#define MATRIX_SIZE_MAX 5   // �Թ���С 
#define STEP_CNT        4   // �ĸ��ߵķ��� 
      
int step;           // ��¼�Թ���·�Ĳ��� 
int startX, startY; // ��ʼλ��x, y 
int endX, endY;     // �յ�λ��x, y 
      
// �Թ� 
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
            // ����ߵ��Թ� 
            maze[newRow][newCol] = ++step; 
      
            // ����߳��Թ������ӡ 
            if (endX == newRow && endY == newCol) 
            { 
                maze[startX][startY] = 2;// ��ʼ����Ϊ2 
                printMaze(); 
                //return;   //���ֻҪ��һ�ִ𰸣��ҵ����˳� 
            } 
            else
            { 
                dfs(newRow, newCol); 
            } 
      
            // ��ԭ�ֳ� 
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