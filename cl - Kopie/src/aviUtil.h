

#define AVIIF_KEYFRAME  0x00000010L // this frame is a key frame.


#include <memory.h>
#include <mmsystem.h>
#include <vfw.h>
#include <stdint.h>
#include "x264\x264.h"

x264_t* x264encoder = 0;

BOOL AVI_Init()
{
        /* first let's make sure we are running on 1.1 */
        WORD wVer = HIWORD(VideoForWindowsVersion());
		AVIFileInit();

        return TRUE;
}

BOOL AVI_FileOpenWrite(PAVIFILE * pfile, char *filename)
{
        HRESULT hr = AVIFileOpen(pfile,           // returned file pointer
                       filename,                  // file name
                       OF_WRITE | OF_CREATE,      // mode to open file with
                       NULL);                     // use handler determined
                                                  // from file extension....
        if (hr != AVIERR_OK)
                return FALSE;

        return TRUE;
}

DWORD getFOURCC(char* value)
{
	return mmioFOURCC(value[0],value[1],value[2],value[3]);
}

// Fill in the header for the video stream....
// The video stream will run in rate this  per second....
BOOL AVI_CreateVideoStream(PAVIFILE pfile, PAVISTREAM * ps, int rate, unsigned long buffersize, int rectwidth, int rectheight, char* _compressor, int quality)
{
	//strhdr.fccHandler             = mmioFOURCC('D','I','B',' '); // Uncompressed
	//strhdr.fccHandler             = mmioFOURCC('C','V','I','D'); // Cinpak
	//strhdr.fccHandler             = mmioFOURCC('I','V','3','2'); // Intel video 3.2
	//strhdr.fccHandler             = mmioFOURCC('M','S','V','C'); // Microsoft video 1
	//strhdr.fccHandler             = mmioFOURCC('I','V','5','0'); // Intel video 5.0

		AVISTREAMINFO strhdr;
		memset(&strhdr, 0, sizeof(strhdr));
        strhdr.fccType                = streamtypeVIDEO;// stream type
		strhdr.fccHandler             = getFOURCC(_compressor);
        strhdr.dwScale                = 1;
        strhdr.dwRate                 = rate;               // rate fps
        strhdr.dwSuggestedBufferSize  = buffersize;
		strhdr.dwQuality              = quality; // use the default
		//strhdr.dwSampleSize           = 
        SetRect(&strhdr.rcFrame, 0, 0, (int) rectwidth, (int) rectheight);

        // And create the stream;
        HRESULT hr = AVIFileCreateStream(pfile, ps, &strhdr);
        if (hr != AVIERR_OK) {
                return FALSE;
        }

        return TRUE;
}

void getFOURCCVAsString(char*buffer, DWORD value)
{
	char tmpbuf[500];
	memset(tmpbuf, 500, 0);
	unsigned int cur = 0;

	DWORD ch0 = value & 0x000000FF;
	tmpbuf[cur++] = (char)ch0;
	DWORD ch1 = (value & 0x0000FF00)>>8;
	tmpbuf[cur++] = (char)ch1;
	DWORD ch2 = (value & 0x00FF0000)>>16;
	tmpbuf[cur++] = (char)ch2;
	DWORD ch3 = (value & 0xFF000000)>>24;
	tmpbuf[cur++] = (char)ch3;

	strcpy(buffer, tmpbuf);

}

bool dumpAVICOMPRESSOPTIONS(char*buffer, AVICOMPRESSOPTIONS opts)
{
	char tmp[255];
	char getfourcc[255];

	strcat(buffer, "Dump of AVICOMPRESSOPTIONS\n");


	memset(tmp, 0, 255);
	memset(getfourcc, 0, 255);
	getFOURCCVAsString(getfourcc, opts.fccType);
	sprintf(tmp, "DWORD  fccType = streamtype(%s)\n", getfourcc);
	strcat(buffer, tmp);
 

	memset(tmp, 0, 255);
	memset(getfourcc, 0, 255);
	getFOURCCVAsString(getfourcc, opts.fccHandler);
	sprintf(tmp, "DWORD  fccHandler = %s\n", getfourcc);
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "DWORD  dwKeyFrameEvery = %d\n", opts.dwKeyFrameEvery);
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "DWORD  dwQuality = %d\n", opts.dwQuality);
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "DWORD  dwBytesPerSecond = %d\n", opts.dwBytesPerSecond);
	strcat(buffer, tmp);

	if((opts.dwFlags & AVICOMPRESSF_DATARATE)== AVICOMPRESSF_DATARATE) { strcat(buffer, "DWORD  fccType = AVICOMPRESSF_DATARATE\n");}
	else if((opts.dwFlags & AVICOMPRESSF_INTERLEAVE) == AVICOMPRESSF_INTERLEAVE){ strcat(buffer, "DWORD  fccType = AVICOMPRESSF_INTERLEAVE\n");}
	else if((opts.dwFlags & AVICOMPRESSF_KEYFRAMES) == AVICOMPRESSF_KEYFRAMES){ strcat(buffer, "DWORD  fccType = AVICOMPRESSF_KEYFRAMES\n");}
	else if((opts.dwFlags & AVICOMPRESSF_VALID) == AVICOMPRESSF_VALID){ strcat(buffer, "DWORD  fccType = AVICOMPRESSF_VALID\n");}
	else { memset(tmp, 0, 255); sprintf(tmp, "DWORD  dwFlags = Unknown(%d)\n", opts.dwFlags);}
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "LPVOID lpFormat = %d\n", (int)opts.lpFormat);
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "DWORD  cbFormat = %d\n", (int)opts.cbFormat);
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "LPVOID lpParms = %d\n", (int)opts.lpParms);
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "DWORD  cbParms = %d\n", (int)opts.cbParms);
	strcat(buffer, tmp);

	memset(tmp, 0, 255);
	sprintf(tmp, "DWORD  dwInterleaveEvery = %d\n", opts.dwInterleaveEvery);
	strcat(buffer, tmp);

	return true;
}

BOOL AVI_SetOptions(PAVISTREAM * ps, PAVISTREAM * psCompressed, LPBITMAPINFOHEADER lpbi, char* _compressor)
{ 
        AVICOMPRESSOPTIONS opts;
        AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};

		memset(&opts, 0, sizeof(opts));
		opts.fccType = streamtypeVIDEO;
		opts.fccHandler             = getFOURCC(_compressor);
		//opts.fccHandler  = 0;
		//opts.fccHandler            = mmioFOURCC('D','I','B',' '); // Uncompressed
		//opts.fccHandler             = mmioFOURCC('C','V','I','D'); // Cinpak
		//opts.fccHandler             = mmioFOURCC('I','V','3','2'); // Intel video 3.2
		//opts.fccHandler             = mmioFOURCC('M','S','V','C'); // Microsoft video 1
		//opts.fccHandler             = mmioFOURCC('I','V','5','0'); // Intel video 5.0
		//opts.dwKeyFrameEvery = 5;
		//opts.dwQuality
		//opts.dwBytesPerSecond
		//opts.dwFlags                = AVICOMPRESSF_KEYFRAMES;
		//opts.lpFormat 
		//opts.cbFormat
		//opts.lpParms
		//opts.cbParms 
		//opts.dwInterleaveEvery

		/* display the compression options dialog box if specified compressor is unknown */
		if(getFOURCC(_compressor) == NULL)
		{
			if (!AVISaveOptions(NULL, 0, 1, ps, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
			{
				return FALSE;
			}

			//printf("%s", dumpAVICOMPRESSOPTIONS(opts));
			char*buffer = new char[0xFFFF];
			memset(buffer, 0, 0xFFFF);
			dumpAVICOMPRESSOPTIONS(buffer, opts);

			MessageBox(NULL, buffer, "AVICOMPRESSOPTIONS", MB_OK);
			delete[] buffer;

		}		

        HRESULT hr = AVIMakeCompressedStream(psCompressed, *ps, &opts, NULL);
        if (hr != AVIERR_OK)
		{
			return FALSE;
        }

        hr = AVIStreamSetFormat(*psCompressed, 0,
                               lpbi,                    // stream format
                               lpbi->biSize             // format size
                                   + lpbi->biClrUsed * sizeof(RGBQUAD)
                                   );
        if (hr != AVIERR_OK)
		{
			return FALSE;
        }

        return TRUE;
}

BOOL AVI_SetText(PAVIFILE pfile, PAVISTREAM psText, char *szText, int width, int height, int TextHeight)
{
        // Fill in the stream header for the text stream....
        AVISTREAMINFO strhdr;
        DWORD dwTextFormat;
        // The text stream is in 60ths of a second....

		memset(&strhdr, 0, sizeof(strhdr));
        strhdr.fccType                = streamtypeTEXT;
        strhdr.fccHandler             = mmioFOURCC('D', 'R', 'A', 'W');
        strhdr.dwScale                = 1;
        strhdr.dwRate                 = 60;
        strhdr.dwSuggestedBufferSize  = sizeof(szText);
        SetRect(&strhdr.rcFrame, 0, (int) height,
            (int) width, (int) height + TextHeight); // #define TEXT_HEIGHT 20

        // ....and create the stream.
        HRESULT hr = AVIFileCreateStream(pfile, &psText, &strhdr);
        if (hr != AVIERR_OK) {
                return FALSE;
        }

        dwTextFormat = sizeof(dwTextFormat);
        hr = AVIStreamSetFormat(psText, 0, &dwTextFormat, sizeof(dwTextFormat));
        if (hr != AVIERR_OK) {
                return FALSE;
        }

        return TRUE;
}

BOOL AVI_AddFrame(PAVISTREAM psCompressed, int time, LPBITMAPINFOHEADER lpbi)
{
	int ImageSize = lpbi->biSizeImage;
	if (ImageSize == 0)
	{
		if (lpbi->biBitCount == 24)
		{
			ImageSize = lpbi->biWidth * lpbi->biHeight * 3;
		}
	}
	//data is a pointer to you RGB structure
	int srcstride = lpbi->biWidth * 3; //RGB stride is just 3*width
	//sws_scale(convertCtx, &data, &srcstride, 0, lpbi->biHeight, pic_in.img.plane, pic_in.img.stride);
	x264_nal_t* nals;
	int i_nals;

	x264_picture_t pic_in, pic_out;
	x264_picture_alloc(&pic_in, X264_CSP_I420, lpbi->biWidth, lpbi->biHeight);

	int width = lpbi->biWidth;
	int height = lpbi->biHeight;

	memcpy(pic_in.img.plane[0], lpbi+sizeof(LPBITMAPINFOHEADER), width*height);
	memcpy(pic_in.img.plane[1], lpbi + sizeof(LPBITMAPINFOHEADER) + width*height - 1, width*height / 2);

	int frame_size = x264_encoder_encode(x264encoder, &nals, &i_nals, &pic_in, &pic_out);
	if (frame_size >= 0)
	{
		printf("ENCODED :DDD\n");
	}
	else {
		printf("FUCKING E-BIN\n");
	}

	HRESULT hr = AVIStreamWrite(psCompressed, // stream pointer
		time, // time of this frame
		1, // number to write
		(LPBYTE) lpbi + // pointer to data
		lpbi->biSize +
		lpbi->biClrUsed * sizeof(RGBQUAD),
		ImageSize, // lpbi->biSizeImage, // size of this frame
		AVIIF_KEYFRAME, // flags....
		NULL,
		NULL);
	if (hr != AVIERR_OK)
	{
		char errmsg[255];
		sprintf(errmsg, "Error: AVIStreamWrite, error %d",hr);
		MessageBoxA(NULL, errmsg, "avi error!", MB_OK);

		return FALSE;
	}
	
	return TRUE;
}

BOOL AVI_AddText(PAVISTREAM psText, int time, char *szText)
{
        int iLen = strlen(szText);

        HRESULT hr = AVIStreamWrite(psText,
                        time,
                        1,
                        szText,
                        iLen + 1,
                        AVIIF_KEYFRAME,
                        NULL,
                        NULL);
        if (hr != AVIERR_OK)
                return FALSE;

        return TRUE;
}

BOOL AVI_CloseStream(PAVISTREAM ps, PAVISTREAM psCompressed, PAVISTREAM psText)
{
        if (ps)
                AVIStreamClose(ps);

        if (psCompressed)
                AVIStreamClose(psCompressed);

        if (psText)
                AVIStreamClose(psText);



        return TRUE;
}

BOOL AVI_CloseFile(PAVIFILE pfile)
{
        if (pfile)
                AVIFileClose(pfile);
        
        return TRUE;
}

BOOL AVI_Exit()
{
        AVIFileExit();

        return TRUE;
}

/* Here are the additional functions we need! */


PAVIFILE pfile = NULL; 
PAVISTREAM ps = NULL;
PAVISTREAM psCompressed = NULL; 
int count = 0;


// Initialization... 
bool START_AVI(char* file_name, int width, int height, int fps)
{
    if(! AVI_Init())
	{
		//printf("Error - AVI_Init()\n");
		return false;
	}

    if(! AVI_FileOpenWrite(&pfile, file_name))
	{
		//printf("Error - AVI_FileOpenWrite()\n");
		return false;
	}

	x264_param_t param;
	x264_param_default_preset(&param, "veryfast", "zerolatency");
	param.i_threads = 1;
	param.i_width = width;
	param.i_height = height;
	param.i_fps_num = fps;
	param.i_fps_den = 1;
	// Intra refres:
	param.i_keyint_max = fps;
	param.b_intra_refresh = 1;
	//Rate control:
	param.rc.i_rc_method = X264_RC_CRF;
	param.rc.f_rf_constant = 25;
	param.rc.f_rf_constant_max = 35;
	//For streaming:
	param.b_repeat_headers = 1;
	param.b_annexb = 1;
	x264_param_apply_profile(&param, "baseline");

	if (!x264encoder)
	{
		x264encoder = x264_encoder_open(&param);
	}
	//struct SwsContext* convertCtx = sws_getContext(in_w, in_h, PIX_FMT_RGB24, out_w, out_h, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

	return true;
}
  
//Now we can add frames
// ie. ADD_FRAME_FROM_DIB_TO_AVI(yourDIB, "CVID", 25);
bool ADD_FRAME_FROM_DIB_TO_AVI(HANDLE dib, char* _compressor, int _frameRate, int quality)
{
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)dib;

	if(count == 0)
	{


		if(!AVI_CreateVideoStream(pfile, &ps, _frameRate, (unsigned long) lpbi->biSizeImage,  (int) lpbi->biWidth,  (int) lpbi->biHeight, _compressor, quality))
		{
			//printf("Error - AVI_CreateStream()\n");
			return false;
		} 



		if(! AVI_SetOptions(&ps, &psCompressed, lpbi, _compressor))
		{
			//printf("Error - AVI_SetOptions()\n");
			return false;
		}

	}


	if(! AVI_AddFrame(psCompressed, count * 1, lpbi))
	{
		//printf("Error - AVI_AddFrame()\n");
		return false;
	}

	count++;
	return true;
}

// The end... 
bool STOP_AVI()
{
     if(! AVI_CloseStream(ps, psCompressed, NULL))
	 {
		 //printf("Error - AVI_CloseStream()\n");
		 return false;
	 }

     if(! AVI_CloseFile(pfile))
	 {
		//printf("Error - AVI_CloseFile()\n");
		return false;
	 }
	 
     if(! AVI_Exit())
	 {
		//printf("Error - AVI_Exit()\n");
		return false;
	 }

	 return true;
} 

