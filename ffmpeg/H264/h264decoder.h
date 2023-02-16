#pragma once
#ifndef H264DECODER_H
#define H264DECODER_H
extern"C" {
#include <libavcodec/avcodec.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
}
class H264Decoder {
public:

    H264Decoder()
    {
        __init();
    }
    ~H264Decoder()
    {
        __flushFrame();
        avcodec_free_context(&c);
        av_frame_free(&frame);
        av_packet_free(&packet);
    }
    void* decodeH264(char* pInBuf, int nInBufLen)
    {
        packet->data = (uint8_t*)pInBuf;
        packet->size = nInBufLen;
        __decode(packet, frame);
    }
private:
    void __init()
    {
        int ret;
        frame = av_frame_alloc();
        packet = av_packet_alloc();
        codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        c = avcodec_alloc_context3(codec);
        ret = avcodec_open2(c, codec, NULL);
    }
    void __decode(AVPacket* packet, AVFrame* frame)
    {

        int ret;
        ret = avcodec_send_packet(c, packet);
        if (ret < 0) {
            fprintf(stderr, "Error sending a packet for decoding\n");
            exit(1);
        }

        while (ret >= 0) {
            ret = avcodec_receive_frame(c, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                return;
            else if (ret < 0) {
                fprintf(stderr, "Error during decoding\n");
                exit(1);
            }
        }
    }
    void __flushFrame()
    {
        packet->data = NULL;
        packet->size = 0;
        __decode(packet, frame);
    }
private:
    AVFrame* frame;
    AVPacket* packet;
    AVCodec* codec;
    AVCodecContext* c;

};



#endif H264DECODER_H