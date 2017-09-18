#include "StdAfx.h"
#include "MenuEx.h"

CMenuEx::CMenuEx(void)
{
	m_iWidth=0;
}

CMenuEx::~CMenuEx(void)
{
}

void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
  lpMeasureItemStruct->itemWidth=m_iWidth;
  lpMeasureItemStruct->itemHeight=20;
	
}

void CMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
   ASSERT(lpDrawItemStruct->CtlType==ODT_MENU);
   HDC hDc=lpDrawItemStruct->hDC;
   CRect rcItem=lpDrawItemStruct->rcItem;
   PMENUITEM pMenuItem=(PMENUITEM)lpDrawItemStruct->itemData;
   if(lpDrawItemStruct->itemState&ODS_SELECTED)
   {
	   FillRect(hDc,rcItem,CreateSolidBrush(RGB(84,193,26)));
   }
   else
   {
	   FillRect(hDc,rcItem,CreateSolidBrush(RGB(255,255,255)));
   }
   if(pMenuItem)
   {   
	   
	   if(pMenuItem->hIcon)
	   {
		   CRect rcIcon;
		   rcIcon.left=2;
		   rcIcon.right=18;
		   rcIcon.top=rcItem.top+2;
		   rcIcon.bottom=rcIcon.top+16;
		   DrawIconEx(hDc,rcIcon.left,rcIcon.top,pMenuItem->hIcon,rcIcon.Width(),rcIcon.Height(),NULL,NULL,DI_NORMAL);
	   } 
	   int nOldMode=SetBkMode(hDc,TRANSPARENT);
	   if(!pMenuItem->strMenu.IsEmpty())
	   {
		   DrawText(hDc,pMenuItem->strMenu,pMenuItem->strMenu.GetLength(),CRect(20,rcItem.top,rcItem.right,rcItem.bottom),DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	   }
	   SetBkMode(hDc,nOldMode);
   }
 }

void CMenuEx::SetMenuWidth( int nWidth )
{
    m_iWidth=nWidth;
}