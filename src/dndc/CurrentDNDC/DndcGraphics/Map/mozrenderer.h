#if !defined(AFX_MOZRENDERER_H__93F229D3_845A_49F0_8B33_BE84E1198727__INCLUDED_)
#define AFX_MOZRENDERER_H__93F229D3_845A_49F0_8B33_BE84E1198727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMoZRenderer wrapper class

class CMoZRenderer : public COleDispatchDriver
{
public:
	CMoZRenderer() {}		// Calls COleDispatchDriver default constructor
	CMoZRenderer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoZRenderer(const CMoZRenderer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetTag();
	void SetTag(LPCTSTR);
	long GetSymbolType();
	void SetSymbolType(long);
	short GetBreakCount();
	void SetBreakCount(short);
	long GetValueCalculation();
	void SetValueCalculation(long);
	BOOL GetDrawBackground();
	void SetDrawBackground(BOOL);

// Operations
public:
	double GetBreak(short index);
	void SetBreak(short index, double newValue);
	LPDISPATCH GetSymbol(short index);
	void RampColors(unsigned long startColor, unsigned long endColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOZRENDERER_H__93F229D3_845A_49F0_8B33_BE84E1198727__INCLUDED_)
