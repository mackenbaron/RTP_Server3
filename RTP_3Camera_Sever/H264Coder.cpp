#include "StdAfx.h"
#include "H264Coder.h"
#include <opencv2/core/core.hpp>//OpenCV����ͷ�ļ�  
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/opencv.hpp>  
//#include "afxdialogex.h"
#include "windows.h"

CH264Coder::CH264Coder(void)
{
	capture = cvCaptureFromCAM(-1);
	InitializeCriticalSection(&CriticalSection);
}

CH264Coder::~CH264Coder(void)
{
}


void CH264Coder::enCodeTransition( int codec_id, unsigned char ID )
{
	
    AVCodec *codec;  
    AVCodecContext *c= NULL;  
    int i, ret, x, y, got_output;  

    AVFrame *frame;  
    AVPacket pkt;  
    //uint8_t endcode[] = { 0, 0, 1, 0xb7 };//�ļ��Ľ�β Ҫд�ļ����ֽڡ�  
  
    /* find the mpeg1 video encoder */  
    codec = avcodec_find_encoder((AVCodecID)codec_id);//�ҵ���Ӧ��<strong>����</strong>�������codec_id����Ϊ�ҵ��ġ�  
    if (!codec) {//�ҵ��󷵻�AVCodec�ṹ������codec->id==code_id  
		AfxMessageBox(_T("Codec not found"));
        exit(1);  
    }  

    c = avcodec_alloc_context3(codec);//�õ�<strong>����</strong>���������ġ�  
    if (!c) {  
        AfxMessageBox(_T("Could not allocate video codec context"));  
        exit(1);  
    }
	
	
	/////////////////////camera//////////////////////////////
	//camera//////////////
	IplImage* img;// = cvLoadImage("hello.jpg");//cv�ӿڡ�
	InitCamera();
	img = cvQueryFrame(capture);
		
	//-----------initial encode para---------start
    /* put sample parameters */  
    c->bit_rate = 400000;  
    /* resolution must be a multiple of two */  
    //c->width  = uiWidth;  
    //c->height = uiHeight;//ӰƬ�Ŀ�Ⱥ͸߶�  
	c->width  = img->width;  
	c->height = img->height;//ӰƬ�Ŀ�Ⱥ͸߶� 
    /* frames per second */  
    AVRational  a={1,25};//����δ�鵽���AVRational�ṹ������塣�����������Ӣ�Ĳ²⣬Ϊ��1���Ӳ���25֡��  
    c->time_base=a;                //ÿ��25֡
    c->gop_size = 10; /* emit one intra frame every ten frames *///GOP��Group of Pictures������Ӱ�������������νGOP����˼�ǻ����飬һ��GOP����һ�������Ļ��档  
    c->max_b_frames=0;  
    c->pix_fmt = AV_PIX_FMT_YUV420P;//ӰƬ�ı����ʽ������4��2��0��yuv��ʽ��  
  
    if(codec_id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "superfast", "zerolatency", 0);          //�Ա����ٶ���Ӱ��
	//av_opt_set(c->priv_data, "preset", "slow", 0);          //�Ա����ٶ���Ӱ��
	/*av_opt_set(c->priv_data, "preset", "ultrafast", 0);
    av_opt_set(c->priv_data, "tune","stillimage,fastdecode,zerolatency",0);
    av_opt_set(c->priv_data, "x264opts","crf=26:vbv-maxrate=728:vbv-bufsize=364:keyint=25",0);*/
  
    /* open it */  
    if (avcodec_open2(c, codec, NULL) < 0) {//�����H264<strong>����</strong>��  
        AfxMessageBox(_T("Could not open codec\n"));  
        exit(1);  
    }  
  
	AVFrame* pic_BGR = avcodec_alloc_frame();
	SwsContext* swc_BGR2YUV = sws_getContext(img->width, img->height, PIX_FMT_BGR24, img->width, img->height, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);
	//-----------initial encode para---------end
	//-----------initial decode para-------------start	
	//avcodec_register_all();
	int  got_picture;		/*�Ƿ����һ֡ͼ��*/
	int  consumed_bytes; /*���������ĵ���������*/
	int cnt = 0;
	/*���� H264 CODEC*/
	codec = avcodec_find_decoder(CODEC_ID_H264);
	if (!codec)
		return;
	AVCodecContext *c1;
	/*��ʼ��CODEC��Ĭ�ϲ���*/
	c1 = avcodec_alloc_context3(codec);
	c1->max_b_frames = 0;
	c1->width = img->width;
	c1->height = img->height;
	//if (c1->priv_data){}
	if (!c1)
		return;

	/*1. ��CODEC�������ʼ��H.264������������decode_init���غ���*/
	if (avcodec_open2(c1, codec, NULL) < 0)
		return;

	//int i;
	AVFrame * picture1 = avcodec_alloc_frame();
	static IplImage* bgrImg1 = cvCreateImage(cvSize(c1->width, c1->height), IPL_DEPTH_8U, 3);  //�����⣬������Ч���ݳ���
	if (!picture1)
		return;
	AVFrame* pic_BGR2 = avcodec_alloc_frame();
	SwsContext* swc_YUV2BGR = sws_getContext(bgrImg1->width, bgrImg1->height, PIX_FMT_YUV420P, bgrImg1->width, bgrImg1->height, PIX_FMT_BGR24, SWS_POINT, NULL, NULL, NULL); //init swscontext
	//------------initial decode para------------end
    frame = avcodec_alloc_frame();//��frame�ṹ������ڴ档������Ϊ��frame->data�����ڴ档  
    if (!frame) {  
        AfxMessageBox(_T("Could not allocate video frame\n"));  
        exit(1);  
    }  
    //frame->format = c->pix_fmt;//��ʽ  
    //frame->width  = c->width;//��͸�  
    //frame->height = c->height;  
	frame->pts = 0;
  
	
    /* the image can be allocated by any means and av_image_alloc() is 
     * just the most convenient way if av_malloc() is to be used */  
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,  c->pix_fmt, 32);   //ʵ�ʷ����ڴ档��ǰ���avcodec_alloc_frame()�������ʹ�á�  
    if (ret < 0) {//  
        AfxMessageBox(_T("Could not allocate raw picture buffer\n"));//�����ڴ�ʧ�ܡ�  
        exit(1);  
    }  
    	
	unsigned int count=0;  //pts ����

    /* encode YUV */  
    while(true) //����д��200֡�����ݡ�200/25=8,,,��˲�����8s��ӰƬ��
	{
		img = cvQueryFrame(capture);
		EnterCriticalSection(&CriticalSection);

		//-----------encode start
        av_init_packet(&pkt);//��Ȼ��ʼ���ˣ�  
        pkt.data = NULL;    // packet data will be allocated by the encoder�������û�����=null���������vs�в���0xdddddddd����  
        pkt.size = 0;  
		pkt.pts = AV_NOPTS_VALUE;
		pkt.dts = AV_NOPTS_VALUE;
        //fflush(stdout);//

		DWORD start_time = GetTickCount(); //��ʱ
		

		avpicture_fill((AVPicture*)pic_BGR, (uint8_t*)img->imageData, PIX_FMT_BGR24, img->width, img->height);               //img->pic_BGR
		
		sws_scale(swc_BGR2YUV, pic_BGR->data, pic_BGR->linesize, 0, c->height, frame->data, frame->linesize );   //rgbתyuv����pic_BGR��frame		

		
		//delete [] imageConverted.pData;
         frame->pts=count;
		 count++;
        /* encode the image */  		
        ret = avcodec_encode_video2(c, &pkt, frame, &got_output);                //h264���� ,�ѱ���������д��pkt.data�С�  
        if (ret < 0) {     
            AfxMessageBox(_T("Error encoding frame\n"));
            exit(1);  
        }				
		//---------encode  end----------
		//--------decode-------start		
		consumed_bytes = avcodec_decode_video2(c1, picture1, &got_picture, &pkt);  //h264����,��pkt�⵽picture1	
		
		avpicture_fill((AVPicture*)pic_BGR2, (uint8_t*)bgrImg1->imageData, PIX_FMT_BGR24, c->width, c->height);          //�˴��޸�bgrimg1Ϊc1���������bug ,��ʼ����				
		sws_scale(swc_YUV2BGR, picture1->data, picture1->linesize, 0, c->height, pic_BGR2->data, pic_BGR2->linesize);  //yuv2rgb, picture1->pic_BGR2
		//int64_t a = pic_BGR2->pts;
		//printf("%I64d", pic_BGR->pts);//
		/*TRACE("wewe:%I64d", a);
		TRACE("\n");*/
		avpicture_layout((AVPicture*)pic_BGR2, PIX_FMT_BGR24, bgrImg1->width, bgrImg1->height, (uchar*)bgrImg1->imageData, bgrImg1->imageSize);  //copy data from avpicture into buffer,pic_BGR->bgrImg1
		
		//av_frame_free(&pic_BGR);
		/*if (NULL != pkt.data)
		{
			free(pkt.data);
			pkt.data = NULL;
		}*/
		cvShowImage("����ͼ��", bgrImg1); //
		
		//cout << "The run time is:" << (end_time - start_time) << "ms!" << endl;//�������ʱ��
		cvWaitKey(20);
		DWORD end_time = GetTickCount();
		unsigned long ati = end_time - start_time;
		//TRACE("time:%I64d", ati);
		//--------decode------end
		int nBufLen;
		if( got_output )
		{

			//-----------
			
			//m_rtpTransport.SendRtpPacket(pkt.data, pkt.size );
			pkt.data[pkt.size] = 0x01;
			m_rtpTransport.SendRtpPacket(pkt.data, pkt.size + 1);
			/*Sleep(20);
			pkt.data[pkt.size] = 0x02;
			m_rtpTransport.SendRtpPacket(pkt.data, pkt.size + 1);
			Sleep(20);
			pkt.data[pkt.size] = 0x03;
			m_rtpTransport.SendRtpPacket(pkt.data, pkt.size + 1);
			Sleep(200);*/
			//FILE *f;
			//f = fopen("hgl.264", "ab");
			//fwrite(pkt.data, 1, pkt.size, f);//�ѱ�������д��ӰƬ��
			//fclose(f);			
			
			av_free_packet(&pkt);
		}
		LeaveCriticalSection(&CriticalSection);    //unlock
		//Sleep(50);

		


    }
	av_frame_free(&pic_BGR);
	av_frame_free(&pic_BGR2);
	sws_freeContext(swc_YUV2BGR);	
	sws_freeContext(swc_BGR2YUV);
	avcodec_close(c);  
	avcodec_close(c1);
    av_free(c);  
	av_free(c1);
    av_freep(&frame->data[0]);  
    av_frame_free(&frame);  
}

int CH264Coder::FindStartCode (unsigned char *Buf, int zeros_in_startcode)
{
	int info;
	int i;

	info = 1;
	for (i = 0; i < zeros_in_startcode; i++)
		if(Buf[i] != 0)
			info = 0;

	if(Buf[i] != 1)
		info = 0;
	return info;
}
int  CH264Coder::getNextNal(FILE* inpf, unsigned char* Buf)
{
	int pos = 0;
	int StartCodeFound = 0;
	int info2 = 0;
	int info3 = 0;

	while(!feof(inpf) && (Buf[pos++]=fgetc(inpf))==0);


	while (!StartCodeFound)
	{
		if (feof (inpf))
		{
			return pos-1;
		}
		Buf[pos++] = fgetc (inpf);
		info3 = FindStartCode(&Buf[pos-4], 3);
		if(info3 != 1)
			info2 = FindStartCode(&Buf[pos-3], 2);
		StartCodeFound = (info2 == 1 || info3 == 1);
	}
	fseek (inpf, -4, SEEK_CUR);
	return pos - 4;
}

bool CH264Coder::InitDecoder()
{
	/*CODEC�ĳ�ʼ������ʼ��һЩ������*/
	//avcodec_init(); 

	/*ע��CODEC*/
	avcodec_register_all(); 

	/*���� H264 CODEC*/
	codec = avcodec_find_decoder(CODEC_ID_H264);

	if (!codec) 
		return 0; 

	/*��ʼ��CODEC��Ĭ�ϲ���*/
	c = avcodec_alloc_context3(codec); 

	if(!c)  
		return 0;

	/*1. ��CODEC�������ʼ��H.264������������decode_init���غ���*/
	if (avcodec_open2(c, codec, NULL) < 0) 	
		return 0;  

	/*ΪAVFrame����ռ䣬������*/
	picture   = avcodec_alloc_frame();

	if(!picture) 	
		return 0;

	return 1;
} 

bool CH264Coder::InputFilePath( char *path)
{
	bool bRet = true;
    m_inFile = fopen(path,"rb");
	if (NULL == m_inFile)
	{
       bRet = false;
	}

    m_outFile = fopen("outFileYUV.yuv","wb");
	if (NULL != m_outFile)
	{
		bRet = false;
	}

    return bRet;

}

//�̺߳���
DWORD WINAPI translationPic1(LPVOID lpparam)
{
	CH264Coder* p = (CH264Coder*)lpparam;
	p->enCodeTransition( AV_CODEC_ID_H264, 0x01 );
	return 0;
}
/*
DWORD WINAPI translationPic2(LPVOID lpparam)
{
	CH264Coder* p = (CH264Coder*)lpparam;
	p->enCodeTransition( AV_CODEC_ID_H264, 0x02 );
	return 0;
}

DWORD WINAPI translationPic3(LPVOID lpparam)
{
	CH264Coder* p = (CH264Coder*)lpparam;
	p->enCodeTransition( AV_CODEC_ID_H264, 0x03 );
	return 0;
}
*/
void CH264Coder::SendNalBuffer(  )
{
 	int nalLen;			/*NAL ����*/
	//unsigned char* nalBuf;	/*H.264����*/
	
	int cnt=0;


    AVPacket avPkt;
	//avPkt.data = (uint8_t *)malloc(1024*1024*4*sizeof(uint8_t));
	//memset(&avPkt.data,0,sizeof(1024*1024*4*sizeof(uint8_t)));
	//avPkt.size = 0;

	/*�����ڴ棬����ʼ��Ϊ0*/
	//nalBuf = (unsigned char*)calloc (500*1024, sizeof(char));
    //memset(nalBuf,0,sizeof(500*1024*sizeof(char)));
	DWORD dwthread1;
	::CreateThread(NULL, 0, translationPic1, (LPVOID)this, 0, &dwthread1);  //����
	//DWORD dwthread2;
	//::CreateThread(NULL, 0, translationPic2, (LPVOID)this, 0, &dwthread2);
	//DWORD dwthread3;
	//::CreateThread(NULL, 0, translationPic3, (LPVOID)this, 0, &dwthread3);

	//enCodeTransition( AV_CODEC_ID_H264, ID );

	//SuspendThread(&dwthread1);
	/*3. �ر�CODEC���ͷ���Դ,����decode_end���غ���*/
	if(c) 
	{
		avcodec_close(c); 
		av_free(c);
		c = NULL;
	} 
	
	/*if (NULL != m_inFile)
	{
		fclose(m_inFile);
		m_inFile = NULL;
	}
	
	if (NULL != m_outFile)
	{
		fclose(m_outFile);
		m_outFile = NULL;
	}
	
	if (NULL != avPkt.data)
	{
		free(avPkt.data);
		avPkt.data = NULL;
	}*/
	
	//if (NULL != picture)
	//{
	//	free(picture);
	//	picture = NULL;
	//}

}

void CH264Coder::InitCamera()
{
	int n = 3; //camera num
	capture = cvCreateCameraCapture(n);
}
