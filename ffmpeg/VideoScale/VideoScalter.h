//#pragma once
//class VideoScaling {
//public:
//    VideoScaling(){}
//    ~VideoScaling()
//    {
//        av_freep(&des_data[0]);
//        sws_freeContext(c);
//        c = nullptr;
//    }
//    void convert(AVFrame*src_frame,int width,int height)//������Ƶ֡���������λ��src_des;width/heightΪĿ�����
//    {
//        static int i = 0;
//        if (i == 0)
//        {
//            __init(src_frame,width,height);
//        }
//        //��������
//        if (width != des_width || height != des_height)
//        {
//            __deInit();
//            __init(src_frame,width,height);
//        }
//        sws_scale(c, src_frame->data, src_frame->linesize, 0, src_frame->height, des_data, des_linesize);
//    }
//    //sws_scale(sws_ctx, (const uint8_t* const*)src_data,
//    //    src_linesize, 0, src_h, dst_data, dst_linesize);
//private:
//    void __init(AVFrame* src_frame, int width, int height)
//    {
//        int ret;
//        ret = av_image_alloc(des_data, des_linesize, width, height, AV_PIX_FMT_RGB24, 0);
//        c = sws_getContext(src_frame->width, src_frame->height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
//        des_width = width;
//        des_height = height;
//    }
//    void __deInit()
//    {
//        av_freep(&des_data[0]);
//        sws_freeContext(c);
//    }
//private:
//    SwsContext* c;
//    uint8_t*des_data[4];
//    int des_linesize[4];
//    int des_width;
//    int des_height;
//};