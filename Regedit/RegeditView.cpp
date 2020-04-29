
// RegeditView.cpp: CRegeditView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Regedit.h"
#endif

#include "RegeditDoc.h"
#include "RegeditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRegeditView

IMPLEMENT_DYNCREATE(CRegeditView, CView)

BEGIN_MESSAGE_MAP(CRegeditView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CRegeditView 构造/析构

CRegeditView::CRegeditView() noexcept
{
	// TODO: 在此处添加构造代码

}

CRegeditView::~CRegeditView()
{
}

BOOL CRegeditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CRegeditView 绘图

void CRegeditView::OnDraw(CDC* /*pDC*/)
{
	CRegeditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CRegeditView 打印

BOOL CRegeditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CRegeditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CRegeditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CRegeditView 诊断

#ifdef _DEBUG
void CRegeditView::AssertValid() const
{
	CView::AssertValid();
}

void CRegeditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRegeditDoc* CRegeditView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRegeditDoc)));
	return (CRegeditDoc*)m_pDocument;
}
#endif //_DEBUG


// CRegeditView 消息处理程序
