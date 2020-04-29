// common.h : 
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROTATE_H__73DFDF03_3596_48CE_851E_4BD6D70B5495__INCLUDED_)
#define AFX_ROTATE_H__73DFDF03_3596_48CE_851E_4BD6D70B5495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int LINERANGE = 20;
const double PI = 3.14159265358979323846;
#include "math.h"

inline void rotate(CPoint& beginPos,CPoint& endPos,double dblR)
{
  double x = (beginPos.x + endPos.x)/2.0;
  double y = (beginPos.y + endPos.y)/2.0;
  
  CPoint pos;
  
  pos.x = (long)((beginPos.x - x)*cos(dblR) - (beginPos.y - y)*sin(dblR) + x);
  pos.y = (long)((beginPos.x - x)*sin(dblR) - (beginPos.y - y)*cos(dblR) + y);
  
  
  beginPos = pos;
  
  pos.x = (long)((endPos.x - x)*cos(dblR) - (endPos.y - y)*sin(dblR) + x);
  pos.y = (long)((endPos.x - x)*sin(dblR) - (endPos.y - y)*cos(dblR) + y);
  
  endPos = pos;
}

#endif // !defined(AFX_ROTATE_H__73DFDF03_3596_48CE_851E_4BD6D70B5495__INCLUDED_)
