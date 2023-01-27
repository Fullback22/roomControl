#include "QtGuiDisplay.h"

QtGuiDisplay::QtGuiDisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	this->updateGeometry();

	connect(ui.horSB_forTempImg, &QScrollBar::valueChanged, this, &QtGuiDisplay::slot_mouvePixmap);
	connect(ui.verSB_forTempImg, &QScrollBar::valueChanged, this, &QtGuiDisplay::slot_mouvePixmap);
	
	connect(ui.pb_zoomIncress, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_In);
	connect(ui.pb_zoomDeduce, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_Out);
	connect(ui.pb_allWindow, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_AllLabl);
	
	connect(ui.pb_saveAs, &QPushButton::clicked, this, &QtGuiDisplay::slot_setSaveDirectory);
	connect(ui.pb_createScreenshot, &QPushButton::clicked, this, &QtGuiDisplay::slot_createSkrinshot);
	connect(ui.pb_startRecord, &QPushButton::clicked, this, &QtGuiDisplay::slot_startRecord);
	connect(ui.pb_stopRecord, &QPushButton::clicked, this, &QtGuiDisplay::slot_stopRecord);


	connect(ui.label_for_TempImg, &myLabel::mousePos, this, &QtGuiDisplay::slot_mouseCurrentPos);
	connect(ui.label_for_TempImg, &myLabel::mousePressed, this, &QtGuiDisplay::slot_mousePressed);
	connect(ui.label_for_TempImg, &myLabel::mouseRelease, this, &QtGuiDisplay::slot_mouseRelease);
	connect(ui.label_for_TempImg, &myLabel::mouseLeft, this, &QtGuiDisplay::slot_mouseLeft);
	connect(ui.label_for_TempImg, &myLabel::mouseLeftMouveRoi, this, &QtGuiDisplay::slot_mouseLeft);
}

QtGuiDisplay::~QtGuiDisplay()
{
	if (outVideo.isOpened())
		outVideo.release();
}

void QtGuiDisplay::setSizeScrollBar()
{
	if (ui.label_for_TempImg->getScaledImgSize()->width() > ui.label_for_TempImg->width())
	{
		ui.horSB_forTempImg->show();
		ui.horSB_forTempImg->setRange(0, std::abs(ui.label_for_TempImg->getScaledImgSize()->width() - ui.label_for_TempImg->width()));
	}
	else
	{
		ui.horSB_forTempImg->hide();
	}
	if (ui.label_for_TempImg->getScaledImgSize()->height() > ui.label_for_TempImg->height())
	{
		ui.verSB_forTempImg->show();
		ui.verSB_forTempImg->setRange(0, std::abs(ui.label_for_TempImg->getScaledImgSize()->height() - ui.label_for_TempImg->height()));
	}
	else
	{
		ui.verSB_forTempImg->hide();
	}
}

void QtGuiDisplay::slot_mouvePixmap()
{
	if (ui.pb_allWindow->isEnabled())
		ui.label_for_TempImg->show_partImg(ui.horSB_forTempImg->value(), ui.verSB_forTempImg->value(), ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
}

void QtGuiDisplay::slot_mouseCurrentPos()
{
	if (activProcessedObj != nullptr)
	{
		if (event_img)
		{
			int dr_x, dr_y;
			ui.label_for_TempImg->moveImg(dr_x, dr_y);
			ui.horSB_forTempImg->setSliderPosition(dr_x);
			ui.verSB_forTempImg->setSliderPosition(dr_y);
		}
	}
}

void QtGuiDisplay::slot_mousePressed()
{
	event_img = true;
}

void QtGuiDisplay::slot_mouseLeft()
{
	myCursor.setShape(Qt::ArrowCursor);
	this->setCursor(myCursor);
}

void QtGuiDisplay::slot_mouseRelease()
{
	event_img = false;
}

void QtGuiDisplay::slot_ZoomImg_In()
{
	if (!isZoomNow)
	{
		isZoomNow = true;
		int dr_x, dr_y;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(1, dr_x, dr_y);
		if (activ_scaled == 500)
		{
			ui.pb_zoomIncress->setEnabled(false);
		}
		else
		{
			ui.pb_zoomIncress->setEnabled(true);
		}
		ui.label_Scale->setText(QString::number(activ_scaled) + "%");
		ui.pb_zoomDeduce->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->scaledPixmap();
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pb_allWindow->isEnabled())
			ui.pb_allWindow->setEnabled(true);
		ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_Out()
{
	if (!isZoomNow)
	{
		isZoomNow = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		int dr_x, dr_y;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(-1, dr_x, dr_y);
		if (activ_scaled == 25)
		{
			ui.pb_zoomDeduce->setEnabled(false);
		}
		else
		{
			ui.pb_zoomDeduce->setEnabled(true);
		}

		ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
		ui.pb_zoomIncress->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->scaledPixmap();
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pb_allWindow->isEnabled())
			ui.pb_allWindow->setEnabled(true);
		ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_AllLabl()
{	
	if (!isZoomNow)
	{
		isZoomNow = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		this->updateGeometry();
		int max, min;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(0, max, min);
		ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
		ui.label_for_TempImg->getMaxMinScal(max, min);
		if (max < 10)
			ui.pb_zoomIncress->setEnabled(true);
		else
			ui.pb_zoomIncress->setEnabled(false);
		if (min > 0)
			ui.pb_zoomDeduce->setEnabled(true);
		else
			ui.pb_zoomDeduce->setEnabled(false);
		ui.pb_allWindow->setEnabled(false);
		ui.label_for_TempImg->show_partImg(0, 0, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::slot_setSaveDirectory()
{
	directoryForSave = QFileDialog::getExistingDirectory(this);
}

void QtGuiDisplay::slot_startRecord()
{
	videoIsWrite = true;
	QString saveName(directoryForSave + "/record_");
	saveName += currentDataTimeToString();
	saveName += ".avi";
	
	outVideo = cv::VideoWriter::VideoWriter(cv::String(saveName.toStdString()), cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), 23, activProcessedObj->getMat().size(), true);
	ui.pb_stopRecord->setEnabled(true);
	ui.pb_startRecord->setDisabled(true);
}

void QtGuiDisplay::slot_stopRecord()
{
	outVideo.release();
	videoIsWrite = false;
	ui.pb_stopRecord->setDisabled(true);
	ui.pb_startRecord->setEnabled(true);
}

void QtGuiDisplay::slot_createSkrinshot()
{
	QString saveName(directoryForSave + "/skrin_");
	saveName += currentDataTimeToString();
	saveName += ".png";

	QPixmap out{ activProcessedObj->getPixmap() };
	out.save(saveName);
}

void QtGuiDisplay::setActivProcessObj(VmbFrame&activObj, bool master, int number)
{
	this->updateGeometry();
	activProcessedObj = &activObj;                  
 	ui.label_for_TempImg->setAlignment(Qt::AlignCenter);
	if (activProcessedObj->frameIsNull())
	{
		ui.label_for_TempImg->set_myPixmap(&QPixmap("NoImg.png"));
	}
	else if (master)
	{
		ui.label_for_TempImg->set_myPixmap(&activObj.getPixmap());
	}
	else
	{
		ui.label_for_TempImg->set_myPixmap(&activObj.getPixmap());
	}
	this->slot_ZoomImg_AllLabl();
	if (!activProcessedObj->frameIsNull())
	{
		ui.pb_zoomDeduce->show();
		ui.pb_zoomIncress->show();
		ui.pb_allWindow->show();
		ui.label_Scale->show();
		ui.pb_saveAs->show();
		ui.pb_createScreenshot->show();
	}
	else
	{
		ui.pb_zoomDeduce->hide();
		ui.pb_zoomIncress->hide();
		ui.pb_allWindow->hide();
		ui.label_Scale->hide();
		ui.pb_saveAs->hide();
		ui.pb_createScreenshot->hide();
	}
}

void QtGuiDisplay::setEnableWidtsGrouBox(bool enable)
{
	if (enable)
	{
		ui.pb_zoomDeduce->show();
		ui.pb_zoomIncress->show();
		ui.pb_allWindow->show();
		ui.label_Scale->show();
		ui.pb_saveAs->show();
		ui.pb_createScreenshot->show();
	}
	else
	{
		ui.pb_zoomDeduce->hide();
		ui.pb_zoomIncress->hide();
		ui.pb_allWindow->hide();
		ui.label_Scale->hide();
		ui.pb_saveAs->hide();
		ui.pb_createScreenshot->hide();
	}
}

QRect QtGuiDisplay::getLabelRect()
{
	QRect bufer;
	bufer.setX(this->x() + ui.label_for_TempImg->x());
	bufer.setY(this->y() + ui.label_for_TempImg->y());
	bufer.setWidth(ui.label_for_TempImg->rect().width());
	bufer.setHeight(ui.label_for_TempImg->rect().height());

	return bufer;
}

void QtGuiDisplay::changeImgFormat(int formatType)
{
	ui.label_for_TempImg->formatImage(formatType);
}

void QtGuiDisplay::slot_updateImg()
{
	ui.label_for_TempImg->show_partImg();
}

void QtGuiDisplay::updateFrame()
{
	emit signal_updateFrame();
}

void QtGuiDisplay::updateProcessObj(VmbFrame& activObj)
{
	activProcessedObj = &activObj; 
	ui.label_for_TempImg->update_myPixmap(activObj.getPixmap());

	this->setSizeScrollBar();
	ui.label_for_TempImg->show_partImg();
	activ_scaled = ui.label_for_TempImg->get_activScaled();
	ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
	if (outVideo.isOpened())
	{
		outVideo.write(activObj.getMat());
	}
}

void QtGuiDisplay::resizeEvent(QResizeEvent* event)
{
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	int dr_x{ui.label_for_TempImg->getOldSize().width() - ui.label_for_TempImg->size().width() };
	int dr_y{ ui.label_for_TempImg->getOldSize().height() - ui.label_for_TempImg->size().height() };
	ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->size().width() - 1, ui.label_for_TempImg->size().height() - 1);
	ui.horSB_forTempImg->setSliderPosition(ui.horSB_forTempImg->value() + dr_x);
	ui.verSB_forTempImg->setSliderPosition(ui.verSB_forTempImg->value() + dr_y);
	ui.label_for_TempImg->getDrPoint(dr_x, dr_y);
	ui.label_for_TempImg->setAllImgScaled();
	if (!ui.pb_allWindow->isEnabled())
		slot_ZoomImg_AllLabl();
	else
		this->setSizeScrollBar();
}

QString QtGuiDisplay::currentDataTimeToString()
{
	QDateTime currentDataTime{ QDateTime::currentDateTime() };
	QString out{};
	out += QString::number(currentDataTime.date().day());
	out += "_" + QString::number(currentDataTime.date().month());
	out += "_" + QString::number(currentDataTime.date().year());

	out += "__" + QString::number(currentDataTime.time().hour());
	out += "_" + QString::number(currentDataTime.time().minute());
	out += "_" + QString::number(currentDataTime.time().second());
	return out;
}
