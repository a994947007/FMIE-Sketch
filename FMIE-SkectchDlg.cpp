
// FMIE-SkectchDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FMIE-Skectch.h"
#include "FMIE-SkectchDlg.h"
#include "afxdialogex.h"
#include "FMIESketch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFMIESkectchDlg 对话框



CFMIESkectchDlg::CFMIESkectchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FMIESKECTCH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFMIESkectchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFMIESkectchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECT_SRC_FILE, &CFMIESkectchDlg::OnBnClickedSelectSrcFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CFMIESkectchDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CFMIESkectchDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFMIESkectchDlg 消息处理程序

BOOL CFMIESkectchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码


	//设置默认参数
	setDefaultValue(IDC_PACKET_NUM_LIMIT, _T("2000000"));
	setDefaultValue(IDC_FILTER_PKT_NUM, _T("1"));
	setDefaultValue(IDC_IDENTIFY_PKT_NUM, _T("3"));
	setDefaultValue(IDC_CUCKOO_ROW1, _T("10"));
	setDefaultValue(IDC_CUCKOO_COL1, _T("4"));
	setDefaultValue(IDC_CUCKOO_ROW2, _T("10"));
	setDefaultValue(IDC_CUCKOO_COL2, _T("4"));
	setDefaultValue(IDC_FILTER_SHRESHOLD, _T("16"));
	setDefaultValue(IDC_IDENTIFY_ROW, _T("1024"));
	setDefaultValue(IDC_IDENTIFY_COL, _T("4"));
	setDefaultValue(IDC_IDENTIFY_THRESHOLD, _T("5.0"));
	setDefaultValue(IDC_SKETCH_COUNT, _T("3"));
	setDefaultValue(IDC_SKETCH_SIZE, _T("2048"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFMIESkectchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFMIESkectchDlg::OnPaint()
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
HCURSOR CFMIESkectchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



inline void CFMIESkectchDlg::setDefaultValue(const ULONG controID, LPCTSTR value)
{
	((CEdit*)GetDlgItem(controID))->SetWindowTextW(value);
}

void CFMIESkectchDlg::OnBnClickedSelectSrcFile()
{
	CString strFile = _T("");
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CFileDialog dlgFile(TRUE, _T("cap"), NULL, dwFlags, _T("Ip trace(*.hsn)|*.hsn||"), NULL);
	if (dlgFile.DoModal() != IDOK)
	{
		return;
	}

	POSITION pos = dlgFile.GetStartPosition();
	while (NULL != pos) {
		CString strFileName = dlgFile.GetNextPathName(pos);
		string fileName = CT2A(strFileName);
		m_configInfo.fileList.push_back(fileName);

		CString oldMessage;
		GetDlgItem(IDC_MSG_AREA)->GetWindowTextW(oldMessage);
		GetDlgItem(IDC_MSG_AREA)->SetWindowTextW(oldMessage + strFileName + "\r\n");
	}
}


void CFMIESkectchDlg::OnBnClickedButton2()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("选择需要保存的路径");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl) {
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	string resultPath = CT2A(CString(szBuffer));
	m_configInfo.resultPath = resultPath;
}


void CFMIESkectchDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_configInfo.fileList.empty())
	{
		CString msg("您还没有选择数据源文件！");
		AfxMessageBox(msg);
		return;
	}

	m_configInfo.PACKET_NUM_LIMIT = getValueByControID<ULONG>(IDC_PACKET_NUM_LIMIT);
	m_configInfo.FILTER_PKT_NUM = getValueByControID<ULONG>(IDC_FILTER_PKT_NUM);
	m_configInfo.IDENTIFY_PKT_NUM = getValueByControID<ULONG>(IDC_IDENTIFY_PKT_NUM);
	m_configInfo.CUCKOO_ROW1 = (ULONG)pow(2, getValueByControID<ULONG>(IDC_CUCKOO_ROW1));
	m_configInfo.CUCKOO_COL1 = getValueByControID<ULONG>(IDC_CUCKOO_COL1);
	m_configInfo.CUCKOO_ROW2 = (ULONG)pow(2, getValueByControID<ULONG>(IDC_CUCKOO_ROW2));
	m_configInfo.CUCKOO_COL2 = getValueByControID<ULONG>(IDC_CUCKOO_COL2);
	m_configInfo.FILTER_SHRESHOLD = getValueByControID<ULONG>(IDC_FILTER_SHRESHOLD);
	m_configInfo.IDENTIFY_ROW = getValueByControID<ULONG>(IDC_IDENTIFY_ROW);
	m_configInfo.IDENTIFY_COL = getValueByControID<ULONG>(IDC_IDENTIFY_COL);
	m_configInfo.IDENTIFY_THRESHOLD = getValueByControID<double>(IDC_IDENTIFY_THRESHOLD);
	m_configInfo.SKETCH_COUNT = getValueByControID<ULONG>(IDC_SKETCH_COUNT);
	m_configInfo.SKETCH_SIZE = getValueByControID<ULONG>(IDC_SKETCH_SIZE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	FMIESketch sketch(m_configInfo);
	sketch.run();

	CString msg("统计完毕！");
	AfxMessageBox(msg);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}


template<typename T>
inline T CFMIESkectchDlg::getValueByControID(const ULONG controID)
{
	CString m_str;
	GetDlgItem(controID)->GetWindowTextW(m_str);
	CStringA m_strA(m_str);
	if (std::is_same<T, ULONG>::value)
	{
		UINT value = atoi(m_strA);
		return value;
	}
	else if((std::is_same<T, double>::value))
	{
		double value = atof(m_strA);
		return value;
	}
	return T();
}
