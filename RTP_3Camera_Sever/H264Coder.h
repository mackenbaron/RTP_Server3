#pragma once
#include "RtpTransport.h"
#include<opencv\cv.h>  
#include<opencv\highgui.h>

#include "windows.h"
#include <string>
#include <cstring>
#include <tchar.h>
#include <iostream>
extern "C"
{

#include <libavutil/opt.h>  
#include <libavcodec/avcodec.h>  
#include <libavutil/channel_layout.h>  
#include <libavutil/common.h>  
#include <libavutil/imgutils.h>  
#include <libavutil/mathematics.h>  
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
#include <libavformat\avformat.h>
#include <libavformat\avio.h>

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")

}
/////camera/////////////////////////////////////////////
//
// The maximum number of cameras on the bus.
//
#define _MAX_CAMS       32

//
// The index of the camera to grab from.
//
#define _CAMERA_INDEX   0

//
// What file format should we save the processed image as?
//
//#define SAVE_FORMAT     FLYCAPTURE_422YUV8

//
// Register defines
// 
#define INITIALIZE         0x000
#define CAMERA_POWER       0x610


#define MODE 0
/////camera//////////////////////////////////////////////

class CH264Coder
{
public:
	CH264Coder(void);
	~CH264Coder(void);
	bool InitDecoder();
	bool InputFilePath(char *path);
	void SendNalBuffer();
	void enCodeTransition( int codec_id, unsigned char ID );
	void InitCamera();
	int shmem();
	
private:
	int FindStartCode (unsigned char *Buf, int zeros_in_startcode);
    int getNextNal(FILE* inpf, unsigned char* Buf);
	
	
   
private:
	AVCodec *codec;			  /* �����CODEC*/
	AVCodecContext *c;		  /* �����CODEC context*/
	AVFrame *picture;		  /* ������ͼ��*/	
	IplImage *image_src;    //���ڴ��ж�ȡ��ͼ��

	

	FILE *m_inFile;
	FILE * m_outFile;

public:
	// rtp 
	CRITICAL_SECTION CriticalSection;
	CRtpTransport m_rtpTransport;
	CvCapture* capture; // ������Ҫͨ�������豸���õ�һ��CvCapture���� 
	IplImage* frame;
		  
};
