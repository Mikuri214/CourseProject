
// ����ý��ʵ��.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImgApp: 
// �йش����ʵ�֣������ ����ý��ʵ��.cpp
//

class CImgApp : public CWinApp
{
public:
	CImgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImgApp theApp;