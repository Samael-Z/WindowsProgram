#pragma once
#include <afxwin.h>

class IShape :public CObject
{
public:
    IShape();
    virtual ~IShape();
    CPoint GetBeginPoint() const { return m_ptBegin; }
    CPoint GetEndPoint() const { return m_ptEnd; }
    void   SetBeginPoint(CPoint val) { m_ptBegin = val; }
    void   SetEndPoint(CPoint val) { m_ptEnd = val; }
	void  MoveShape(int offsetX, int offsetY);
	void RotateShape();
    virtual void OnDraw(CDC* pDC) = 0;
	virtual bool ISelect(CPoint pt) = 0;
	virtual void OnselectDraw(CDC* pDC) = 0;
	virtual void Serialize(CArchive& ar) override;
protected:
    CPoint  m_ptBegin;
    CPoint  m_ptEnd;
    

public:
	int GetPenStyle() const { return m_nPenStyle; }
	int GetPenWidth() const { return m_nPenWidth; }
	int GetBrushStyle() const { return m_nBrushStyle; }
	int GetShadowStyle() const { return m_nShadowStyle; }
	void SetPenStyle(int val) { m_nPenStyle = val; }
	void SetPenWidt(int val) { m_nPenWidth = val; }
	void SetBrushStyle(int val) { m_nBrushStyle = val; }
	void SetShadowStyle(int val) { m_nShadowStyle = val; }
	void SetPenColor(COLORREF val) { m_clrPenColor = val; }
	void SetBrushColor(COLORREF val) { m_clrBrushColor = val; }
	COLORREF GetPenColor() const { return m_clrPenColor; }
	COLORREF GetBrushColor() const { return m_clrBrushColor; }

protected:
	int m_nPenStyle;
	int m_nPenWidth;
	COLORREF m_clrPenColor;
	int m_nBrushStyle;
	int m_nShadowStyle;
	COLORREF m_clrBrushColor;

};
class IFactory
{
public:
    virtual IShape* CreateShape() = 0;

};



