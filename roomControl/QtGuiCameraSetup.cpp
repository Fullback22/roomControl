#include "QtGuiCameraSetup.h"

QtGuiCameraSetup::QtGuiCameraSetup(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.setRoiWid, &Widget::CoordItemChange, this, &QtGuiCameraSetup::slot_setOffset);							//при изменении положения РОИ меняются значения спинбоксов
	connect(ui.setRoiWid, &Widget::ItemFromWidgetSizeChange, this, &QtGuiCameraSetup::slot_setSizeItemInSpinBox);		//при изменении width ,height ROI изменяется значение в спинбоксах
	connect(this, &QtGuiCameraSetup::sl_buttonChangeSizeClicked, ui.setRoiWid, &Widget::st_buttonChangeSizeClicked);
	connect(ui.spinBox_binningHor, qOverload<int>(&QSpinBox::valueChanged), this, &QtGuiCameraSetup::slot_changeBinning);							//binning horizontal
	connect(ui.spinBox_binningVer, qOverload<int>(&QSpinBox::valueChanged), this, &QtGuiCameraSetup::slot_changeBinning);							//binning vertical
	connect(ui.pb_setRoi, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushSetRoi);
	connect(ui.pb_full, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushFull);
	connect(ui.pb_oneQuarter, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushOneQuarter);
	connect(ui.pb_oneEighth, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushOneEighth);
	connect(ui.pb_continuous, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushContinous);
	connect(ui.pb_once, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushOnce);
	connect(ui.pb_off, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushOff);
	connect(ui.spinBox_ofsetX, qOverload<int>(&QSpinBox::valueChanged), this, &QtGuiCameraSetup::slot_cahgeOfsetX);
	connect(ui.spinBox_ofsetY, qOverload<int>(&QSpinBox::valueChanged), this, &QtGuiCameraSetup::slot_cahgeOfsetY);
	connect(ui.spinBox_height, qOverload<int>(&QSpinBox::valueChanged), this, &QtGuiCameraSetup::slot_cahgeHeigth);
	connect(ui.spinBox_width, qOverload<int>(&QSpinBox::valueChanged), this, &QtGuiCameraSetup::slot_cahgeWidth);
	connect(this, &QtGuiCameraSetup::signal_getNewOffsetX, ui.setRoiWid, &Widget::slot_setNewOffsetX);
	connect(this, &QtGuiCameraSetup::signal_getNewOffsetY, ui.setRoiWid, &Widget::slot_setNewOffsetY);
	connect(this, &QtGuiCameraSetup::signal_getNewHeigth, ui.setRoiWid, &Widget::slot_setNewHeigth);
	connect(this, &QtGuiCameraSetup::signal_getNewWidth, ui.setRoiWid, &Widget::slot_setNewWidth);

	connect(ui.horSlider_exprosureSeconds, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeExprosureSeconds);
	connect(ui.horSlider_exprosureMilSeconds, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeExprosureMilSeconds);
	connect(ui.horSlider_exprosureMicSeconds, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeExprosureMicSeconds);
	connect(ui.horSlider_exprosureSeconds, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeExprosure);
	connect(ui.horSlider_exprosureMilSeconds, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeExprosure);
	connect(ui.horSlider_exprosureMicSeconds, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeExprosure);

	connect(ui.pb_GainContinuous, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushGainContinous);
	connect(ui.pb_GainOonce, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushGainOnce);
	connect(ui.pb_GainOff, &QPushButton::clicked, this, &QtGuiCameraSetup::slot_pushGainOff);
	connect(ui.horSlider_gain, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeGain);
	connect(ui.horSlider_exprosureAutoTarget, &QSlider::valueChanged, this, &QtGuiCameraSetup::slot_changeExprosureAutoTarget);

	setAttribute(Qt::WA_DeleteOnClose, true);
}

QtGuiCameraSetup::~QtGuiCameraSetup()
{
}

double QtGuiCameraSetup::getRoiResizeCoefficient(int value)
{
	return 1.0 / (1 + abs(ui.spinBox_binningHor->value() - ui.spinBox_binningVer->value()));
}

void QtGuiCameraSetup::setCameraParamsInGui()
{

	AVT::VmbAPI::FeaturePtrVector pFeaturs{};

	VmbInt64_t buferForIntParams{ 0 };
	AVT::VmbAPI::FeaturePtr pFeature{};
	AVT::VmbAPI::CameraPtr camera{ sensorObject->getCameraPtr() };

	camera->GetFeatures(pFeaturs);

	camera->GetFeatureByName("BinningHorizontal", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.spinBox_binningHor->setValue(static_cast<int>(buferForIntParams));

	camera->GetFeatureByName("BinningVertical", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.spinBox_binningVer->setValue(static_cast<int>(buferForIntParams));

	camera->GetFeatureByName("Height", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.spinBox_height->setValue(static_cast<int>(buferForIntParams));

	camera->GetFeatureByName("Width", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.spinBox_width->setValue(static_cast<int>(buferForIntParams));

	camera->GetFeatureByName("OffsetX", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.spinBox_ofsetX->setValue(static_cast<int>(buferForIntParams));

	camera->GetFeatureByName("OffsetY", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.spinBox_ofsetY->setValue(static_cast<int>(buferForIntParams));

	camera->GetFeatureByName("ExposureAutoTarget", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.horSlider_exprosureAutoTarget->setValue(static_cast<int>(buferForIntParams));

	setExprosureValue();

	std::string buferForStringParams{};
	camera->GetFeatureByName("ExposureAuto", pFeature);
	pFeature->GetValue(buferForStringParams);
	if (buferForStringParams == "Continuous")
	{
		ui.pb_off->setDown(false);
		ui.pb_once->setDown(false);
		ui.pb_continuous->setDown(true);
		ui.le_exprosureMilSeconds->setEnabled(false);
		ui.le_exprosureMicSecods->setEnabled(false);
		ui.le_exprosureSeconds->setEnabled(false);
		ui.horSlider_exprosureMicSeconds->setEnabled(false);
		ui.horSlider_exprosureMilSeconds->setEnabled(false);
		ui.horSlider_exprosureSeconds->setEnabled(false);
	}
	else if (buferForStringParams == "Once")
	{
		ui.pb_off->setDown(false);
		ui.pb_once->setDown(true);
		ui.pb_continuous->setDown(false);
		ui.le_exprosureMilSeconds->setEnabled(false);
		ui.le_exprosureMicSecods->setEnabled(false);
		ui.le_exprosureSeconds->setEnabled(false);
		ui.horSlider_exprosureMicSeconds->setEnabled(false);
		ui.horSlider_exprosureMilSeconds->setEnabled(false);
		ui.horSlider_exprosureSeconds->setEnabled(false);
	}
	else if (buferForStringParams == "Off")
	{
		ui.pb_off->setDown(true);
		ui.pb_once->setDown(false);
		ui.pb_continuous->setDown(false);

		ui.le_exprosureMilSeconds->setEnabled(true);
		ui.le_exprosureMicSecods->setEnabled(true);
		ui.le_exprosureSeconds->setEnabled(true);
		ui.horSlider_exprosureMicSeconds->setEnabled(true);
		ui.horSlider_exprosureMilSeconds->setEnabled(true);
		ui.horSlider_exprosureSeconds->setEnabled(true);
	}

	setGainValue();
	camera->GetFeatureByName("GainAuto", pFeature);
	pFeature->GetValue(buferForStringParams);
	if (buferForStringParams == "Continuous")
	{
		ui.pb_GainOff->setDown(false);
		ui.pb_GainOonce->setDown(false);
		ui.pb_GainContinuous->setDown(true);

		ui.le_gain->setEnabled(false);
		ui.horSlider_gain->setEnabled(false);
	}
	else if (buferForStringParams == "Once")
	{
		ui.pb_GainOff->setDown(false);
		ui.pb_GainOonce->setDown(true);
		ui.pb_GainContinuous->setDown(false);

		ui.le_gain->setEnabled(false);
		ui.horSlider_gain->setEnabled(false);
	}
	else if (buferForStringParams == "Off")
	{
		ui.pb_GainOff->setDown(true);
		ui.pb_GainOonce->setDown(false);
		ui.pb_GainContinuous->setDown(false);

		ui.le_gain->setEnabled(true);
		ui.horSlider_gain->setEnabled(true);
	}
}

void QtGuiCameraSetup::slot_updateSensorObject(VmbFrame* sensorObj)
{
	sensorObject->updateFrame(sensorObj->getMat(), sensorObj->getPixmap());
	if (ui.setupSensorParams->currentIndex() == 0)
	{
		setExprosureValue();
		setGainValue();
	}
}

void QtGuiCameraSetup::slot_cahgeOfsetX(int newOffsetX)
{
	if (chagheROI)
	{
		ui.spinBox_width->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() - newOffsetX);
		emit signal_getNewOffsetX(newOffsetX / (MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / ui.setRoiWid->size().width()));
	}
}

void QtGuiCameraSetup::slot_cahgeOfsetY(int newOffsetY)
{
	if (chagheROI)
	{
		ui.spinBox_height->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() - newOffsetY);
		emit signal_getNewOffsetY(newOffsetY / (MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / ui.setRoiWid->size().height()));
	}
}

void QtGuiCameraSetup::slot_cahgeHeigth(int newHeigth)
{
	if (chagheROI)
	{
		ui.spinBox_ofsetY->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() - newHeigth);
		emit signal_getNewHeigth(newHeigth / (MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / ui.setRoiWid->size().height()));
	}
}

void QtGuiCameraSetup::slot_cahgeWidth(int newWidth)
{
	if (chagheROI)
	{
		ui.spinBox_ofsetX->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() - newWidth);
		emit signal_getNewWidth(newWidth / (MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / ui.setRoiWid->size().width()));
	}
}

void QtGuiCameraSetup::slot_setOffset(QRectF& point)
{
	chagheROI = false;
	ui.spinBox_ofsetY->setValue(point.y() * (MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / ui.setRoiWid->size().height()));
	ui.spinBox_ofsetX->setValue(point.x() * (MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / ui.setRoiWid->size().width()));
	ui.spinBox_height->setValue(point.height() * (MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / ui.setRoiWid->size().height()));
	ui.spinBox_width->setValue(point.width() * (MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / ui.setRoiWid->size().width()));

	ui.spinBox_ofsetY->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() - point.height());
	ui.spinBox_ofsetX->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() - point.width());
	ui.spinBox_height->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() - point.y());
	ui.spinBox_width->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() - point.x());

	if (ui.spinBox_width->maximum() >= MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value())
		ui.spinBox_width->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value());
	if (ui.spinBox_ofsetY->maximum() < 0)
		ui.spinBox_ofsetY->setMaximum(0);
	if (ui.spinBox_ofsetX->maximum() < 0)
		ui.spinBox_ofsetX->setMaximum(0);
	if (ui.spinBox_height->maximum() >= MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value())
		ui.spinBox_height->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value());
	chagheROI = true;
}

void QtGuiCameraSetup::slot_setSizeItemInSpinBox(QSizeF& itemSize)
{
	ui.spinBox_height->setValue(itemSize.height() * (MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / MAX_FRAME_SIZE.height()));
	ui.spinBox_width->setValue(itemSize.width() * (MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / MAX_FRAME_SIZE.width()));
}

void QtGuiCameraSetup::slot_changeBinning(int value)
{

	double K = getRoiResizeCoefficient(value);
	K = K + 0;

	if (ui.spinBox_binningHor->value() - ui.spinBox_binningVer->value() < 0) //		H - W
	{
		ui.setRoiWid->setFixedHeight(256 * K);  //H

	}
	else if (ui.spinBox_binningHor->value() - ui.spinBox_binningVer->value() > 0)
	{
		ui.setRoiWid->setFixedWidth(256 * K);  //W
	}
	else
	{
		ui.setRoiWid->setFixedWidth(256);  //W
		ui.setRoiWid->setFixedHeight(256);;  //H
	}

	ui.spinBox_height->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value());
	ui.spinBox_width->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value());

	chagheROI = false;
	ui.spinBox_ofsetX->setValue(0);
	ui.spinBox_ofsetY->setValue(0);
	ui.spinBox_height->setValue(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value());
	ui.spinBox_width->setValue(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value());
	chagheROI = true;

	emit sl_buttonChangeSizeClicked(1);
}

void QtGuiCameraSetup::slot_pushFull()
{
	ui.pb_full->setDown(true);
	ui.pb_oneEighth->setDown(false);//1/4
	ui.pb_oneQuarter->setDown(false);//1/8

	ui.spinBox_height->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value());
	ui.spinBox_width->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value());

	chagheROI = false;
	ui.spinBox_ofsetY->setValue(0);
	ui.spinBox_ofsetX->setValue(0);
	ui.spinBox_height->setValue(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value());
	ui.spinBox_width->setValue(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value());
	chagheROI = true;
	emit sl_buttonChangeSizeClicked(1);
}

void QtGuiCameraSetup::slot_pushOneQuarter()
{
	ui.pb_full->setDown(false);
	ui.pb_oneQuarter->setDown(true);//1/4
	ui.pb_oneEighth->setDown(false);//1/8

	ui.spinBox_height->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / 4);
	ui.spinBox_width->setMaximum((MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / 4));
	chagheROI = false;
	ui.spinBox_ofsetY->setValue(0);
	ui.spinBox_ofsetX->setValue(0);
	ui.spinBox_height->setValue(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / 4);
	ui.spinBox_width->setValue((MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / 4));
	chagheROI = true;
	emit sl_buttonChangeSizeClicked(1.0 / 4);
}

void QtGuiCameraSetup::slot_pushOneEighth()
{
	ui.pb_full->setDown(false);
	ui.pb_oneQuarter->setDown(false);//1/4
	ui.pb_oneEighth->setDown(true);//1/8


	ui.spinBox_height->setMaximum(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / 8);
	ui.spinBox_width->setMaximum(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / 8);

	chagheROI = false;
	ui.spinBox_ofsetY->setValue(0);
	ui.spinBox_ofsetX->setValue(0);
	ui.spinBox_height->setValue(MAX_FRAME_SIZE.height() / ui.spinBox_binningVer->value() / 8);
	ui.spinBox_width->setValue(MAX_FRAME_SIZE.width() / ui.spinBox_binningHor->value() / 8);
	chagheROI = true;
	emit sl_buttonChangeSizeClicked(1.0 / 8);
}

void QtGuiCameraSetup::slot_pushSetRoi()
{

	//LOG.logMessege("Start changing camera settings", _INFO_);
	//camera->SaveCameraSettings("camera.xml");
	AVT::VmbAPI::FeaturePtr pFeature;
	AVT::VmbAPI::CameraPtr camera{ sensorObject->getCameraPtr() };
	camera->GetFeatureByName("AcquisitionStop", pFeature);
	pFeature->RunCommand();

	camera->EndCapture();
	camera->FlushQueue();
	camera->RevokeAllFrames();
	/***********************************/
	//binning разрешения в tab2
	camera->GetFeatureByName("BinningHorizontal", pFeature);
	pFeature->SetValue(ui.spinBox_binningHor->value());

	camera->GetFeatureByName("BinningVertical", pFeature);
	pFeature->SetValue(ui.spinBox_binningVer->value());

	camera->GetFeatureByName("Height", pFeature);
	pFeature->SetValue(ui.spinBox_height->value());

	camera->GetFeatureByName("Width", pFeature);
	int tmp{ ui.spinBox_width->value() };
	if (tmp % 4 != 0)
	{
		tmp /= 4;
		++tmp;
		tmp *= 4;
	}

	pFeature->SetValue(tmp);

	camera->GetFeatureByName("OffsetX", pFeature);
	pFeature->SetValue(ui.spinBox_ofsetX->value());

	camera->GetFeatureByName("OffsetY", pFeature);
	pFeature->SetValue(ui.spinBox_ofsetY->value());
	/****************************************/


	// Get the image size for the required buffer								Получите размер изображения для выбора необходимого буфера
	// Allocate memory for frame buffer											Выделите память для буфера,который будет хранить кадр
	// Register frame observer/callback for each frame							Зарегистрируйте наблюдатель кадров / обратный вызов для каждого кадра
	// Announce frame to the API												Предоставьте кадр API


	camera->GetFeatureByName("PayloadSize", pFeature);							//Получить функцию по имени "Размер полезной нагрузки"(Размер кадра камеры). Полученную функцию возвращаем в pFeature  Feature-характеристика, функция
	VmbInt64_t nPLS;// Payload size value
	VmbInt64_t oldNPLS{ nPLS };
	pFeature->GetValue(nPLS);													//Запрос значения размера полезной нагрузки

	AVT::VmbAPI::FramePtrVector frames{ 3 }; // Frame array
	for (AVT::VmbAPI::FramePtrVector::iterator iter = frames.begin(); frames.end() != iter; ++iter)		//FramePtrVector - вектор указателей на кадры, frames.begin()- первый кадр, frames.end()- последний кадр. Цикл прохождения по каждому кадру
	{
		if (oldNPLS != nPLS)
			(*iter).reset(new AVT::VmbAPI::Frame(nPLS));											//сброс предыдущих настроек. Указываем новый размер для ,буффера кадра ( теперь он будет равен величине nPLS)
		(*iter)->RegisterObserver(AVT::VmbAPI::IFrameObserverPtr(new FrameObserver(camera, videoDisplay, sensorObject)));//Зарегистрировать наблюдателя camera(уже ссылается на нашу камеру,которую мы присвоили по ID)
		camera->AnnounceFrame(*iter);											//Предоставляем кадр из camera API
	}

	// Start the capture engine (API)											Запуск механизма захвата кадров
	camera->StartCapture();
	for (AVT::VmbAPI::FramePtrVector::iterator iter = frames.begin(); frames.end() != iter; ++iter)
	{
		// Put frame into the frame queue										Поместить кадр в очередь кадров
		camera->QueueFrame(*iter);
	}

	// Start the acquisition engine (camera)									Запустите механизм сбора данных (камеру)
	camera->GetFeatureByName("AcquisitionStart", pFeature);						//AcquisitionStart начать получение изображения
	pFeature->RunCommand();
	//LOG.logMessege("Camera settings set", _INFO_);
}

void QtGuiCameraSetup::slot_pushContinous()
{
	ui.pb_off->setDown(false);
	ui.pb_once->setDown(false);
	ui.pb_continuous->setDown(true);

	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("ExposureAuto", pFeature);
	std::string temp{ "Continuous" };
	pFeature->SetValue(temp.c_str());

	ui.le_exprosureMilSeconds->setEnabled(false);
	ui.le_exprosureMicSecods->setEnabled(false);
	ui.le_exprosureSeconds->setEnabled(false);
	ui.horSlider_exprosureMicSeconds->setEnabled(false);
	ui.horSlider_exprosureMilSeconds->setEnabled(false);
	ui.horSlider_exprosureSeconds->setEnabled(false);
}

void QtGuiCameraSetup::slot_pushOnce()
{
	ui.pb_off->setDown(false);
	ui.pb_once->setDown(true);
	ui.pb_continuous->setDown(false);

	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("ExposureAuto", pFeature);
	std::string temp{ "Once" };
	pFeature->SetValue(temp.c_str());

	ui.le_exprosureMilSeconds->setEnabled(false);
	ui.le_exprosureMicSecods->setEnabled(false);
	ui.le_exprosureSeconds->setEnabled(false);
	ui.horSlider_exprosureMicSeconds->setEnabled(false);
	ui.horSlider_exprosureMilSeconds->setEnabled(false);
	ui.horSlider_exprosureSeconds->setEnabled(false);
}

void QtGuiCameraSetup::slot_pushOff()
{
	ui.pb_off->setDown(true);
	ui.pb_once->setDown(false);
	ui.pb_continuous->setDown(false);

	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("ExposureAuto", pFeature);
	std::string temp{ "Off" };
	pFeature->SetValue(temp.c_str());

	ui.le_exprosureMilSeconds->setEnabled(true);
	ui.le_exprosureMicSecods->setEnabled(true);
	ui.le_exprosureSeconds->setEnabled(true);
	ui.horSlider_exprosureMicSeconds->setEnabled(true);
	ui.horSlider_exprosureMilSeconds->setEnabled(true);
	ui.horSlider_exprosureSeconds->setEnabled(true);
}

void QtGuiCameraSetup::slot_dataToGuiWorkWithCamera()
{
	updateCameraParams = true;
}

void QtGuiCameraSetup::slot_changeExprosureSeconds(int newValue)
{
	ui.le_exprosureSeconds->setText(QString::number(newValue));
	if (newValue == 60)
	{
		ui.le_exprosureMilSeconds->setEnabled(false);
		ui.le_exprosureMicSecods->setEnabled(false);
		ui.horSlider_exprosureMicSeconds->setEnabled(false);
		ui.horSlider_exprosureMilSeconds->setEnabled(false);
	}
	else if (!ui.le_exprosureMicSecods->isEnabled() && ui.le_exprosureSeconds->isEnabled())
	{
		ui.le_exprosureMilSeconds->setEnabled(true);
		ui.le_exprosureMicSecods->setEnabled(true);
		ui.horSlider_exprosureMicSeconds->setEnabled(true);
		ui.horSlider_exprosureMilSeconds->setEnabled(true);
	}
}

void QtGuiCameraSetup::slot_changeExprosureMilSeconds(int newValue)
{
	ui.le_exprosureMilSeconds->setText(QString::number(newValue));
}

void QtGuiCameraSetup::slot_changeExprosureMicSeconds(int newValue)
{
	ui.le_exprosureMicSecods->setText(QString::number(newValue));
}

void QtGuiCameraSetup::setVmbSensor(VmbFrame* sensorObj)
{
	sensorObject = sensorObj;
	//sensorObject->getCameraPtr()->SaveCameraSettings("oldCameraParams.xml");
	//oldFrameDelay = sensorObj->getFrameDelay();
	//ui.spinBox_trigerDelay->setValue(sensorObject->getFrameDelay());
	setCameraParamsInGui();
}

void QtGuiCameraSetup::setVideoDisplay(QtGuiDisplay* display)
{
	videoDisplay = display;
}

void QtGuiCameraSetup::closeEvent(QCloseEvent* event)
{
	/*if (isClose)
	{
		if (!changes)
		{
			sensorObject->getCameraPtr()->LoadCameraSettings("oldCameraParams.xml");
			sensorObject->setFrameDelay(oldFrameDelay);
		}
		this->close();
	}
	QtSetupSimulator::closeEvent(event);*/
}

void QtGuiCameraSetup::setGainValue()
{
	VmbInt64_t buferForIntParams{ 0 };
	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("GainRaw", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.horSlider_gain->setValue(buferForIntParams);

	sensorObject->getCameraPtr()->GetFeatureByName("GainAutoMax", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.horSlider_gain->setMaximum(buferForIntParams);

	sensorObject->getCameraPtr()->GetFeatureByName("GainAutoMin", pFeature);
	pFeature->GetValue(buferForIntParams);
	ui.horSlider_gain->setMinimum(buferForIntParams);

	ui.horSlider_gain->setTickInterval((ui.horSlider_gain->maximum() - ui.horSlider_gain->minimum()) / 6);
}

void QtGuiCameraSetup::setExprosureValue()
{
	double buferForDoubleParams{ 0.0 };
	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("ExposureTimeAbs", pFeature);
	pFeature->GetValue(buferForDoubleParams);
	ui.horSlider_exprosureSeconds->setValue(static_cast<int>(buferForDoubleParams) / 1000000);
	ui.horSlider_exprosureMilSeconds->setValue((static_cast<int>(buferForDoubleParams) % 1000000) / 1000);
	ui.horSlider_exprosureMicSeconds->setValue(static_cast<int>(buferForDoubleParams) % 1000000000);
}

void QtGuiCameraSetup::slot_changeExprosure()
{
	if (ui.horSlider_exprosureSeconds->value() != 60)
		ui.le_exprosureSumm->setText(QString::number(ui.horSlider_exprosureSeconds->value() * 1000000 + ui.horSlider_exprosureMilSeconds->value() * 1000 + ui.horSlider_exprosureMicSeconds->value()));
	else
		ui.le_exprosureSumm->setText(QString::number(ui.horSlider_exprosureSeconds->value() * 1000000));

	if (ui.horSlider_exprosureSeconds->isEnabled())
	{
		AVT::VmbAPI::FeaturePtr pFeature;
		sensorObject->getCameraPtr()->GetFeatureByName("ExposureTimeAbs", pFeature);
		pFeature->SetValue(ui.le_exprosureSumm->text().toDouble());
	}
}

void QtGuiCameraSetup::slot_pushGainContinous()
{
	ui.pb_GainOff->setDown(false);
	ui.pb_GainOonce->setDown(false);
	ui.pb_GainContinuous->setDown(true);

	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("GainAuto", pFeature);
	std::string temp{ "Continuous" };
	pFeature->SetValue(temp.c_str());

	ui.le_gain->setEnabled(false);
	ui.horSlider_gain->setEnabled(false);
}

void QtGuiCameraSetup::slot_pushGainOnce()
{
	ui.pb_GainOff->setDown(false);
	ui.pb_GainOonce->setDown(true);
	ui.pb_GainContinuous->setDown(false);

	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("GainAuto", pFeature);
	std::string temp{ "Once" };
	pFeature->SetValue(temp.c_str());

	ui.le_gain->setEnabled(false);
	ui.horSlider_gain->setEnabled(false);
}

void QtGuiCameraSetup::slot_pushGainOff()
{
	ui.pb_GainOff->setDown(true);
	ui.pb_GainOonce->setDown(false);
	ui.pb_GainContinuous->setDown(false);

	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("GainAuto", pFeature);
	std::string temp{ "Off" };
	pFeature->SetValue(temp.c_str());

	ui.le_gain->setEnabled(true);
	ui.horSlider_gain->setEnabled(true);
}

void QtGuiCameraSetup::slot_changeGain(int newValue)
{
	ui.le_gain->setText(QString::number(newValue));
	if (ui.horSlider_gain->isEnabled())
	{
		AVT::VmbAPI::FeaturePtr pFeature;
		sensorObject->getCameraPtr()->GetFeatureByName("GainRaw", pFeature);
		pFeature->SetValue(newValue);
	}
}

void QtGuiCameraSetup::slot_changeExprosureAutoTarget(int newValue)
{
	ui.le_exprosureAutoTarget->setText(QString::number(newValue));
	AVT::VmbAPI::FeaturePtr pFeature;
	sensorObject->getCameraPtr()->GetFeatureByName("ExposureAutoTarget", pFeature);
	pFeature->SetValue(newValue);
}