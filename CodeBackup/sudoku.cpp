/**
* @defgroup sudoku.c
* @{ 
* @code 
Copyright (c) 2013, �㶫С��ſƼ����޹�˾
All rights reserved.

�ļ����ƣ� sudoku.c
�ļ���ʶ�� ����������
�ļ�ժҪ�� ����������
�޸���ʷ�� 
�汾    ����        ����     �޸�����
------  ----------  -------  ----------------------------
V1.0    2013.08.02  ¬��Ⱥ    ����ʼ��д
* @endcode
*/

#include<iostream>
using namespace std;

#define SQUARE_SLIDE	2	// ����С�����εĳ���(���޸������ά�ȣ������ɲ�ͬ�׼�������)
#define MAX_NUM	(SQUARE_SLIDE * SQUARE_SLIDE)	// �����������������
#define ROW_MAX	(SQUARE_SLIDE * SQUARE_SLIDE)	// �����
#define COL_MAX	(SQUARE_SLIDE * SQUARE_SLIDE)	// �����

int maze[ROW_MAX][COL_MAX];	// Ҫ�������
int sudokuCnt;				// �����������ĸ���

void initSudoku();
void printSudoku();
void createSudoku(int row, int col);
bool checkSudoku(int curRow, int curCol);

/**
* @brief  ������ʼ��
* @param [in] ��
* @return ��
* @par �޸���ʷ��
* @code
����				����				˵��
--------		----------		--------------------
¬��Ⱥ			2013.08.08		����ʼ��д
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
* @brief  ��ӡ����
* @param [in] ��
* @return ��
* @par �޸���ʷ��
* @code
����				����				˵��
--------		----------		--------------------
¬��Ⱥ			2013.08.08		����ʼ��д
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
* @brief  �������
* @param [in] curRow ��ǰ��
			  curCol ��ǰ��
* @return false ���ܹ�������
		  true ��ʱ��������Ҫ��
* @par �޸���ʷ��
* @code
����				����				˵��
--------		----------		--------------------
¬��Ⱥ			2013.08.08		����ʼ��д
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

	// ������Ƿ����Ҫ��
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

	// ������Ƿ����Ҫ��
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

	// ���С������
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

					// �����Ǵ����ң����ϵ��¼��ģ���ôֻҪ������ǰ���и��У���ô�Ժ�Ŀ϶��Ͳ�Ҫ����ˣ���Ϊ��û�
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
* @brief  ����������ʹ������������������κμ�֦��
* @param [in] row ��ǰ��
			  col ��ǰ��
* @return ��
* @par �޸���ʷ��
* @code
����				����				˵��
--------		----------		--------------------
¬��Ⱥ			2013.08.08		����ʼ��д
* @endcode
*/
void createSudoku(int row, int col)
{
	int nextRow, nextCol;

	if (row >= ROW_MAX)	// �������������������
	{
		if (checkSudoku(ROW_MAX-1, COL_MAX-1))// ����Ҫ��ľ͸���ӡ����
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
			
			if (checkSudoku(row, col))	// ����Ҫ��
			{
				nextRow = (col==COL_MAX-1) ? (row+1) : (row);
				nextCol = (col==COL_MAX-1) ? (0) : (col+1);
				createSudoku(nextRow, nextCol);
			}

			maze[row][col] = 0;
		}
	}
}

// 4 * 4��������288�����ɷ���
// 9 * 9 ��������6670903752021072936960�����ɷ���������9 * 9�������ó������޷��������н�ġ�

int main()
{
	// freopen("out.txt", "w", stdout);

	sudokuCnt = 0;
	initSudoku();
	createSudoku(0, 0);
	cout << SQUARE_SLIDE << " �� " << SQUARE_SLIDE <<"����������ϣ��ܹ��� " << sudokuCnt << " �����ɷ���" << endl;
	return 0;
}