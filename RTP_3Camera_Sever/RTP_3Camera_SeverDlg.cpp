
// RTP_3Camera_SeverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RTP_3Camera_Sever.h"
#include "RTP_3Camera_SeverDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEST_IP  "211.211.211.32"
//#define DEST_IP  "127.0.0.1"
#define DEST_Port  3000

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
/*
DWORD WINAPI threadFunction1(LPVOID lpparam)
{
	CRTP_3Camera_SeverDlg* p = (CRTP_3Camera_SeverDlg*)lpparam;
	p->h264Coder1->SendNalBuffer( 0x01 );
	return 0;
}

DWORD WINAPI threadFunction2(LPVOID lpparam)
{
	CRTP_3Camera_SeverDlg* p = (CRTP_3Camera_SeverDlg*)lpparam;
	p->h264Coder2->SendNalBuffer( 0x02 );
	return 0;
}

DWORD WINAPI threadFunction3(LPVOID lpparam)
{
	CRTP_3Camera_SeverDlg* p = (CRTP_3Camera_SeverDlg*)lpparam;
	p->h264Coder3->SendNalBuffer( 0x03 );
	return 0;
}
*/





class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRTP_3Camera_SeverDlg �Ի���



CRTP_3Camera_SeverDlg::CRTP_3Camera_SeverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRTP_3Camera_SeverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	h264Coder1 = new CH264Coder;
	h264Coder2 = new CH264Coder;
	h264Coder3 = new CH264Coder;
}

void CRTP_3Camera_SeverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRTP_3Camera_SeverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRTP_3Camera_SeverDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CRTP_3Camera_SeverDlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRTP_3Camera_SeverDlg ��Ϣ�������

BOOL CRTP_3Camera_SeverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	h264Coder1->m_rtpTransport.SetRtpInfo(DEST_IP, DEST_Port);
	h264Coder1->InitDecoder();
	//h264Coder2->m_rtpTransport.SetRtpInfo(DEST_IP,3000);
	//h264Coder2->InitDecoder();
	//h264Coder3->m_rtpTransport.SetRtpInfo(DEST_IP,3000);
	//h264Coder3->InitDecoder();
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRTP_3Camera_SeverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRTP_3Camera_SeverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRTP_3Camera_SeverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRTP_3Camera_SeverDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CRTP_3Camera_SeverDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( 1 )
	{
		//SetTimer(1, 1000, NULL);
		h264Coder1->SendNalBuffer();
	}
	
	/*
	if( h264Coder1->capture )
	{
		//SetTimer(1, 1000, NULL);
		DWORD dwthread1;
		::CreateThread(NULL, 0, threadFunction1, (LPVOID)this, 0, &dwthread1);
	}
	if( h264Coder2->capture )
	{
		DWORD dwthread2;
		::CreateThread(NULL, 0, threadFunction2, (LPVOID)this, 0, &dwthread2);
	}
	if( h264Coder3->capture )
	{
		DWORD dwthread3;
		::CreateThread(NULL, 0, threadFunction3, (LPVOID)this, 0, &dwthread3);
	}
	*/
}


void CRTP_3Camera_SeverDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IplImage *frame, *frame_copy = NULL;
	static int cnt = 0;
	
	//DrawPicToHDC(frame, IDC_ShowImg);
	//cvSaveImage("D:\\OpenCV Project\\picture\\", frame);
	h264Coder1->SendNalBuffer();
	CDialogEx::OnTimer(nIDEvent);
}


