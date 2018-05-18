

#ifndef MYBUTTON_H
#define MYBUTTON_H

#include "UIEventListener.hpp"

class MyBTN : public CButton
{
};

class BtnEvent : public EventListener<CButton>
{
    virtual void mouseMoveEvent(int, int)
    {
        MessageBoxA(NULL, "TESTBOX", "WIN32", MB_OK);
    };
    virtual void mouseDownEvent(EMouseButton, int, int)
    {
        MessageBoxA(NULL, "TESTBOX", "WIN32", MB_OK);
    };
};

class BtnView : public UIEventDispatcher<CButton>
{
};





//class PixmapView : public CView, public UIEventDispatcher<Pixmap_canvas> {
//    DECLARE_DYNCREATE(PixmapView)
//    ÍõË§ 14:07 : 24
//        GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
//    ScreenToClient(&rect);
//    pixmap_canvas_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
//    +-
//        Pixmap_item_ptr pixmap = pixmap_canvas_->Open("D:/Replication-Vs2010/bin/x64/wuhan/1.tif");
//    //pixmap_canvas_->Open("D:/2-data/wuhan/2.tif");
//
//    boost::shared_ptr<ValidRegionTrackball> trackball = boost::shared_ptr<ValidRegionTrackball>(new ValidRegionTrackball);
//    trackball->setActivedItem(pixmap);
//    pixmap_canvas_->addEventListener(trackball);
//    trackball->setEnabled(true);
//



#endif