/*
*  Copyright (C) 2000 Brian Harris
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/*

  Why this is here:
  The mozilla team has decided that they are going to follow W3C standards rather than maintain
  backwards compatibility by only providing tooltips for "title" rather than "alt"
  While this is really great for standards, it's pretty crappy for users, so this little implementation
  will check the node to see if it has a "title" tag, and if it doesn't it will use the "alt" tag.

*/

#include "StdAfx.h"
#include "Tooltips.h"

// The Object:

NS_IMETHODIMP CTooltipTextProvider::GetNodeText(nsIDOMNode *aNode, PRUnichar **aText, PRBool *_retval)
{

/*
   *_retval = PR_FALSE;

   nsCOMPtr<nsIDOMNamedNodeMap> attributes;
   aNode->GetAttributes(getter_AddRefs(attributes));
   if (attributes)
   {
      nsString tipText;
      nsCOMPtr<nsIDOMNode> attrNode;
      nsAutoString attr;

      // first try to find the title
      attr.AssignWithConversion("title");
      attributes->GetNamedItem(attr, getter_AddRefs(attrNode));
      if (attrNode)
      {
         attrNode->GetNodeValue(tipText);
         *aText = ToNewUnicode(tipText);
         *_retval = PR_TRUE;
         return NS_OK;
      }

      // then look for the alt
      attr.AssignWithConversion("alt");
      attributes->GetNamedItem(attr, getter_AddRefs(attrNode));
      if (attrNode)
      {
         nsString tipText;
         attrNode->GetNodeValue(tipText);
         *aText = ToNewUnicode(tipText);
         *_retval = PR_TRUE;
         return NS_OK;
      }
   }
   else {
      nsresult rv2 = NS_OK;
      nsCOMPtr<nsIContent> content(do_QueryInterface(aNode, &rv2));
      if(NS_SUCCEEDED(rv2)) {
         nsCOMPtr<nsIAtom> atom;
         content->GetTag(*getter_AddRefs(atom));
         if (atom) {
            nsAutoString tag;
            atom->ToString(tag);

            char buf[4096];
            tag.ToCString(buf, tag.Length()+1);
//            ::MessageBox(NULL, buf, NULL, MB_OK);
         }      
      }      
   }

   return NS_OK;
*/

   NS_ENSURE_ARG_POINTER(aNode);
   NS_ENSURE_ARG_POINTER(aText);
   
   nsString titleText;
   nsString altText;

   nsString tagName;
   
   nsCOMPtr<nsIDOMNode> current ( aNode );
   while (!titleText.Length() && current ) {
      nsCOMPtr<nsIDOMElement> currElement ( do_QueryInterface(current) );
      if ( currElement ) {

         // first try the normal title attribute...
         currElement->GetAttribute(NS_LITERAL_STRING("title"), titleText);

         // that didn't work, try it in the XLink namespace         
         if (!titleText.Length())
            currElement->GetAttributeNS(NS_LITERAL_STRING("http://www.w3.org/1999/xlink"), NS_LITERAL_STRING("title"), titleText);

         // it's possible that the "alt" definition will be found
         // before "title"...if this happens, we don't need to keep
         // checking for "alt" tags
         if ( !titleText.Length() && !altText.Length() ) {
            // no dice, check for an "alt" tag
            currElement->GetAttribute(NS_LITERAL_STRING("alt"), altText);
            
            // still nothing, look for an "alt" tag in the XLink namespace
            if (!altText.Length())
               currElement->GetAttributeNS(NS_LITERAL_STRING("http://www.w3.org/1999/xlink"), NS_LITERAL_STRING("title"), altText);
         }
      }
      
      // title not found, walk up to the parent and keep trying
      if ( titleText.Length() == 0 ) {
         nsCOMPtr<nsIDOMNode> temp ( current );
         temp->GetParentNode(getter_AddRefs(current));
      }
   } // while not found

   if (titleText.Length())
      *aText = ToNewUnicode(titleText);
   else if (altText.Length())
      *aText = ToNewUnicode(altText);
   else
      *aText = NULL;

   *_retval = *aText ? PR_TRUE : PR_FALSE;

   return NS_OK;
}



// The Factory:


NS_GENERIC_FACTORY_CONSTRUCTOR(CTooltipTextProvider)

/*
static nsModuleComponentInfo components[] = {
   { "Tooltip Text Provider",
    NS_TOOLTIPTEXTPROVIDER_CID, 
    NS_TOOLTIPTEXTPROVIDER_CONTRACTID,
    CTooltipTextProviderConstructor }
};

NS_IMPL_NSGETMODULE("CTooltipTextProvider", components )
*/

/* nsISupports Implementation for the class */
NS_IMPL_ISUPPORTS1(CTooltipTextProvider,
                   nsITooltipTextProvider)



 
//*****************************************************************************
// CTooltipTextProviderFactory
//*****************************************************************************   

class CTooltipTextProviderFactory : public nsIFactory
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFACTORY

  CTooltipTextProviderFactory();
  virtual ~CTooltipTextProviderFactory();
};

//*****************************************************************************   

NS_IMPL_ISUPPORTS1(CTooltipTextProviderFactory, nsIFactory)

CTooltipTextProviderFactory::CTooltipTextProviderFactory()
{
  NS_INIT_ISUPPORTS();
}

CTooltipTextProviderFactory::~CTooltipTextProviderFactory()
{
}

NS_IMETHODIMP CTooltipTextProviderFactory::CreateInstance(nsISupports *aOuter, const nsIID & aIID, void **aResult)
{
  NS_ENSURE_ARG_POINTER(aResult);
  
  *aResult = NULL;  
  CTooltipTextProvider *inst = new CTooltipTextProvider;    
  if (!inst)
    return NS_ERROR_OUT_OF_MEMORY;
    
  nsresult rv = inst->QueryInterface(aIID, aResult);
  if (rv != NS_OK) {  
    // We didn't get the right interface, so clean up  
    delete inst;  
  }  
    
  return rv;
}

NS_IMETHODIMP CTooltipTextProviderFactory::LockFactory(PRBool lock)
{
    return NS_OK;
}


nsresult NewTooltipTextProviderFactory(nsIFactory** aFactory) {
   NS_ENSURE_ARG_POINTER(aFactory);
   *aFactory = nsnull;
   CTooltipTextProviderFactory *result = new CTooltipTextProviderFactory;
   if (!result)
      return NS_ERROR_OUT_OF_MEMORY;
   NS_ADDREF(result);
   *aFactory = result;
   return NS_OK;
}



// our tooltip window class

#include "MfcEmbed.h"
extern CMfcEmbedApp theApp;


BEGIN_MESSAGE_MAP(CKmToolTip, CWnd)
	//{{AFX_MSG_MAP(CKmToolTip)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CKmToolTip::CKmToolTip() {
   m_pszWndClass = AfxRegisterWndClass(CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW);
   pszText = NULL;
   
   NONCLIENTMETRICS ncm = {0};
   ncm.cbSize = sizeof(ncm);
   SystemParametersInfo(SPI_GETNONCLIENTMETRICS,0,(PVOID)&ncm,FALSE);
   m_pFont = CreateFontIndirect(&ncm.lfStatusFont);
}

void CKmToolTip::Create(CWnd *pWnd) {
   CWnd::Create(m_pszWndClass, NULL, WS_POPUP | WS_BORDER,
      CRect(10,10,150,50), pWnd, NULL, NULL);
}

CKmToolTip::~CKmToolTip() {
   DestroyWindow();
   UnregisterClass(m_pszWndClass, theApp.m_hInstance);
   DeleteObject(m_pFont);
   delete pszText;
}

void CKmToolTip::Hide() {
   ShowWindow(SW_HIDE);
   delete pszText;
   pszText = NULL;
}

void CKmToolTip::Show(const char *text, int x, int y) {
   
   delete pszText;
   pszText = strdup(text);
   
   // measure the size of the text
   CDC *DC = GetWindowDC();
   int nSavedDC = DC->SaveDC();

   DC->SelectObject(m_pFont);
   CRect rect;
   rect.top = 0;
   rect.bottom = 0;
   rect.left = 0;
   rect.right = 300; // max width of the tooltip

   DC->DrawText(text, rect, DT_CALCRECT | DT_WORDBREAK );
   DC->RestoreDC(nSavedDC);
   
   // add a bit of padding on the sides
   int width = rect.Width() + 10;
   int height = rect.Height() +2;
   
   // keep the tooltip from running off the window
   CWnd *pWndParent = GetParent();
   pWndParent->GetWindowRect(&rect);
   pWndParent->ScreenToClient(&rect);
   if ( x + width > rect.right)
      x = rect.right - width;
   if (y + height > rect.bottom)
      y = rect.bottom - height;
   
   SetWindowPos(NULL, x, y, width, height, SWP_NOZORDER /* | SWP_DRAWFRAME*/);
   ShowWindow(SW_SHOW);
}


void CKmToolTip::OnPaint() {      
   CPaintDC DC(this);
   int nSavedDC = DC.SaveDC();
   
   // Draw background
   COLORREF clrBackground = RGB(255, 255, 255);
   clrBackground = ::GetSysColor(COLOR_INFOBK);
   CRect ClientRect;
   GetClientRect(ClientRect);
   DC.FillSolidRect(ClientRect, clrBackground);
   
   // Draw text
   DC.SelectObject(m_pFont);
   DC.SetTextColor(::GetSysColor(COLOR_INFOTEXT));
   DC.SetBkMode(TRANSPARENT);
   
   ClientRect.left += 3;
   
   DC.DrawText(pszText, -1, ClientRect, DT_WORDBREAK);
   
   DC.RestoreDC(nSavedDC);
}

