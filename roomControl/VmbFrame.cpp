#include "VmbFrame.h"

VmbFrame::VmbFrame()
{
}

VmbFrame::VmbFrame(cv::Mat inputMat, QPixmap inputPixmap, bool imageIsNull, int frameDelay):
	originalMat_{ inputMat },
	originalPixmap_{ inputPixmap },
	imageIsNull_{ imageIsNull },
	frameDelay_{ frameDelay },
	camera_{}
{
}

VmbFrame::VmbFrame(const VmbFrame& dep):
	originalMat_{ dep.originalMat_ },
	originalPixmap_{ dep.originalPixmap_ },
	imageIsNull_{ dep.imageIsNull_ },
	frameDelay_{ dep.frameDelay_ },
	camera_{ dep.camera_ }
{
}

void VmbFrame::setFrameDelay(int newDelay)
{
	frameDelay_ = newDelay;
}

int VmbFrame::getFrameDelay() const
{
	return frameDelay_;
}

AVT::VmbAPI::CameraPtr VmbFrame::getCameraPtr() const
{
	return camera_;
}

cv::Mat VmbFrame::getMat() const
{
	return originalMat_;
}

QPixmap VmbFrame::getPixmap() const
{
	return originalPixmap_;
}

bool VmbFrame::frameIsNull() const
{
	return imageIsNull_;
}

void VmbFrame::updateFrame(cv::Mat newMat, QPixmap newPixmap)
{
	originalMat_ = newMat;
	originalPixmap_ = newPixmap;
}

void VmbFrame::setStatusFrame(const bool isNull)
{
	imageIsNull_ = isNull;
}

void VmbFrame::setCameraPtr(AVT::VmbAPI::CameraPtr newCameraPtr)
{
	camera_ = newCameraPtr;
}

VmbFrame& VmbFrame::operator=(const VmbFrame& drop)
{
	originalMat_ = drop.originalMat_;
	originalPixmap_ = drop.originalPixmap_;
	imageIsNull_ = drop.imageIsNull_;
	frameDelay_ = drop.frameDelay_;
	camera_ = drop.camera_;
	return *this;
}
