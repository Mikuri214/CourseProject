
// CImgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "数字媒体实验.h"
#include "CImgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CImgDlg 对话框



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


// CImgDlg 消息处理程序

BOOL CImgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CImgDlg::OnBnClickedButtonSelectFile ()
{
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (UpdateData(TRUE))//有输入
	{
		//缩放图片
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (UpdateData(TRUE))//有输入
	{
		//旋转图片
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
