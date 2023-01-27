#pragma once
#include "qpixmap.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "VimbaCPP.h"
#include <VimbaCPP/Include/IFrameObserver.h>

class VmbFrame
{
    cv::Mat originalMat_{};
    QPixmap originalPixmap_{};
    bool imageIsNull_{false};
    int frameDelay_{ 0 };
    AVT::VmbAPI::CameraPtr camera_{};   
    
public:
    VmbFrame();
    VmbFrame(cv::Mat inputMat, QPixmap inputPixmap, bool imageIsNull = false, int frameDelay = 0);
    VmbFrame(const VmbFrame& dep);
    VmbFrame& operator=(const VmbFrame& drop);
    
    void setFrameDelay(int newDelay);
    void setCameraPtr(AVT::VmbAPI::CameraPtr newCameraPtr);
    
    int getFrameDelay() const;
    AVT::VmbAPI::CameraPtr getCameraPtr() const;
    cv::Mat getMat() const;
    QPixmap getPixmap() const;
    bool frameIsNull() const;

    void updateFrame(cv::Mat newMat, QPixmap newPixmap);
    void setStatusFrame(const bool isNull);
};

