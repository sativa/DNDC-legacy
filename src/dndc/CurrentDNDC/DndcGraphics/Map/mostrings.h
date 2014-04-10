#if !defined(AFX_MOSTRINGS_H__7AEA7BB0_BD68_4AC3_A8ED_2B1F064AC856__INCLUDED_)
#define AFX_MOSTRINGS_H__7AEA7BB0_BD68_4AC3_A8ED_2B1F064AC856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CMoStrings wrapper class

class CMoStrings : public COleDispatchDriver
{
public:
	CMoStrings() {}		// Calls COleDispatchDriver default constructor
	CMoStrings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMoStrings(const CMoStrings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	BOOL GetUnique();
	void SetUnique(BOOL);
	long GetCount2();
	void SetCount2(long);
	BOOL GetPopulateUsingNewConstants();
	void SetPopulateUsingNewConstants(BOOL);

// Operations
public:
	CString Item(const VARIANT& Item);
	BOOL Add(LPCTSTR string);
	void Clear();
	long Find(LPCTSTR itemName, const VARIANT& startPos);
	void PopulateWithUnits();
	void PopulateWithSpheroids();
	void PopulateWithDatums();
	void PopulateWithMeridians();
	void PopulateWithProjections();
	void PopulateWithProjectedCoordSys();
	void PopulateWithGeographicCoordSys();
	void PopulateWithGeoTransformations();
	void PopulateWithParameters(long Projection);
	void PopulateWithMethods();
	void Sort(long sortOrder);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOSTRINGS_H__7AEA7BB0_BD68_4AC3_A8ED_2B1F064AC856__INCLUDED_)
