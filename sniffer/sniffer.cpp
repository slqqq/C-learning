#include <winsock2.h>  
#include <WS2tcpip.h>  
#include <stdio.h>  
#include <iostream>
#include <Mstcpip.h>  
#include "ipstandard.h"
#pragma comment(lib,"ws2_32.lib")  
using namespace std;

#define SOURCE_PORT 8000 //源端口  
#define DESTINATION_PORT 7532 //目的端口  
#define MAX_RECEIVEBUF  20000  

//预定名称  
#define PROTOCOL_STRING_ICMP_TXT "ICMP"  
#define PROTOCOL_STRING_TCP_TXT "TCP"  
#define PROTOCOL_STRING_UDP_TXT "UDP"
#define PROTOCOL_STRING_UNKNOW_TXT "UNKNOW"  
BOOL capTCP = TRUE;
BOOL capUDP = FALSE;
BOOL capICMP = FALSE;


const char * GetProtocol(int proto)
{
	switch (proto)
	{
	case IPPROTO_ICMP: return PROTOCOL_STRING_ICMP_TXT;
	case IPPROTO_TCP:  return PROTOCOL_STRING_TCP_TXT;
	case IPPROTO_UDP:  return PROTOCOL_STRING_UDP_TXT;
	default:     return PROTOCOL_STRING_UNKNOW_TXT;
	}
}


int main(int argc, char**argv)
{


	FILE * f = fopen("D://a.txt", "w");

	WSADATA WSAData;  //  
	SOCKET sock;      //定义一个socket  
	SOCKADDR_IN addr_in;
	IP_HEADER ipHeader;  //定义一个Ip头部  
	TCP_HEADER tcpHeader; //定义一个TCP头部
	UDP_HEADER udpHeader;
	ICMP_HEADER icmpHeader;
	WORD versionRequest;
	versionRequest = MAKEWORD(2, 2);
	char szSendBuf[60] = { 0 };
	BOOL flag;
	int err;
	err = WSAStartup(versionRequest, &WSAData);
	if (!err)
	{
		printf("Socket is opening\n");
	}
	else
	{
		printf("error: Scoket can not open!");
		return 1;
	}
	if ((sock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("error:  WSASocket()! error_number:%d", WSAGetLastError());
		return 1;
	}

	flag = true;
	//设置IP头部  
	if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&flag, sizeof(flag)) == SOCKET_ERROR)
	{
		printf("setsockopt() error! error_number:%d", WSAGetLastError());
		return 1;
	}

	char  LocalName[16];
	struct hostent *pHost;
	// 获取本机名   
	if (gethostname((char*)LocalName, sizeof(LocalName) - 1) == SOCKET_ERROR)
	{
		printf("gethostname error! %d", WSAGetLastError());
		return FALSE;
	}

	// 获取本地 IP 地址   
	if ((pHost = gethostbyname((char*)LocalName)) == NULL)
	{
		printf("gethostbyname error! %d", WSAGetLastError());
		return FALSE;
	}
	addr_in.sin_addr = *(in_addr *)pHost->h_addr_list[0]; // IP   
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(57274);
	if (bind(sock, (struct sockaddr *)&addr_in, sizeof(addr_in)) != 0)
	{
		printf("bind error! %d", WSAGetLastError());
		return 1;
	}
	DWORD dwValue = 1;
	if (ioctlsocket(sock, SIO_RCVALL, &dwValue) != 0)
	{
		printf("ioctlsocket error! %d", WSAGetLastError());
		return 1;
	}

	char recvBuf[MAX_RECEIVEBUF] = { 0 };
	char msg[MAX_RECEIVEBUF] = { 0 };
	char *ptr = NULL;
	DWORD nTCPCnt = 0, nUDPCnt = 0, nICMPCnt = 0;
	while (1)
	{
		int returnValue = recv(sock, recvBuf, MAX_RECEIVEBUF, 0);
		if (returnValue == SOCKET_ERROR)
			printf("%d, recv(sock, recvBuf, MAX_RECEIVEBUF, 0) failed!\n", GetLastError());
		if (returnValue > 0)
		{
			ipHeader = *(IP_HEADER*)recvBuf;

			// 取得正确的IP头长度  
			int iIphLen = sizeof(unsigned long) * (ipHeader.h_verlen & 0xf);
			int cpysize = 0;

			if (ipHeader.proto == IPPROTO_TCP && capTCP)
			{
				nTCPCnt++;
				tcpHeader = *(TCP_HEADER*)(recvBuf + iIphLen);
				printf("取得 %d TCP包", nTCPCnt);
				printf("协议： %s \r\n", GetProtocol(ipHeader.proto));
				printf("IP源地址： %s \r\n", inet_ntoa(*(in_addr*)&ipHeader.sourceIP));
				printf("IP目标地址: %s \r\n", inet_ntoa(*(in_addr*)&ipHeader.destIP));
				printf("TCP源端口号： %d \r\n", tcpHeader.th_sport);
				printf("TCP目标端口号：%d \r\n", tcpHeader.th_dport);
				printf("数据包长度： %d \r\n", ntohs(ipHeader.total_len));
				printf("TCP数据包的报文内容：\r\n");

				ptr = recvBuf + iIphLen + (4 * ((tcpHeader.th_lenres & 0xf0) >> 4 | 0));
				cpysize = ntohs(ipHeader.total_len) - (iIphLen + (4 * ((tcpHeader.th_lenres & 0xf0) >> 4 | 0)));

				// ASCII码  
				memcpy(msg, ptr, cpysize);
				for (int i = 0; i < cpysize; i++)
				{
					putc(msg[i], f);
					if (msg[i] >= 0 && msg[i] < 255)
					{
						printf("%c", (unsigned char)msg[i]);
					}
					else
					{
						printf(".");
					}
				}
				printf("\r\n \r\n");
			}


			if (ipHeader.proto == IPPROTO_ICMP && capICMP)
			{
				nICMPCnt++;
				icmpHeader = *(ICMP_HEADER*)(recvBuf + iIphLen);
				printf("取得 %d ICMP包", nICMPCnt);
				printf("协议： %s\r\n", GetProtocol(ipHeader.proto));
				printf("IP源地址： %s\r\n", inet_ntoa(*(in_addr*)&ipHeader.sourceIP));
				printf("IP目标地址: %s\r\n", inet_ntoa(*(in_addr*)&ipHeader.destIP));
				printf("ICMP返回类型：%d\r\n", icmpHeader.i_type);
				printf("ICMP返回代码：%d\r\n", icmpHeader.i_code);
				printf("数据包长度： %d\r\n\r\n\r\n", ntohs(ipHeader.total_len));


				ptr = recvBuf + iIphLen;
				cpysize = ntohs(ipHeader.total_len);

				// ASCII码  
				memcpy(msg, ptr, cpysize);
				for (int i = 0; i < cpysize; i++)
				{
					if (msg[i] >= 32 && msg[i] < 255)
					{
						printf("%c", (unsigned char)msg[i]);
					}
					else
					{
						printf(".");
					}
				}
				printf("\r\n \r\n");

			}




			if (ipHeader.proto == IPPROTO_UDP && capUDP)
			{
				nUDPCnt++;
				udpHeader = *(UDP_HEADER*)(recvBuf + iIphLen);
				printf("取得 %d UDP包", nUDPCnt);
				printf("协议： %s\r\n", GetProtocol(ipHeader.proto));
				printf("IP源地址： %s\r\n", inet_ntoa(*(in_addr*)&ipHeader.sourceIP));
				printf("IP目标地址: %s\r\n", inet_ntoa(*(in_addr*)&ipHeader.destIP));
				printf("UDP源端口号： %d\r\n", udpHeader.uh_sport);
				printf("UDP目标端口号：%d\r\n", udpHeader.uh_dport);
				printf("数据包长度： %d\r\n", ntohs(ipHeader.total_len));
				printf("UDP数据包的报文内容：\r\n");

				ptr = recvBuf + iIphLen + 8;
				cpysize = ntohs(ipHeader.total_len) - (iIphLen + 8);
				memcpy(msg, ptr, cpysize);

				printf("ASCII码格式: \r\n");
				for (int i = 0; i < cpysize; i++)
				{
					if (msg[i] >= 32 && msg[i] < 255)
					{
						printf("%c", (unsigned char)msg[i]);
					}
					else
					{
						printf(".");
					}
				}
				printf("\r\n\r\n");
				printf("16进制码格式: \r\n");
				for (int i = 0; i < cpysize; i++)
				{
					printf("%2.2X ", (unsigned char)msg[i]);
				}
				printf("\r\n\r\n");

			}

		}
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}
