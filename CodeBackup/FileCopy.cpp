#include <iostream>
#include <stdio.h> 
#include <windows.h> 
#include <string>
#include <io.h>

using namespace std;

#define FILE_EXIST	0

enum 
{
	CHECK_DISK_EXIST,	// 检查文件是否存在
	CHECK_UPD_EXIST,	// 检查UPD文件是否存在
	WAIT_DISK_BUFF,		// 等待磁盘缓冲
	COPY_UPD_TO_DISK,	// 复制UPD 文件
};

bool readWriteFile(const char *readPath, const char *writePath)
{
	const int MAXLEN =1024 * 100;
	FILE * outfile, *infile;
	int infileSize = 0;	// 传入文件的大小
	int writeCnt = 0;	// 写的次数
	double percentage;

	infile = fopen(readPath, "rb");
	outfile = fopen(writePath, "wb" );

	unsigned char buf[MAXLEN];

	if( outfile == NULL || infile == NULL )
	{
		return false;
	}
	else
	{
		infileSize = filelength(fileno(infile));
	}

	int rc;
	int wc = 1;
	while((rc = fread(buf, sizeof(unsigned char), MAXLEN, infile)) != 0 && wc)
	{
		wc = fwrite( buf, sizeof(unsigned char), rc, outfile);
		writeCnt++;
		percentage = writeCnt * MAXLEN * 100.0 / infileSize;
		if (0 == writeCnt % (infileSize / MAXLEN / 5) || 1 == writeCnt || percentage >= 100.0)
		{
			cout << "文件成功复制" << int((percentage > 100.0) ? (100) : (percentage)) << "%" << endl;
		}
	}

	if(writeCnt < (infileSize / MAXLEN))
	{
		cout << endl << "★★★★★★你在复制文件的过程中拔出了USB连接线，请重新复制★★★★★★" << endl << endl;
		return false;
	}

	cout << endl;

	fclose(infile);
	fclose(outfile);

	return true;
}

int main()
{
	int state = CHECK_DISK_EXIST;
	int len = 0;
	bool finish = false;
	bool error = false;

	string source = "E:\\R1\\Code\\DownEnv\\desktop\\desktop.upd";	// 设置原始的文件
	//string source = "F:\\R1_Project\\R1\\Code\\DownEnv\\desktop\\desktop.upd";
	string des = "I:\\desktop.upd";									// 设置需要复制的地方的文件
	string disk = des.substr(0, 2);
	string dosCmd = "copy " + source + " " + disk;	// dos指令

	int tryCopyCnt = 0;
	int timerID = 1; 
	int	timerCnt = 0;
	HWND   hwndTimer;       
	MSG   msg;

	SetTimer(NULL, timerID, 100, NULL); 

	while ((!finish) && (!error) && GetMessage(&msg, NULL,NULL,NULL))
	{   
		if(msg.message == WM_TIMER)   
		{   
			//cout << "I have receive a timer msg" << endl;
			//cout << state << endl;
			timerCnt++;
			switch(state)
			{
			case CHECK_DISK_EXIST:// 检测R1是否连接电脑
				if(access(disk.c_str(), FILE_EXIST) != -1)
				{
					if(timerCnt > 10)
					{
						system("cls");
						cout << "清掉前面的提示信息" << endl << endl;
					}
					cout  << "步骤一、" << disk << " 磁盘" << "存在" << endl;
					state = CHECK_UPD_EXIST;
				}
				else
				{
					if (0 == timerCnt % 10)
					{
						cout << (timerCnt/10) << " 系统等待你插入R1磁盘，或者你手动关闭程序" << endl;
					}
				}
				break;
			case CHECK_UPD_EXIST:// 检测原文件是否存在
				if(access(source.c_str(), FILE_EXIST) != -1)
				{
					state = (timerCnt <= 3) ? (COPY_UPD_TO_DISK) : (WAIT_DISK_BUFF);
					timerCnt = 0;
					cout << "步骤二、" << source << " 原文件文件存在" << endl;
				}
				else
				{
					cout << source << "原文件不存在！！！！！！" << endl;
					error = true;
				}
				break;
			case WAIT_DISK_BUFF:// 进行磁盘缓存
				if (1 == timerCnt % 5)
				{
					cout << "    等待磁盘缓冲ing" << endl;
				}
				if (20 == timerCnt)
				{
					cout << "    磁盘缓冲完毕ed" << endl;
					state = COPY_UPD_TO_DISK;
				}
				break;
			case COPY_UPD_TO_DISK:
				cout << "步骤三、开始复制文件" << endl << endl;
				// system(dosCmd.c_str());
				if (readWriteFile(source.c_str(), des.c_str()))
				{
					cout << "★★★★★★恭喜你，文件复制成功★★★★★★" << endl;
					finish = true;
					tryCopyCnt = 0;
				} 
				else
				{
					cout << "文件复制失败，系统重新开始执行复制步骤" << endl;
					state = CHECK_DISK_EXIST;
					timerCnt = 0;
				}
				break;
			default:
				break;
			}

			TranslateMessage(&msg);   
			DispatchMessage(&msg);     
		}   
	}

//	cout << "给个延时，看看上面是否提示是否成功" << endl;

	Sleep(500);

	return 0;
}




