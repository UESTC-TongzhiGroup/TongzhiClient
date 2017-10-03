// DlgPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "DlgPanel.h"
#include "CvvImage.h"
#include "resource.h"

#include "Config.h"
#include "StrUtil.h"
//#include "DBUtil.h"

// CDlgPanel 对话框

IMPLEMENT_DYNAMIC(CDlgPanel, CDialogEx)

std::deque<bool> CDlgPanel::status;

CDlgPanel::CDlgPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPanel::IDD, pParent)
{
	m_bIsPressed = FALSE;
	//m_DblClk = FALSE;
	m_pThis = this;

	resizingArea = false;
}

CDlgPanel::~CDlgPanel()
{
	//SetEvent(m_hThreadEvent);
}

void CDlgPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPanel, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDlgPanel 消息处理程序


void CDlgPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient, RGB(125, 125, 125));
	if (m_bIsPressed)
	{
		int width = 1;
		CRect rcRight = rcClient;
		rcRight.right = rcClient.left + width;
		dc.FillSolidRect(rcRight, RGB(240, 248, 251));
		CRect rcLeft = rcClient;
		rcLeft.left = rcClient.right - width;
		dc.FillSolidRect(rcLeft, RGB(240, 248, 251));
		CRect rcTop = rcClient;
		rcTop.bottom = rcClient.top + width;
		dc.FillSolidRect(rcTop, RGB(240, 248, 251));
		CRect rcBottom = rcClient;
		rcBottom.top = rcClient.bottom - width;
		dc.FillSolidRect(rcBottom, RGB(240, 248, 251));
	}
}

CDlgPanel * CDlgPanel::GetInstance()
{
	return m_pThis;
}

CDlgPanel * CDlgPanel::m_pThis = NULL;

void CDlgPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	resizingArea = false;
	CRect area;
	area.SetRect(downPoint.x, downPoint.y, currentPoint.x, currentPoint.y);
	area.NormalizeRect();
	try {
		//DBHelper helper;
		//helper.updateCamArea(m_index, area);
	}
	catch (std::exception e) {
#ifdef _DEBUG
		TRACE(e.what());
		TRACE("\n");
#endif // _DEBUG
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	if (resizingArea)
	{
		currentPoint.x = point.x;
		currentPoint.y = point.y;
		Invalidate();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CDlgPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	resizingArea = true;
	needDrawArea = true;
	//记录起点
	CRect rcClient;
	GetClientRect(rcClient);
	downPoint.x = point.x;
	downPoint.y = point.y;

	setSelected(TRUE);
	CWnd * pWndParent = GetParent();
	ASSERT(pWndParent);
	WORD wID = GetDlgCtrlID();
	pWndParent->SendMessage(WM_COMMAND, MAKEWPARAM(wID, BN_CLICKED), (LPARAM)m_hWnd);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlgPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//m_DblClk = !m_DblClk;
	CWnd * pWndParent = GetParent();
	ASSERT(pWndParent);
	WORD wID = GetDlgCtrlID();
	pWndParent->SendMessage(WM_COMMAND, MAKEWPARAM(wID, BN_DOUBLECLICKED), (LPARAM)m_hWnd);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CDlgPanel::setSelected(BOOL bIsSelected)
{
	if (bIsSelected != m_bIsPressed)
	{
		m_bIsPressed = bIsSelected;
		Invalidate();
	}
}

void CDlgPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	//CRect rcCtrl;
	rcCtrl.SetRect(rcClient.left + padding, rcClient.top + padding, rcClient.right - padding, rcClient.bottom - padding);
	if (GetDlgItem(IDC_ShowVideo))
	{
		GetDlgItem(IDC_ShowVideo)->MoveWindow(rcCtrl);
	}
}

void CDlgPanel::drawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // 复制图片
	cimg.DrawToHDC(hDC, &rect); // 将图片绘制到显示控件的指定区域内
	ReleaseDC(pDC);
}

void CDlgPanel::onVideoPlay()
{
	status[m_index] = true;
	AfxBeginThread(taskVideo, this);
}

void CDlgPanel::onVideoShutdown()
{
	status[m_index] = false;
}

UINT taskVideo(LPVOID param)
{
	CDlgPanel * pTaskMain = (CDlgPanel *)param;

	int pad = CDlgPanel::padding;

	int &sel = pTaskMain->m_index;
	auto &status = pTaskMain->status[sel];
	std::string videoPath = Cams::getCamInfo()[sel].getFullURL();
	cv::VideoCapture cap;
	TRACE(videoPath.c_str());
	TRACE("\n");
	cap.open(videoPath);

	if (cap.isOpened())
	{
		cv::Mat frame;
		IplImage* pFrame = nullptr;
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 2.0, 1.0, 0, 3, 8);
		while (1)
		{
			cap >> frame;
			if (!frame.empty())
			{
				//绘制监视区域
				if (pTaskMain->needDrawArea){
					rectangle(frame, pTaskMain->downPoint, pTaskMain->currentPoint, Scalar(0, 255, 0), 2);
				}
				
				pFrame = &IplImage(frame);
				if (!pFrame) {
					status = false;
					break;
				}
				if (warningroute == 0)
				{
					//TODO: 绘制图像位置坐标
					cvRectangle(pFrame, cvPoint(pad, pad), cvPoint(frame.cols-pad, frame.rows-pad), cvScalar(0, 0, 255), 5, 3, 0);
				}
				
				//TODO: 绘制文字的位置
				cvPutText(pFrame, StrUtil::format("%02d",sel).c_str(), cvPoint(1200, 65), &font, CV_RGB(255, 255, 255));
				//pTaskMain->widthRatio = double((frame.cols) /(pTaskMain->rcCtrl.Width()));
				//pTaskMain->heightRatio = double((frame.rows) /(pTaskMain->rcCtrl.Height()));
				
				pTaskMain->drawPicToHDC(pFrame, IDC_ShowVideo);
				//每次响应在33ms内的按键输入
				char c = cvWaitKey(33);
				//按下的键为Esc的话
				if (c == VK_ESCAPE)
					status = false;
			}
			if (status == false)
				break;
		}
		//结束时的资源释放
		cap.release();
		frame.release();
		pTaskMain->Invalidate();
	}
	return TRUE;
}
