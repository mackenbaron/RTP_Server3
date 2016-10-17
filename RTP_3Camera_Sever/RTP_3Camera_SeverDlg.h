
// RTP_3Camera_SeverDlg.h : ͷ�ļ�
//

#pragma once
#include "H264Coder.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"


// CRTP_3Camera_SeverDlg �Ի���
class CRTP_3Camera_SeverDlg : public CDialogEx
{
// ����
public:
	CRTP_3Camera_SeverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RTP_3CAMERA_SEVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CH264Coder* h264Coder1;
	CH264Coder* h264Coder2;
	CH264Coder* h264Coder3;
	
	
	afx_msg void OnBnClickedButton1();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
