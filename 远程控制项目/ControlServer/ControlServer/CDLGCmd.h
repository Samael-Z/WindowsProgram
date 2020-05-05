#pragma once


// CDLGCmd 对话框
#include "CTcpSocket.h"

class CDLGCmd : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGCmd)

public:
	CDLGCmd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLGCmd();
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_CMD };
#endif
public:
	//static DWORD WINAPI RecvPackageThreadProc(LPVOID lpParam);
	CTcpSocket* m_pSocket;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCertain();
	afx_msg	LRESULT OnRecvCMD(WPARAM lpWparam, LPARAM lpLparam);
	CEdit m_edtCmd;
	CEdit m_edtCmdShow;
	virtual BOOL OnInitDialog();
};
