#pragma once

#include <QWidget>
#include "ui_QtGuiDisplay.h"
#include <QTime>
#include "qfiledialog.h"
#include "myLabel.h"
#include "../VmbFrame.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>

class QtGuiDisplay : public QWidget
{
	Q_OBJECT

	QCursor myCursor{};
	VmbFrame* activProcessedObj{ nullptr };
	QString directoryForSave{""};
	cv::VideoWriter outVideo{};
	
	double activ_scaled{};
	double hor_scaled{};
	double vert_scaled{}; 

	bool isZoomNow{ false };
	bool event_img{ false };
	bool videoIsWrite{ false };



public:
	QtGuiDisplay(QWidget *parent = Q_NULLPTR);
	~QtGuiDisplay();
	void setSizeScrollBar();
	
	void setActivProcessObj(VmbFrame&activObj, bool master=true, int number=0);
	void setEnableWidtsGrouBox(bool enable);
	
	QRect getLabelRect();
	void changeImgFormat(int formatType);
	void updateFrame();
	
	void updateProcessObj(VmbFrame& activObj);
	void resizeEvent(QResizeEvent* event);

private:
	Ui::QtGuiDisplay ui;
	QString currentDataTimeToString();

protected slots:
	void slot_mouvePixmap();
	void slot_mouseCurrentPos();
	void slot_mousePressed();
	void slot_mouseLeft();
	void slot_mouseRelease();
	void slot_ZoomImg_In();
	void slot_ZoomImg_Out();
	void slot_ZoomImg_AllLabl();
	void slot_createSkrinshot();
	void slot_setSaveDirectory();
	void slot_startRecord();
	void slot_stopRecord();
	void slot_updateImg();

signals:
	void signal_updateFrame();
};
