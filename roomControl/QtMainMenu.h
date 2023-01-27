#pragma once

#include <QMainWindow>
#include "ui_QtMainMenu.h"
#include <VimbaCPP.h>
#include "VmbFrame.h"
#include "WorkWithCamera/FrameObserver.h"
#include "Display/QtGuiDisplay.h"

class QtMainMenu : public QMainWindow
{
	Q_OBJECT

public:
	QtMainMenu(QWidget *parent = nullptr);
	~QtMainMenu();
	
	VmbFrame cameraLife{};
	bool cameraIsPlayed{ false };

private:
	Ui::QtMainMenuClass ui;

public slots:
	void slot_getCameraInformation(AVT::VmbAPI::CameraPtr sensor);
	void slot_updateFrame();
signals:
	//void dataToSetingSensor(ProcessedObjectSensor* sensorObj, ProcessedObject* masterObject, QtGuiDisplay* videoDisplay);
	//void updateFrameInSetupSensor(ProcessedObjectSensor* sensorObj);
	void workWithCamera_close();
};
