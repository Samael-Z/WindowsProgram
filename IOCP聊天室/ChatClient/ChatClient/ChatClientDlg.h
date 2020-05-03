
// ChatClientDlg.h: 头文件
//

#pragma once

#include "CTcpSocket.h"
#include "CMyChatServer.h"
#include "CJsonObject.hpp"
#include <string>
using namespace neb;
// CChatClientDlg 对话框
class CChatClientDlg : public CDialogEx
{
// 构造
public:
	CChatClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CTcpSocket m_TcpSocketClients;
	sockaddr_in m_siServer;
	static DWORD WINAPI ThreadProc(LPVOID lpParam);
	void  EnableSendUI(BOOL bEnable);
	CString m_csUserName;   //当前用户名
	CString m_csToUsername; //私聊对方用户名
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
    afx_msg void OnBnClickedLogin();
    afx_msg void OnBnClickedSend();
    CListCtrl m_listUser;
	
	afx_msg void OnClose();
    afx_msg void OnBnClickedLogout();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSendto();
	afx_msg void OnNMClickUser(NMHDR* pNMHDR, LRESULT* pResult);
};
