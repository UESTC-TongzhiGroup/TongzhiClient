// PeopleManagement.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "PeopleManagement.h"
#include "Config.h"
#include "StrUtil.h"

#define IDC_EDIT			10000
#define IDC_LICENSE_LIST	10001
#define IDC_SAVE			10002
#define IDC_REFRESH			10003

// CPeopleManagement 对话框

IMPLEMENT_DYNAMIC(CPeopleManagement, CDialogEx)

CPeopleManagement::CPeopleManagement(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPeopleManagement::IDD, pParent)
{
	licenseList.m_hWnd = NULL;
	edit.m_hWnd = NULL;
	save.m_hWnd = NULL;
	refresh.m_hWnd = NULL;
}

CPeopleManagement::~CPeopleManagement()
{
}

void CPeopleManagement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPeopleManagement, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LICENSE_LIST, &CPeopleManagement::OnEditList)
	ON_EN_KILLFOCUS(IDC_EDIT, &CPeopleManagement::OnEditLostFocus)
	ON_BN_CLICKED(IDC_SAVE, &CPeopleManagement::OnSave)
	ON_BN_CLICKED(IDC_REFRESH, &CPeopleManagement::OnRefresh)
END_MESSAGE_MAP()


// CPeopleManagement 消息处理程序


void CPeopleManagement::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect (rcClient,RGB(240,248,251));
	CDialogEx::OnPaint();
}


int CPeopleManagement::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
		if (CDialogEx::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rcClient;
		GetClientRect(&rcClient);
		if (licenseList.m_hWnd == NULL)
		{
			licenseList.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rcClient, this, IDC_LICENSE_LIST);
		}
		licenseList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		licenseList.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rcClient.Width() / 8, 0);
		licenseList.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, rcClient.Width() / 4, 1);
		licenseList.InsertColumn(2, _T("密码"), LVCFMT_CENTER, rcClient.Width() / 2, 2);

		if (save.m_hWnd == NULL)
		{
			save.CreateEx(_T("保存"), WS_VISIBLE | WS_CHILD, NULL, NULL, NULL, NULL, m_hWnd, (HMENU)IDC_SAVE);
			save.SetImageForButton(IDB_DESKBUTTON);
		}
		if (refresh.m_hWnd == NULL)
		{
			refresh.CreateEx(_T("刷新"), WS_VISIBLE | WS_CHILD, NULL, NULL, NULL, NULL, m_hWnd, (HMENU)IDC_REFRESH);
			refresh.SetImageForButton(IDB_DESKBUTTON);
		}
		if (edit.m_hWnd == NULL)
		{
			edit.Create(WS_CHILD, rcClient, this, IDC_EDIT);
		}
	return 0;
}


void CPeopleManagement::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rcClient;
	GetClientRect(&rcClient);
	if (licenseList.m_hWnd != NULL)
	{
		CRect rc = rcClient;
		rc.DeflateRect(5, 30, 5, 5);
		licenseList.MoveWindow(rc);
	}
	if (save.m_hWnd != NULL)
	{
		save.MoveWindow(rcClient.left + 5, rcClient.top, 50, 30);
	}
	if (refresh.m_hWnd != NULL)
	{
		refresh.MoveWindow(rcClient.left + 55, rcClient.top, 50, 30);
	}
}

void CPeopleManagement::OnEditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CRect rc;
	CString strTemp;
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	rowSel = pNMListView->iItem;
	colSel = pNMListView->iSubItem;

	if (pNMListView->iItem == -1)    //选择空白处，添加一行，并设置焦点为最后一行，第二列  
	{
		rowSel = licenseList.GetItemCount();
		strTemp.Format(_T("%d"), rowSel + 1);
		licenseList.InsertItem(rowSel, strTemp);
		licenseList.SetItemState(rowSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		licenseList.EnsureVisible(rowSel, FALSE);
		colSel = 1;
	}

	if (colSel != 0) // 选择子项  
	{
		licenseList.GetSubItemRect(rowSel, colSel, LVIR_LABEL, rc);
		edit.SetParent(&licenseList);
		edit.MoveWindow(rc);
		edit.SetWindowTextW(licenseList.GetItemText(rowSel, colSel));
		edit.ShowWindow(SW_SHOW);
		edit.SetFocus();//设置Edit焦点  
		edit.ShowCaret();//显示光标  
		edit.SetSel(0, -1);//全选  
	}
	*pResult = 0;
}

void CPeopleManagement::OnEditLostFocus()
{
	CString str;
	edit.GetWindowTextW(str);
	licenseList.SetItemText(rowSel, colSel, str);
	edit.ShowWindow(SW_HIDE);
}

bool CPeopleManagement::check()
{
	for (int i = 0; i < licenseList.GetItemCount(); i++) {
		string user = StrUtil::CStr2std(licenseList.GetItemText(i, 1)),
			pass = StrUtil::CStr2std(licenseList.GetItemText(i, 2));
		CString title;
		if (user.empty()) {
			title.Format(_T("第%d行出现错误"), i + 1);
			MessageBox(_T("用户名不应当为空"), title);
			return false;
		}
		if (pass.empty()) {
			title.Format(_T("第%d行出现错误"), i + 1);
			MessageBox(_T("密码不应当为空"), title);
			return false;
		}
	}
	return true;
}

void CPeopleManagement::OnSave()
{
	auto &lmap = Config::getLicensesMap();
	lmap.clear();
	if (check()) {
		for (int i = 0; i < licenseList.GetItemCount(); i++) {
			string user = StrUtil::CStr2std(licenseList.GetItemText(i, 1)),
				pass = StrUtil::CStr2std(licenseList.GetItemText(i, 2));
			lmap.insert({ user,pass });
		}
		Config::saveLicense();
		OnRefresh();
		MessageBox(_T("保存完毕"), _T("成功"));
	}
}

void CPeopleManagement::OnRefresh()
{
	licenseList.DeleteAllItems();
	for (auto &itor : Config::getLicensesMap()) {
		int i = licenseList.GetItemCount();
		CString str;
		str.Format(_T("%d"), i + 1);
		licenseList.InsertItem(i, str);
		licenseList.SetItemText(i, 1, StrUtil::std2CStr(itor.first));
		licenseList.SetItemText(i, 2, StrUtil::std2CStr(itor.second));
	}
}


BOOL CPeopleManagement::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OnRefresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
