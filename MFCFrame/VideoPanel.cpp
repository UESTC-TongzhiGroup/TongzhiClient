// VideoPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "VideoPanel.h"

// CVideoPanel 对话框

IMPLEMENT_DYNAMIC(CVideoPanel, CDialogEx)


RectManager::RectManager(CRect _area,int _padding) :area(_area),padding(_padding) {
	area.NormalizeRect();
}
RectManager::RectManager() :RectManager(CRect(),0) {}

CRect RectManager::split8(int index) {
		int width = area.Width() / 4,
			height = area.Height() / 4;
		int l = 0, t = 0, r = 0, b = 0;
		switch (index) {
		case 0:
			l = area.left;
			t = area.top;
			r = area.left + 3 * width;
			b = area.top + 3 * height;
			break;
		case 1:
		case 2:
		case 3:
			l = area.left + 3 * width;
			r = l + width;
			b = area.top + index*height;
			t = b - height;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			t = area.top + 3 * height;
			b = area.bottom;
			l = area.left + (index - 4)*width;
			r = l + width;
			break;
		default:
			throw std::out_of_range("Area index out of range!");
		}
		return CRect(l + padding, t + padding, r - padding, b - padding);
	}

RectManager& RectManager::setPadding(int padding) {
	this->padding = padding;
	return *this;
}

RectManager& RectManager::resize(CRect rect) {
	rect.NormalizeRect();
	area = rect;
	return *this;
}

CVideoPanel::CVideoPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoPanel::IDD, pParent)
{
	m_pThis = this;
}

CVideoPanel::~CVideoPanel()
{
	if (nullptr != m_VideoWindow.pDlgVideo)
	{
		for (int i = 0; i<m_VideoWindow.channels; i++)
		{
			m_VideoWindow.pDlgVideo[i].DestroyWindow();
		}
		/**
		*TODO: 以后有需求再改回动态分配
		*		目前为静态在栈内存上分配

		delete[] m_VideoWindow.pDlgVideo;
		m_VideoWindow.pDlgVideo = nullptr;
		*/
	}
}

void CVideoPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoPanel, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CVideoPanel 消息处理程序

int CVideoPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CreateVideoComponents();
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CVideoPanel::OnSize(UINT nType, int cx, int cy)
{
	UpdateComponents();
	CDialogEx::OnSize(nType, cx, cy);
}

void CVideoPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect (rcClient,RGB(240,248,251));
}

void CVideoPanel::Update()
{
	m_VideoWindow.channels = nSplitWindow;
	UpdateComponents();
}

CVideoPanel * CVideoPanel::GetInstance()
{
	return m_pThis;
}

CVideoPanel * CVideoPanel::m_pThis = NULL;

BOOL CVideoPanel::DestroyWindow()
{
	return CDialogEx::DestroyWindow();
}

BOOL CVideoPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CRect rcClient;
	GetClientRect(&rcClient);
	MoveWindow(rcClient);//926, 727 1024, 768
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CVideoPanel::CreateVideoComponents()
{
	int &channels = m_VideoWindow.channels;
	if (channels>MAX_VIDEO_WINDOW_NUM)
		channels = MAX_VIDEO_WINDOW_NUM;
	/**
	 *TODO: 以后有需求再改回动态分配
	 *		目前为静态在栈内存上分配
	 */
	//m_VideoWindow.pDlgVideo = new CDlgPanel[MAX_VIDEO_WINDOW_NUM];
	for (int i = 0; i<channels; i++)
	{
		//	m_VideoWindow.pDlgVideo[i].SetMainDlg(this, i);
		m_VideoWindow.pDlgVideo[i].Create(IDD_DIALOG_PANEL, this);
		m_VideoWindow.pDlgVideo[i].setIndex(i);
		m_VideoWindow.pDlgVideo[i].ShowWindow(SW_HIDE);
	}
	CDlgPanel::initAllStatus(MAX_VIDEO_WINDOW_NUM);
}

void CVideoPanel::UpdateComponents()
{
	//移动准备
	HDWP hDwp = BeginDeferWindowPos(64);//该函数为一个多窗口位置结构分配内存并且返回该结构的句柄。
	UINT uFlags = SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER;

	CRect	rcClient;
	GetClientRect(&rcClient);
	if (rcClient.IsRectEmpty())
		return;
	int iPanelWidth = rcClient.Width();
	CRect	rcPanel;
	rcPanel.SetRect(rcClient.left, rcClient.top, rcClient.left + iPanelWidth - 4, rcClient.bottom);//320

	rectMan.resize(rcPanel);
	UpdateVideoComponents(&rcPanel);

}

void CVideoPanel::UpdateVideoComponents(LPRECT lpRect)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcTmp;
	rcTmp.SetRect(rcClient.left, rcClient.top, rcClient.left + rcClient.Width() / 2, rcClient.top + rcClient.Height() / 2);

	if (m_VideoWindow.fullScreenId == -1)
	{
		switch (m_VideoWindow.channels)
		{
		case 1:
		{
			m_VideoWindow.pDlgVideo[0].MoveWindow(lpRect);
			if (!m_VideoWindow.pDlgVideo[0].IsWindowVisible())
				m_VideoWindow.pDlgVideo[0].ShowWindow(SW_SHOW);
			break;
		}
		case 4:
		{
			for (int i = 0; i < 4; i++)
			{
				rcTmp = rectMan.split<2, 2>(i);
				m_VideoWindow.pDlgVideo[i].MoveWindow(&rcTmp);
				if (!m_VideoWindow.pDlgVideo[i].IsWindowVisible())
					m_VideoWindow.pDlgVideo[i].ShowWindow(SW_SHOW);
			}
			break;
		}
		case 6:
		{
			for (int i = 0; i < 6; i++)
			{
				rcTmp = rectMan.split<2, 3>(i);
				m_VideoWindow.pDlgVideo[i].MoveWindow(&rcTmp);
				if (!m_VideoWindow.pDlgVideo[i].IsWindowVisible())
					m_VideoWindow.pDlgVideo[i].ShowWindow(SW_SHOW);
			}
			break;
		}
		case 8:
		{
			for (int i = 0; i < 8; i++)
			{
				rcTmp = rectMan.split8(i);
				m_VideoWindow.pDlgVideo[i].MoveWindow(&rcTmp);
				if (!m_VideoWindow.pDlgVideo[i].IsWindowVisible())
					m_VideoWindow.pDlgVideo[i].ShowWindow(SW_SHOW);
			}
			break;
		}
		case 9:
		{
			for (int i = 0; i < 9; i++)
			{
				rcTmp = rectMan.split<3, 3>(i);
				m_VideoWindow.pDlgVideo[i].MoveWindow(&rcTmp);
				if (!m_VideoWindow.pDlgVideo[i].IsWindowVisible())
					m_VideoWindow.pDlgVideo[i].ShowWindow(SW_SHOW);
			}
			break;
		}
		case 12:
		{
			for (int i = 0; i < 12; i++)
			{
				rcTmp = rectMan.split<3, 4>(i);
				m_VideoWindow.pDlgVideo[i].MoveWindow(&rcTmp);
				if (!m_VideoWindow.pDlgVideo[i].IsWindowVisible())
					m_VideoWindow.pDlgVideo[i].ShowWindow(SW_SHOW);
			}
			break;
		}
		case 16:
		{
			for (int i = 0; i < 16; i++)
			{
				rcTmp = rectMan.split<4, 4>(i);
				m_VideoWindow.pDlgVideo[i].MoveWindow(&rcTmp);
				if (!m_VideoWindow.pDlgVideo[i].IsWindowVisible())
					m_VideoWindow.pDlgVideo[i].ShowWindow(SW_SHOW);
			}
			break;
		}
		}

		for (int i = m_VideoWindow.channels; i < MAX_VIDEO_WINDOW_NUM; i++) {
			m_VideoWindow.pDlgVideo[i].ShowWindow(SW_HIDE);
			m_VideoWindow.pDlgVideo[i].onVideoShutdown();
		}
	}
	else
	{
		for (int i = 0; i<m_VideoWindow.channels; i++)
		{
			if (m_VideoWindow.pDlgVideo[i].IsWindowVisible())
			{
				m_VideoWindow.pDlgVideo[i].ShowWindow(SW_HIDE);
			}
		}
		m_VideoWindow.pDlgVideo[m_VideoWindow.fullScreenId].MoveWindow(&rcTmp);
		m_VideoWindow.pDlgVideo[m_VideoWindow.fullScreenId].ShowWindow(SW_SHOW);
	}
	Invalidate();
	UpdateWindow();
}

BOOL CVideoPanel::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	if (wNotifyCode == BN_CLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT(hWndButton);

		int nCount = m_VideoWindow.channels;
		for (int i = 0; i<nCount; i++)
		{
			ASSERT(m_VideoWindow.pDlgVideo[i]);

			if (m_VideoWindow.pDlgVideo[i].m_hWnd == hWndButton)
			{
				
				m_VideoWindow.pDlgVideo[i].setSelected(TRUE);
				m_VideoLabels = i;
			}
			else
			{
				m_VideoWindow.pDlgVideo[i].setSelected(FALSE);
			}

		}
	}
	if (wNotifyCode == BN_DOUBLECLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT(hWndButton);

		int nCount = m_VideoWindow.channels;
		if (m_DblClk == TRUE)
		{
			for (int i = 0; i<nCount; i++)
			{
				ASSERT(m_VideoWindow.pDlgVideo[i]);

				if (m_VideoWindow.pDlgVideo[i].m_hWnd == hWndButton)
				{
					CRect	rcClient;
					GetClientRect(&rcClient);
					m_VideoWindow.pDlgVideo[i].MoveWindow(&rcClient);

				}
				else
				{
					m_VideoWindow.pDlgVideo[i].ShowWindow(SW_HIDE);
				}
				Invalidate();
			}
		}
		else
		{
			for (int i = 0; i<nCount; i++)
			{
				ASSERT(m_VideoWindow.pDlgVideo[i]);

				if (m_VideoWindow.pDlgVideo[i].m_hWnd == hWndButton)
				{
					UpdateComponents();

				}
			}

		}

	}
	return CWnd::OnCommand(wParam, lParam);
}

void  CVideoPanel::OnVideoPlay(int m_VideoLabels, int camIndex)
{
	if (m_VideoLabels != -1)
		m_VideoWindow.pDlgVideo[m_VideoLabels].onVideoPlay(camIndex);
}

void CVideoPanel::OnVideoStop(int m_VideoLabels)
{
	if (m_VideoLabels != -1)
		m_VideoWindow.pDlgVideo[m_VideoLabels].onVideoShutdown();
}

