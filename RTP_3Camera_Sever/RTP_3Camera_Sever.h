
// RTP_3Camera_Sever.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRTP_3Camera_SeverApp:
// �йش����ʵ�֣������ RTP_3Camera_Sever.cpp
//

class CRTP_3Camera_SeverApp : public CWinApp
{
public:
	CRTP_3Camera_SeverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRTP_3Camera_SeverApp theApp;