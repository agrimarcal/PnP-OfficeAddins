// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
// DDPIActiveXCtrl.cpp : Implementation of the CDDPIActiveXCtrl ActiveX Control class.

#include "stdafx.h"
#include "DDPIActiveX.h"
#include "DDPIActiveXCtrl.h"
#include "DDPIActiveXPropPage.h"
#include "afxdialogex.h"
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDDPIActiveXCtrl, COleControl)

// Message map

BEGIN_MESSAGE_MAP(CDDPIActiveXCtrl, COleControl)
	ON_MESSAGE(OCM_COMMAND, &CDDPIActiveXCtrl::OnOcmCommand)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// Dispatch map

BEGIN_DISPATCH_MAP(CDDPIActiveXCtrl, COleControl)
	DISP_FUNCTION_ID(CDDPIActiveXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Event map

BEGIN_EVENT_MAP(CDDPIActiveXCtrl, COleControl)
END_EVENT_MAP()

// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CDDPIActiveXCtrl, 1)
	PROPPAGEID(CDDPIActiveXPropPage::guid)
END_PROPPAGEIDS(CDDPIActiveXCtrl)

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CDDPIActiveXCtrl, "MFCACTIVEXCONTRO.DDPIActiveXCtrl.1",
	0x253a1346,0x9adb,0x4781,0xbe,0xaf,0xf6,0xcf,0x24,0xcb,0x2e,0xd1)

// Type library ID and version

IMPLEMENT_OLETYPELIB(CDDPIActiveXCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID IID_DDDPIActiveX = {0xfff12b99,0x09e3,0x468f,{0x90,0xe9,0x41,0xe1,0x12,0x26,0x46,0xe2}};
const IID IID_DDDPIActiveXEvents = {0x780478ee,0x97ef,0x479f,{0xb2,0x69,0x4b,0x22,0xb9,0x6f,0x0f,0x33}};

// Control type information

static const DWORD _dwDDPIActiveXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_IGNOREACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDDPIActiveXCtrl, IDS_DDPIACTIVEX, _dwDDPIActiveXOleMisc)

CMainDialog m_MainDialog;


// CDDPIActiveXCtrl::CDDPIActiveXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CDDPIActiveXCtrl

BOOL CDDPIActiveXCtrl::CDDPIActiveXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DDPIACTIVEX,
			IDB_DDPIACTIVEX,
			afxRegInsertable | afxRegApartmentThreading,
			_dwDDPIActiveXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CDDPIActiveXCtrl::CDDPIActiveXCtrl - Constructor

CDDPIActiveXCtrl::CDDPIActiveXCtrl()
{
	InitializeIIDs(&IID_DDDPIActiveX, &IID_DDDPIActiveXEvents);
	// TODO: Initialize your control's instance data here.
}

// CDDPIActiveXCtrl::~CDDPIActiveXCtrl - Destructor

CDDPIActiveXCtrl::~CDDPIActiveXCtrl()
{
	// TODO: Cleanup your control's instance data here.
}

// CDDPIActiveXCtrl::OnDraw - Drawing function

void CDDPIActiveXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	if (!m_MainDialog)
	{
		//CWnd* hwndParent = pdc->GetWindow();
		//if (!m_MainDialog.Create(IDD_MAINDIALOG, hwndParent))
		//{
			return;
		//}
	}
	
	// To size the main dialog window and fill the background
	m_MainDialog.MoveWindow(rcBounds, TRUE);
	CBrush brBackGnd(TranslateColor(AmbientBackColor()));
	pdc->FillRect(rcBounds, &brBackGnd);

	DoSuperclassPaint(pdc, rcBounds);

	if (!IsOptimizedDraw())
	{
		// The container does not support optimized drawing.

		// TODO: if you selected any GDI objects into the device context *pdc,
		//		restore the previously-selected objects here.
	}
}

// CDDPIActiveXCtrl::DoPropExchange - Persistence support

void CDDPIActiveXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}


// CDDPIActiveXCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CDDPIActiveXCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();

	// The control's output is not being clipped.
	// The control guarantees that it will not paint outside its
	// client rectangle.
	dwFlags &= ~clipPaintDC;

	// The control will not be redrawn when making the transition
	// between the active and inactivate state.
	dwFlags |= noFlickerActivate;

	// The control can receive mouse notifications when inactive.
	// TODO: if you write handlers for WM_SETCURSOR and WM_MOUSEMOVE,
	//		avoid using the m_hWnd member variable without first
	//		checking that its value is non-NULL.
	dwFlags |= pointerInactive;

	// The control can optimize its OnDraw method, by not restoring
	// the original GDI objects in the device context.
	dwFlags |= canOptimizeDraw;
	return dwFlags;
}


// CDDPIActiveXCtrl::OnResetState - Reset control to default state

void CDDPIActiveXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


// CDDPIActiveXCtrl::AboutBox - Display an "About" box to the user

void CDDPIActiveXCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_DDPIACTIVEX);
	dlgAbout.DoModal();
}


// CDDPIActiveXCtrl::PreCreateWindow - Modify parameters for CreateWindowEx

BOOL CDDPIActiveXCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("STATIC");
	BOOL bRet = COleControl::PreCreateWindow(cs);
	cs.hMenu = NULL;
	return bRet;
}

// CDDPIActiveXCtrl::IsSubclassedControl - This is a subclassed control

BOOL CDDPIActiveXCtrl::IsSubclassedControl()
{
	return TRUE;
}

// CDDPIActiveXCtrl::OnOcmCommand - Handle command messages

LRESULT CDDPIActiveXCtrl::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);

	// TODO: Switch on wNotifyCode here.

	return 0;
}


// CDDPIActiveXCtrl message handlers


int CDDPIActiveXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the main dialog
	if (!m_MainDialog.Create(IDD_MAINDIALOG, this))
	{
		return -1;
	}

	return 0;
}
