#include "StdAfx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "RtpTransport.h"

// rpt ���ɷ��Ͱ��Ĵ�С
#define  MAX_RTP_PACKETSIZE 1300

CRtpTransport::CRtpTransport(void)
:m_destIp(0)
,m_strDestIp("")
,m_nDestPort(0)
,m_nPortBase(6666)
{

	WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
}

CRtpTransport::~CRtpTransport(void)
{
	  WSACleanup();
}

void CRtpTransport::checkerror( int err )
{
	if (err < 0) 
	{   
		string errstr = jrtplib::RTPGetErrorString(err);    
		printf("Error:%s\\n", errstr);    
		exit(-1);  
	}   
}

void CRtpTransport::SetRtpInfo( string strIp,int ndestPort )
{
    //���ڱ�����inet_addrת�����ip��ַ
	
    unsigned long destIp =  inet_addr(strIp.c_str());
	
	if (destIp == INADDR_NONE)//INADDR_NONE��ʾ255.255.255.255
	{
		std::cerr << "Bad IP address specified" << std::endl;
		return ;
	}

	//inet_addr���ص��������ֽڵĵ�ַ���������ת��Ϊ�����ֽ�
    destIp = ntohl(destIp);
   
	sessparams.SetOwnTimestampUnit(1.0/10.0);
	//����ʱ�������λ���룬��ʹ��RTP�Ự����8000Hz��������Ƶ����ʱ������ʱ��
	//ÿ���ӽ�����8000������ʱ����Ԫ��Ӧ��Ӧ�ñ����ó�1/8000
	sessparams.SetAcceptOwnPackets(true);
	transparams.SetPortbase(m_nPortBase);
	int status = m_rtpSess.Create(sessparams,&transparams);	
	checkerror(status);

	jrtplib::RTPIPv4Address addr(destIp,ndestPort);

	status = m_rtpSess.AddDestination(addr);
	checkerror(status);
}

void CRtpTransport::SendRtpPacket( unsigned char *buf,int nBufLen )
{
    int status;
    int nLeftPacket = nBufLen/MAX_RTP_PACKETSIZE + 1;
    int nPos = 0;
	int nPacketLen = 0;
	// �����Ƿ�Ϊ���һ����
	bool bMark = false;
    if (1 == nLeftPacket)
    {   
		// ���һ����
		bMark = true;
        status = m_rtpSess.SendPacket((void *)buf,nBufLen,0,bMark,10);
    }
    else
	{
		for (int i=0;i<nLeftPacket;++i)
		{   
			nPacketLen = MAX_RTP_PACKETSIZE;
			// ���������һ��
			if (i == nLeftPacket - 1)
			{   
				bMark = true;
				nPacketLen = nBufLen%MAX_RTP_PACKETSIZE;
			}
			 status = m_rtpSess.SendPacket((void *)(buf + nPos),nPacketLen,0,bMark,10);

			nPos += MAX_RTP_PACKETSIZE;
		}

	}
	
	checkerror(status);
	Sleep(10);
}