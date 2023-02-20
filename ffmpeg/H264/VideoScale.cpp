#include"VideoScale.h"
VideoScaling::VideoScaling() {}
VideoScaling::~VideoScaling()
{
    av_freep(&des_data[0]);
    sws_freeContext(c);
    c = nullptr;
}
void VideoScaling::convert(AVFrame* src_frame, int width, int height)//������Ƶ֡���������λ��src_des;width/heightΪĿ�����
{
    static int i = 0;
    if (i == 0)
    {
        __init(src_frame, width, height);
    }
    //输出宽高发生改变，重新初始化转码器
    if (width != des_width || height != des_height)
    {
        __deInit();
        __init(src_frame, width, height);
    }
    sws_scale(c, src_frame->data, src_frame->linesize, 0, src_frame->height, des_data, des_linesize);
}
void VideoScaling::__init(AVFrame* src_frame, int width, int height)
{

    des_size = av_image_alloc(des_data, des_linesize, width, height, AV_PIX_FMT_RGB24, 1);
    c = sws_getContext(src_frame->width, src_frame->height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
    des_width = width;
    des_height = height;
}
void VideoScaling::__deInit()
{
    av_freep(&des_data[0]);
    sws_freeContext(c);
}

uint8_t** VideoScaling::getDesData() const
{
    return (uint8_t**)des_data;
}

int* VideoScaling::getDesLinesize() const
{
    return (int*)des_linesize;
}

int VideoScaling::getDesWidth() const
{
    return des_width;
}

int VideoScaling::getDesHeight() const
{
    return des_height;
}

int VideoScaling::getDesSize() const
{
    return des_size;
}
