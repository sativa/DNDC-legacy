#if !defined(AFX_MOLABELPLACER_H__6361669C_5BAD_4717_9911_CEB1B8D909F7__INCLUDED_)
#define AFX_MOLABELPLACER_H__6361669C_5BAD_4717_9911_CEB1B8D909F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CMoTextSymbol;

/////////////////////////////////////////////////////////////////////////////
// CMoLabelPlacer wrapper class

class CMoLabelPlacer : public COleDispatchDriver
{
public:
	CMoLabelPlacer() {}		// Calls COleDispatchDriver default constructor
	CMoLabelPlacer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoLabelPlacer(const CMoLabelPlacer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetField();
	void SetField(LPCTSTR);
	BOOL GetDrawBackground();
	void SetDrawBackground(BOOL);
	BOOL GetAllowDuplicates();
	void SetAllowDuplicates(BOOL);
	BOOL GetPlaceAbove();
	void SetPlaceAbove(BOOL);
	BOOL GetPlaceBelow();
	void SetPlaceBelow(BOOL);
	BOOL GetPlaceOn();
	void SetPlaceOn(BOOL);
	CMoTextSymbol GetDefaultSymbol();
	void SetDefaultSymbol(LPDISPATCH);
	BOOL GetUseDefault();
	void SetUseDefault(BOOL);
	short GetValueCount();
	void SetValueCount(short);
	CString GetValueField();
	void SetValueField(LPCTSTR);
	LPDISPATCH GetBackgroundRenderer();
	void SetBackgroundRenderer(LPDISPATCH);
	BOOL GetMaskLabels();
	void SetMaskLabels(BOOL);
	unsigned long GetMaskColor();
	void SetMaskColor(unsigned long);
	short GetSymbolWidth();
	void SetSymbolWidth(short);
	short GetSymbolHeight();
	void SetSymbolHeight(short);
	CString GetTag();
	void SetTag(LPCTSTR);

// Operations
public:
	CMoTextSymbol GetSymbol(short index);
	CString GetValue(short index);
	void SetValue(short index, LPCTSTR lpszNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOLABELPLACER_H__6361669C_5BAD_4717_9911_CEB1B8D909F7__INCLUDED_)
