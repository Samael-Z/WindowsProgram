
// CADView.h: CCADView 类的接口
//

#pragma once
#include <utility>
#include <math.h>

#include "IShape.h"
#include "CDrawEllipse.h"
#include "CDrawFiveStar.h"
#include "CDrawLine.h"
#include "CDrawRectangle.h"
#include <memory>
#include "CPenSet.h"
#include "CBrush.h"
#include "rota.h"
#include "COptMannager.h"
#include "CRotateOpt.h"
#include "CMoveOpt.h"
#include "CADDoc.h"
class CCADView : public CView
{
protected: // 仅从序列化创建
	CCADView() noexcept;
	DECLARE_DYNCREATE(CCADView)

// 特性
public:
	CCADDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:


	////保存所有图形的链表
	//CList<IShape*> m_LstIshapes;    
	//当前在绘制哪个图形就new一个该图形赋值给m_pCurrentIshape
	IShape*  m_pCurrentIShape;		
	//图形工厂，每次画图需要new一个对象出来，否则每次画出来的图形坐标点一样，始终只有一个坐标
	//我们需要在每次鼠标点击下去的时候new一个对象出来，然后就不用担心对象一样的问题，因为是new出来的对象
	//我们要做内存回收的问题就是delete，这次我们用只能指针就可以不用手动做内存管理了
	shared_ptr<IShapeFactory>  m_pShapeFactory;
	//  保存被选中的图形
	IShape* m_pSelectShape;
	//保存移动之后的起点
	CPoint  m_ptMoveBegin;

	COptMannager m_OptMgr;


	CPoint  m_ptOldBegin;
	CPoint  m_ptOldEnd;


	/*添加关于画笔的数据成员*/
	int			m_nPenStyle;
	int			m_nPenWidth;
	COLORREF	m_clrPenColor;

	/*添加关于画刷的数据成员*/
	int			m_nBrushStyle;
	int			m_nShadowStyle;
	COLORREF	m_varBrushColor;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLine();
    afx_msg void OnRect();
    afx_msg void OnStar();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnEllipse();
    afx_msg void OnPen();
    afx_msg void OnBrush();
    afx_msg void OnChange();
    afx_msg void OnDelete();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRote();
	afx_msg void OnSiglepen();
	afx_msg void OnSiglebrush();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	virtual void Serialize(CArchive& ar);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};

#ifndef _DEBUG  // CADView.cpp 中的调试版本
inline CCADDoc* CCADView::GetDocument() const
   { return reinterpret_cast<CCADDoc*>(m_pDocument); }
#endif

