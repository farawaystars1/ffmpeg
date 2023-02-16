//
// Created by hc on 2020/5/14.

#include "CCodec.h"

CCodec::CCodec() {
    m_iResult.m_nOutBufLen = 2048;
    m_iResult.m_pOutBuf = (char *) new char[2048];
    m_iResult.m_eType = AUDIO_CODING_TYPE::eUnSupport;
}



void CCodec::__DecodeAdpcm2Pcm(char *cInBuf, int nInBufLen) {
    adpcm_state iAdpcmState{0};
    iAdpcmState.valprev = (cInBuf[5] << 8) | (cInBuf[4] & 0xFF);
    iAdpcmState.index = cInBuf[6];
    nInBufLen -= 8;
    cInBuf += 8;
    adpcm_decoder((char *) cInBuf, (short *) m_iResult.m_pOutBuf, nInBufLen * 2, &iAdpcmState);
    m_iResult.m_nOutBufLen = 4 * nInBufLen;
    m_iResult.m_eType = AUDIO_CODING_TYPE::eAdpcm;
}

void CCodec::__DecodeG711A2Pcm(char *cInBuf, int nInBufLen) {
    int nRet = g711a_decode((short *) m_iResult.m_pOutBuf, (unsigned char *) cInBuf, nInBufLen);
    m_iResult.m_nOutBufLen = nRet;
    m_iResult.m_eType = AUDIO_CODING_TYPE::eG711A;
}

void CCodec::__DecodeG711U2Pcm(char *cInBuf, int nInBufLen) {
    int nRet = g711u_decode((short *) m_iResult.m_pOutBuf, (unsigned char *) cInBuf, nInBufLen);
    m_iResult.m_nOutBufLen = nRet;
    m_iResult.m_eType = AUDIO_CODING_TYPE::eG711U;
}

void CCodec::__DecodeG7262Pcm(char* pInBuf, int nInBufLen)
{
    //��ʼ��c->bitpersample
    // static int i = 0;
    // if (i == 0)
    // {
    //     switch (nInBufLen)
    //     {
    //     case 80: c->bits_per_coded_sample = 2; break;
    //     case 120:c->bits_per_coded_sample = 3; break;
    //     case 160:c->bits_per_coded_sample = 4; break;
    //     case 200:c->bits_per_coded_sample = 5; break;
    //     }
    //     i = 1;
    // }
    g726_decoder->decodeAudio(pInBuf,nInBufLen,&m_iResult);

    
}

DECODE_RESULT &CCodec::DecodeAudio(char *pInBuf, int nInBufLen, AUDIO_CODING_TYPE eType) {
    switch (eType) {
        case AUDIO_CODING_TYPE::eG711A:
          //  LOG_INFO << "eG711A";
            __DecodeG711A2Pcm(pInBuf, nInBufLen);
            break;
        case AUDIO_CODING_TYPE::eG711U:
           // LOG_INFO << "eG711U";
            __DecodeG711U2Pcm(pInBuf, nInBufLen);
            break;
        case AUDIO_CODING_TYPE::eG726:
          //  LOG_INFO << "G726";
            __DecodeG7262Pcm(pInBuf, nInBufLen);
            break;
        case AUDIO_CODING_TYPE::eAdpcm:
         //   LOG_INFO << "eAdpcm";
            __DecodeAdpcm2Pcm(pInBuf, nInBufLen);
            break;
        default:
            m_iResult.m_eType = AUDIO_CODING_TYPE::eUnSupport;
            break;
    }
    return m_iResult;
}
