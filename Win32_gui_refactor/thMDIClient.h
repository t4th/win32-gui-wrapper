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

                        thMDIClient();
public:
                        thMDIClient(thForm *, int, int);
                        ~thMDIClient();

    void                Cascade(void);          // arrange childen in cascade format
    void                TileHorizontal(void);   // arrange childen in horizontal tile format
    void                TileVertical(void);     // arrange childen in vertical tile format
    void                ArrangeIcons(void);     // arrange minimazed icons
};

