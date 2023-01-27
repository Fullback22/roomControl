#include "QtMainMenu.h"

QtMainMenu::QtMainMenu(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	ui.cameraSetups->setVideoDisplay(ui.display);

	connect(ui.display, &QtGuiDisplay::signal_updateFrame, this, &QtMainMenu::slot_updateFrame);
}

QtMainMenu::~QtMainMenu()
{
	emit workWithCamera_close();
}

void QtMainMenu::slot_updateFrame()
{
	ui.display->updateProcessObj(cameraLife);
}

void QtMainMenu::slot_getCameraInformation(AVT::VmbAPI::CameraPtr sensor)
{
	//LOG.logMessege("start conect with in QtGuiWorkWithCamera", _INFO_);
	cameraLife.setCameraPtr(sensor);
	
	try {
		if (!cameraIsPlayed)
		{
			if (VmbErrorSuccess == sensor->Open(VmbAccessModeFull))											//��������� ������ � ������ "������ ��� ������ � ������". ����������� ���� ����� ��� ��������� ������� ������ � ��������� �����������
			{
				int a;
				a = 10;
			}
			AVT::VmbAPI::FeaturePtr pFeature;
			if (VmbErrorSuccess == sensor->GetFeatureByName("PayloadSize", pFeature))							//�������� ������� �� ����� "������ �������� ��������"(������ ����� ������). ���������� ������� ���������� � pFeature  Feature-��������������, �������
			{
				int a; 
				a = 10;
			}
			ui.cameraSetups->setVmbSensor(&cameraLife);
			VmbInt64_t nPLS;															// Payload size value
			pFeature->GetValue(nPLS);													//������ �������� ������� �������� ��������
			AVT::VmbAPI::FramePtrVector frames{ 3 };
			for (AVT::VmbAPI::FramePtrVector::iterator iter = frames.begin(); frames.end() != iter; ++iter)		//FramePtrVector - ������ ���������� �� �����, frames.begin()- ������ ����, frames.end()- ��������� ����. ���� ����������� �� ������� �����
			{
				(*iter).reset(new AVT::VmbAPI::Frame(nPLS));											//����� ���������� ��������. ��������� ����� ������ ��� ,������� ����� ( ������ �� ����� ����� �������� nPLS)
				(*iter)->RegisterObserver(AVT::VmbAPI::IFrameObserverPtr(new FrameObserver(sensor, ui.display, &cameraLife)));
				sensor->AnnounceFrame(*iter);
				//������������� ���� �� camera API
			}
			// Start the capture engine (API)											������ ��������� ������� ������
			sensor->StartCapture();
			for (AVT::VmbAPI::FramePtrVector::iterator iter = frames.begin(); frames.end() != iter; ++iter)
			{
				// Put frame into the frame queue										��������� ���� � ������� ������
				sensor->QueueFrame(*iter);
			}
			// Start the acquisition engine (camera)									��������� �������� ����� ������ (������)
			sensor->GetFeatureByName("AcquisitionStart", pFeature);						//AcquisitionStart ������ ��������� �����������
			pFeature->RunCommand();
			cameraIsPlayed = true;
		}
	}
	catch (...)
	{
		//LOG.logMessege("frame read error", _ERROR_);
	}
	//LOG.logMessege("Camera connected", _INFO_);
	ui.display->setActivProcessObj(cameraLife);
}
