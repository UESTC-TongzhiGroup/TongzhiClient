#include "StdAfx.h"
#include <windows.h>
#include <iostream>  
#include <string>  
#include <sstream>  
#include "MenuBarPanel.h"
#include "resource.h"
#include "GlobalSetting.h"
//#include "WarningPanel.h"
#include "AlarmLog.h"
#include "EventBus.h"
#include "Events.h"
using namespace  std;

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

HHOOK g_FilterHook=NULL;
CMenuBarPanel* g_MenuBar=NULL;
BOOL g_bShowMenu=FALSE;

CMenuBarPanel::CMenuBarPanel(void)
{
	m_vecPMenuItem.clear();
	m_bTracking=false;
	m_AlarmValue = 0;
	m_pThis = this;
	m_ConnectState = FALSE;
}

CMenuBarPanel::~CMenuBarPanel(void)
{
	ClearMenu();
	session.Close();
	if (m_ConnectState)
	{
		delete pConnection;
	}
	PlaySound(NULL, NULL, NULL);
}
BEGIN_MESSAGE_MAP(CMenuBarPanel, CPanel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_COMMAND(ID_VIDEO_FILE, &CMenuBarPanel::OnVideoFile)
	ON_COMMAND(ID_QUIT, &CMenuBarPanel::OnQuit)
	ON_COMMAND(ID_LOCK, &CMenuBarPanel::OnLock)
	ON_COMMAND(ID_SWITCH_USER, &CMenuBarPanel::OnSwitchUser)
	ON_COMMAND(ID_BACKUP_DATA, &CMenuBarPanel::OnBackupData)
	ON_COMMAND(ID_RESTORE_DATA, &CMenuBarPanel::OnRestoreData)
	ON_COMMAND(ID_USER_MANAGEMENT, &CMenuBarPanel::OnUserManagement)
	ON_COMMAND(ID_SET, &CMenuBarPanel::OnSet)
	ON_COMMAND(ID_PEOPLE_COUNT, &CMenuBarPanel::OnPeopleCount)
	ON_COMMAND(ID_CALENDAR, &CMenuBarPanel::OnCalendar)
	ON_COMMAND(ID_SERVER_SET, &CMenuBarPanel::OnServerSet)
	ON_COMMAND(ID_CAMARE_SET, &CMenuBarPanel::OnCamareSet)
	ON_COMMAND(ID_USER_MANUAL, &CMenuBarPanel::OnUserManual)
	ON_COMMAND(ID_ABOUT, &CMenuBarPanel::OnAbout)
	ON_MESSAGE(GLOBAL_EVENT, &CMenuBarPanel::OnUserLogin)
END_MESSAGE_MAP()

LRESULT CMenuBarPanel::OnUserLogin(WPARAM EID, LPARAM _event) {
	CMenu* submenu = m_menu.GetSubMenu(3);
	submenu->EnableMenuItem(ID_CAMARE_SET, MF_BYCOMMAND | MF_ENABLED);
	return TRUE;
}

void CMenuBarPanel::OnPaint()
{
	CPaintDC dc(this);
	CRect rt;
	GetClientRect(rt);
	if(!m_imgMenuBar.IsNull())
	{
		m_imgMenuBar.Draw(dc.GetSafeHdc(),rt);
	}
    //绘制菜单项
	for(int i=0;i<m_vecPMenuItem.size();i++)
	{
		CMenuItem* pMenuItem=m_vecPMenuItem.at(i);
		if(pMenuItem)
		{
			pMenuItem->Draw(&dc);
		}
	}
}
CMenuBarPanel * CMenuBarPanel::GetInstance()
{
	return m_pThis;
}

CMenuBarPanel * CMenuBarPanel::m_pThis = NULL;

int CMenuBarPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	EventBus::regist(Events::UserLogin::id(), GetSafeHwnd());
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(m_imgMenuBar.IsNull())
	{
		m_imgMenuBar.LoadFromResource(AfxGetInstanceHandle(),IDB_CAPTION);
	}
	LoadMenuFromResource(IDR_MENU1);
	return 0;
}

void CMenuBarPanel::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);
}

void CMenuBarPanel::ClearMenu()
{
   for(int i=0;i<m_vecPMenuItem.size();i++)
   {
	   CMenuItem* pMenuItem=m_vecPMenuItem.at(i);
	   if(pMenuItem)
	   {
          delete pMenuItem;
		  pMenuItem=NULL;
	   }
   }
}

void CMenuBarPanel::LoadMenuFromResource(UINT  uIDResource)
{
   m_menu.LoadMenu(uIDResource);
   int MenuItemCount=m_menu.GetMenuItemCount();
   for(int i=0;i<MenuItemCount;i++)
   {
     CMenuItem* pMenuItem=new CMenuItem;
	 if(pMenuItem)
	 {
		 if (i == 3)
		 {
			 pMenuItem->m_rcMenu = CRect(i * 40, 0, (i + 1) * 45, 27);
			 m_menu.GetMenuString(i, pMenuItem->m_csMenuText, MF_BYPOSITION);
			 pMenuItem->m_hPopMenu = m_menu.GetSubMenu(i)->m_hMenu;
			 pMenuItem->m_dwState = CMenuItem::Normal;
			 m_vecPMenuItem.push_back(pMenuItem);
		 }
		 else if (i == 4)
		 {
			 pMenuItem->m_rcMenu = CRect(i * 45, 0, (i + 1) * 45, 27);
			 m_menu.GetMenuString(i, pMenuItem->m_csMenuText, MF_BYPOSITION);
			 pMenuItem->m_hPopMenu = m_menu.GetSubMenu(i)->m_hMenu;
			 pMenuItem->m_dwState = CMenuItem::Normal;
			 m_vecPMenuItem.push_back(pMenuItem);
		 }
		 else 
		 {
			 pMenuItem->m_rcMenu = CRect(i * 40, 0, (i + 1) * 40, 27);
			 m_menu.GetMenuString(i, pMenuItem->m_csMenuText, MF_BYPOSITION);
			 pMenuItem->m_hPopMenu = m_menu.GetSubMenu(i)->m_hMenu;
			 pMenuItem->m_dwState = CMenuItem::Normal;
			 m_vecPMenuItem.push_back(pMenuItem);
		 }
       
	 }
   }
}

CMenuItem::CMenuItem()
{
    m_rcMenu=CRect(0,0,0,0);
	m_csMenuText=_T("");
	m_hPopMenu=NULL;
	m_dwState=Normal;
}

CMenuItem::~CMenuItem()
{

}

void CMenuItem::Draw(CDC* pDC)
{
  int nMode=pDC->SetBkMode(TRANSPARENT);
  CFont font;
  font.CreateFont(14,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));
  CFont* pFont=pDC->SelectObject(&font);
  CRect DrawRect;
  DrawRect.CopyRect(m_rcMenu);
  DrawRect.DeflateRect(0,3,0,3);
  if(m_dwState==Normal)
  {
  }
  else if(m_dwState==Hover)
  {
	  CPen PenHover(PS_SOLID,1,RGB(186,201,211));
	  CPen* PHover =pDC->SelectObject(&PenHover);
	  pDC->Rectangle(DrawRect);
	  pDC->SelectObject(PHover);
  }
  else if(m_dwState==Pressed)
  {
	  CPen pen(PS_SOLID,1,RGB(173,186,194));
	  CPen* Pen =pDC->SelectObject(&pen);
	  pDC->Rectangle(DrawRect);
	  pDC->SelectObject(Pen);
  }
  pDC->DrawText(m_csMenuText,m_csMenuText.GetLength(),DrawRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_WORD_ELLIPSIS);
  pDC->SelectObject(pFont);
  pDC->SetBkMode(nMode);
}

void CMenuItem::UpDate(CRgn RefrshRgn)
{
}

BOOL CMenuBarPanel::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMenuBarPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rt;
	GetClientRect(rt);
	if(rt.PtInRect(point))
	{
       HitTest(point);
	   if(!m_bTracking)
	   {
		   m_bTracking=TRUE;
		   TRACKMOUSEEVENT tme;
		   tme.cbSize=sizeof(TRACKMOUSEEVENT);
		   tme.dwFlags=TME_LEAVE;
		   tme.dwHoverTime=1;
		   tme.hwndTrack=m_hWnd;
		   TrackMouseEvent(&tme);
	   }
	}
	CPanel::OnMouseMove(nFlags, point); 
}

void CMenuBarPanel::HitTest(CPoint pt)
{
	CRgn RgnTotal;
	CRgn RgnHover;
	CRgn RgnNormal;
    for(int i=0;i<m_vecPMenuItem.size();i++)
	{
        CMenuItem* pMenuItem=m_vecPMenuItem.at(i);
		if(pMenuItem)
		{
				if(pMenuItem->m_rcMenu.PtInRect(pt))
				{
					if(pMenuItem->m_dwState==CMenuItem::Normal)
					{
						pMenuItem->m_dwState=CMenuItem::Hover;
					}
					RgnHover.CreateRectRgnIndirect(pMenuItem->m_rcMenu);
					InvalidateRect(pMenuItem->m_rcMenu);
				}
				else
				{
					if(pMenuItem->m_dwState!=CMenuItem::Normal)
					{
						pMenuItem->m_dwState=CMenuItem::Normal;
						RgnNormal.CreateRectRgnIndirect(pMenuItem->m_rcMenu);
					}
				}
		}
	}
	RgnTotal.CreateRectRgn(0,0,1,1);
	RgnTotal.CombineRgn(&RgnHover,&RgnNormal,RGN_OR);
	InvalidateRgn(&RgnTotal);
}

void CMenuBarPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPanel::OnLButtonDown(nFlags, point);
	int nIndex=GetMenuIndex(point);
	if(nIndex!=HTERROR)
	{
		CMenuItem* pMenuItem=GetMenuItem(nIndex);
		if(pMenuItem)
		{
		  pMenuItem->m_dwState=CMenuItem::Pressed;
		  InvalidateRect(pMenuItem->m_rcMenu);
          ShowPopupMenu(nIndex,CPoint(pMenuItem->m_rcMenu.left,pMenuItem->m_rcMenu.bottom));
		}
	}
}

LRESULT CALLBACK CMenuBarPanel::MessageProc(int code,WPARAM wParam,LPARAM lParam)
{
    MSG* pMsg=(MSG*)lParam;
	g_bShowMenu=TRUE;
	CPoint pt;
	pt.x = LOWORD(pMsg->lParam); 
	pt.y = HIWORD(pMsg->lParam);
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
		   if(g_MenuBar)
		   {
			   CPoint ptTemp=pt;
			   g_MenuBar->ScreenToClient(&ptTemp);
			   int nIndex=g_MenuBar->GetMenuIndex(ptTemp);
			   g_MenuBar->OnMouseMove(0,ptTemp);
		   }
		}
		break;
	case WM_MOUSELEAVE:
		{
              
		}
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	 default:
		break;
	}
	return CallNextHookEx(g_FilterHook,code,wParam,lParam);
}

void CMenuBarPanel::ShowPopupMenu(int nIndex,CPoint pt)
{
	 this->ClientToScreen(&pt);
     CMenuItem* pMenuItem=GetMenuItem(nIndex);
	 g_MenuBar=this;
	 if(pMenuItem)
	 {
		 if(g_FilterHook==NULL)
		 {
            g_FilterHook=SetWindowsHookEx(WH_MSGFILTER,MessageProc,NULL,GetCurrentThreadId());
		 }
		 ::TrackPopupMenu(pMenuItem->m_hPopMenu,TPM_TOPALIGN|TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,0,m_hWnd,0);
		 if(g_FilterHook)
		 {
			 UnhookWindowsHookEx(g_FilterHook);
			 g_FilterHook=NULL;
		 }
	 }
}

int CMenuBarPanel::GetMenuIndex(CPoint pt )
{
	for(int i=0;i<m_vecPMenuItem.size();i++)
	{
		CMenuItem* pMenuItem=m_vecPMenuItem.at(i);
		if(pMenuItem)
		{
			if(pMenuItem->m_rcMenu.PtInRect(pt))
			{
				return i;
			}
		}
	}
	return HTERROR;
}

CMenuItem* CMenuBarPanel::GetMenuItem(int nIndex)
{
  CMenuItem* pMenuItem=NULL;
  if(nIndex>=0 && nIndex<m_vecPMenuItem.size())
  {
	  pMenuItem=m_vecPMenuItem.at(nIndex);
  }
  return pMenuItem;
}
void CMenuBarPanel::OnMouseLeave()
{
	CPanel::OnMouseLeave();
	m_bTracking=false;

	for(int i=0;i<m_vecPMenuItem.size();i++)
	{
		CMenuItem* pMenuItem=m_vecPMenuItem.at(i);
		if(pMenuItem->m_dwState==CMenuItem::Pressed)
		{
			break;
		}
		if(pMenuItem->m_dwState!=CMenuItem::Normal)
		{
			pMenuItem->m_dwState=CMenuItem::Normal;
			InvalidateRect(pMenuItem->m_rcMenu);
			return ; 
		}	
	}  
}


void CMenuBarPanel::OnVideoFile()//打开视频文件
{
	
	CFileDialog COpenFileDlg(true,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,TEXT("视频集文件(*.avr)|*.avi|"),NULL);	
	if(COpenFileDlg.DoModal() == IDOK)  //显示对话框并判断返回值，
	{
		//返回IDOK,表示按了确定，这里处理按确定的结果。
		CString	strFileName = COpenFileDlg.GetPathName();
	//	ShellExecute(GetSafeHwnd(),NULL,strFileName,NULL,NULL,SW_SHOWNORMAL);	
	}
	Invalidate();
	return; 
}


void CMenuBarPanel::OnQuit() //退出
{
	// TODO: 在此添加命令处理程序代码
}


void CMenuBarPanel::OnLock()//锁定
{
	// TODO: 在此添加命令处理程序代码
}


void CMenuBarPanel::OnSwitchUser()//切换用户
{
	// TODO: 在此添加命令处理程序代码
}


void CMenuBarPanel::OnBackupData()//备份数据
{
	// TODO: 在此添加命令处理程序代码
}


void CMenuBarPanel::OnRestoreData()//还原数据
{
	// TODO: 在此添加命令处理程序代码
}


void CMenuBarPanel::OnUserManagement()//用户管理
{
	// TODO: 在此添加命令处理程序代码
}


void CMenuBarPanel::OnSet()//设置
{
	// TODO: 在此添加命令处理程序代码

}


void CMenuBarPanel::OnPeopleCount()//人流量统计
{
	// TODO: 在此添加命令处理程序代码
}


void CMenuBarPanel::OnCalendar()//日程表
{
	// TODO: 在此添加命令处理程序代码
	PlaySound(_T("image\\warning.wav"), NULL, SND_FILENAME | SND_SYNC);
	
}

void CMenuBarPanel::OnServerSet()//服务器设置对话框
{
	m_ServerSet.DoModal();
}


void CMenuBarPanel::OnCamareSet()//摄像头设置
{
	m_CameraSet.DoModal();
}


void CMenuBarPanel::connectServer()
{
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * 20);
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);
	pConnection = session.GetHttpConnection(strTextIp, (INTERNET_PORT)_ttoi(strTexPort));
	CHttpFile* pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,
		TEXT("/submit?user=222"));

	CString headers = NULL;
	pFile->AddRequestHeaders(headers);
	pFile->SendRequest();//向http服务器发送请求

	DWORD dwRet;
	pFile->QueryInfoStatusCode(dwRet);
	if (dwRet != HTTP_STATUS_OK)
	{
		CString errText;
		errText.Format(_T("连接出错，错误码：%d"), dwRet);
		AfxMessageBox(errText);
	}
	else
	{
		int len = pFile->GetLength();
		char buf[2000];
		int numread;
		CString filepath;
		CString strFile = L"response.txt";
		filepath.Format(L".\\%s", strFile);
		CFile myfile(filepath,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		while ((numread = pFile->Read(buf, sizeof(buf)-1)) > 0)
		{
		buf[numread] = '\0';
		strFile += buf;
		myfile.Write(buf, numread);
		}
		CString sucText;

		sucText.Format(_T("连接成功"));
		AfxMessageBox(sucText);
		m_ConnectState = TRUE;
		myfile.Close();
		pFile->Close();
		delete pFile;
	}
	
	CString str = _T("http://");
	std::string message;
	str = str + strTextIp + _T(":") + strWebSocketPost;
	message = CT2CA(str.GetBuffer(0));
	//TODO:
	//endpoint.connect(message);
}

void CMenuBarPanel::OnUserManual()//用户手册
{
	// TODO: 在此添加命令处理程序代码
	/*CString s1("\u6f2b\u53cb11\u5e744\u6708\u4e2d");
	AfxMessageBox(s1);*/
}

void CMenuBarPanel::OnAbout()//关于
{
	// TODO: 在此添加命令处理程序代码
	m_Aboutdlg.DoModal();
}

void CMenuBarPanel::sendSetList(CString SendData)
{
	/*TODO:
	CCameraSet *pWnd = CCameraSet::GetInstance();
	if (pWnd == NULL) return;
	CHttpFile* pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, SendData);
	pFile->SendRequest();
	char cha_html[1024] = { 0 };
	pFile->Read((void*)cha_html, 1024);
	Json::Reader reader;
	Json::Value root;

	if (!reader.parse(cha_html, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
		return;
	int stroute = 0;
	int size = root.size();
	const Json::Value arrayObj = root;
	for (int i = 0; i < arrayObj.size(); i++)
	{
		std::string m_OncrossState  = arrayObj[size - 1 - i]["cross"].asString();
		std::string m_OninvadeState = arrayObj[size - 1 - i]["invade"].asString();
		std::string m_OnwanderState = arrayObj[size - 1 - i]["wander"].asString();
		std::string m_OnfightState = arrayObj[size - 1 - i]["fight"].asString();
		CString str0(m_OncrossState.c_str());
		CString str1(m_OninvadeState.c_str());
		CString str2(m_OnwanderState.c_str());
		CString str3(m_OnfightState.c_str());
		pWnd->m_EventList.SetItemText(stroute, 1, str0);
		pWnd->m_EventList.SetItemText(stroute, 2, str1);
		pWnd->m_EventList.SetItemText(stroute, 3, str2);
		pWnd->m_EventList.SetItemText(stroute, 4, str3);
		stroute += 1;
	}
	*/
}
void CMenuBarPanel::sendData(CString SendData)
{
	
	CHttpFile* pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, SendData);
	pFile->SendRequest();
	pFile->Close();
	delete pFile;
}

UINT taskRecieve0(LPVOID param)
{
	//AfxBeginThread(taskRecieve0, this);
	CMenuBarPanel * pTaskMain = (CMenuBarPanel *)param;
	while (1)
	{
		//	pTaskMain->InsertAlarm();
		//	pTaskMain->m_AlarmValue += 1;
	}
	return TRUE;
}

void CMenuBarPanel::InsertAlarm(CString strtime, int stroute, CString strname, CString strcontent)
{
	/*TODO:
	CWarningPanel *pWnd = CWarningPanel::GetInstance();
	if (pWnd == NULL) return;
	CString str;
	str.Format(_T("%d"), m_AlarmValue);
	pWnd->m_WarningList.InsertItem(m_AlarmValue, str);
	pWnd->m_WarningList.SetItemText(m_AlarmValue, 1, strtime);
	str.Format(_T("%d"), stroute);
	pWnd->m_WarningList.SetItemText(m_AlarmValue, 2, str);
	//pWnd->m_WarningList.SetItemText(m_AlarmValue, 3, strname);
	if (strcontent == "CrossBorder")
	{
		pWnd->m_WarningList.SetItemText(m_AlarmValue, 4, _T("越界"));
	}
	if (strcontent == "walkDectect")
	{
		pWnd->m_WarningList.SetItemText(m_AlarmValue, 4, _T("入侵"));
	}
	if (strcontent == "actionDectect")
	{
		pWnd->m_WarningList.SetItemText(m_AlarmValue, 4, _T("徘徊"));
	}
	if (strcontent == "fightDectect")
	{
		pWnd->m_WarningList.SetItemText(m_AlarmValue, 4, _T("打架"));
	}
	//pWnd->m_WarningList.SetItemText(m_AlarmValue, 5, strproduct);
	pWnd->m_WarningList.SetItemText(m_AlarmValue, 6, _T(" "));
	pWnd->m_WarningList.SetItemText(m_AlarmValue, 7, _T(" "));

	if (warning)
	{
		warning = FALSE;
		PlaySound(_T("image\\warning.wav"), NULL, SND_FILENAME | SND_SYNC);
	}
	pWnd->Invalidate();*/
}
void CMenuBarPanel::InsertAlarmLog(CString strtime, int stroute, CString strname, CString strcontent)
{
	CAlarmLog *pWnd = CAlarmLog::GetInstance();
	if (pWnd == NULL) return;
	CString str;
	str.Format(_T("%d"), m_AlarmValue);
	pWnd->m_AlarmLogList.InsertItem(m_AlarmValue, str);
	pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 1, strtime);
	str.Format(_T("%d"), stroute);
	pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 2, str);
	//pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 3, strname);
	if (strcontent == "CrossBorder")
	{
		pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 4, _T("越界"));
	}
	if (strcontent == "walkDectect")
	{
		pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 4, _T("入侵"));
	}
	if (strcontent == "actionDectect")
	{
		pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 4, _T("徘徊"));
	}
	if (strcontent == "fightDectect")
	{
		pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 4, _T("打架"));
	}
	//pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 5, strproduct);
	pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 6, _T(" "));
	pWnd->m_AlarmLogList.SetItemText(m_AlarmValue, 7, _T(" "));
}
