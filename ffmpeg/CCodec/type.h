    #ifndef _TYPE_H
    #define _TYPE_H
    enum AUDIO_CODING_TYPE {
        eG711A,
        eG711U,
        eG726,
        eAdpcm,
        eUnSupport,
    };


    typedef struct {
        char *m_pOutBuf;
        int m_nOutBufLen;
        AUDIO_CODING_TYPE m_eType;
    } DECODE_RESULT;

    #endif //_TYPE_H