
// CADView.cpp: CCADView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CAD.h"
#endif

#include "CADDoc.h"
#include "CADView.h"
#include "CEllipse.h"
#include "CLine.h"
#include "CRectangle.h"
#include "CStar.h"
#include "CDrawSetting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCADView

IMPLEMENT_DYNCREATE(CCADView, CView)

BEGIN_MESSAGE_MAP(CCADView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDB_LINE, &CCADView::OnIdbLine)
	ON_COMMAND(IDB_RECT, &CCADView::OnIdbRect)
	ON_COMMAND(IDB_CIRCLE, &CCADView::OnIdbCircle)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDB_STAR, &CCADView::OnIdbStar)
	ON_COMMAND(IDM_SETTING, &CCADView::OnSetting)
    ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
    ON_COMMAND(IDB_ROTATE, &CCADView::OnRotate)
	ON_COMMAND(IDB_DELETE, &CCADView::OnDelete)
	ON_COMMAND(ID_EDIT_REDO, &CCADView::OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, &CCADView::OnEditUndo)
END_MESSAGE_MAP()

// CCADView 构造/析构

CCADView::CCADView() noexcept
{
	// TODO: 在此处添加构造代码
	

}


CCADView::~CCADView()
{
	if (m_pFactoryShape!= nullptr)
	{
		delete m_pFactoryShape;
		m_pFactoryShape = nullptr;
	}
	
}

BOOL CCADView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCADView 绘图

void CCADView::OnDraw(CDC* pDC)
{
	CCADDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmpMem;
	CRect rect;
	GetClientRect(&rect);
	bmpMem.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	
	dcMem.SelectObject(&bmpMem);
	dcMem.FillSolidRect(&rect,RGB(255,255,255));
	
	//画笔设置
	
	//画刷设置
	
	// TODO: 在此处为本机数据添加绘制代码
	
		POSITION pos = GetDocument()->m_listShapes.GetHeadPosition();
		while (pos)
		{
			IShape* pShape = GetDocument()->m_listShapes.GetNext(pos);
			if (pShape == m_pSelectShape)
			{
				pShape->OnselectDraw(&dcMem);
			}
			else
			{
				pShape->OnDraw(&dcMem);
			}
			
		}

		if (m_bOnDraw)
		{
			m_pCurrentShape->OnDraw(&dcMem);
		}
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
		//dcMem.SelectObject(&hOldPen);
}


// CCADView 打印

BOOL CCADView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCADView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCADView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
	
}


// CCADView 诊断

#ifdef _DEBUG
void CCADView::AssertValid() const
{
	CView::AssertValid();
}

void CCADView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCADDoc* CCADView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCADDoc)));
	return (CCADDoc*)m_pDocument;
}
#endif //_DEBUG


// CCADView 消息处理程序


void CCADView::OnIdbLine()
{
	if (m_pFactoryShape!= nullptr)
	{
		delete m_pFactoryShape;
		m_pFactoryShape = nullptr;
	}
	m_pFactoryShape = new CLineFactory;
}


void CCADView::OnIdbRect()
{
	if (m_pFactoryShape != nullptr)
	{
		delete m_pFactoryShape;
		m_pFactoryShape = nullptr;
	}
	m_pFactoryShape = new CRectangleFactory;
}


void CCADView::OnIdbCircle()
{
	if (m_pFactoryShape != nullptr)
	{
		delete m_pFactoryShape;
		m_pFactoryShape = nullptr;
	}
	m_pFactoryShape = new CEllipseFactory;
	
}


void CCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

		m_pCurrentShape = m_pFactoryShape->CreateShape();
		m_pCurrentShape->SetPenStyle(m_nViewPenStyle);
		m_pCurrentShape->SetPenColor(m_clrViewPenColor);
		m_pCurrentShape->SetPenWidt(m_nViewPenWidth);
		m_pCurrentShape->SetBrushColor(m_clrViewBrushColor);
		m_pCurrentShape->SetShadowStyle(m_nViewShadowStyle);
		m_pCurrentShape->SetBrushStyle(m_nViewBrushStyle);
		m_pCurrentShape->SetBeginPoint(point);
		m_bOnDraw = TRUE;
		SetCapture();		
	
		
	CView::OnLButtonDown(nFlags, point);
}


void CCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bOnDraw)
	{
		m_pCurrentShape->SetEndPoint(point);
		InvalidateRect(NULL, FALSE);
		ReleaseCapture();
		m_bOnDraw = FALSE;
		GetDocument()->m_listShapes.AddTail(m_pCurrentShape);
	}
	IAction* pAction = new CInsertAction(m_pCurrentShape, &GetDocument()->m_listShapes);
	m_actionmanager.InsertAction(pAction);

	CView::OnLButtonUp(nFlags, point);
}


void CCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_MoveShape)
	{
		m_pSelectShape->MoveShape(point.x - m_MoveBegin.x, point.y-m_MoveBegin.y);
		InvalidateRect(NULL,FALSE);
		m_MoveBegin = point;
	}
	if (m_bOnDraw)
	{
		m_pCurrentShape->SetEndPoint(point);
		InvalidateRect(NULL,FALSE);
	}
		
	
	

	CView::OnMouseMove(nFlags, point);
}


void CCADView::OnIdbStar()
{
	if (m_pFactoryShape != nullptr)
	{
		delete m_pFactoryShape;
		m_pFactoryShape = nullptr;
	}
	m_pFactoryShape = new CStarFactory;
}


void CCADView::OnSetting()
{
	CDrawSetting  Dlg;
	Dlg.SetPenColor(m_clrViewPenColor);
	Dlg.SetPenStyle(m_nViewPenStyle);
	Dlg.SetPenWidt(m_nViewPenWidth);
	Dlg.SetBrushColor(m_clrViewBrushColor);
	Dlg.SetBrushStyle(m_nViewBrushStyle);
	Dlg.SetShadowStyle(m_nViewShadowStyle);
	if (Dlg.DoModal() == IDOK)
	{
		m_clrViewPenColor = Dlg.GetPenColor();
		m_nViewPenStyle = Dlg.GetPenStyle();
		m_nViewPenWidth = Dlg.GetPenWidth();
		m_clrViewBrushColor = Dlg.GetBrushColor();
		m_nViewBrushStyle = Dlg.GetBrushStyle();
		m_nViewShadowStyle = Dlg.GetShadowStyle();
	}
}


void CCADView::OnRButtonDown(UINT nFlags, CPoint point)
{
	POSITION pos = GetDocument()->m_listShapes.GetTailPosition();
	while (pos)
	{
		IShape* pShape = GetDocument()->m_listShapes.GetPrev(pos);
		if ( pShape->ISelect(point) )
		{
			m_pSelectShape = pShape;
			InvalidateRect(NULL,false);
			m_MoveShape = TRUE;
			m_MoveBegin = point;
			m_ptOriMoveBegin = point;
			break;
		}
	}

    CView::OnRButtonDown(nFlags, point);
}


void CCADView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_MoveShape = FALSE;
	//把移动图形的操作放到栈中，等待被撤销
	CMoveAction* pMoveAction = new CMoveAction(m_pSelectShape, point, m_ptOriMoveBegin);
	m_actionmanager.InsertAction(pMoveAction);
	CView::OnRButtonUp(nFlags, point);
}


void CCADView::OnRotate()
{
    // TODO: 在此添加命令处理程序代码
	if (m_pSelectShape!= NULL)
	{
		m_pSelectShape->RotateShape();
		InvalidateRect(NULL, FALSE);
	}
}


void CCADView::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pSelectShape!=NULL)
	{
		
		IAction* pAction = new CRemoveAction(m_pSelectShape, &GetDocument()->m_listShapes);
		m_actionmanager.InsertAction(pAction);

		GetDocument()->m_listShapes.RemoveAt(GetDocument()->m_listShapes.Find(m_pSelectShape));
		
		InvalidateRect(NULL, FALSE);
		/*delete m_pSelectShape;
		m_pSelectShape = nullptr;*/
	}
}


void CCADView::OnEditRedo()
{
	// TODO: 在此添加命令处理程序代码

	m_actionmanager.redo();
	InvalidateRect(NULL, FALSE);

}


void CCADView::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	m_actionmanager.undo();
	InvalidateRect(NULL, FALSE);
}


void CCADView::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {	// storing code
	/*	POSITION pos = m_listShapes.GetHeadPosition();*/

    }
    else
    {	// loading code
    }
}
