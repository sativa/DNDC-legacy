#if !defined(AFX_MOUNIT_H__0723B59C_6A25_478E_897A_D5428D5B3D9A__INCLUDED_)
#define AFX_MOUNIT_H__0723B59C_6A25_478E_897A_D5428D5B3D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMoUnit wrapper class

class CMoUnit : public COleDispatchDriver
{
public:
	CMoUnit() {}		// Calls COleDispatchDriver default constructor
	CMoUnit(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoUnit(const CMoUnit& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetType();
	void SetType(long);
	CString GetName();
	void SetName(LPCTSTR);
	double GetFactor();
	void SetFactor(double);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOUNIT_H__0723B59C_6A25_478E_897A_D5428D5B3D9A__INCLUDED_)