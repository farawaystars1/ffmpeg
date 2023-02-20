#pragma once
#include"../ffmpeg.h"
class VideoScaling {
public:
    VideoScaling();
    ~VideoScaling();
    void convert(AVFrame* src_frame, int width, int height);//������Ƶ֡���������λ��src_des;width/heightΪĿ�����

    //sws_scale(sws_ctx, (const uint8_t* const*)src_data,
    //    src_linesize, 0, src_h, dst_data, dst_linesize);
    uint8_t** getDesData()const;
    int* getDesLinesize()const;
    int getDesWidth()const;
    int getDesHeight()const;
    int getDesSize()const;
private:
    void __init(AVFrame* src_frame, int width, int height);
    void __deInit();


private:
    //输出数据
    uint8_t* des_data[4];
    int des_linesize[4];
    int des_width;
    int des_height;
    int des_size;
private:
    SwsContext* c;

};