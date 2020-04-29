
// CADView.h: CCADView 类的接口
//

#pragma once

#include <utility>
#include "IShape.h"
#include "CLine.h"
#include <stack>
#include "CMoveAction.h"
#include "IAction.h"
#include "CInsertAction.h"
#include "CActionManager.h"
#include "CRemoveAction.h"
using namespace std;
class CCADView : public CView
{
protected: // 仅从序列化创建
	CCADView() noexcept;
	DECLARE_DYNCREATE(CCADView)

// 特性
public:
	CCADDoc* GetDocument() const;
	
private:
	
	BOOL		m_bOnDraw = FALSE;	//是否开始画直线
	BOOL        m_MoveShape = FALSE;
	CPoint      m_MoveBegin;
	
	IShape* m_pCurrentShape =  NULL;
	IShape* m_pSelectShape = NULL;
	
	IFactory* m_pFactoryShape = new CLineFactory;


	int m_nViewPenStyle = PS_SOLID;
	int m_nViewPenWidth = 1;
	COLORREF m_clrViewPenColor = RGB(0,0,0);
	int m_nViewBrushStyle = BS_HOLLOW;
	int m_nViewShadowStyle = HS_CROSS;
	COLORREF m_clrViewBrushColor = RGB(0,255,0);

	CPoint    m_ptOriMoveBegin;
	//撤销重做类
	CActionManager m_actionmanager;
	

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

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIdbLine();
	afx_msg void OnIdbRect();
	afx_msg void OnIdbCircle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnIdbStar();
	afx_msg void OnSetting();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRotate();
    afx_msg void OnDelete();
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	virtual void Serialize(CArchive& ar);
};

#ifndef _DEBUG  // CADView.cpp 中的调试版本
inline CCADDoc* CCADView::GetDocument() const
   { return reinterpret_cast<CCADDoc*>(m_pDocument); }
#endif

