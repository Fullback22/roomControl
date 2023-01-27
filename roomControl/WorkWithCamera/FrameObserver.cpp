#include "FrameObserver.h"


FrameObserver::FrameObserver(AVT::VmbAPI::CameraPtr pCamera, QtGuiDisplay* videaDisplay, VmbFrame* loadObj0):
	IFrameObserver(pCamera),
	display(videaDisplay),
	newFrame(loadObj0)
{
}

FrameObserver::FrameObserver(FrameObserver& f) : IFrameObserver(f)
{
}

FrameObserver::~FrameObserver()
{
	//LOG.logMessege("Frame obser destr", _INFO_);
}

void FrameObserver::FrameReceived(const AVT::VmbAPI::FramePtr pFrame)
{
	VmbFrameStatusType eReceiveStatus;
	if (VmbErrorSuccess == pFrame->GetReceiveStatus(eReceiveStatus))
	{
		if (VmbFrameStatusComplete == eReceiveStatus)
		{
			VmbUchar_t* pbuffer;
			unsigned int w, h;

			pFrame->GetImage(pbuffer);
			pFrame->GetHeight(h);
			pFrame->GetWidth(w);

			QImage newImage{ pbuffer, static_cast<int>(w), static_cast<int>(h), QImage::Format_Indexed8 };
			QPixmap newPixmap{ QPixmap::fromImage(newImage) };

			cv::Mat newMat{ static_cast<int>(h), static_cast<int>(w), CV_8UC1, pbuffer };
			cv::cvtColor(newMat, newMat, cv::COLOR_GRAY2BGR);
			newFrame->updateFrame(newMat, newPixmap);
			newFrame->setStatusFrame(false);
			display->updateFrame();
			//LOG.logMessege("new frame craded", _DEBUG_);
		}
		else
		{
			// Put your code here to react on an unsuccessfully received frame 
			//std::cout << "pnh";
		}
	}
	m_pCamera->QueueFrame(pFrame);
}


