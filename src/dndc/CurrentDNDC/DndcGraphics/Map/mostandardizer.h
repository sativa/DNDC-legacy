#if !defined(AFX_MOSTANDARDIZER_H__8736DF29_A52B_4B33_8339_CBAC598135BA__INCLUDED_)
#define AFX_MOSTANDARDIZER_H__8736DF29_A52B_4B33_8339_CBAC598135BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMoStandardizer wrapper class

class CMoStandardizer : public COleDispatchDriver
{
public:
	CMoStandardizer() {}		// Calls COleDispatchDriver default constructor
	CMoStandardizer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoStandardizer(const CMoStandardizer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetValid();
	void SetValid(BOOL);
	short GetFieldCount();
	void SetFieldCount(short);
	CString GetStandardizingRules();
	void SetStandardizingRules(LPCTSTR);
	long GetLastError();
	void SetLastError(long);
	CString GetIntersectionStandardizingRules();
	void SetIntersectionStandardizingRules(LPCTSTR);

// Operations
public:
	BOOL StandardizeAddress(LPCTSTR address);
	CString GetFieldName(short index);
	CString GetFieldValue(LPCTSTR FieldName);
	void SetFieldValue(LPCTSTR FieldName, LPCTSTR lpszNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOSTANDARDIZER_H__8736DF29_A52B_4B33_8339_CBAC598135BA__INCLUDED_)
