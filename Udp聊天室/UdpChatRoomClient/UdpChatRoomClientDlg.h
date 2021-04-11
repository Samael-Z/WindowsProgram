
// UdpChatRoomClientDlg.h: 头文件
//

#pragma once
#include "UdpSocket.h"
#include "CLock.h"
#define   WM_HANDLEPKG   WM_USER + 1
// CUdpChatRoomClientDlg 对话框
class CUdpChatRoomClientDlg : public CDialogEx
{
// 构造
public:
	CUdpChatRoomClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDPCHATROOMCLIENT_DIALOG };
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


public:
	CListBox _LBOnlineUsers;
	CString _StrNick;
	CListBox _LBShowMsg;
	CString _PrivateTo;
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedLogout();
	CButton _RbPublicChat;
	CString _StrInput;

public:
	USERINFO   _ServerInfo;
	USERINFO   _SelfInfo;
	CLock     _Lock;

	list<PACKAGE>   _PackageInfo;
	HANDLE			_HRecv;
	SOCKET			_SocketSelf;
	CString         _StrSelPrivate;

protected:
	afx_msg LRESULT OnHandlepkg(WPARAM wParam, LPARAM lParam);
public:

    void OnS2CLogin(PACKAGE pkg);
    void OnS2CLogout(PACKAGE pkg);
    void OnS2CGroup(PACKAGE pkg);
    void OnS2CPrivate(PACKAGE pkg);
	afx_msg void OnDblclkLbOnlinuser();


	void  SendChatMsg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton _rbPrivate;
};
