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
			if (VmbErrorSuccess == sensor->Open(VmbAccessModeFull))											//открываем камеру в режиме "доступ для чтения и записи". Используйте этот режим для настройки функций камеры и получения изображений
			{
				int a;
				a = 10;
			}
			AVT::VmbAPI::FeaturePtr pFeature;
			if (VmbErrorSuccess == sensor->GetFeatureByName("PayloadSize", pFeature))							//Получить функцию по имени "Размер полезной нагрузки"(Размер кадра камеры). Полученную функцию возвращаем в pFeature  Feature-характеристика, функция
			{
				int a; 
				a = 10;
			}
			ui.cameraSetups->setVmbSensor(&cameraLife);
			VmbInt64_t nPLS;															// Payload size value
			pFeature->GetValue(nPLS);													//Запрос значения размера полезной нагрузки
			AVT::VmbAPI::FramePtrVector frames{ 3 };
			for (AVT::VmbAPI::FramePtrVector::iterator iter = frames.begin(); frames.end() != iter; ++iter)		//FramePtrVector - вектор указателей на кадры, frames.begin()- первый кадр, frames.end()- последний кадр. Цикл прохождения по каждому кадру
			{
				(*iter).reset(new AVT::VmbAPI::Frame(nPLS));											//сброс предыдущих настроек. Указываем новый размер для ,буффера кадра ( теперь он будет равен величине nPLS)
				(*iter)->RegisterObserver(AVT::VmbAPI::IFrameObserverPtr(new FrameObserver(sensor, ui.display, &cameraLife)));
				sensor->AnnounceFrame(*iter);
				//Предоставляем кадр из camera API
			}
			// Start the capture engine (API)											Запуск механизма захвата кадров
			sensor->StartCapture();
			for (AVT::VmbAPI::FramePtrVector::iterator iter = frames.begin(); frames.end() != iter; ++iter)
			{
				// Put frame into the frame queue										Поместить кадр в очередь кадров
				sensor->QueueFrame(*iter);
			}
			// Start the acquisition engine (camera)									Запустите механизм сбора данных (камеру)
			sensor->GetFeatureByName("AcquisitionStart", pFeature);						//AcquisitionStart начать получение изображения
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
