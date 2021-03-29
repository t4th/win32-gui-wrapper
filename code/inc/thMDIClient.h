#pragma once
#include "thWindow.h"

class thForm;

class thMDIClient : public thWindow
{
    friend LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
    //friend LRESULT CALLBACK MDIClientWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
protected:
    virtual int         getDebugIndex();
private:
    static int          m_indexPool;

public:
                        thMDIClient() = delete;
                        thMDIClient( thForm * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                        ~thMDIClient();

    void                Cascade(void);          // arrange childen in cascade format
    void                TileHorizontal(void);   // arrange childen in horizontal tile format
    void                TileVertical(void);     // arrange childen in vertical tile format
    void                ArrangeIcons(void);     // arrange minimazed icons
};

