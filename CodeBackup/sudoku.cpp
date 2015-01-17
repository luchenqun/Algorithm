/**
* @defgroup sudoku.c
* @{ 
* @code 
Copyright (c) 2013, 广东小天才科技有限公司
All rights reserved.

文件名称： sudoku.c
文件标识： 数独的生成
文件摘要： 数独的生成
修改历史： 
版本    日期        作者     修改内容
------  ----------  -------  ----------------------------
V1.0    2013.08.02  卢郴群    程序开始编写
* @endcode
*/

#include<iostream>
using namespace std;

#define SQUARE_SLIDE	2	// 数独小正方形的长度(请修改这里的维度，以生成不同阶级的数独)
#define MAX_NUM	(SQUARE_SLIDE * SQUARE_SLIDE)	// 填到数独里面最大的数字
#define ROW_MAX	(SQUARE_SLIDE * SQUARE_SLIDE)	// 最大行
#define COL_MAX	(SQUARE_SLIDE * SQUARE_SLIDE)	// 最大列

int maze[ROW_MAX][COL_MAX];	// 要填的数独
int sudokuCnt;				// 可生成数独的个数

void initSudoku();
void printSudoku();
void createSudoku(int row, int col);
bool checkSudoku(int curRow, int curCol);

/**
* @brief  数独初始化
* @param [in] 无
* @return 无
* @par 修改历史：
* @code
姓名				日期				说明
--------		----------		--------------------
卢郴群			2013.08.08		程序开始编写
* @endcode
*/
void initSudoku()
{
	for (int row=0; row<ROW_MAX; ++row)
	{
		for (int col=0; col<COL_MAX; ++col)
		{
			maze[row][col] = 0;
		}
	}
}

/**
* @brief  打印数独
* @param [in] 无
* @return 无
* @par 修改历史：
* @code
姓名				日期				说明
--------		----------		--------------------
卢郴群			2013.08.08		程序开始编写
* @endcode
*/
void printSudoku()
{
	for (int row=0; row<ROW_MAX; ++row)
	{
		for (int col=0; col<COL_MAX; ++col)
		{
			cout << maze[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

/**
* @brief  检查数独
* @param [in] curRow 当前行
			  curCol 当前列
* @return false 不能构成数独
		  true 暂时符合数独要求
* @par 修改历史：
* @code
姓名				日期				说明
--------		----------		--------------------
卢郴群			2013.08.08		程序开始编写
* @endcode
*/
bool checkSudoku(int curRow, int curCol)
{
	bool ret = true;
	bool flag[SQUARE_SLIDE * SQUARE_SLIDE + 1];
	int elemCnt = SQUARE_SLIDE * SQUARE_SLIDE + 1;
	int row, col;
	int squRow, squCol;
	int checkRow, checkCol;

	memset(flag, false, elemCnt * sizeof(bool));

	// 检查行是否符合要求
	for (row=0; row<=curRow; ++row)
	{
		checkCol = (row == curRow) ? (curCol+1) : (COL_MAX);
		for (col=0; col<checkCol; ++col)
		{
			if (flag[maze[row][col]])
			{
				ret = false;
				goto endReturn;
			} 
			else
			{
				flag[maze[row][col]] = true;
			}
		}
		memset(flag, false, elemCnt * sizeof(bool));
	}

	// 检查列是否符合要求
	for (col=0; col<COL_MAX; ++col)
	{
		checkRow = (col <= curCol) ? (curRow+1) : (curRow);
		for (row=0; row<checkRow; ++row)
		{
			if (flag[maze[row][col]])
			{
				ret = false;
				goto endReturn;
			} 
			else
			{
				flag[maze[row][col]] = true;
			}
		}

		memset(flag, false, elemCnt * sizeof(bool));
	}

	// 检查小正方形
	for (row=0; row<ROW_MAX; row+=SQUARE_SLIDE)
	{
		for (col=0; col<COL_MAX; col+=SQUARE_SLIDE)
		{
			for (squRow=row; squRow<row+SQUARE_SLIDE; squRow++)
			{
				for (squCol=col; squCol<col+SQUARE_SLIDE; squCol++)
				{
					if (flag[maze[squRow][squCol]])
					{
						ret = false;
						goto endReturn;
					} 
					else
					{
						if(maze[squRow][squCol] > 0)
						{
							flag[maze[squRow][squCol]] = true;
						}
					}

					// 由于是从左到右，从上到下检查的，那么只要碰到当前的行跟列，那么以后的肯定就不要检查了（因为还没填）
					if (squRow == curRow && squCol == curCol)
					{
						goto endReturn;
					}
				}
			}
			memset(flag, false, elemCnt * sizeof(bool));
		}
	}

endReturn: 
	return ret;
}

/**
* @brief  生成数独（使用深度优先搜索，无任何剪枝）
* @param [in] row 当前行
			  col 当前列
* @return 无
* @par 修改历史：
* @code
姓名				日期				说明
--------		----------		--------------------
卢郴群			2013.08.08		程序开始编写
* @endcode
*/
void createSudoku(int row, int col)
{
	int nextRow, nextCol;

	if (row >= ROW_MAX)	// 深度优先搜索结束条件
	{
		if (checkSudoku(ROW_MAX-1, COL_MAX-1))// 符合要求的就给打印出来
		{
			printSudoku();
			sudokuCnt++;
		}
		return;
	}
	else
	{
		for(int num=1; num<=MAX_NUM; ++num)
		{
			maze[row][col] = num;
			
			if (checkSudoku(row, col))	// 符合要求
			{
				nextRow = (col==COL_MAX-1) ? (row+1) : (row);
				nextCol = (col==COL_MAX-1) ? (0) : (col+1);
				createSudoku(nextRow, nextCol);
			}

			maze[row][col] = 0;
		}
	}
}

// 4 * 4的数独有288种生成方法
// 9 * 9 的数独有6670903752021072936960种生成方法。所以9 * 9的数独该程序是无法生成所有解的。

int main()
{
	// freopen("out.txt", "w", stdout);

	sudokuCnt = 0;
	initSudoku();
	createSudoku(0, 0);
	cout << SQUARE_SLIDE << " × " << SQUARE_SLIDE <<"数独生成完毕，总共有 " << sudokuCnt << " 种生成方法" << endl;
	return 0;
}