#pragma once

#include <QWidget>
#include "ui_QtGuiCameraSetup.h"

#include <QWidget>
#include "ui_QtGuiSetupSensor.h"

#include "widget.h"
#include "qspinbox.h"
#include "WorkWithCamera/FrameObserver.h"
//#include "Loger/simplLoger.h"
#include "VmbFrame.h"

class QtGuiCameraSetup : public QWidget
{
	Q_OBJECT

public:
	QtGuiCameraSetup(QWidget *parent = nullptr);
	~QtGuiCameraSetup();
	friend class FrameObserver;
	double getRoiResizeCoefficient(int value);  //коэффициент уменьшения окна в зависимости от спинбоксов

	void setVmbSensor(VmbFrame* sensorObj);
	void setVideoDisplay(QtGuiDisplay* display);

private:
	Ui::QtGuiCameraSetupClass ui;
	QtGuiDisplay* videoDisplay{ nullptr };
	VmbFrame* sensorObject{ nullptr };
	QSizeF const MAX_FRAME_SIZE{ 4872.0, 3248.0 };
	int oldFrameDelay{};
	
	bool chagheROI{ false };
	bool updateCameraParams{ false };


	void setCameraParamsInGui();
	void closeEvent(QCloseEvent* event);
	void setExprosureValue();
	void setGainValue();
public slots:
	void slot_updateSensorObject(VmbFrame* sensorObj);

	void slot_cahgeOfsetX(int newOffsetX);
	void slot_cahgeOfsetY(int newOffsetY);
	void slot_cahgeHeigth(int newHeigth);
	void slot_cahgeWidth(int newWidth);


	void slot_setOffset(QRectF& point);
	void slot_setSizeItemInSpinBox(QSizeF& itemSize);
	void slot_changeBinning(int value);

	void slot_pushFull();
	void slot_pushOneQuarter();
	void slot_pushOneEighth();
	void slot_pushSetRoi();

	//expouser auto
	void slot_pushContinous();//continous
	void slot_pushOnce();//once
	void slot_pushOff();//off
	void slot_pushGainContinous();//continous
	void slot_pushGainOnce();//once
	void slot_pushGainOff();//off
	void slot_dataToGuiWorkWithCamera();
	void slot_changeExprosureSeconds(int newValue);
	void slot_changeExprosureMilSeconds(int newValue);
	void slot_changeExprosureMicSeconds(int newValue);
	void slot_changeExprosure();
	void slot_changeGain(int newValue);
	void slot_changeExprosureAutoTarget(int newValue);
signals:
	void sl_BinningChange(int value);
	void sl_buttonChangeSizeClicked(double changeKoef);
	void signal_getNewOffsetX(int newOffsetX);
	void signal_getNewOffsetY(int newOffsetY);
	void signal_getNewHeigth(int newHeigth);
	void signal_getNewWidth(int newWidth);
};
