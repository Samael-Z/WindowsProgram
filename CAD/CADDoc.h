
// CADDoc.h: CCADDoc 类的接口
//

#include "IShape.h"

#pragma once
using namespace std;


class CCADDoc : public CDocument
{
protected: // 仅从序列化创建
	CCADDoc() noexcept;
	DECLARE_DYNCREATE(CCADDoc)
    //DECLARE_SERIAL(CCADDoc)
// 特性
public:
   

// 操作
public:

    CString  m_TreeSelect;
    //保存所有图形的链表
    CList<IShape*> m_LstIshapes;
    //当前在绘制哪个图形就new一个该图形赋值给m_pCurrentIshape
    //IShape* m_pCurrentIShape;
    ////图形工厂，每次画图需要new一个对象出来，否则每次画出来的图形坐标点一样，始终只有一个坐标
    ////我们需要在每次鼠标点击下去的时候new一个对象出来，然后就不用担心对象一样的问题，因为是new出来的对象
    ////我们要做内存回收的问题就是delete，这次我们用只能指针就可以不用手动做内存管理了
    //shared_ptr<IShapeFactory>  m_pShapeFactory;
    ////  保存被选中的图形
    //IShape* m_pSelectShape;
    ////保存移动之后的起点
    //CPoint  m_ptMoveBegin;

    //COptMannager m_OptMgr;


    //CPoint  m_ptOldBegin;
    //CPoint  m_ptOldEnd;


    ///*添加关于画笔的数据成员*/
    //int			m_nPenStyle;
    //int			m_nPenWidth;
    //COLORREF	m_clrPenColor;

    ///*添加关于画刷的数据成员*/
    //int			m_nBrushStyle;
    //int			m_nShadowStyle;
    //COLORREF	m_varBrushColor;

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

private:
    

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
