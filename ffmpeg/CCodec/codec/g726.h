#pragma once
#ifndef G726_H
#define G726_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include"../type.h"
#include"../../ffmpeg.h"
//extern"C"
//{
//#include <libavcodec/avcodec.h>
//#include <libavutil/channel_layout.h>
//#include <libavutil/common.h>
//#include <libavutil/frame.h>
//#include <libavutil/samplefmt.h>
//}

class G726
{
public:
	// enum AUDIO_CODING_TYPE {
	// 	eG711A,
	// 	eG711U,
	// 	eG726,
	// 	eAdpcm,
	// 	eUnSupport,
	// };
	// typedef struct {
	// 	char* m_pOutBuf;
	// 	int m_nOutBufLen;
	// 	AUDIO_CODING_TYPE m_eType;
	// } DECODE_RESULT;
	G726();
	~G726();
public:
	
	void decodeAudio(char* pInBuf, int nInBufLen, DECODE_RESULT* ret);
private:
	void __init(int bits_per_codec_sample);
	void __decode(AVPacket* packet, AVFrame* frame, DECODE_RESULT* result);
private:
	DECODE_RESULT* result;
	AVFrame* frame;
	AVPacket* packet;
	AVCodec* codec;
	AVCodecContext* c;



};



#endif // G726_H