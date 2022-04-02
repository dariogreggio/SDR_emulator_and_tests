// SDR emulatorDoc.h : interface of the CSDRemulatorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDREMULATORDOC_H__44151FC0_1447_4500_896E_94ADD2194083__INCLUDED_)
#define AFX_SDREMULATORDOC_H__44151FC0_1447_4500_896E_94ADD2194083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSDRemulatorDoc : public CDocument {
protected: // create from serialization only
	CSDRemulatorDoc();
	DECLARE_DYNCREATE(CSDRemulatorDoc)

// Attributes
public:

// Operations
public:
	CView *getView() { POSITION pos=GetFirstViewPosition(); return GetNextView(pos); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDRemulatorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSDRemulatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSDRemulatorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CSDRemulatorDoc2 : public CDocument {
protected: // create from serialization only
	CSDRemulatorDoc2();
	DECLARE_DYNCREATE(CSDRemulatorDoc2)

// Attributes
public:

// Operations
public:
	CView *getView() { POSITION pos=GetFirstViewPosition(); return GetNextView(pos); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDRemulatorDoc2)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSDRemulatorDoc2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSDRemulatorDoc2)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDREMULATORDOC_H__44151FC0_1447_4500_896E_94ADD2194083__INCLUDED_)
