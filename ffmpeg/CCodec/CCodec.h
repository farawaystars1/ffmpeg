//
// Created by hc on 2020/5/14.
//
#ifndef _CODEC_H
#define _CODEC_H
#include <iostream>
#include <memory>
#include"codec/Adpcm.h"
#include"codec/G711.h"
#include"codec/g726.h"
#include"type.h"

class CCodec {
public:
    CCodec();

    ~CCodec()
    {
         if (m_iResult.m_pOutBuf != nullptr && m_iResult.m_nOutBufLen != 0) {
        delete[]m_iResult.m_pOutBuf;
        m_iResult.m_nOutBufLen = 0;
    }
    }

public:

    // enum AUDIO_CODING_TYPE {
    //     eG711A,
    //     eG711U,
    //     eG726,
    //     eAdpcm,
    //     eUnSupport,
    // };
    // typedef struct {
    //     char *m_pOutBuf;
    //     int m_nOutBufLen;
    //     AUDIO_CODING_TYPE m_eType;
    // } DECODE_RESULT;

    DECODE_RESULT &DecodeAudio(char *pInBuf, int nInBufLen, AUDIO_CODING_TYPE eType);

public:
    DECODE_RESULT m_iResult;
private:
    G726* g726_decoder;

private:
    void __DecodeG711A2Pcm(char *pInBuf, int nInBufLen);

    void __DecodeG711U2Pcm(char *pInBuf, int nInBufLen);

    void __DecodeAdpcm2Pcm(char *pInBuf, int nInBufLen);

    void __DecodeG7262Pcm(char* pInBuf, int nInBufLen);


};

//typedef CCodec::DECODE_RESULT DECODE_RESULT;
//typedef CCodec::AUDIO_CODING_TYPE AUDIO_CODING_TYPE;
#endif //_CODEC_H
