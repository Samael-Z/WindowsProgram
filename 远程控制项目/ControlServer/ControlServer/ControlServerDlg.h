
// ControlServerDlg.h: 头文件
//

#pragma once


#include "CFuntionDlg.h"

//自定义消息
enum MYMSG
{
	WM_NEWCLIENT = WM_USER + 1,		//新的客户端链接
	WM_CLIENTOUT,					//客户端退出
	WM_CMD,							//获取cmd命令行
	WM_PROCESS,						//获取进程列表
	WM_GETREGLST,					//获取注册表的左侧列表

};


// CControlServerDlg 对话框
class CControlServerDlg : public CDialogEx
{
// 构造
public:
	CControlServerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	BOOL EnableUI(BOOL bVal);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:


	
	CTcpSocket m_tcpServer;
	static DWORD WINAPI AcceptThreadProc(LPVOID lpParam);
	BOOL m_bStartServer = TRUE;
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_listUserClient;
    afx_msg void OnBnClickedStart();
    afx_msg void OnBnClickedStop();
	afx_msg void OnNMDblclkClients(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedRefresh();
	afx_msg	LRESULT OnNewClient(WPARAM lpWparam,LPARAM lpLparam);

	afx_msg LRESULT OnClientOut(WPARAM lpWparam, LPARAM lpLparam);
};
