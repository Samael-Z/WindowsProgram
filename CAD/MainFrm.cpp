
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "CAD.h"

#include "MainFrm.h"
#include "CadLeftTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // TODO: 在此添加专用代码和/或调用基类


	CRect  rc;
	GetClientRect(&rc);
	m_SplitterCad.CreateStatic(this, 1, 2);
    m_SplitterCad.CreateView(
        0,
        0,
        RUNTIME_CLASS(CadLeftTreeView),
		SIZE{ rc.Width() / 5, rc.Height() },
        pContext);
    m_SplitterCad.CreateView(
        0,
        1,
        RUNTIME_CLASS(CCADView),
        SIZE{ rc.Width(), rc.Height() },
        pContext);

	return true;
   // return CFrameWnd::OnCreateClient(lpcs, pContext);
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类

    if (CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;
    CCADDoc* pDoc = (CCADDoc*)GetActiveDocument();
	
    if (pDoc == NULL)
    {
        return FALSE;
    }
	POSITION  pos = pDoc->GetFirstViewPosition();
    while (pos != NULL)
    {
        CView* pNextView = pDoc->GetNextView(pos);
        if (pNextView != GetActiveView())
        {
            if (pNextView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
                return TRUE;
        }
    }

    return FALSE;


	//return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
