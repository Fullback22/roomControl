#pragma once
#include "VimbaCPP.h"
#include <VimbaCPP/Include/IFrameObserver.h>
#include "iostream"
#include <opencv2/opencv.hpp>
#include <qimage.h>

#include "../VmbFrame.h"

#include "../Display/QtGuiDisplay.h"
//#include "Loger/simplLoger.h"

using namespace cv;

class FrameObserver : public AVT::VmbAPI::IFrameObserver
{
private:
	QtGuiDisplay* display{ nullptr };
	VmbFrame* newFrame{ nullptr };


public:
	FrameObserver(AVT::VmbAPI::CameraPtr pCamera, QtGuiDisplay* videaDisplay, VmbFrame* loadObj0);
	FrameObserver(FrameObserver& f);
	~FrameObserver();

	void FrameReceived(const AVT::VmbAPI::FramePtr pFrame) override;
};

