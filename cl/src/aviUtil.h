


#include <stdint.h>
#include "x264\x264.h"
#include "leynet.h"

x264_t* x264encoder = 0;
x264_picture_t *x264_pic_in = 0;
x264_picture_t *x264_pic_out = 0;

unsigned int count = 0;



// Initialization... 
bool START_ENCODING(char*preset, char*line, int width, int height, int fps)
{
	count = 0;
	DeleteFile("test.h264");

	x264_param_t param;
	if (x264_param_default_preset(&param, preset, "zerolatency"))
		return false;

	param.i_width = width;
	param.i_height = height;

	
	param.i_threads = 0;
	param.rc.i_vbv_buffer_size = 2000000;
	param.rc.i_vbv_max_bitrate = 90000;
	param.i_fps_num = fps;
	param.i_fps_den = 1;
	// Intra refres:
	param.i_keyint_max = fps;
	param.b_intra_refresh = 1;
	//Rate control:
	param.rc.i_rc_method = X264_RC_CRF;
	param.rc.f_rf_constant = 28.f;
	//For streaming:
	param.b_repeat_headers = 1;
	param.b_annexb = 1;

	x264_param_apply_profile(&param, line);

	x264encoder  = x264_encoder_open(&param);

	if (!x264encoder)
		return false;

	if (!x264_pic_in)
	{
		x264_pic_in = new x264_picture_t;
		x264_pic_out = new x264_picture_t;

		x264_picture_alloc(x264_pic_in, X264_CSP_I420, width, height);
	}

	return true;
}

//thanks to stackoverflow for rgb2yuv

void Bitmap2Yuv420p_calc2(uint8_t *destination, uint8_t *rgb, size_t width, size_t height)
{
	size_t image_size = width * height;
	size_t upos = image_size;
	size_t vpos = upos + upos / 4;
	size_t i = 0;

	for (size_t line = 0; line < height; ++line)
	{
		if (!(line % 2))
		{
			for (size_t x = 0; x < width; x += 2)
			{
				uint8_t r = rgb[3 * i];
				uint8_t g = rgb[3 * i + 1];
				uint8_t b = rgb[3 * i + 2];

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;

				destination[upos++] = ((-38 * r + -74 * g + 112 * b) >> 8) + 128;
				destination[vpos++] = ((112 * r + -94 * g + -18 * b) >> 8) + 128;

				r = rgb[3 * i];
				g = rgb[3 * i + 1];
				b = rgb[3 * i + 2];

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
			}
		}
		else
		{
			for (size_t x = 0; x < width; x += 1)
			{
				uint8_t r = rgb[3 * i];
				uint8_t g = rgb[3 * i + 1];
				uint8_t b = rgb[3 * i + 2];

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
			}
		}
	}
}

int fillImage(uint8_t* buffer, int width, int height, x264_picture_t*pic)
{
	pic->img.i_plane = 3; // Y, U and V
	pic->img.i_stride[0] = width;
	// U and V planes are half the size of Y plane
	pic->img.i_stride[1] = width / 2;
	pic->img.i_stride[2] = width / 2;
	int uvsize = ((width + 1) >> 1) * ((height + 1) >> 1);
	pic->img.plane[0] = buffer; // Y Plane pointer
	pic->img.plane[1] = buffer + (width * height); // U Plane pointer
	pic->img.plane[2] = pic->img.plane[1] + uvsize; // V Plane pointer
	return 1;
}


bool ADD_FRAME(leynet_tcp *tcp, void* vbitmap, unsigned int width, unsigned int height, unsigned int imgsize, int fps, int quality)
{
	uint8_t *img = (uint8_t*)(vbitmap);
	uint8_t *converted = new uint8_t[imgsize];


	printf("SIZE: %lu  _ %x\n", imgsize, imgsize);

	Bitmap2Yuv420p_calc2(converted, img, width, height);

	x264_pic_in->i_type = X264_TYPE_AUTO;  //frame type, let x264 decide
	x264_pic_in->i_qpplus1 = 0;  //frame qp adjustment
	x264_pic_in->i_pts = count;//framenum
	x264_pic_in->img.i_csp = X264_CSP_I420;

	fillImage(converted, width, height, x264_pic_in);

	x264_nal_t* nals;
	int i_nal = 0;
	int frame_size = x264_encoder_encode(x264encoder, &nals, &i_nal, x264_pic_in, x264_pic_out);
	
	delete converted;

	if (frame_size > 0)
	{
		printf("encoded :)");
	}
	else {
		printf("RIP ENCODING");

		return false;
	}

	for (int i = 0; i < i_nal; i++)
	{
		if (tcp->Send((const char*)nals[i].p_payload, nals[i].i_payload))
			return true;
	}
	//if (tcp->Send((const char*)nals[0].p_payload, frame_size))
	//	return true;

	count++;
	return false;
}

bool END_ENCODING()
{
	count = 0;
	x264_encoder_close(x264encoder);
	return true;
} 

