#pragma once
#include "Resource.h"
#include <cv.h>
//#include "highgui.h"
//#include "afxwin.h"

#include <iostream>
#include <deque>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
using namespace std;
// CDlgPanel 对话框

class CDlgPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPanel)

public:
	CDlgPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPanel();

// 对话框数据
	enum { IDD = IDD_DIALOG_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void setIndex(int);
	void setSelected(BOOL bIsSelected);
	static CDlgPanel * GetInstance();

	void onVideoPlay(int);
	void onVideoShutdown();
	void drawPicToHDC(IplImage *img, UINT ID);

	static bool isPlaying(int);

	static void initAllStatus(int);
	friend UINT taskVideo(LPVOID, int);
private:
	BOOL m_bIsPressed;
	static CDlgPanel * m_pThis;

	bool resizingArea;
	bool needDrawArea;
	CRect rcCtrl;
	Point downPoint;
	Point currentPoint;

	int m_index = -1;

	void shutdownAll();

	//double widthRatio;
	//double heightRatio;
	static deque<bool> status;

	const static int padding = 3;
};

inline bool CDlgPanel::isPlaying(int index) {
	return status[index];
}

inline void CDlgPanel::shutdownAll() {
	status.assign(status.size(), false);
}

inline void CDlgPanel::setIndex(int i) {
	m_index = i;
}

inline void CDlgPanel::initAllStatus(int num) {
	status.resize(num, false);
}