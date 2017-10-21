
// CImgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "����ý��ʵ��.h"
#include "CImgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

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


// CImgDlg �Ի���



CImgDlg::CImgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgDlg::IDD, pParent)
	, scale_rate ( 0 )
	, rotate_degree ( 0 )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Text ( pDX, IDC_EDIT_SCALE, scale_rate );
	DDX_Text ( pDX, IDC_EDIT_ROTATE, rotate_degree );
}

BEGIN_MESSAGE_MAP(CImgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED ( IDC_BUTTON_SELECT_FILE, &CImgDlg::OnBnClickedButtonSelectFile )
	ON_EN_CHANGE ( IDC_EDIT_SCALE, &CImgDlg::OnEnChangeEditScale )
	ON_EN_CHANGE ( IDC_EDIT_ROTATE, &CImgDlg::OnEnChangeEditRotate )
	ON_BN_CLICKED ( IDC_BUTTON_COLORSTEP, &CImgDlg::OnBnClickedButtonColorstep )
	ON_BN_CLICKED ( IDC_BUTTON_LIGHTBALANCE, &CImgDlg::OnBnClickedButtonLightbalance )
	ON_BN_CLICKED ( IDC_BUTTON_SKETCH, &CImgDlg::OnBnClickedButtonSketch )
	ON_BN_CLICKED ( IDC_BUTTON1, &CImgDlg::OnBnClickedButton1 )
	ON_BN_CLICKED ( IDC_BUTTON2, &CImgDlg::OnBnClickedButton2 )
	ON_BN_CLICKED ( IDC_BUTTON3, &CImgDlg::OnBnClickedButton3 )
END_MESSAGE_MAP ()


// CImgDlg ��Ϣ�������

BOOL CImgDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImgDlg::OnPaint()
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
HCURSOR CImgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CImgDlg::OnBnClickedButtonSelectFile ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR  szFilter[] = _T ( "JPEG(*.jpg)|*.jpg|BMP(*.jpg)|*.bmp|TIFF (*.tif)|*.tif|All Files (*.*)|*.*||" );
	CFileDialog fileOpenDlg ( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	if ( fileOpenDlg.DoModal () == IDOK )
	{
		CString imFilePath;
		VERIFY ( imFilePath = fileOpenDlg.GetPathName () );
		if ( m_pImgSrc != NULL )
		{
			m_pImgSrc->Destroy ();
			delete m_pImgSrc;
		}
		m_pImgSrc = new CImage ();
		m_pImgSrc->Load ( imFilePath );

		pdc= GetDC ();
		(*m_pImgSrc).StretchBlt ( pdc->m_hDC,40,40,m_pImgSrc->GetWidth(),m_pImgSrc->GetHeight() );
	}
}


void CImgDlg::OnEnChangeEditScale ()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE))//������
	{
		//����ͼƬ
		CImage dst ;
		dst.Create ( m_pImgSrc->GetWidth ()*scale_rate, m_pImgSrc->GetHeight ()*scale_rate, m_pImgSrc->GetBPP () );
		ThreadParam p;
		p.src = m_pImgSrc;
		p.dst = &dst;
		p.startIndex = 0;
		p.endIndex = dst.GetWidth () *dst.GetHeight () - 1;
		p.u.scale_rate = 1.0f/scale_rate;
		ImageProcess::scale ( (void*)(&p) );
		CDC* pDC = GetDC ();
		dst.StretchBlt ( pDC->m_hDC, 40, 40, dst.GetWidth (), dst.GetHeight () );
	}

}


void CImgDlg::OnEnChangeEditRotate ()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE))//������
	{
		//��תͼƬ
		CImage dst;

		float sinval = sin ( rotate_degree * 1.0 * M_PI * ( 1.0 / 180 ) );
		float cosval = cos ( rotate_degree * 1.0 * M_PI * ( 1.0/ 180)  );

		int nwidth = int(m_pImgSrc->GetWidth () * 1.0f * cosval + m_pImgSrc->GetHeight () * 1.0f * sinval + 0.5);
		int nheight = int ( m_pImgSrc->GetWidth () * 1.0f * sinval + m_pImgSrc->GetHeight () * 1.0f * cosval + 0.5 );

		dst.Create (  nwidth, nheight, m_pImgSrc->GetBPP () );
		ThreadParam p;
		p.src = m_pImgSrc;
		p.dst = &dst;
		p.startIndex = 0;
		p.endIndex = dst.GetWidth () *dst.GetHeight () - 1;
		p.u.rotate_arc = rotate_degree * 1.0 * M_PI * ( 1.0 / 180 );
		ImageProcess::rotate ( ( void* ) ( &p ) );
		CDC* pDC = GetDC ();
		dst.StretchBlt ( pDC->m_hDC, 40, 40, dst.GetWidth (), dst.GetHeight () );
	}
}


void CImgDlg::OnBnClickedButtonColorstep ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	autolevelThreadParam p;
	p.src = m_pImgSrc;
	p.dst = m_pImgSrc;
	p.startIndex = 0;
	p.endIndex = m_pImgSrc->GetWidth () *m_pImgSrc->GetHeight () - 1;

	long hist[3][256] = { 0 };
	Histogram ( m_pImgSrc, hist );

	byte* newmin = new byte[3];
	byte* newmax = new byte[3];
	byte* oldmin = new byte[3];
	byte* oldmax = new byte[3];

	static double lowCat = 0.01;
	static double hightCut = 0.01;
	lowCat += 0.001;
	hightCut += 0.001;
	getBounderValue ( hist, newmin, newmax, oldmin, oldmax, lowCat, hightCut, m_pImgSrc->GetWidth ()* m_pImgSrc->GetHeight () );

	p.newmin = newmin;
	p.newmax = newmax;
	p.oldmin = oldmin;
	p.oldmax = oldmax;

	ImageProcess::autoLevels ( ( void* ) ( &p ) );
	CDC* pDC = GetDC ();
	m_pImgSrc->StretchBlt ( pDC->m_hDC, 40, 40, m_pImgSrc->GetWidth (), m_pImgSrc->GetHeight () );
}


void CImgDlg::OnBnClickedButtonLightbalance ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ThreadParam p;
	p.src = m_pImgSrc;
	p.dst = m_pImgSrc;
	p.startIndex = 0;
	p.endIndex = m_pImgSrc->GetWidth () *m_pImgSrc->GetHeight () - 1;

	ImageProcess::autoWhiteBalance ( ( void* ) ( &p ) );
	CDC* pDC = GetDC ();
	m_pImgSrc->StretchBlt ( pDC->m_hDC, 40, 40, m_pImgSrc->GetWidth (), m_pImgSrc->GetHeight () );
}


void CImgDlg::OnBnClickedButtonSketch ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ThreadParam p;
	p.src = m_pImgSrc;
	CImage dst;
	dst.Create ( m_pImgSrc->GetWidth (), m_pImgSrc->GetHeight (), m_pImgSrc->GetBPP () );
	p.startIndex = 0;
	p.endIndex = m_pImgSrc->GetWidth () *m_pImgSrc->GetHeight () - 1;
	p.dst = &dst;
	ImageProcess::sketchTranslate ( ( void* ) ( &p ) );
	CDC* pDC = GetDC ();
	dst.StretchBlt ( pDC->m_hDC, 40, 40, dst.GetWidth (), dst.GetHeight () );
}


void CImgDlg::OnBnClickedButton1 ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CImage dd;
	dd.Create ( dst->GetWidth (), dst->GetHeight (),dst->GetBPP() );
	mixThreadParam p;
	p.img1= m_pImgSrc;
	p.img2 = dst;
	p.dst = &dd;
	p.startIndex = 0;
	p.endIndex = m_pImgSrc->GetWidth () *m_pImgSrc->GetHeight () - 1;
	p.alpha = 0.3;

	ImageProcess::mixture ( ( void* ) ( &p ) );
 pdc = GetDC ();
	dd.StretchBlt ( pdc->m_hDC, 500, 40, dd.GetWidth (), dd.GetHeight () );

}


void CImgDlg::OnBnClickedButton2 ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR  szFilter[] = _T ( "JPEG(*.jpg)|*.jpg|BMP(*.jpg)|*.bmp|TIFF (*.tif)|*.tif|All Files (*.*)|*.*||" );
	CFileDialog fileOpenDlg ( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	if ( fileOpenDlg.DoModal () == IDOK )
	{
		CString imFilePath;
		VERIFY ( imFilePath = fileOpenDlg.GetPathName () );
		dst = new CImage ();
		dst->Load ( imFilePath );


		dst->StretchBlt ( pdc->m_hDC, 200, 40, dst->GetWidth (), dst->GetHeight () );
	}

}


void CImgDlg::OnBnClickedButton3 ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CImage dsti;
	dsti.Create ( m_pImgSrc->GetWidth (), m_pImgSrc->GetHeight (), m_pImgSrc->GetBPP () );
	bbfThreadParam p;
	p.src = m_pImgSrc;
	p.dst = &dsti;
	p.startIndex = 0;
	p.endIndex = m_pImgSrc->GetWidth () *m_pImgSrc->GetHeight () - 1;
	p.r = 3;

	ImageProcess::Bilateral_Filter ( ( void* ) ( &p ) );
	CDC* pDC = GetDC ();
	dsti.StretchBlt ( pDC->m_hDC, 40, 40, dsti.GetWidth (), dsti.GetHeight () );
}
