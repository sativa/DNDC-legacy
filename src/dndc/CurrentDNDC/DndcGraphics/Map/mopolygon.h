#if !defined(AFX_MOPOLYGON_H__012D66F0_4750_46E1_9A10_B07EAACB3562__INCLUDED_)
#define AFX_MOPOLYGON_H__012D66F0_4750_46E1_9A10_B07EAACB3562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CMoRectangle;
class CMoParts;
class CMoPoint;
class CMoPoints;

/////////////////////////////////////////////////////////////////////////////
// CMoPolygon wrapper class

class CMoPolygon : public COleDispatchDriver
{
public:
	CMoPolygon() {}		// Calls COleDispatchDriver default constructor
	CMoPolygon(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoPolygon(const CMoPolygon& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CMoRectangle GetExtent();
	void SetExtent(LPDISPATCH);
	CMoParts GetParts();
	void SetParts(LPDISPATCH);
	CMoPoint GetCentroid();
	void SetCentroid(LPDISPATCH);
	double GetArea();
	void SetArea(double);
	double GetPerimeter();
	void SetPerimeter(double);
	long GetShapeType();
	void SetShapeType(long);

// Operations
public:
	BOOL IsPointIn(LPDISPATCH Point);
	void Offset(double deltaX, double deltaY);
	double DistanceTo(LPDISPATCH shape);
	CMoPoints GetCrossings(LPDISPATCH shape);
	LPDISPATCH Union(LPDISPATCH anotherShape, const VARIANT& Extent);
	LPDISPATCH Xor(LPDISPATCH anotherShape, const VARIANT& Extent);
	LPDISPATCH Difference(LPDISPATCH anotherShape, const VARIANT& Extent);
	LPDISPATCH Intersect(LPDISPATCH anotherShape, const VARIANT& Extent);
	LPDISPATCH Buffer(double distance, const VARIANT& Extent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOPOLYGON_H__012D66F0_4750_46E1_9A10_B07EAACB3562__INCLUDED_)
