#include"h264decoder.h"
H264Decoder::H264Decoder()
{
    frame = nullptr;
    packet = nullptr;
}

H264Decoder::~H264Decoder()
{
    __flushFrame();
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&packet);
    delete scale;
    scale = nullptr;
}

void H264Decoder::decodeH264(uint8_t* pInBuf, int nInBufLen)
{
    static int i = 0;
    if (i == 0) {
        __init();
        i++;
    }
    std::cout << "h264input\n";
    std::cout << nInBufLen << std::endl;
    packet->data = pInBuf;
    std::cout << "end" << std::endl;
    packet->size = nInBufLen;
    __decode(packet, frame);
}

void H264Decoder::__init()
{
    int ret;
    frame = av_frame_alloc();
    packet = av_packet_alloc();
    if (packet == nullptr)std::cout << "error" << std::endl;
    scale = new VideoScaling();
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    c = avcodec_alloc_context3(codec);
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0)std::cout << "open h264 codec error" << std::endl;
    std::cout << "xx" << std::endl;
}

void H264Decoder::__decode(AVPacket* packet, AVFrame* frame)
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
        scale->convert(frame, frame->width, frame->height);
        printf("receive rgb24{width:%d,height:%d}\n",scale->getDesWidth(),scale->getDesHeight());
        av_frame_unref(frame);
    }
}

void H264Decoder::__flushFrame()
{
    packet->data = NULL;
    packet->size = 0;
    __decode(packet, frame);
}
