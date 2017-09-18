#pragma once
#include<vector>
using namespace std;
#include "DeskContant.h"
#include "DeskButton.h"

typedef struct
{
	CDeskButton destButton;
	CDeskContant wndPanel;
}DESK,*PDESK;

class CDeskPanel :
	public CPanel
{
public:
	CDeskPanel(void);
	~CDeskPanel(void);
public:
	int AddDesk(CString strTitle);
	DESK* GetDesk(int nIndex);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	void ArrangeDesk();
	vector<DESK*> m_vecDesk;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	
};

