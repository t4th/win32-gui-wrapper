#pragma once
#include "thObject.h"
#include "thText.h"
#include "thWidth.h"
#include "thHeight.h"
#include "thPosX.h"
#include "thPosY.h"
#include "thPopupMenu.h"
#include "thFont.h"

class thWindow;
class thText;
class thWidth;
class thHeight;
class thPosX;
class thPosY;
class thPopupMenu;

typedef LRESULT(*thCallbackFunc_t)(thWindow * pOwner);

class thWindow : public thObject
{
    friend LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
    friend LRESULT CALLBACK ChildWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
protected:
    thWindow *              m_pParent;
    HWND                    m_hWinHandle; // TODO: move to private and make public GetHandle() method

    // TODO: Remove.
    struct {
        DWORD       dwExStyle;
        LPCTSTR     lpClassName;
        LPCTSTR     lpWindowName;
        DWORD       dwStyle;
        int         nX;         // set in thWindow c-tor
        int         nY;         // set in thWindow c-tor
        int         nWidth;     // set in thWindow c-tor
        int         nHeight;    // set in thWindow c-tor
        HWND        hWndParent; // set in thWindow c-tor
        HMENU       hMenu;      // set in thObject via thWindow
        HINSTANCE   hInstance;  // set in thWindow c-tor
        LPVOID      lpParam;
    }   m_sWindowArgs;          // arguments for CreateWindowEx

    void                    create();
    virtual LRESULT         onCreate();     // WM_CREATE
    virtual LRESULT         onNCCreate();   // WM_NCCREATE
    virtual LRESULT         onClose();      // WM_CLOSE
    virtual LRESULT         onDestroy();    // WM_DESTROY
    virtual LRESULT         onSetText(LPARAM);    // WM_SETTEXT
    virtual LRESULT         onResize(HWND, WPARAM, LPARAM); // WM_SIZE
    virtual LRESULT         onGetMinMax(LPARAM);    // WM_GETMINMAXINFO
    virtual LRESULT         onContextMenu(WPARAM, LPARAM); //WM_CONTEXTMENU
    virtual LRESULT         onKeyDown(WPARAM, LPARAM); //WM_KEYDOWN

    LRESULT                 processMessage(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         processMenuCommandMessage(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         processNotifyMessage(HWND, UINT, WPARAM, LPARAM);

    virtual int             getDebugIndex() = 0;
    void                    createDebugName();

    // children windows accessors
    void                    addChildrenWindow(thWindow *);
    void                    removeChildrenWindow(thWindow *);
    thWindow * const        findChildrenByID(WORD);
    thWindow * const        findChildrenByHwnd(HWND);
private:
    std::vector<thWindow*>  m_children;

    // old child window position. Used by Anchors.
    RECT                    m_rcOldPosition;

    void                    GetRect(RECT &);
public:
                            thWindow() = delete;
                            thWindow( thWindow *, int, int);
    virtual                 ~thWindow();

    thText                  Text;
    thWidth                 Width;
    thHeight                Height;
    thPosX                  X;
    thPosY                  Y;
    thPopupMenu *           PopupMenu;
    thFont                  Font;

    struct {
        bool_t Left;      // true by default
        bool_t Right;     // false by default
        bool_t Bottom;    // false by default
        bool_t Top;       // true by default
    } Anchors;

    struct {
        int MinHeight;
        int MinWidth;
        int MaxHeight;
        int MaxWidth;
    } Constraints;

    void                    Show(void);
    void                    Hide(void);

    void                    Enable(void);
    void                    Disable(void);

    void                    Destroy(void);

    virtual void            SetFocus(void);

    thCallbackFunc_t        OnDestroy;

    thWindow *              GetParent() const;

    // todo:
    // invalidate
#if 1
    // win32 specific
    // this should be hidden behind public interface
    void                    StoreCurrentRect(void);
    HWND                    GetHandle(void) const;
#endif
};

