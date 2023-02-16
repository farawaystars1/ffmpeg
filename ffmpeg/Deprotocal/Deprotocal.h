#ifndef _DEPROTOCAL_H
#define _DEPROTOCAL_H
#include"../JT1078Header.h"
#include"../CCodec/CCodec.h"
#include"../Buffer/Buffer.h"
#include"../CCodec/type.h"
class Deprotocal {
public:
    enum CURRENT_RECEIVE_STATE
    {
        eInit,
        eReceiving,
        eCompleted,
    };
    enum SUBCONTRACT_PROCESSING_MARKE
    {
        eAtomic,
        eFirst,
        eLast,
        eIntermediate,
    };
    enum DATA_TYPE
    {
        eVideoI,
        eVideoP,
        eVideoB,
        eAudio,
        ePassthrough,
    };
    enum SKIP
    {
        eSkipPaser16,
        eSkipPaserTheRestData,
        eSkipNon,

    };
    enum ERRS
    {
        eSTATError,
        eMarkeError,
        eTypeError,
        eNoError,
        eLengthError,
        eCodingTypeError,
    };
    enum AV_CODING_TYPE
    {
        eG711A,
        eG711U,
        eG726,
        eAdpcm,
        eH264,
        eUnSupport,
    };
public:
    Deprotocal();
    ~Deprotocal();
    bool Decode(Buffer* buf);
    //bool DecodeHeader(Buffer*buf);
    //bool DecodeBody(Buffer*buf);

private:
    int  __checkError();
    void __SetAVCodingType();
    int __CheckSubMark();
    int __GetTailLenAndSetDataType();
private:

    //CURRENT_RECEIVE_STATE m_eCurrentStat;
    SUBCONTRACT_PROCESSING_MARKE m_eSubMarke;
    DATA_TYPE m_eDataType;
    //SKIP m_eSkip;
    ERRS m_eErr;
    AV_CODING_TYPE m_eCodingType;
    //unsigned int  m_iRecvLen;
    unsigned int m_iBodyLength;
    JT_1078_HEADER m_iHeader;
    std::string m_sData;
    CCodec* m_audioDecoder = nullptr;


};

#endif //_DEPROTOCAL_H