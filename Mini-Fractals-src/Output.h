/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//


#pragma once
#include "afxwin.h"

class CBlackMandelDlg ;
class ShadersApp ;

// COutput dialog

class COutput : public CDialogEx
{
	DECLARE_DYNAMIC(COutput)

	friend class CBlackMandelDlg ;
	friend class ShadersApp;
	 
protected:
	bool m_Does_Dialog_Exist ;   // helper 
	bool m_is_sizing ;
	bool m_is_starting ;
	bool m_is_zooming;
	int mouseX ;
	int mouseY ;
	bool zoomIn ;
	bool zoomOut ;
	CRect  rPic ;    // our picture dimensions
	CBlackMandelDlg*  m_pBase;    // helper to parent.
	CStatic m_Picture;
	 
	 
public:
	COutput(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutput();

// Dialog Data
	enum { IDD = IDD_DIALOG_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnExitSizeMove();
protected:
	afx_msg LRESULT OnFgzZoomStart(WPARAM wParam, LPARAM lParam);
	
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
