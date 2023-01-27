#pragma once

#include <QWidget>
#include <qstring.h>
#include "ui_QtConnect.h"

#include <VimbaCPP.h>


#include "..\QtMainMenu.h"
#include "..\ipFunction.h"


class QtConnect : public QWidget
{
	Q_OBJECT

public:
	QtConnect(QWidget *parent = Q_NULLPTR);
	~QtConnect();

private:
	Ui::QtConnect ui;
	AVT::VmbAPI::VimbaSystem& system_{ AVT::VmbAPI::VimbaSystem::GetInstance() };
	AVT::VmbAPI::CameraPtrVector cameras_{};
	AVT::VmbAPI::InterfacePtrVector interfaces_{};
	QtMainMenu* mainMenu_{ nullptr };


	void setIpSettingsToGui();
	void setReadOnlyForDeviseGroup();
	void setReadAndWriteForDeviseGroup();
	void setDisablePushButton();
	void setEnablePushButton();
	void updateActivInterface();
signals:
	void moveCameraInformation(AVT::VmbAPI::CameraPtr activSensor);
	void closeMainForm();

public slots:
	void slot_shutdownCamera();
	
protected slots:
	void slot_pbClicked();
	void slot_openConnectMenu();
	void slot_openIpMenu();
	void slot_setNewIpParams();
};
