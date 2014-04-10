#if !defined(AFX_MOGROUPRENDERER_H__D8109BAA_180E_4B66_B320_62883AA3A045__INCLUDED_)
#define AFX_MOGROUPRENDERER_H__D8109BAA_180E_4B66_B320_62883AA3A045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMoGroupRenderer wrapper class

class CMoGroupRenderer : public COleDispatchDriver
{
public:
	CMoGroupRenderer() {}		// Calls COleDispatchDriver default constructor
	CMoGroupRenderer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoGroupRenderer(const CMoGroupRenderer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	BOOL GetDrawBackground();
	void SetDrawBackground(BOOL);
	CString GetTag();
	void SetTag(LPCTSTR);

// Operations
public:
	LPDISPATCH GetRenderer(short index);
	void SetRefRenderer(short index, LPDISPATCH newValue);
	short Add(LPDISPATCH Renderer);
	void Remove(short index);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOGROUPRENDERER_H__D8109BAA_180E_4B66_B320_62883AA3A045__INCLUDED_)
