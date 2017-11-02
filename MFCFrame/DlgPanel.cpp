#include "stdafx.h"
#include "DlgPanel.h"
#include "NetHandler.h"
#include "NetMessage.h"
#include "StrUtil.h"

#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "ippicvmt.lib")
#pragma comment(lib, "zlibd.lib")
#pragma comment(lib, "libjpegd.lib")
#pragma comment(lib, "libpngd.lib")
#pragma comment(lib, "libtiffd.lib")
#pragma comment(lib, "libjasperd.lib")
#pragma comment(lib, "libwebpd.lib")
#pragma comment(lib, "IlmImfd.lib")
#pragma comment(lib, cvLIB("world"))

#include <cv.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

IMPLEMENT_DYNAMIC(CDlgPanel, CDialogEx)

BEGIN_MESSAGE_MAP(CDlgPanel, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CDlgPanel::CDlgPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPanel::IDD, pParent), videoDrawer(this)
{
	focusArea = { 0,0,1,1 };
}

CDlgPanel::~CDlgPanel()
{
	//SetEvent(m_hThreadEvent);
}

void CDlgPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CDlgPanel::SetCam(CamID ID) {
	VideoStop();
	cam_play = ID;
}

void CDlgPanel::VideoPlay()
{
	ready = true;
}

void CDlgPanel::VideoStop()
{
	ready = false;
}

void CDlgPanel::VideoToggle()
{
	if (!ready) {
		VideoPlay();
	}
	else {
		VideoStop();
	}
}

void CDlgPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	areaResize = true;
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(padding, padding, padding, padding);
	if (GetDlgItem(IDC_ShowVideo))
	{
		GetDlgItem(IDC_ShowVideo)->MoveWindow(rcClient);
	}
}

void CDlgPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, RGB(125, 125, 125));
	//选中后描白边
	if (isSelected)
	{
		CBrush border(RGB(240, 248, 251));
		dc.FrameRect(rcClient, &border);
	}
	CDialogEx::OnPaint();
}

void CDlgPanel::SetSelected(bool sel = true) {
	isSelected = sel;
}

void CDlgPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(isSelected)
	{
		resizingArea = true;
		focusArea = { 0,0,0,0 };
		CRect rcClient;
		GetClientRect(&rcClient);
		focusArea.p1.x = point.x / (double)rcClient.Width();
		focusArea.p1.y = point.y / (double)rcClient.Height();
	}
	else
	{
		SetSelected();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlgPanel::OnMouseMove(UINT nFlag, CPoint point) {
	if (resizingArea) {
		areaResize = true;
		CRect rcClient;
		GetClientRect(&rcClient);
		focusArea.p2.x = point.x / (double)rcClient.Width();
		focusArea.p2.y = point.y / (double)rcClient.Height();
	}
	CDialogEx::OnMouseMove(nFlag, point);
}

void CDlgPanel::OnLButtonUp(UINT nFlag, CPoint point) {
	if (resizingArea) {
		areaResize = true;
		CRect rcClient;
		GetClientRect(&rcClient);
		focusArea.p2.x = point.x / (double)rcClient.Width();
		focusArea.p2.y = point.y / (double)rcClient.Height();
		Message::AreaMsg msg{
			cam_play,
			focusArea.p1.x,focusArea.p1.y,
			focusArea.p2.x,focusArea.p2.y
		};
		auto reply = MsgHandler::sendReqMsg(msg);
		if (!reply.isSuccess()) {
			//TODO: 更新失败
			throw std::exception(reply.meta.c_str());
		}
	}
}

using cv::rectangle;
void CDlgPanel::DrawVideo() {
	string cam_path;
	cv::VideoCapture cap;

	cv::Mat frame;
	IplImage* pFrame = nullptr;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 2.0, 1.0, 0, 3, 8);

	while (true) {
		CamInfo &cam = Cams::getCamInfo()[cam_play];
		cam_path = cam.getFullURL();
		if (!cap.open(cam_path)) {
			//打开失败处理
			VideoStop();
		}
		TRACE(_T("%s\n"), StrUtil::std2CStr(cam_path));
		while (!ready) {
			std::this_thread::yield();
		}
		cap >> frame;
		while (!(cap >> frame, frame.empty()))
		{
			static CRect area;
			if (areaResize) {
				//绘制监视区域
				CRect client;
				GetClientRect(client);

				area.SetRect(
					focusArea.p1.x*client.Width(),
					focusArea.p1.y*client.Height(),
					focusArea.p2.x*client.Width(),
					focusArea.p2.y*client.Height()
				);

				area.NormalizeRect();
				area &= client;
				areaResize = false;
			}
			rectangle(frame, cv::Rect(area.top, area.left, area.Width(), area.Height()), CV_RGB(0, 255, 0), 2);
			pFrame = &IplImage(frame);
			if (!pFrame) {
				VideoStop();
				break;
			}

			//TODO: 报警高亮红框
			//cvRectangle(pFrame, cvPoint(pad, pad), cvPoint(frame.cols - pad, frame.rows - pad), cvScalar(0, 0, 255), 5, 3, 0);

			//TODO: 绘制文字的位置
			cvPutText(pFrame, cam.ID.c_str(), cvPoint(1200, 65), &font, CV_RGB(255, 255, 255));
			drawPicToHDC(pFrame, IDC_ShowVideo);
			//每次响应在33ms内的按键输入
			char c = cvWaitKey(33);
			//按下的键为Esc的话
			if (c == VK_ESCAPE)
				VideoStop();
		}
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