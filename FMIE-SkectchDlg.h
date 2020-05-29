
// FMIE-SkectchDlg.h: 头文件
//

#pragma once

#include "UserConfig.h"


// CFMIESkectchDlg 对话框
class CFMIESkectchDlg : public CDialogEx
{
// 构造
public:
	CFMIESkectchDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FMIESKECTCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	UserConfig m_configInfo;

private:
	template <typename T>  
	inline T getValueByControID(const ULONG controID);
	inline void setDefaultValue(const ULONG controID, LPCTSTR value);

public:
	afx_msg void OnBnClickedSelectSrcFile();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
};

