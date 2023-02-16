//#include<iostream>
//#include<string>
//#include"Buffer/Buffer.h"
//#include"ffmpeg.h"
//#include"VideoScale/VideoScalter.h"
//#include"CCodec/CCodec.h"
//#include"util.h"
//#include"Deprotocal/Deprotocal.h"
//#include"CCodec/CCodec.h"
//#include<fstream>
//using namespace std;
//int main()
//{
//	CCodec codec;
//	ifstream fileIn("/home/xingxing/dafengchui.g726",std::ios::binary);
//	ofstream fileOut("/home/xingxing/dafengchui.pcm",std::ios::binary);
//	char a[320];
//	int n;
//	while (1)
//	{
//		fileIn.read(a, 160);
//		int ret = fileIn.gcount();
//		if (fileIn.gcount() != 160)break;
//		codec.DecodeAudio(a, 160, AUDIO_CODING_TYPE::eG726);
//		fileOut.write(codec.m_iResult.m_pOutBuf,codec.m_iResult.m_nOutBufLen);
//	}
//	fileIn.close();
//	fileOut.close();
//}


#include<iostream>
#include<string>
#include"Buffer/Buffer.h"
#include"ffmpeg.h"
#include"VideoScale/VideoScalter.h"
#include"CCodec/CCodec.h"
#include"util.h"
#include"Deprotocal/Deprotocal.h"
#include"CCodec/CCodec.h"
//static int receiveData(int sockfd, Buffer* buf);
int main(int argc, char** argv)
{
    
    struct sockaddr_in serv_addr = { 0 };
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);//创建客户端socket
    int ret;

    // Deprotocal* deprotocal = new Deprotocal();
    Deprotocal deprotocal;
    Buffer buf;
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(8505);
    if (sockfd < 0)return -1;
    ret = connect(sockfd, (struct sockaddr*)&serv_addr, (socklen_t)sizeof(serv_addr));//连接服务器
    if (ret < 0) {
        fprintf(stdout, "connect error");
        //   delete deprotocal;
        exit(-1);
    }
    ret = send(sockfd, "Request", 5, 0);
    // std::future<int> fut1 = std::async(receiveData, sockfd, &buf);//创建接收服务器数据线程   ->buf


    int nready;
    fd_set rset;
    FD_ZERO(&rset);
    timeval val = { 1,0 };
    char a[1024];//缓存
    int n;
    while (1)
    {
        FD_SET(sockfd, &rset);

        nready = select(sockfd + 1, &rset, NULL, NULL, &val);
        if (nready == 1)
        {
         
            n = recv(sockfd, a, 1024, 0);
            buf.append(a, n);
            deprotocal.Decode(&buf);
            

            
            


        }
    }
}


//#include<stdlib.h>
//#include<memory.h>
//#include<stdio.h>
//#include<stdint.h>
//#include<iostream>
//#include<future>
//#include<thread>
//#include<fcntl.h>
//#include<string>
//using namespace std;
////fifo1 fifo2
//void print1(int fifo)
//{
//	char buf[1024]={0};
//	int n=read(fifo, buf, 1024);
//	printf("%s", buf);
//}
//int main()
//{
//	int fifo1 = open("/home/xingxing/fifo2.txt", O_WRONLY);
//	int fifo2=open("/home/xingxing/fifo1.txt", O_RDONLY);
//	future<void> fu1 = async(print1,fifo2);
//	string line;
//	while (1)
//	{
//		getline(std::cin, line);
//		if (line.size() == 0)break;
//		write(fifo1, line.data(), line.size());
//		line.clear();
//	}
//	close(fifo1);
//	close(fifo2);
//
//}