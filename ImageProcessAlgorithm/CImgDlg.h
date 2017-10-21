
// CImgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ImageProcess.h"

// CImgDlg �Ի���
class CImgDlg : public CDialogEx
{
// ����
public:
	CImgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CImage *m_pImgSrc;//ͼ��
	CImage *dst;
	CDC* pdc;
	double scale_rate;
	int rotate_degree;
	int m_nThreadNum;//�߳���
	ThreadParam* m_pThreadParam;


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectFile ();
	afx_msg void OnEnChangeEditScale ();

	afx_msg void OnEnChangeEditRotate ();
	afx_msg void OnBnClickedButtonColorstep ();
	afx_msg void OnBnClickedButtonLightbalance ();
	afx_msg void OnBnClickedButtonSketch ();
	afx_msg void OnBnClickedButton1 ();
	afx_msg void OnBnClickedButton2 ();
	afx_msg void OnBnClickedButton3 ();
};
