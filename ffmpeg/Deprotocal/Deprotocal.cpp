#include"Deprotocal.h"

Deprotocal::Deprotocal(){
    m_audioDecoder = new CCodec();
}
Deprotocal::~Deprotocal(){
    if (m_audioDecoder)
    {
        delete m_audioDecoder;
        m_audioDecoder = nullptr;
    }
}

 bool Deprotocal::Decode(Buffer* buf)
 {
     while (buf->readableBytes() > 979)//一帧数据最大950 +包头30
     {
         uint8_t ret;
         //提取包头前16字节数据
         m_iHeader.DWFramHeadMark = buf->readInt32();
         ret = buf->readInt8();
         m_iHeader.V2 = (ret >> 6) & 0x03;
         m_iHeader.P1 = (ret >> 5) & 0x01;
         m_iHeader.X1 = (ret >> 4) & 0x01;
         m_iHeader.CC4 = ret & 0x0F;
         ret = buf->readInt8();
         m_iHeader.M1 = (ret >> 7) & 0x01;
         m_iHeader.PT7 = ret & 0x7f;
         m_iHeader.WdPackageSequence = buf->readInt16();
         m_iHeader.BCDSIMCardNumber[0] = buf->readInt8();
         m_iHeader.BCDSIMCardNumber[1] = buf->readInt8();
         m_iHeader.BCDSIMCardNumber[2] = buf->readInt8();
         m_iHeader.BCDSIMCardNumber[3] = buf->readInt8();
         m_iHeader.BCDSIMCardNumber[4] = buf->readInt8();
         m_iHeader.BCDSIMCardNumber[5] = buf->readInt8();
         m_iHeader.Bt1LogicChannelNumber = buf->readInt8();
         ret = buf->readInt8();
         m_iHeader.DataType4 = (ret & 0xf0) >> 4;
         m_iHeader.SubpackageHandleMark4 = ret & 0x0f;
         //检查错误
         __checkError();
         //提取包头剩余数据
         if (m_eDataType == eVideoB || m_eDataType == eVideoI || m_eDataType == eVideoP)
         {
             m_iHeader.Bt8timeStamp = buf->readInt64();
             m_iHeader.WdLastIFrameInterval = buf->readInt16();
             m_iHeader.WdLastFrameInterval = buf->readInt16();
         }
         else if (m_eDataType == eAudio)//音频无帧时间间隔
         {
             m_iHeader.Bt8timeStamp = buf->readInt64();
         }
         //透传数据没有时间间隔 时间戳
         
         
         m_iHeader.WdBodyLen = buf->readInt16();//包体长度
      
         m_iBodyLength = m_iHeader.WdBodyLen;
         //去除包体海斯头
         ret = buf->peekInt32();
         uint8_t a[4];
         a[0] = (ret & 0xff000000) >> 24;
         a[1] = (ret & 0xff0000) >> 16;
         a[2] = (ret & 0xff00) >> 8;
         a[3] = ret & 0xff;
         if (a[0] == 0 && a[1] == 1 && a[2] == 0&&a[2]==(m_iBodyLength-4)/2)
         {
             std::cout<<"去除海斯头"<<std::endl;
             buf->retrieve(4);
             m_iBodyLength -= 4;
         }

         //音视频解码
         switch (m_eDataType)
         {
         case DATA_TYPE::eAudio:
             if (m_iBodyLength > buf->readableBytes())std::cout << "audio--------------------" << std::endl;
             std::cout <<"body_length:"<< m_iBodyLength << ',' << "readablebyes:"<<buf->readableBytes() << std::endl;
             std::cout<<m_iHeader.DWFramHeadMark << std::endl;
             m_sData = buf->retrieveAsString(m_iBodyLength);//提取音频帧数据
             //解码音频至pcms16le
             AUDIO_CODING_TYPE type;
             switch (m_eCodingType)
             {
             case eG711A:type = AUDIO_CODING_TYPE::eG711A;break;
             case eG711U:type = AUDIO_CODING_TYPE::eG711U;break;
             case eG726:type = AUDIO_CODING_TYPE::eG726;break;
             case eAdpcm:type = AUDIO_CODING_TYPE::eAdpcm;break;
             default:
                 type = AUDIO_CODING_TYPE::eUnSupport;
             }
             std::cout<<"decode audio...\n";
             m_audioDecoder->DecodeAudio(m_sData.data(), m_sData.length(), type);
             m_sData.clear();
             break;
         case DATA_TYPE::eVideoI:
         case DATA_TYPE::eVideoP:
         case DATA_TYPE::eVideoB:
             switch (m_eSubMarke)
             {
             case eFirst:
             case eIntermediate:
                 if (m_iBodyLength > buf->readableBytes())std::cout <<"video1--------------------" << std::endl;
                 m_sData += buf->retrieveAsString(m_iBodyLength);
                 break;
             case eLast:
                 //解码视频。。。。
                 std::cout << "decode video" << std::endl;
                 if (m_iBodyLength > buf->readableBytes())std::cout << "video2--------------" << std::endl;
                 m_sData += buf->retrieveAsString(m_iBodyLength);
                 m_sData.clear();
             }
             break;
         case DATA_TYPE::ePassthrough:
             std::cout << "透传...\n";
             if (m_iBodyLength > buf->readableBytes())std::cout << "透传---------------------" << std::endl;
             std::cout << "body_length:" << m_iBodyLength << ',' << "readablebyes:" << buf->readableBytes() << std::endl;
             buf->retrieve(m_iBodyLength);
             break;
         default:
             std::cout << "others..." << std::endl;


             //m_sData=buf->retrieve(m_iHeader.WdBodyLen);


         }



     }
     return true;
 }


 void Deprotocal::__SetAVCodingType()
 {
     switch (m_iHeader.PT7)
     {
     case 6:
         m_eCodingType = eG711A;
         break;
     case 7:
         m_eCodingType = eG711U;
         break;
     case 26:
         m_eCodingType = eAdpcm;
         break;
     case 98:
         m_eCodingType = eH264;
         break;
     default:
         m_eCodingType = eUnSupport;
         break;
     }
 }

 int Deprotocal::__CheckSubMark()
 {
     switch (m_iHeader.SubpackageHandleMark4)
     {
     case 0x00:
         m_eSubMarke = eAtomic;break;
     case 0x01:
         m_eSubMarke = eFirst;break;
     case 0x02:
         m_eSubMarke = eLast;break;
     case 0x03:
         m_eSubMarke = eIntermediate;break;
     default:
         m_eErr = eMarkeError;
         return -1;
     }
     return 0;
 }

 int Deprotocal::__GetTailLenAndSetDataType()
 {
     switch (m_iHeader.DataType4)
     {
     case 0x00://I
         m_eDataType = eVideoI;
         break;
     case 0x01://P
         m_eDataType = eVideoP;
         break;
     case 0x02://B
         m_eDataType = eVideoB;
         break;
     case 0x03://音频
         m_eDataType = eAudio;
         break;
     case 0x04://透传
         m_eDataType = ePassthrough;
         break;
     default:
         m_eErr = eTypeError;
         return -1;
     }
     return 0;
 }

 int Deprotocal::__checkError()
 {
     //设置数据类型
     if(__GetTailLenAndSetDataType()<0)
     return -1;
     //初始化subMark原子包或视频分包
     if(__CheckSubMark()<0)
     return -1;
     //setAvCodingType
     __SetAVCodingType();
     return 0;

 }

