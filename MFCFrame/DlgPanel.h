#pragma once
#include "Resource.h"
#include "Frame.h"
#include "CamUtils.h"
#include <iostream>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "CvvImage.h"

class CDlgPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPanel)
public:
	CDlgPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPanel();
	enum { IDD = IDD_DIALOG_PANEL };

private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	const static int padding = 3;
	DECLARE_MESSAGE_MAP()

private:
	std::atomic<bool> ready = false;
	bool isSelected = false;
	bool resizingArea = false;
	bool areaResize = false;

	void DrawVideo();

	void drawPicToHDC(IplImage * img, UINT ID);

	CamID cam_play;

	struct Area{
		struct {
			double x, y;
		}p1, p2;
	} focusArea;

public:
	void VideoPlay();
	void VideoStop();
	void VideoToggle();
	void SetSelected(bool);
	void SetCam(CamID);

	inline bool isPlaying() 
	{
		return ready;
	}
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	class drawThread{
	private:
		std::atomic<bool> &ready, run = true;
		std::mutex m;
		std::condition_variable play;
		std::thread th;
	public:
		drawThread(CDlgPanel &panel) :
			ready(panel.ready), th(&CDlgPanel::DrawVideo, &panel) {}

		void waitPlay() {
			std::unique_lock <std::mutex> lck(m);
			//play.wait(lck, [this]()->bool {return ready; });
			play.wait(lck);
		}

		void pause() {
			ready = false;
		}

		void resume() {
			ready = true;
			play.notify_one();
		}

		void stop() {
			run = false;
			ready = false;
			play.notify_all();
		}

		bool isRunning() {
			return run;
		}

		~drawThread() {
			th.join();
		}
	}videoDrawer;
};