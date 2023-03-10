#include"g726.h"
#include"../../ffmpeg.h"
G726::G726()
{

}
G726::~G726()
{
	//flushˢ��frame
	packet->data = nullptr;
	packet->size = 0;
	__decode(packet, frame, result);//清空frame残留帧数据

	avcodec_free_context(&c);
	av_packet_free(&packet);
	av_frame_free(&frame);
}

void G726::decodeAudio(char* pInBuf, int nInBufLen, DECODE_RESULT* ret)
{
	static int i = 0;
	if (i == 0)
	{
		switch (nInBufLen)
		{
		case 80: __init(2); break;
		case 120:__init(3); break;
		case 160:__init(4); break;

		case 200:__init(5); break;
		}

		i = 1;
	}
	packet->data = (uint8_t*)pInBuf;
	packet->size = nInBufLen;
	__decode(packet, frame, ret);


}
void G726::__init(int bits_per_codec_sample)
{
	int ret;
	frame = new AVFrame();
	packet = new AVPacket();
	codec = avcodec_find_decoder(AV_CODEC_ID_ADPCM_G726LE);
	c = avcodec_alloc_context3(codec);
	c->bits_per_coded_sample = bits_per_codec_sample;
	c->sample_rate = 8000;
	c->sample_fmt = AV_SAMPLE_FMT_S16;
	c->channel_layout = 1;
	c->bit_rate = c->bits_per_coded_sample * c->bit_rate;//����
	ret = avcodec_open2(c, codec, NULL);
}
void G726::__decode(AVPacket* packet, AVFrame* frame, DECODE_RESULT* result)
{
	int i, ch;
	int ret, data_size;
	this->result = result;
	/* send the packet with the compressed data to the decoder */
	ret = avcodec_send_packet(c, packet);
	if (ret < 0) {
		fprintf(stderr, "Error submitting the packet to the decoder\n");
		exit(1);
	}

	/* read all the output frames (in general there may be any number of them */
	while (ret >= 0) {
		ret = avcodec_receive_frame(c, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during decoding\n");
			exit(1);
		}

		memcpy(result->m_pOutBuf, frame->data[0], frame->nb_samples * 2);//pcms16leÿ�����������ֽ�
		result->m_eType = AUDIO_CODING_TYPE::eG726;
		result->m_nOutBufLen = frame->nb_samples * 2;

		av_frame_unref(frame);
		// 
			//pcms16le���ֽ�
			//data_size = av_get_bytes_per_sample(c->sample_fmt);//
			//if (data_size < 0) {
			//	/* This should not occur, checking just for paranoia */
			//	fprintf(stderr, "Failed to calculate data size\n");
			//	exit(1);
			//}
			/*for (i = 0; i < frame->nb_samples; i++)
				for (ch = 0; ch < dec_ctx->ch_layout.nb_channels; ch++)
					fwrite(frame->data[ch] + data_size * i, 1, data_size, outfile);*/
	}
}