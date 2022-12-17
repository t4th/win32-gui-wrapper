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

class thWindow : public thObject
{
    friend LRESULT CALLBACK WinProc( HWND, UINT, WPARAM, LPARAM);
    friend LRESULT CALLBACK ChildWindProc( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
public:
                            thWindow() = delete;
                            thWindow( thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
    virtual                 ~thWindow();

    thText                  Text;
    thWidth                 Width;
    thHeight                Height;
    thPosX                  X;
    thPosY                  Y;
    thPopupMenu *           PopupMenu;
    thFont                  Font;

    struct {
        bool_t Left{ true};
        bool_t Right{ false};
        bool_t Bottom{ false};
        bool_t Top{ true};
    } Anchors;

    struct {
        int MinHeight{ 0};
        int MinWidth{ 0};
        int MaxHeight{ 0};
        int MaxWidth{ 0};
    } Constraints;

    void                    Show();
    void                    Hide();

    void                    Enable();
    void                    Disable();

    void                    Destroy();

    virtual void            SetFocus();

    thEventCallbackFunc_t   OnDestroy{ nullptr};

    thWindow *              GetParent() const;

    // todo:
    // invalidate
#if 1
    // win32 specific
    // this should be hidden behind public interface
    void                    StoreCurrentRect();
    const HWND              GetHandle() const;
#endif

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
    virtual LRESULT         onSetText( LPARAM);    // WM_SETTEXT
    virtual LRESULT         onResize( HWND, WPARAM, LPARAM); // WM_SIZE
    virtual LRESULT         onGetMinMax( LPARAM);    // WM_GETMINMAXINFO
    virtual LRESULT         onContextMenu( WPARAM, LPARAM); //WM_CONTEXTMENU
    virtual LRESULT         onKeyDown( WPARAM, LPARAM); //WM_KEYDOWN

    LRESULT                 processMessage( HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         processCommandMessage( HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         processMenuCommandMessage( HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         processNotifyMessage( HWND, UINT, WPARAM, LPARAM);

    virtual int             getDebugIndex() = 0;
    void                    createDebugName();

    // children windows accessors
    void                    addChildrenWindow( thWindow *);
    void                    removeChildrenWindow( thWindow *);
    thWindow *              findChildrenByID( const WORD);

private:
    std::vector< thWindow*>  m_children;

    // old child window position. Used by Anchors.
    RECT                    m_rcOldPosition{};

    void                    GetRect( RECT &);
};

