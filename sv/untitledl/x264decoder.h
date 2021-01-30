#ifndef X264DECODER_H
#define X264DECODER_H

#ifdef __cplusplus
#define __STDINT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <iostream>
#include <stdint.h>
#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
}


class x264Decoder{

public:

    x264Decoder(int w = 640, int h = 480);
    ~x264Decoder()
    {
        if(convert_context_)
        {
            sws_freeContext(convert_context_);
            av_frame_free(&av_frame_);
            av_frame_free(&av_frame_RGB_);
            av_free(av_frame_);
            av_free(av_frame_RGB_);
            avcodec_close(av_codec_context_);
        }


        convert_context_ = 0;
        av_frame_ = 0;
        av_frame_RGB_ = 0;
        av_codec_context_ = 0;
        rgb_size_ = 0;
        image_w_ = 0;
        image_h_ = 0;
        rgb_size_ = 0;
    }

    unsigned char* decodeFrame(const char *src_buffer, unsigned int size);

    int image_w_;
    int image_h_;

private:
    AVFrame* av_frame_;
    AVFrame *av_frame_RGB_;

    // Use this context to convert your BGR Image to YUV image since x264 do not support RGB input
    AVCodec* av_codec_ = NULL;
    AVCodecContext* av_codec_context_ = NULL;
    SwsContext* convert_context_;

    int rgb_size_;
};


#endif // X264ENCODER_H
