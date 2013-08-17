/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//


// Output.cpp : implementation file
//

#include "stdafx.h"
#include "glMandel.h"
#include "Output.h"
#include "glMandelDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

// trying to resolve a missing extern symbol _main ...
void _main()
{
	;
}

using namespace std;

// COutput dialog

IMPLEMENT_DYNAMIC(COutput, CDialogEx)

COutput::COutput(CWnd* pParent /*=NULL*/)
	: CDialogEx(COutput::IDD, pParent)
{
	m_is_sizing = false ;
	m_is_zooming = false ;
	zoomIn = false ;
	zoomOut = false ;
	m_is_starting = true ;
	m_pBase = NULL;
}

COutput::~COutput()
{
}

void COutput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_OUT, m_Picture);
}


BEGIN_MESSAGE_MAP(COutput, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_EXITSIZEMOVE()
	ON_MESSAGE(WM_FGZ_ZOOM_START, &COutput::OnFgzZoomStart)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// COutput message handlers



afx_msg LRESULT COutput::OnFgzZoomStart(WPARAM wParam, LPARAM lParam)
{
	static int nstar = 0 ;
	char mys[256];
	if( m_pBase == NULL ) m_pBase = ((CBlackMandelDlg*)m_pParentWnd) ;

	if( m_is_zooming )
    {
		ShadersApp* pSH = m_pBase->m_shader ; // ((CBlackMandelDlg*)m_pParentWnd)->m_mandelbrot ;
		
		float nStep = 1.0f ; // with earilier implimentations this needed to be much higher than 1.0f
		
		if (rPic.left + mouseX < (2 * rPic.Width() / 5))
        {
			pSH->setXPos(pSH->getXPos() - pSH->getXStep() * nStep * log(float(rPic.Width()) / max(1,abs(mouseX-rPic.left)))  );  
        }
        else if (rPic.left + mouseX > (3 * rPic.Width() / 5))
        {
			pSH->setXPos(pSH->getXPos() + pSH->getXStep() * nStep  * log(float(rPic.Width()) / max(1,abs(rPic.right-mouseX)))  );   //   * (double(rPic.Width())/2.0 - (rPic.left + mouseX)) / (2 * rPic.Width() / 5)
        }
		if (rPic.top + mouseY < (2 * rPic.Height() / 5))
        {
			pSH->setYPos(pSH->getYPos() - (pSH->getYStep()) * nStep  * log(float(rPic.Height()) / max(1,abs(mouseY-rPic.top)))  );
			// Beep(3000, 50);
        }
		else if (rPic.top + mouseY > (3 * rPic.Height() / 5))
        {
			pSH->setYPos(pSH->getYPos() + (pSH->getYStep()) * nStep  * log(float(rPic.Height()) / max(1,abs(rPic.bottom-mouseY)))  );
			// Beep(1000, 50);
        }
        if (zoomIn)
        {
            pSH->setXSize(pSH->getXSize() * 0.98f);
        }
        else if (zoomOut)
        {
            pSH->setXSize(pSH->getXSize() / 0.98f);
        }
		
		m_pBase->OnBnClickedButtonMake();
		if( ++nstar > 987654399 ) nstar = 0 ;
		if( (nstar % 100) == 0 )  { 
			sprintf_s(mys, 255, "Zoom image # %d ", nstar );
			std::cout << mys << std::endl;  
			m_pBase->OnBnClickedButtonTime();
		}
		
		if( m_is_zooming ) SetTimer( 4747, 15, NULL );
		 
	}
	else
		nstar = 0 ;
	return 0;
}


void COutput::OnLButtonDown(UINT nFlags, CPoint point)
{
	zoomIn = true ;
	zoomOut = false ;
	mouseX = point.x ;
	mouseY = point.y ;
	// the timer runs the action. This does, on very fast machines, limit the frame rate.
	SetTimer( 4747, 15, NULL );
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void COutput::OnLButtonUp(UINT nFlags, CPoint point)
{
	zoomIn = false ;
	zoomOut = false ;
	mouseX = point.x ;
	mouseY = point.y ;
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void COutput::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_is_zooming ^= true ;
	if( !m_is_zooming )
		std::cout << "stop zooming" << std::endl; 
	else
		std::cout << "start zooming" << std::endl ;
	// to do: MAKE THIS A JUMP TO CENTER ... ?? 
	zoomIn = false ;
	zoomOut = false ;
	mouseX = point.x ;
	mouseY = point.y ;
	
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void COutput::OnRButtonDown(UINT nFlags, CPoint point)
{
	// Zoom out. De-magnify ...
	zoomIn = false ;
	zoomOut = true ;
	mouseX = point.x ;
	mouseY = point.y ;
	
	SetTimer( 4747, 15, NULL );
	
	CDialogEx::OnRButtonDown(nFlags, point);
}


void COutput::OnRButtonUp(UINT nFlags, CPoint point)
{
	zoomIn = false ;
	zoomOut = false ;
	mouseX = point.x ;
	mouseY = point.y ;
	
	CDialogEx::OnRButtonUp(nFlags, point);
}


void COutput::OnMouseHover(UINT nFlags, CPoint point)
{
	mouseX = point.x ;
	mouseY = point.y ;
	
	SetTimer( 4747, 0, NULL );
	
	CDialogEx::OnMouseHover(nFlags, point);
}


void COutput::OnMouseMove(UINT nFlags, CPoint point)
{
	mouseX = point.x ;
	mouseY = point.y ;
	
	SetTimer( 4747, 0, NULL );
	
	CDialogEx::OnMouseMove(nFlags, point);
}


void COutput::OnMouseLeave()
{
	m_is_zooming = false ;
	CDialogEx::OnMouseLeave();
}


void COutput::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	m_is_sizing = true ;
}


void COutput::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	CDC* pDC = GetDC();
	CRect dcR;
	this->GetClientRect( &dcR );
	pDC->FillSolidRect(&dcR , 0x00FFffFF);
	ReleaseDC( pDC );
	int mcx = max( 16, ((min( 16384, cx - 16) / 16) * 16)  );  // these are what keep it from blowing up ! not the four pixel deal, it is 16 pixels !!
	int mcy = max( 16, ((min( 16384, cy - 16) / 16) * 16)  );  // this was true on OpenCL, leaving for GLSL optimizer ...

	this->m_Picture.SetWindowPos( NULL, 7,7, max(1, mcx), max(1, mcy), SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE );
	
	m_is_sizing = false ;
	 
}


void COutput::OnExitSizeMove()
{
	if( m_is_sizing && !m_is_starting ) {
		// send signal to glMandelDlg that picture has changed in size, and wait for it to finish.
		::SendMessage( m_pParentWnd->GetSafeHwnd(), WM_FGZ_UPDATE_PICTURE_SIZE, 0,0);
		((CBlackMandelDlg*)this->GetParent())->OnBnClickedButtonMake();
		m_Picture.GetClientRect( &rPic ); 
	}
	m_is_sizing = false ;
	CDialogEx::OnExitSizeMove();
}


void COutput::OnTimer(UINT_PTR nIDEvent)
{
	static bool IsBusy = false ;
	 
	switch( nIDEvent )
	{
	case 4747: 
		KillTimer( nIDEvent );
		if( !IsBusy ) {   // prevents build up of msgs which makes run-on when image is slow and zooming.
			IsBusy = true ;
			OnFgzZoomStart(0,0);    // this is the action !
			IsBusy = false ;
		}
		break;
	default :
		std::cout << "undefined timer ID" << std::endl ;
		KillTimer( nIDEvent );
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void COutput::OnClose()
{
	m_Does_Dialog_Exist = false ;

	CDialogEx::OnClose();
}


void COutput::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_Does_Dialog_Exist = false;
	
}


void COutput::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	m_Picture.GetClientRect( &rPic ); // initialize this !

	m_Does_Dialog_Exist = (bShow == TRUE) ;
	m_is_starting = false ;
}



BOOL COutput::OnInitDialog()
{
	CDialogEx::OnInitDialog();
 
	// Move This Window to upper top left of desktop.
	this->SetWindowPos( NULL, 0, 0, 0,0, SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOSIZE );  // SWP_NOREPOSITION  has to do with Z-Order

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL COutput::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// set the space-ship cursor ... :) , for the kids.

	::SetCursor( AfxGetApp()->LoadCursor(IDC_CURSOR1) );  // (MAKEINTRESOURCE(IDC_CURSOR1)) );
	return TRUE;

	// return CDialogEx::OnSetCursor(pWnd, nHitTest, message);   // DO NOT CALL THIS !! over-rides the above !
}
