#pragma once


// CMyListView 视图
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
class CMyListView : public CListView
{
	DECLARE_DYNCREATE(CMyListView)

protected:
	CMyListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void EnumValues(HKEY hKey, int nValueCount);
private:

	//数据类型
	const CString m_szRegType[12] = {
		_T("REG_NONE"),
		_T("REG_SZ"),
		_T("REG_EXPAND_SZ"),
		_T("REG_BINARY"),
		_T("REG_DWORD"),
		_T("REG_DWORD_BIG_ENDIAN"),
		_T("REG_LINK"),
		_T("REG_MULTI_SZ"),
		_T("REG_RESOURCE_LIST"),
		_T("REG_FULL_RESOURCE_DESCRIPTOR"),
		_T("REG_RESOURCE_REQUIREMENTS_LIST"),
		_T("REG_QWORD")
	};
	//格式化数值
	CString RegDataToStr(BYTE* lpData, DWORD lpcbData, UINT nID);
public:
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAlter();
};


