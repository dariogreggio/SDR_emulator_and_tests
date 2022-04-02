// SDR emulatorView.h : interface of the CSDRemulatorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDREMULATORVIEW_H__4655BA26_77A7_40C7_B10E_88C67767A766__INCLUDED_)
#define AFX_SDREMULATORVIEW_H__4655BA26_77A7_40C7_B10E_88C67767A766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSDRemulatorView : public CView {
protected: // create from serialization only
	CSDRemulatorView();
	DECLARE_DYNCREATE(CSDRemulatorView)

// Attributes
public:
	CSDRemulatorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDRemulatorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSDRemulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSDRemulatorView)
	afx_msg void OnVisualizzaZoomin();
	afx_msg void OnVisualizzaZoomout();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SDR emulatorView.cpp
inline CSDRemulatorDoc* CSDRemulatorView::GetDocument()
   { return (CSDRemulatorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

class CSDRemulatorView2 : public CView {
protected: // create from serialization only
	CSDRemulatorView2();
	DECLARE_DYNCREATE(CSDRemulatorView2)

// Attributes
public:
	CSDRemulatorDoc2* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDRemulatorView2)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSDRemulatorView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSDRemulatorView2)
	afx_msg void OnVisualizzaZoomin();
	afx_msg void OnVisualizzaZoomout();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SDR emulatorView2.cpp
inline CSDRemulatorDoc2* CSDRemulatorView2::GetDocument()
   { return (CSDRemulatorDoc2*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDREMULATORVIEW_H__4655BA26_77A7_40C7_B10E_88C67767A766__INCLUDED_)
