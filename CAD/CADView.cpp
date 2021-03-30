
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
#include "CDelOpt.h"
#include "CAddOpt.h"

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
    ON_COMMAND(MN_LINE, &CCADView::OnLine)
	ON_COMMAND(MN_RECT, &CCADView::OnRect)
	ON_COMMAND(MN_STAR, &CCADView::OnStar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(MN_ELLIPSE, &CCADView::OnEllipse)
	ON_COMMAND(MN_PEN, &CCADView::OnPen)
	ON_COMMAND(MN_BRUSH, &CCADView::OnBrush)
    ON_COMMAND(MN_CHANGE, &CCADView::OnChange)
	ON_COMMAND(MN_DELETE, &CCADView::OnDelete)
	ON_WM_RBUTTONDOWN()
    ON_COMMAND(MN_ROTE, &CCADView::OnRote)
	ON_COMMAND(MN_SIGLEPEN, &CCADView::OnSiglepen)
	ON_COMMAND(MN_SIGLEBRUSH, &CCADView::OnSiglebrush)
    ON_COMMAND(ACCE_UNDO, &CCADView::OnUndo)
	ON_COMMAND(ACCE_REDO, &CCADView::OnRedo)
END_MESSAGE_MAP()

// CCADView 构造/析构

CCADView::CCADView() noexcept
{
	// TODO: 在此处添加构造代码
	m_pCurrentIShape = nullptr;
	m_pShapeFactory = nullptr;
	m_pSelectShape = nullptr;
	m_nPenStyle = PS_SOLID;
	m_nPenWidth = 1;
	m_clrPenColor = RGB(0, 0, 0);
	
	
	m_nBrushStyle = 0;
	m_nShadowStyle = 0;
	m_varBrushColor = RGB(0, 0, 0);

}

CCADView::~CCADView()
{
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

	// TODO: 在此处为本机数据添加绘制代码
	/*  0. 创建内存(兼容)DC
		1. 创建兼容位图
		2. 将兼容位图选入内存DC
		3. 在内存DC中绘图
		4. 将内存DC中的图片拷贝到窗口
	*/
	CRect  cClientRc;
	GetClientRect(&cClientRc);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap cBtmap;
	cBtmap.CreateCompatibleBitmap(pDC, cClientRc.Width(), cClientRc.Height());
	dcMem.SelectObject(&cBtmap);
	dcMem.FillSolidRect(&cClientRc, RGB(255, 255, 255));
	
	

	
	auto pos = GetDocument()->m_LstIshapes.GetHeadPosition();
	while (pos)
	{
		auto  IShapes = GetDocument()->m_LstIshapes.GetNext(pos);
		if (IShapes == m_pSelectShape)
		{
			//如果是选中的图形就重绘，不是就画原来的图形
			IShapes->OnSelectDraw(&dcMem);
		}
		else
		{
			IShapes->OnDraw(&dcMem);
		}

	}
	if (m_pCurrentIShape != nullptr)
	{
		m_pCurrentIShape->OnDraw(&dcMem);
	}
	
	pDC->BitBlt(0, 0, cClientRc.Width(), cClientRc.Height(), &dcMem, 0, 0, SRCCOPY);
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


void CCADView::OnLine()
{
  
	
	m_pShapeFactory = shared_ptr<IShapeFactory>(new CDrawLineFactory);
   
}

void CCADView::OnRect()
{
	m_pShapeFactory = shared_ptr<IShapeFactory>(new CDrawRectangleFactory);
}


void CCADView::OnStar()
{
	m_pShapeFactory = shared_ptr<IShapeFactory>(new CDrawFiveStarFactory);
}

void CCADView::OnEllipse()
{
	// 绘制椭圆
	m_pShapeFactory = shared_ptr<IShapeFactory>(new CDrawEllipseFactory);
}

void CCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 创建图形对象,设置起点
	if (m_pShapeFactory == nullptr)
	{
		return;
	}

	if (nFlags & MK_CONTROL) //如果ctrl按下就选中图形，否则绘制图形
	{
        auto pos = GetDocument()->m_LstIshapes.GetTailPosition();
        while (pos)
        {
			auto Shape = GetDocument()->m_LstIshapes.GetPrev(pos);
            if (Shape->IsSelect(point) )
            {
				m_pSelectShape = Shape;
				m_ptMoveBegin = point;

                m_ptOldBegin =	m_pSelectShape->GetBeginPt();
                m_ptOldEnd =	m_pSelectShape->GetEndPt();
				break;
            }
        }
		InvalidateRect(NULL, FALSE);
		
	} 
	else
	{
		m_pCurrentIShape =m_pShapeFactory->CreateIShape();
		m_pCurrentIShape->SetBeginPt(point);
        SetCapture();
        //设置画笔样式
		m_pCurrentIShape->SetPenStyle(		m_nPenStyle);
		m_pCurrentIShape->SetPenWith(		m_nPenWidth);
		m_pCurrentIShape->SetClrPenColor(	m_clrPenColor);

        //设置画刷样式
		m_pCurrentIShape->SetBrushStyle(	m_nBrushStyle);
		m_pCurrentIShape->SetShadowStyle(	m_nShadowStyle);
		m_pCurrentIShape->SetClrBrushColor(	m_varBrushColor);
	}

	

	CView::OnLButtonDown(nFlags, point);
}

void CCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pCurrentIShape == nullptr)
	{
		return;
	}
  
	
    if (nFlags & MK_CONTROL)
    {
		CMoveOpt* Popt = new CMoveOpt(
			m_pSelectShape,
			m_ptOldBegin,
			m_ptOldEnd,
			m_pSelectShape->GetBeginPt(),
			m_pSelectShape->GetEndPt());
			m_OptMgr.AddOpt(Popt);
        return;
    }

	m_pCurrentIShape->SetEndPt(point);
	CAddOpt* pOpt = new CAddOpt(
		m_pCurrentIShape,
		&(GetDocument()->m_LstIshapes));
		m_OptMgr.AddOpt(pOpt);

	ReleaseCapture();
	GetDocument()->m_LstIshapes.AddTail(m_pCurrentIShape);

	
	m_pCurrentIShape = nullptr;
	InvalidateRect(NULL, false);
	CView::OnLButtonUp(nFlags, point);
}


void CCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	//移动之前把原来的位置保存下来

	if (nFlags& MK_LBUTTON&& m_pSelectShape !=nullptr && nFlags & MK_CONTROL)
	{
		
        CPoint nOffset;  //保存偏移的距离
        nOffset.x = point.x - m_ptMoveBegin.x;
        nOffset.y = point.y - m_ptMoveBegin.y;

        CPoint ptNewBegin = m_pSelectShape->GetBeginPt();
		//获取原来的起点加上偏移就是新起点。
        ptNewBegin += nOffset;
		m_pSelectShape->SetBeginPt(ptNewBegin);

        CPoint ptNewEnd =m_pSelectShape->GetEndPt();
        ptNewEnd += nOffset;
		m_pSelectShape->SetEndPt(ptNewEnd);
		//保存新起点，不然没法移动
		m_ptMoveBegin = point;

		
		InvalidateRect(NULL, false);
		return;

	}
	if (nFlags & MK_LBUTTON && m_pCurrentIShape != nullptr)
	{

		m_pCurrentIShape->SetEndPt(point);
        InvalidateRect(NULL, false);
	}

	CView::OnMouseMove(nFlags, point);
}




void CCADView::OnPen()
{
	CPenSet  PenSetStyle;
	//保存上一次选择的画笔
	PenSetStyle.m_PenStyle =	m_nPenStyle;
	PenSetStyle.m_edtWidth =	m_nPenWidth;
	PenSetStyle.m_PenClr =		m_clrPenColor;

	PenSetStyle.DoModal();
	//得到更改后的画笔属性
	m_nPenStyle =	PenSetStyle.m_PenStyle;
	m_nPenWidth =	PenSetStyle.m_edtWidth;
	m_clrPenColor = PenSetStyle.m_PenClr;

}


void CCADView::OnBrush()
{
	CBrushSet BrushSetStyle;

	BrushSetStyle.m_nBrushStyle =	m_nBrushStyle;
	BrushSetStyle.m_nShadowStyle =	m_nShadowStyle;
	BrushSetStyle.m_varBrushColor = m_varBrushColor;

	BrushSetStyle.DoModal();
	m_nBrushStyle =		BrushSetStyle.m_nBrushStyle;
	m_nShadowStyle =	BrushSetStyle.m_nShadowStyle;
	m_varBrushColor =	BrushSetStyle.m_varBrushColor;

}


void CCADView::OnChange()
{
	//旋转实现的接口，放到右键菜单上去了
}


void CCADView::OnDelete()
{


  
	if (m_pSelectShape != nullptr)
	{
		//查找索引值
		int nIdx = 0;
		auto pos = GetDocument()->m_LstIshapes.GetHeadPosition();
		while (pos)
		{
			auto pShap = GetDocument()->m_LstIshapes.GetNext(pos);
			if (pShap == m_pSelectShape)
			{
				break;
			}

			++nIdx;
		}

        //保存删除操作
        CDelOpt* pDelOpt = new CDelOpt(
			m_pSelectShape,
			nIdx,
			&(GetDocument()->m_LstIshapes)
			);
		m_OptMgr.AddOpt(pDelOpt);

	}


	//删除操作
    if (m_pSelectShape != NULL)
    {
		auto pos = GetDocument()->m_LstIshapes.Find(m_pSelectShape);
		GetDocument()->m_LstIshapes.RemoveAt(pos);
        

        /*   delete m_pSelectShape;
           m_pSelectShape = nullptr;
           m_pCurrentIShape = nullptr;*/
	
    }

	InvalidateRect(NULL, FALSE);
}


void CCADView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获得当前点击的坐标，加载菜单资源，得到子菜单的返回值，弹出菜单
	CPoint pt;
	GetCursorPos(&pt);
    CMenu mnRMenu;
    mnRMenu.LoadMenuA(IDR_MENU1);
	CMenu* m_SubMenu = mnRMenu.GetSubMenu(0);
	m_SubMenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);

	CView::OnRButtonDown(nFlags, point);
}


void CCADView::OnRote()
{
	auto BeginPt =	m_pSelectShape->GetBeginPt();
	auto EndPt =	m_pSelectShape->GetEndPt();

	rotate(BeginPt, EndPt, PI / 2);

	//保存旋转操作
	auto pOpt = new CRotateOpt(
		m_pSelectShape,
		m_pSelectShape->GetBeginPt(),
		m_pSelectShape->GetEndPt(),
		BeginPt,
		EndPt);
	m_OptMgr.AddOpt(pOpt);


	m_pSelectShape->SetBeginPt(BeginPt);
	m_pSelectShape->SetEndPt(EndPt);
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnSiglepen()
{
    CPenSet  PenSetStyle;
    //保存上一次选择的画笔
    PenSetStyle.m_PenStyle = m_nPenStyle;
    PenSetStyle.m_edtWidth = m_nPenWidth;
    PenSetStyle.m_PenClr =   m_clrPenColor;

    PenSetStyle.DoModal();
    //得到更改后的画笔属性
 

    m_pSelectShape->SetClrPenColor(PenSetStyle.m_PenClr);
    m_pSelectShape->SetPenStyle(PenSetStyle.m_PenStyle);
    m_pSelectShape->SetPenWith(PenSetStyle.m_edtWidth);
	InvalidateRect(NULL, FALSE);

}


void CCADView::OnSiglebrush()
{
	
    CBrushSet BrushSetStyle;

    BrushSetStyle.m_nBrushStyle =	m_nBrushStyle;
    BrushSetStyle.m_nShadowStyle =	m_nShadowStyle;
    BrushSetStyle.m_varBrushColor = m_varBrushColor;

    BrushSetStyle.DoModal();



	m_pSelectShape->SetBrushStyle(BrushSetStyle.m_nBrushStyle);
	m_pSelectShape->SetClrBrushColor(BrushSetStyle.m_varBrushColor);
	m_pSelectShape->SetShadowStyle(BrushSetStyle.m_nShadowStyle);

	InvalidateRect(NULL, FALSE);

}


void CCADView::OnUndo()
{
   //撤销操作。动作管理类
	m_OptMgr.Undo();
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnRedo()
{
	//恢复操作，动作管理类
	m_OptMgr.Redo();
	InvalidateRect(NULL, FALSE);
}


void CCADView::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code

	}
	else
	{	// loading code

	}
}


void CCADView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    // TODO: 在此添加专用代码和/或调用基类
	auto strSlecText = GetDocument()->m_TreeSelect;
	if (strSlecText == "直线")
	{
		OnLine();
	}
    else if (strSlecText == "矩形")
    {
		OnRect();
    }
    else if (strSlecText == "椭圆")
    {
		OnEllipse();
    }
    else if (strSlecText == "五角星")
    {
		OnStar();
	}
    else if (strSlecText == "画笔设置")
    {
		OnPen();
    }
    else if (strSlecText == "画刷设置")
    {
		OnBrush();
    }

}


BOOL CCADView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	

   /* if (CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

    CWandererDoc* pDoc = (CWandererDoc*)GetActiveDocument();
    if (pDoc != NULL)
        POSITION pos = pDoc->GetFirstViewPosition();

    while (pos != NULL)
    {
        CView* pNextView = pDoc->GetNextView(pos);
        if (pNextView != GetActiveView())
        {
            if (pNextView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
                return TRUE;
        }
    }

    return FALSE;*/




	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
