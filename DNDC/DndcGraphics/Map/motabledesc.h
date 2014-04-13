#if !defined(AFX_MOTABLEDESC_H__6A708685_A267_44A4_B09C_3C0795167E00__INCLUDED_)
#define AFX_MOTABLEDESC_H__6A708685_A267_44A4_B09C_3C0795167E00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMoTableDesc wrapper class

class CMoTableDesc : public COleDispatchDriver
{
public:
	CMoTableDesc() {}		// Calls COleDispatchDriver default constructor
	CMoTableDesc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoTableDesc(const CMoTableDesc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetFieldCount();
	void SetFieldCount(short);
	long GetCodePage();
	void SetCodePage(long);

// Operations
public:
	CString GetFieldName(short index);
	void SetFieldName(short index, LPCTSTR lpszNewValue);
	long GetFieldType(short index);
	void SetFieldType(short index, long nNewValue);
	short GetFieldPrecision(short index);
	void SetFieldPrecision(short index, short nNewValue);
	short GetFieldLength(short index);
	void SetFieldLength(short index, short nNewValue);
	short GetFieldScale(short index);
	void SetFieldScale(short index, short nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTABLEDESC_H__6A708685_A267_44A4_B09C_3C0795167E00__INCLUDED_)