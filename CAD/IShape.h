#pragma once
#include "framework.h"
class IShape:public CObject
{
  
public:
    IShape();
    //构建一个绘图的OnDraw纯虚函数基类，这个函数的实现交给具体的图形去完成
    //充分的体现了C++的多态，封装，继承
    //写虚函数的地方
    virtual void OnDraw(CDC* pDC) = 0;
    virtual BOOL IsSelect(CPoint pt) = 0;
    virtual void OnSelectDraw(CDC* pDC) = 0;
            

private:
    //绘制图形的两个坐标点，鼠标按下和弹起的坐标
    CPoint m_ptBegin;
    CPoint m_ptEnd;
    //画笔的变量
    int m_nPenStyle;
    int m_nPenWith;
    COLORREF m_clrPenColor;

    //画刷的变量
    int			m_nBrushStyle;
    int			m_nShadowStyle;
    COLORREF	m_varBrushColor;


public:
    //变量的Get和Set函数
    void    SetBeginPt(CPoint point);
    CPoint  GetBeginPt() const;
    void    SetEndPt(CPoint point);
    CPoint  GetEndPt() const;

    /*在图形基类添加关于画笔的数据成员，让每个派生类图形自己选择属于自己的画笔去绘图*/
    int GetPenStyle() const { return m_nPenStyle; }
    void SetPenStyle(int val) { m_nPenStyle = val; }
    int GetPenWith() const { return m_nPenWith; }
    void SetPenWith(int val) { m_nPenWith = val; }
    COLORREF GetClrPenColor() const { return m_clrPenColor; }
    void SetClrPenColor(COLORREF val) { m_clrPenColor = val; }

    int GetBrushStyle() const { return m_nBrushStyle; }
    void SetBrushStyle(int val) { m_nBrushStyle = val; }
    int GetShadowStyle() const { return m_nShadowStyle; }
    void SetShadowStyle(int val) { m_nShadowStyle = val; }
    COLORREF GetClrBrushColor() const { return m_varBrushColor; }
    void SetClrBrushColor(COLORREF val) { m_varBrushColor = val; }

    virtual void Serialize(CArchive& ar);
};

class IShapeFactory
{
public:

    virtual IShape* CreateIShape() = 0;
};




