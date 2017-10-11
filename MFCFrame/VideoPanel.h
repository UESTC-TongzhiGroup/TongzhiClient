#pragma once
#include"resource.h"
#include "DlgPanel.h"
#include <bitset>

#define MAX_VIDEO_WND			1
#define	MAX_VIDEO_WINDOW_NUM	16

typedef struct
{
	bool fullscreen = false;
	int	fullScreenId = -1;
	int	selectedId = -1;
	int	channels = 16;
	CDlgPanel pDlgVideo[MAX_VIDEO_WINDOW_NUM];
} VIDEO_NODE_T;

class RectManager {
private:
	CRect area;
	int padding;
public:
	RectManager();
	RectManager(CRect, int);

	template<int ROW, int COL>
	CRect split(int index) {
		int width = area.Width() / COL,
			height = area.Height() / ROW;
		int row = index / COL, col = index%COL;
		int right = col == COL ? area.right : area.left + (col + 1)*width;
		int bottom = row == ROW ? area.bottom : area.top + (row + 1)*height;
		return CRect(
			area.left + col*width + padding,
			area.top + row*height + padding,
			right - padding,
			bottom - padding);
	}

	CRect split8(int);
	RectManager& setPadding(int);
	RectManager& resize(CRect);
};

// CVideoPanel 对话框

class CVideoPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoPanel)

public:
	CVideoPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoPanel();

// 对话框数据
	enum { IDD = IDD_VIDEOPANEL };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
protected:
	void UpdateComponents();
	void CreateVideoComponents();
	void UpdateVideoComponents(LPRECT lpRect);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	void Update();
	VIDEO_NODE_T m_VideoWindow;		//视频窗口
	void OnVideoPlay(int,int);
	void OnVideoStop(int);
	static CVideoPanel* GetInstance();
private:
	static CVideoPanel * m_pThis;
	RectManager rectMan;
};
