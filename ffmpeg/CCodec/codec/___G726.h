#ifndef G726_H
#define G726_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
extern"C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
}
class G726
{
public:
	G726();
	~G726();
public:
	void decodeAudio(char* pInBuf, int nInBufLen, DECODE_RESULT&ret);
private:
  __decode(Packet* packet, Frame* frame);

private:
	Frame* frame;
	Packet* packet;
	AVCodec* codec;
	AVCodecContext* c;
    

};



#endif // G726_H