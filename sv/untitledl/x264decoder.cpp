#include "x264decoder.h"


#pragma comment(lib, "../lib/avcodec.lib")
#pragma comment(lib, "../lib/avdevice.lib")
#pragma comment(lib, "../lib/avfilter.lib")
#pragma comment(lib, "../lib/avformat.lib")
#pragma comment(lib, "../lib/avutil.lib")
#pragma comment(lib, "../lib/postproc.lib")
#pragma comment(lib, "../lib/swresample.lib")
#pragma comment(lib, "../lib/swscale.lib")


static AVFrame * icv_alloc_picture_FFMPEG(int pix_fmt, int width, int height, bool alloc)
{
    AVFrame * picture;
    uint8_t * picture_buf;
    int size;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;
    size = avpicture_get_size( (AVPixelFormat) pix_fmt, width, height);
    if(alloc) 
    {
        picture_buf = (uint8_t *) malloc(size);
        if (!picture_buf)
        {
            av_frame_free (&picture);
            std::cout << "picture buff = NULL" << std::endl;
            return NULL;
        }
        avpicture_fill((AVPicture *)picture, picture_buf,  (AVPixelFormat) pix_fmt, width, height);
    }
    return picture;
}


x264Decoder::x264Decoder(int w, int h)
{     
    avcodec_register_all();  
    image_h_ = h;
    image_w_ = w;

    rgb_size_ = avpicture_get_size(AV_PIX_FMT_RGB32, image_w_, image_h_);
    av_codec_ = avcodec_find_decoder(AV_CODEC_ID_H264);
    av_codec_context_ = avcodec_alloc_context3(av_codec_);  
    av_codec_context_->width = image_w_;
    av_codec_context_->height = image_h_;
    av_codec_context_->extradata = NULL;
    av_codec_context_->pix_fmt = AV_PIX_FMT_YUV420P;
    avcodec_open2(av_codec_context_, av_codec_, NULL);

    convert_context_ = sws_getContext(image_w_, image_h_, AV_PIX_FMT_YUV420P, image_w_, image_h_, AV_PIX_FMT_RGB32, SWS_FAST_BILINEAR, NULL, NULL, NULL);
    
    av_frame_ = icv_alloc_picture_FFMPEG(AV_PIX_FMT_YUV420P, image_w_, image_h_, true);
    av_frame_RGB_ = icv_alloc_picture_FFMPEG(AV_PIX_FMT_RGB32, image_w_, image_h_, true);


}

unsigned char* x264Decoder::decodeFrame(const char *src_buffer, unsigned int size)
{
    AVPacket av_packet;
    av_new_packet(&av_packet, size);
    av_packet.data = (uint8_t *)src_buffer;
    av_packet.size = size;

    int frame_finished = 0;

    av_frame_unref(av_frame_);
    int av_return = avcodec_decode_video2(av_codec_context_, av_frame_, &frame_finished, &av_packet );
    av_free_packet(&av_packet);

    if(av_return <= 0 || !frame_finished)
        return 0;

    //Convert the frame from YUV420 to RGB24
    sws_scale(convert_context_, av_frame_->data, av_frame_->linesize, 0, image_h_, av_frame_RGB_->data, av_frame_RGB_->linesize);   



    //Manadatory function to copy the image form an AVFrame to a generic buffer.

    int retbuf_size = avpicture_get_size(AV_PIX_FMT_RGB32, image_w_, image_h_)+1;
    unsigned char *retbuf = new unsigned char[retbuf_size*10];
    memset(retbuf, 0, retbuf_size);

    avpicture_layout((AVPicture *)av_frame_RGB_, AV_PIX_FMT_RGB32, image_w_, image_h_, retbuf, retbuf_size);

    return retbuf;
}

