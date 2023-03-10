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
//__G726不使用
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage <port>\n";
        return 1;
    }
    struct sockaddr_in serv_addr = { 0 };
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);//创建客户端socket
    if (sockfd < 0) {
        std::cout << "create socket error\n" << std::endl;
        exit(-1);
    }
    int ret;


    Deprotocal deprotocal;
    Buffer buf;
    serv_addr.sin_family = AF_INET;
    ret=inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if (ret < 0)
    {
        std::cout << "ip resolve error\n" << std::endl;
        exit(-1);
    }
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (sockfd < 0)return -1;
    ret = connect(sockfd, (struct sockaddr*)&serv_addr, (socklen_t)sizeof(serv_addr));//连接服务器
    if (ret < 0) {
        fprintf(stdout, "connect error");
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
            if (n == 0)return 0;
            buf.append(a, n);
            deprotocal.Decode(&buf);

        }
    }
}
