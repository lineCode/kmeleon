/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: Mozilla-sample-code 1.0
 *
 * Copyright (c) 2002 Netscape Communications Corporation and
 * other contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this Mozilla sample software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Contributor(s):
 *   Chak Nanga <chak@netscape.com> 
 *
 * ***** END LICENSE BLOCK ***** */

// This interface acts as a glue between the required/optional 
// Gecko embedding interfaces and the actual platform specific
// way of doing things - such as updating a statusbar etc.
//
// For ex, in the mfcembed sample the required interfaces such as 
// IWebBrowserChrome etc. are implemented in a XP way in the
// BrowserImp*.cpp files. However, when they get called to update the
// statusbar etc. they call on this interface to get the actual job
// done. During the BrowserFrame creation some object must implement
// this interface and pass the pointer to it via the Init() member of
// the CBrowserImpl class

#ifndef _IBROWSERFRAMEGLUE_H
#define _IBROWSERFRAMEGLUE_H

class CBrowserWrapper;

struct IBrowserGlue {
    // Progress Related Methods
    virtual void UpdateStatusBarText(LPCTSTR aMessage) = 0;
    virtual void UpdateProgress(int aCurrent, int aMax) = 0;
    virtual void UpdateBusyState(BOOL aBusy) = 0;
    virtual void UpdateCurrentURI(nsIURI *aLocation) = 0;
    virtual void UpdateSecurityStatus(int aState) = 0;

    // BrowserFrame Related Methods
    virtual CBrowserWrapper* ReuseWindow(BOOL useCurrent) = 0;
    virtual void DestroyBrowserFrame() = 0;
    virtual void GetBrowserTitle(CString& aTitle) = 0;
    virtual void SetBrowserTitle(LPCTSTR aTitle) = 0;
    virtual void SetBrowserSize(int aCX, int aCY) = 0;
    virtual void SetVisibility(bool aVisible) = 0;
	virtual void GetVisibility(bool *aVisible) = 0;
	virtual void SetFocus() = 0;

    // ContextMenu Related Methods
    virtual void ShowContextMenu(UINT aContextFlags) = 0;

    //Prompt Related Methods
    virtual HWND GetBrowserFrameNativeWnd() = 0;

    // Tooltip function
    virtual void ShowTooltip(int x, int y, LPCTSTR text) = 0;

    virtual BOOL FocusNextElement() = 0;
    virtual BOOL FocusPrevElement() = 0;
	virtual BOOL MouseAction(nsIDOMNode *node, UINT flags) = 0;
	virtual void PopupBlocked(const char* uri) = 0;
	virtual void SetFavIcon(nsIURI* favUri) = 0;
	virtual bool performXULCommand(LPCWSTR id, LPCTSTR uri) = 0; 
	virtual BOOL AllowFlash() = 0;
	virtual void SetFullScreen(bool aFullscreen) = 0;

	virtual ~IBrowserGlue() {};
};

#define NS_DECL_BROWSERGLUE    \
    public: \
        virtual void UpdateStatusBarText(LPCTSTR aMessage);    \
        virtual void UpdateProgress(int aCurrent, int aMax);    \
        virtual void UpdateBusyState(PRBool aBusy);                     \
        virtual void UpdateCurrentURI(nsIURI *aLocation);               \
        virtual void UpdateSecurityStatus(int aState);         \
        virtual CBrowserWrapper* ReuseWindow(BOOL useCurrent); \
		virtual CBrowserWrapper* CreateNewBrowser(PRUint32 chromeMask);   \
        virtual void DestroyBrowserFrame();                         \
		virtual void SetBrowserSize(int aCX, int aCY);  \
		virtual void GetBrowserTitle(CString& aTitle);  \
        virtual void SetBrowserTitle(LPCTSTR aTitle); \
        virtual void SetVisibility(bool aVisible);                    \
		virtual void GetVisibility(bool *aVisible);       \
        virtual void SetFocus();                                        \
        virtual void ShowContextMenu(UINT aContextFlags); \
        virtual HWND GetBrowserFrameNativeWnd();                          \
        virtual void ShowTooltip(int x, int y, LPCTSTR text); \
        virtual BOOL FocusNextElement(); \
        virtual BOOL FocusPrevElement(); \
		virtual BOOL MouseAction(nsIDOMNode *node, UINT flags);\
		virtual void PopupBlocked(const char* uri);\
		virtual void SetFavIcon(nsIURI* favUri);\
		virtual bool performXULCommand(LPCWSTR id, LPCTSTR uri); \
		virtual BOOL AllowFlash(); \
		virtual void SetFullScreen(bool aFullscreen); \

typedef IBrowserGlue *PBROWSERGLUE;

struct IBrowserFrameGlue {
    // Progress Related Methods
    virtual void UpdateStatusBarText(const PRUnichar *aMessage) = 0;
    virtual void UpdateProgress(PRInt32 aCurrent, PRInt32 aMax) = 0;
    virtual void UpdateBusyState(PRBool aBusy) = 0;
    virtual void UpdateCurrentURI(nsIURI *aLocation) = 0;
    virtual void UpdateSecurityStatus(PRInt32 aState) = 0;

    // BrowserFrame Related Methods
    virtual PRBool CreateNewBrowserFrame(PRUint32 chromeMask, 
                            PRInt32 x, PRInt32 y, 
                            PRInt32 cx, PRInt32 cy,
                            nsIWebBrowser ** aWebBrowser) = 0;
    virtual void DestroyBrowserFrame() = 0;
    virtual void GetBrowserFrameTitle(PRUnichar **aTitle) = 0;
    virtual void SetBrowserFrameTitle(const PRUnichar *aTitle) = 0;
    virtual void SetBrowserSize(PRInt32 aCX, PRInt32 aCY) = 0;
	/*virtual void GetBrowserFramePosition(PRInt32 *aX, PRInt32 *aY) = 0;
    virtual void SetBrowserFramePosition(PRInt32 aX, PRInt32 aY) = 0;
    virtual void GetBrowserFrameSize(PRInt32 *aCX, PRInt32 *aCY) = 0;
    virtual void SetBrowserFrameSize(PRInt32 aCX, PRInt32 aCY) = 0;
    virtual void GetBrowserSize(PRInt32 *aCX, PRInt32 *aCY) = 0;
    virtual void GetBrowserFramePositionAndSize(PRInt32 *aX, PRInt32 *aY, PRInt32 *aCX, PRInt32 *aCY) = 0;
    virtual void SetBrowserFramePositionAndSize(PRInt32 aX, PRInt32 aY, PRInt32 aCX, PRInt32 aCY, PRBool fRepaint) = 0;
   virtual void SetBrowserPositionAndSize(PRInt32 aX, PRInt32 aY, PRInt32 aCX, PRInt32 aCY, PRBool fRepaint) = 0;*/
    virtual void ShowBrowserFrame(PRBool aShow) = 0;
    virtual void SetFocus() = 0;
    virtual void FocusAvailable(PRBool *aFocusAvail) = 0;
    virtual void GetBrowserFrameVisibility(PRBool *aVisible) = 0;

    // ContextMenu Related Methods
	virtual void ShowContextMenu(UINT aContextFlags) = 0;

    //Prompt Related Methods
    virtual HWND GetBrowserFrameNativeWnd() = 0;

    // Tooltip function
    virtual void ShowTooltip(PRInt32 x, PRInt32 y, const TCHAR *text) = 0;

    virtual BOOL FocusNextElement() = 0;
    virtual BOOL FocusPrevElement() = 0;
	virtual BOOL MouseAction(nsIDOMNode *node, UINT flags) = 0;
	virtual void PopupBlocked(const char* uri) = 0;
	virtual void SetFavIcon(nsIURI* favUri) = 0;
};

#define NS_DECL_BROWSERFRAMEGLUE    \
    public: \
        virtual void UpdateStatusBarText(const PRUnichar *aMessage);    \
        virtual void UpdateProgress(PRInt32 aCurrent, PRInt32 aMax);    \
        virtual void UpdateBusyState(PRBool aBusy);                     \
        virtual void UpdateCurrentURI(nsIURI *aLocation);               \
        virtual void UpdateSecurityStatus(PRInt32 aState);         \
        virtual PRBool CreateNewBrowserFrame(PRUint32 chromeMask, PRInt32 x, PRInt32 y, PRInt32 cx, PRInt32 cy, nsIWebBrowser** aWebBrowser);   \
        virtual void DestroyBrowserFrame();                         \
		virtual void SetBrowserSize(PRInt32 aCX, PRInt32 aCY);  \
		virtual void GetBrowserFrameTitle(PRUnichar **aTitle);  \
        virtual void SetBrowserFrameTitle(const PRUnichar *aTitle); \
        /*virtual void GetBrowserFramePosition(PRInt32 *aX, PRInt32 *aY); \
        virtual void SetBrowserFramePosition(PRInt32 aX, PRInt32 aY);   \
        virtual void GetBrowserFrameSize(PRInt32 *aCX, PRInt32 *aCY);   \
        virtual void SetBrowserFrameSize(PRInt32 aCX, PRInt32 aCY);     \
        virtual void GetBrowserSize(PRInt32 *aCX, PRInt32 *aCY);        \
        virtual void GetBrowserFramePositionAndSize(PRInt32 *aX, PRInt32 *aY, PRInt32 *aCX, PRInt32 *aCY);  \
        virtual void SetBrowserFramePositionAndSize(PRInt32 aX, PRInt32 aY, PRInt32 aCX, PRInt32 aCY, PRBool fRepaint); \
        virtual void SetBrowserPositionAndSize(PRInt32 aX, PRInt32 aY, PRInt32 aCX, PRInt32 aCY, PRBool fRepaint);  */\
        virtual void ShowBrowserFrame(PRBool aShow);                    \
        virtual void SetFocus();                                        \
        virtual void FocusAvailable(PRBool *aFocusAvail);               \
        virtual void GetBrowserFrameVisibility(PRBool *aVisible);       \
        virtual void ShowContextMenu(UINT aContextFlags); \
        virtual HWND GetBrowserFrameNativeWnd();                          \
        virtual void ShowTooltip(PRInt32 x, PRInt32 y, const TCHAR *text); \
        virtual BOOL FocusNextElement(); \
        virtual BOOL FocusPrevElement(); \
		virtual BOOL MouseAction(nsIDOMNode *node, UINT flags);\
		virtual void PopupBlocked(const char* uri);\
		virtual void SetFavIcon(nsIURI* favUri);\

typedef IBrowserFrameGlue *PBROWSERFRAMEGLUE;

#endif //_IBROWSERFRAMEGLUE_H
