#pragma once
#include "qlabel.h"
#include <qdebug.h>
#include <qevent.h>
#include <qpainter.h>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "../VmbFrame.h"

class myLabel :
	public QLabel
{
	QPixmap my_Pixmap;
	QPixmap my_PixmapOriginal;
	QPixmap *myPixmap_bufer;
	QPixmap *myPixmap_mouve;
	QCursor myCursor;
	QPoint drPoint;
	QPoint original_drPoint;
	QSize originalSize;
	QSize scaledSize;
	QSize oldSize;
	double scalCoef_W;
	double scalCoef_H;
	Qt::AspectRatioMode _aspectRotMod;
	//std::vector<QRect> rect_roi;
	bool add;
	bool up_or_left, down_or_right;
	bool imgIsShow;
	int imgScaled;
	double *_scaled;
	int activ_scaled;
	int min_activ_scaled, max_activ_scaled;
	int x_labl, y_labl;
	int x_lablG, y_lablG;
	int first_x_labl, first_y_labl;
	int f_x_pixMap, f_y_pixMap;
	int x_pixMap, y_pixMap;
	int imageFormat;//0-original, 1-gray, 2-RGB 

	
	Q_OBJECT
public:
	explicit myLabel(QWidget *parent = 0);
	QPixmap myPixmap() const;
	QPixmap getPixmapWithROI(std::vector<QRect> &Input) const;
	void mouseMoveEvent(QMouseEvent *evnt);
	void mousePressEvent(QMouseEvent *evnt);
	void mouseReleaseEvent(QMouseEvent *evnt);
	void leaveEvent(QEvent *evnt);
	void set_myPixmap(const QPixmap* pixmap);
	void show_roi(std::vector<QRect> &Input);
	
	void draw_picture(cv::Mat drawPicture, QRect limitRect);
	void show_roi(std::vector<QRect> &Input, std::vector<int> rect, int circle);
	void show_partImg(int dx, int dy, int width, int height);
	void show_partImg();
	void reset_x_y();
	void getDrPoint(int &inOutX, int &inOutY);
	void moveImg(int &dx, int &dy);
	double scaledPixmap(int scaled, int &dx, int &dy);
	void scaledPixmap();
	void getMaxMinScal(int &max, int &min);
	void toCenterPoint(QPoint &upLeftPoint);
	void toUpLeftpoint(QPoint &centerPoint);
	void toUpLeftpoint(int &x, int &y);
	void resizeEvent(QResizeEvent* event);
	QPoint* getImgPoint(void);
	QSize getOldSize();
	QSize* getScaledImgSize();
	QSize* getOriginalImgSize();
	void setAllImgScaled();
	void getPointInImg(int& x, int& y);
	void setFirstPixmapPoint(QPoint newPoint);
	void toImgCoordinate(int &inOutX, int &inOutY, bool isContains=true);
	void toImgCoordinate_(int &inOutX, int &inOutY, bool isContains=true);
	QPoint getImageCoordinate(bool isContains=true);
	QPoint getFirstImagePoint();
	void formatImage(int formatType = 0); //0-original, 1-gray, 2-RGB
	void update_myPixmap(const QPixmap& img);
	double get_activScaled();
	~myLabel();
signals:
	void mousePressed();
	void mousePos();
	void mouseLeft();
	void mouseLeftMouveRoi(int direct); ///0 - x<-, 1- x->, 2- y^, 3- y_ 
	void mouseRelease();
};

double round(double InputNumber, int const accuracy);