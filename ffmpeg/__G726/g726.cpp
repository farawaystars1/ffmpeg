#include"g726.h"
extern"C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
}
G726::G726()
{
	frame = new AVFrame();

}
G726::~G726()
{
	avcodec_free_context(&c);
	av_packet_free(&packet);
	av_frame_free(&frame);
}
void G726::decodeAudio(char* pInBuf, int nInBufLen, DECODE_RESULT& ret)
{

}

uint8_t* G726::getFrameData()
{
	return frame->data();
}
int G726::getFrameDataLength()
{

}

void __decode()
{

}