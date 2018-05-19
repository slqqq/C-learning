#pragma once
struct IP_HEADER
{
	unsigned char h_verlen;       // 版本 和 首部长度  
	unsigned char diffserv;     // 区分服务  
	unsigned short total_len;        // 总长度  
	unsigned short flag;        // 标识  
	unsigned short flag4_fraoff12; // 标志 和 片偏移  
	unsigned char ttl;          // 生存时间  
	unsigned char proto;                    // 协议  
	unsigned short crc;         // 首部检验和  
	unsigned long sourceIP;     // 源地址  
	unsigned long destIP;     // 目的地址  
};
struct TCP_HEADER
{
	unsigned short th_sport;
	unsigned short th_dport;
	unsigned long snum;
	unsigned long acknum;
	unsigned short th_lenres;
	unsigned short window;
	unsigned short crc;
	unsigned short emerp;
};
struct UDP_HEADER
{
	unsigned short uh_sport;
	unsigned short uh_dport;
	unsigned short tlen;
	unsigned short crc;
};

struct ICMP_HEADER
{
	unsigned char i_type;     //类型
	unsigned char i_code;
	unsigned short chk_sum;    //16位检验和
};
