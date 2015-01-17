#include <iostream>
#include <stdio.h> 
#include <windows.h> 
#include <string>
#include <io.h>

using namespace std;

#define FILE_EXIST	0

enum 
{
	CHECK_DISK_EXIST,	// ����ļ��Ƿ����
	CHECK_UPD_EXIST,	// ���UPD�ļ��Ƿ����
	WAIT_DISK_BUFF,		// �ȴ����̻���
	COPY_UPD_TO_DISK,	// ����UPD �ļ�
};

bool readWriteFile(const char *readPath, const char *writePath)
{
	const int MAXLEN =1024 * 100;
	FILE * outfile, *infile;
	int infileSize = 0;	// �����ļ��Ĵ�С
	int writeCnt = 0;	// д�Ĵ���
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
			cout << "�ļ��ɹ�����" << int((percentage > 100.0) ? (100) : (percentage)) << "%" << endl;
		}
	}

	if(writeCnt < (infileSize / MAXLEN))
	{
		cout << endl << "����������ڸ����ļ��Ĺ����аγ���USB�����ߣ������¸��ơ������" << endl << endl;
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

	string source = "E:\\R1\\Code\\DownEnv\\desktop\\desktop.upd";	// ����ԭʼ���ļ�
	//string source = "F:\\R1_Project\\R1\\Code\\DownEnv\\desktop\\desktop.upd";
	string des = "I:\\desktop.upd";									// ������Ҫ���Ƶĵط����ļ�
	string disk = des.substr(0, 2);
	string dosCmd = "copy " + source + " " + disk;	// dosָ��

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
			case CHECK_DISK_EXIST:// ���R1�Ƿ����ӵ���
				if(access(disk.c_str(), FILE_EXIST) != -1)
				{
					if(timerCnt > 10)
					{
						system("cls");
						cout << "���ǰ�����ʾ��Ϣ" << endl << endl;
					}
					cout  << "����һ��" << disk << " ����" << "����" << endl;
					state = CHECK_UPD_EXIST;
				}
				else
				{
					if (0 == timerCnt % 10)
					{
						cout << (timerCnt/10) << " ϵͳ�ȴ������R1���̣��������ֶ��رճ���" << endl;
					}
				}
				break;
			case CHECK_UPD_EXIST:// ���ԭ�ļ��Ƿ����
				if(access(source.c_str(), FILE_EXIST) != -1)
				{
					state = (timerCnt <= 3) ? (COPY_UPD_TO_DISK) : (WAIT_DISK_BUFF);
					timerCnt = 0;
					cout << "�������" << source << " ԭ�ļ��ļ�����" << endl;
				}
				else
				{
					cout << source << "ԭ�ļ������ڣ�����������" << endl;
					error = true;
				}
				break;
			case WAIT_DISK_BUFF:// ���д��̻���
				if (1 == timerCnt % 5)
				{
					cout << "    �ȴ����̻���ing" << endl;
				}
				if (20 == timerCnt)
				{
					cout << "    ���̻������ed" << endl;
					state = COPY_UPD_TO_DISK;
				}
				break;
			case COPY_UPD_TO_DISK:
				cout << "����������ʼ�����ļ�" << endl << endl;
				// system(dosCmd.c_str());
				if (readWriteFile(source.c_str(), des.c_str()))
				{
					cout << "������﹧ϲ�㣬�ļ����Ƴɹ��������" << endl;
					finish = true;
					tryCopyCnt = 0;
				} 
				else
				{
					cout << "�ļ�����ʧ�ܣ�ϵͳ���¿�ʼִ�и��Ʋ���" << endl;
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

//	cout << "������ʱ�����������Ƿ���ʾ�Ƿ�ɹ�" << endl;

	Sleep(500);

	return 0;
}




