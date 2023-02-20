#pragma once
#ifndef H264DECODER_H
#define H264DECODER_H
#include"../ffmpeg.h"
#include<iostream>
#include"VideoScale.h"
class H264Decoder {
public:

    H264Decoder();
    ~H264Decoder();
    void decodeH264(uint8_t* pInBuf, int nInBufLen);
private:
    void __init();
    void __decode(AVPacket* packet, AVFrame* frame);
    void __flushFrame();
private:
    AVFrame* frame;
    AVPacket* packet;
    AVCodec* codec;
    AVCodecContext* c;
    VideoScaling *scale;

};



#endif //H264DECODER_H