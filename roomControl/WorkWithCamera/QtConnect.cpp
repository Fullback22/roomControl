#include "QtConnect.h"

QtConnect::QtConnect(QWidget *parent): QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	VmbErrorType err = VmbErrorSuccess;
	err = system_.Startup();
	if (err == VmbErrorSuccess)
	{
		system_.GetCameras(cameras_);
		system_.GetInterfaces(interfaces_);
		updateActivInterface();
	}
	if (ui.comboBox_cameras->count() > 0)
	{
		slot_openConnectMenu();
		connect(ui.pb_connect, &QPushButton::clicked, this, &QtConnect::slot_pbClicked);
		connect(ui.pb_connectMenu, &QPushButton::clicked, this, &QtConnect::slot_openConnectMenu);
		connect(ui.pb_ipMenu, &QPushButton::clicked, this, &QtConnect::slot_openIpMenu);
		connect(ui.pb_sendIpCommand, &QPushButton::clicked, this, &QtConnect::slot_setNewIpParams);
	}
	else
	{
		setDisablePushButton();
	}
}

QtConnect::~QtConnect()
{
	cameras_.clear();
	interfaces_.clear();

	system_.Shutdown();
}

void QtConnect::slot_pbClicked()
{
	try
	{
		mainMenu_ = new QtMainMenu();
	}
	catch (...)
	{
		
	}
	connect(this, &QtConnect::moveCameraInformation, mainMenu_, &QtMainMenu::slot_getCameraInformation);
	connect(mainMenu_, &QtMainMenu::workWithCamera_close, this, &QtConnect::slot_shutdownCamera);
	mainMenu_->show();
	emit closeMainForm();
	emit moveCameraInformation(cameras_[ui.comboBox_cameras->currentIndex()]);
	this->hide();
}

void QtConnect::slot_openConnectMenu()
{
	ui.stackedWidget->setCurrentIndex(0);
	this->resize(this->minimumSize());
	ui.pb_connectMenu->setDown(true);
	ui.pb_ipMenu->setDown(false);
}

void QtConnect::slot_openIpMenu()
{
	ui.stackedWidget->setCurrentIndex(1);
	this->resize(this->maximumSize());
	ui.pb_connectMenu->setDown(false);
	ui.pb_ipMenu->setDown(true);
	setIpSettingsToGui();
}

void QtConnect::slot_setNewIpParams()
{

	VmbInt64_t buferForIntParams;
	AVT::VmbAPI::FeaturePtr feature{};
	VmbErrorType err = VmbErrorSuccess;
	

	const char* kFeatureSend = "GevDeviceForceIP";
	const char* featureIpAddress = "GevDeviceForceIPAddress";
	const char* featureSubnetMask = "GevDeviceForceSubnetMask";
	const char* featureGateway = "GevDeviceForceGateway";
	const char* kFeatureDeviceSelector = "DeviceSelector";

	AVT::VmbAPI::InterfacePtr mInterfacePtr{ interfaces_[ui.comboBox_cameras->currentIndex()] };
	err = mInterfacePtr->Open();
	
	AVT::VmbAPI::FeaturePtr selectorFeature;
	err = mInterfacePtr->GetFeatureByName(kFeatureDeviceSelector, selectorFeature);
	buferForIntParams = ui.comboBox_cameras->currentIndex();
	err = selectorFeature->SetValue(buferForIntParams);

	err = mInterfacePtr->GetFeatureByName(featureIpAddress, feature);
	buferForIntParams = convertIpV4ToNumber(ui.le_ipAdressForce->text());
	err = feature->SetValue(buferForIntParams);
	if (VmbErrorSuccess != err)
	{

	}
	
	buferForIntParams = 0;
	feature->GetValue(buferForIntParams);
	QString asd{ convertNumberToIpV4(buferForIntParams) };
	err = mInterfacePtr->GetFeatureByName(featureSubnetMask, feature);
	buferForIntParams = convertIpV4ToNumber(ui.le_subnetMaskForce->text());
	err = feature->SetValue(buferForIntParams);
	if (VmbErrorSuccess != err)
	{

	}

	err = mInterfacePtr->GetFeatureByName(featureGateway, feature);
	buferForIntParams = convertIpV4ToNumber(ui.le_gatewayForce->text());
	err = feature->SetValue(buferForIntParams);
	if (VmbErrorSuccess != err)
	{

	}
	err = mInterfacePtr->GetFeatureByName(kFeatureSend, feature);
	err = feature->RunCommand();
	err = mInterfacePtr->Close();

	cameras_.clear();
	interfaces_.clear();
	system_.Shutdown();

	err = system_.Startup();

	system_.GetCameras(cameras_);
	system_.GetInterfaces(interfaces_);
	updateActivInterface();
}

void QtConnect::slot_shutdownCamera()
{
	delete this;
}

void QtConnect::setIpSettingsToGui()
{
	if (interfaces_.size() >= 1) 
	{	
		setReadAndWriteForDeviseGroup();
		VmbInt64_t buferForIntParams;
		AVT::VmbAPI::FeaturePtr feature{};
		VmbErrorType err = VmbErrorSuccess;
		AVT::VmbAPI::InterfacePtr mInterfacePtr{ interfaces_[ui.comboBox_cameras->currentIndex()]};
		
		const char* featureMacAddress = "GevDeviceMACAddress";
		const char* featureIpAddress = "GevDeviceIPAddress";
		const char* featureSubnetMask = "GevDeviceSubnetMask";
		const char* featureGateway = "GevDeviceForceGateway";
		const char* kFeatureDeviceSelector = "DeviceSelector";
		
		err = mInterfacePtr->Open();
		err = mInterfacePtr->GetFeatureByName("DeviceUpdateList", feature);
		if (VmbErrorSuccess == err)
		{
			err = feature->RunCommand();
		}
		
		AVT::VmbAPI::FeaturePtr selectorFeature;
		err = mInterfacePtr->GetFeatureByName(kFeatureDeviceSelector, selectorFeature);
		buferForIntParams = ui.comboBox_cameras->currentIndex();
		err = selectorFeature->SetValue(buferForIntParams);
		err = mInterfacePtr->GetFeatureByName(featureMacAddress, feature);
		if (VmbErrorSuccess != err)
		{
			setReadOnlyForDeviseGroup();
			mInterfacePtr->Close();
		}
		feature->GetValue(buferForIntParams);
		QString mac{ convertNumberToMac(buferForIntParams) };
		ui.le_macAdressDevice->setText(mac);
		ui.le_macAdressForce->setText(mac);
		
		err = mInterfacePtr->GetFeatureByName(featureIpAddress, feature);
		if (VmbErrorSuccess != err)
		{
			setReadOnlyForDeviseGroup();
			mInterfacePtr->Close();
		}
		feature->GetValue(buferForIntParams);
		QString ip{ convertNumberToIpV4(buferForIntParams) };
		ui.le_ipAdressDevice->setText(ip);
		ui.le_ipAdressForce->setText(ip);

		err = mInterfacePtr->GetFeatureByName(featureSubnetMask, feature);
		if (VmbErrorSuccess != err)
		{
			setReadOnlyForDeviseGroup();
			mInterfacePtr->Close();
		}
		feature->GetValue(buferForIntParams);
		QString mask{ convertNumberToIpV4(buferForIntParams) };
		ui.le_subnetMaskDevice->setText(mask);
		ui.le_subnetMaskForce->setText(mask);

		err = mInterfacePtr->GetFeatureByName(featureGateway, feature);
		if (VmbErrorSuccess != err)
		{
			setReadOnlyForDeviseGroup();
			mInterfacePtr->Close();
		}
		feature->GetValue(buferForIntParams);
		QString gateway{ convertNumberToIpV4(buferForIntParams) };
		ui.le_gatewayDevice->setText(gateway);
		ui.le_gatewayForce->setText(gateway);

		setReadOnlyForDeviseGroup();
		mInterfacePtr->Close();
	}
	else
	{

	}
}

void QtConnect::setReadOnlyForDeviseGroup()
{
	ui.le_macAdressForce->setReadOnly(true);

	ui.le_macAdressDevice->setReadOnly(true);
	ui.le_ipAdressDevice->setReadOnly(true);
	ui.le_subnetMaskDevice->setReadOnly(true);
	ui.le_gatewayDevice->setReadOnly(true);
}

void QtConnect::setReadAndWriteForDeviseGroup()
{
	ui.le_macAdressForce->setReadOnly(false);

	ui.le_macAdressDevice->setReadOnly(false);
	ui.le_ipAdressDevice->setReadOnly(false);
	ui.le_subnetMaskDevice->setReadOnly(false);
	ui.le_gatewayDevice->setReadOnly(false);
}

void QtConnect::setDisablePushButton()
{
	ui.pb_cancelIpCommand->setDisabled(true);
	ui.pb_connect->setDisabled(true);
	ui.pb_sendIpCommand->setDisabled(true);
	ui.pb_connectMenu->setDisabled(true);
	ui.pb_ipMenu->setDisabled(true);
}

void QtConnect::setEnablePushButton()
{
	ui.pb_cancelIpCommand->setEnabled(true);
	ui.pb_connect->setEnabled(true);
	ui.pb_sendIpCommand->setEnabled(true);
	ui.pb_connectMenu->setEnabled(true);
	ui.pb_ipMenu->setEnabled(true);
}

void QtConnect::updateActivInterface()
{
	interfaces_.clear();
	ui.comboBox_cameras->clear();

	VmbErrorType err = VmbErrorSuccess;
	err = system_.GetInterfaces(interfaces_);
	
	if (err == VmbErrorSuccess)
	{
		std::string deviceId{};
		std::string deviceModelName{};

		for (auto& interfaceIterator : interfaces_)
		{
			AVT::VmbAPI::FeaturePtr feature{};
			err = interfaceIterator->Open();
			if (err == VmbErrorSuccess)
			{
				err = interfaceIterator->GetFeatureByName("DeviceModelName", feature);
				if (err == VmbErrorSuccess)
					err = feature->GetValue(deviceModelName);

				err = interfaceIterator->GetFeatureByName("DeviceID", feature);
				if (err == VmbErrorSuccess)
					err = feature->GetValue(deviceId);
				
				if(err == VmbErrorSuccess)
					ui.comboBox_cameras->addItem(QString::fromStdString(deviceModelName + " ID:" + deviceId));
				interfaceIterator->Close();
			}
		}
	}
}
