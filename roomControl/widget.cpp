#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(256, 256);
    scene=new QGraphicsScene(this);
    scene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());//Устанавливаем размеры x,y - координаты верхнего левого угла
    scene->installEventFilter(this); //запускаем фильтр событий
    ui->graphicsView->setGeometry(0,0,this->width(),this->height());
    scene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    

    ui->graphicsView->setScene(scene);

    //myitem=new MySquare(scene->sceneRect());
    myitem=new MySquare();
    myitem->setPos(0,0);
    scene->addItem(myitem);

    connect(myitem,SIGNAL(CoordinateChange(QRectF& )),this,SLOT(st_CoordItemChange(QRectF& )));    //если двигаем итем
    connect(myitem,SIGNAL(sizeChange(QSizeF& )),this,SLOT(st_ItemFromWidgetSizeChange(QSizeF& ))); //если изменяем размеры итема
    connect(this,SIGNAL(signal_newSizeScene(QSizeF )), myitem,SLOT(slot_updateSceneSize(QSizeF ))); //если изменяем размеры итема
    connect(this,SIGNAL(sl_changeSizeFromButton(QSizeF )),myitem,SLOT(changeSizeFromButton(QSizeF))); //если изменяем размеры итема
    connect(this, SIGNAL(signal_new_offsetX(int)), myitem, SLOT(slot_changeOffX(int)));
    connect(this, SIGNAL(signal_new_offsetY(int)), myitem, SLOT(slot_changeOffY(int)));
    connect(this, SIGNAL(signal_new_heigth(int)), myitem, SLOT(slot_changeHeight(int)));
    connect(this, SIGNAL(signal_new_width(int)), myitem, SLOT(slot_changeWidth(int)));
    //connect(this,SIGNAL(ItemHeightChange(int ,int )), myitem,SLOT(st_ItemHeightChangeee(int ,int )));                                       //если изменяем размеры итема через спинбоксы
    //connect(this,SIGNAL(ItemWidthChange(int, int)), myitem,SLOT(st_ItemWidthChangeee(int ,int )));                                         //если изменяем размеры итема через спинбоксы
    //connect(this,SIGNAL(SpinBoxChangeOff_Y(int, int)), myitem,SLOT(st_SpinBoxChangeOffY(int ,int )));                                      //изменяем значение спинбоксами
    //connect(this,SIGNAL(SpinBoxChangeOff_X(int, int)), myitem,SLOT(st_SpinBoxChangeOffX(int ,int )));                                      //изменяем значение спинбоксами

}

Widget::~Widget()
{
    delete ui;
}

void Widget ::st_buttonChangeSizeClicked(double changeKoefficient)
{
    //qDebug() << this->width() * changeKoefficient;
    //QPointF size = QPointF(this->width() * changeKoefficient, this->height() * changeKoefficient);
    emit sl_changeSizeFromButton(QSizeF(this->width() * changeKoefficient, this->height() * changeKoefficient));
}

//void Widget::st_SpinBoxChangeOff_Y(int offsetY, int m_kH)
//{
//    SpinBoxChangeOff_Y(offsetY,m_kH);
//}
//
//void Widget::st_SpinBoxChangeOff_X(int offsetX, int m_kW)
//{
//    SpinBoxChangeOff_X(offsetX,m_kW);
//}
//
//
//void Widget::st_ItemWidthChange(int width, int m_kW)
//{
//    ItemWidthChange(width,m_kW);
//}
//
//void Widget::st_ItemHeightChange(int height, int m_kH)
//{
//    ItemHeightChange(height,m_kH);
//}

void Widget::st_CoordItemChange(QRectF& itemCoord)
{
    emit CoordItemChange(itemCoord);
}

void Widget::slot_setNewOffsetX(int newOffsetX)
{
    emit signal_new_offsetX(newOffsetX);
}

void Widget::slot_setNewOffsetY(int newOffsetY)
{
    emit signal_new_offsetY(newOffsetY);
}

void Widget::slot_setNewHeigth(int newHeigth)
{
    emit signal_new_heigth(newHeigth);
}

void Widget::slot_setNewWidth(int newWidth)
{
    emit signal_new_width(newWidth);
}

void Widget::st_ItemFromWidgetSizeChange(QSizeF& itemSize)
{
    emit ItemFromWidgetSizeChange(itemSize);
}


bool Widget::eventFilter(QObject *obj, QEvent *event)  //филтр событий для выцепливания нужного события связанного с нужным нам объектом
{
    if (obj ==  scene)//если событие пришло от сцены(нужный нам объект)
    {
        qreal adjust=5;

        //смотрим,является ли пришедшее событие движением курсора(нужное нам событие)
        if (event->type() == QEvent::GraphicsSceneMouseMove&& myitem->m_Pressed==false)
        {
            const QGraphicsSceneMouseEvent* const me = static_cast<const QGraphicsSceneMouseEvent*>(event);
            const QPointF position = me->scenePos();
            QRectF drawRect{ myitem->getDrawRect() };
            //будем проверять находится ли курсор над границей фигуры,если да,то меняем курсор
            myitem->setCursor(QCursor(Qt::ArrowCursor));
            //cursor on draw rect
            if (drawRect.contains(position))
            {
                myitem->setCursor(QCursor(Qt::ClosedHandCursor));
            }
            //cursor on left or right border 
            if (QRectF(drawRect.x(), drawRect.y() + adjust, adjust, drawRect.height() - 2 * adjust).contains(position) ||
                QRectF(drawRect.x()+drawRect.width()-adjust, drawRect.y() + adjust, adjust, drawRect.height() - 2 * adjust).contains(position))
            {
                myitem->setCursor(QCursor(Qt::SizeHorCursor));
            }
            //cursor on top or low border 
            else if (QRectF(drawRect.x() + adjust, drawRect.y(), drawRect.width() - 2 * adjust, adjust).contains(position) ||
                    QRectF(drawRect.x() + adjust, drawRect.y() + drawRect.height() - adjust, drawRect.width() - 2 * adjust, adjust).contains(position))
            {
                myitem->setCursor(QCursor(Qt::SizeVerCursor));
            }
            //cursor on top left or low right border
            else if (QRectF(drawRect.x(), drawRect.y(), adjust, adjust).contains(position) ||
                    QRectF(drawRect.x()+drawRect.width() - adjust, drawRect.y() + drawRect.height() - adjust, adjust, adjust).contains(position))
            {
              myitem->setCursor(QCursor(Qt::SizeFDiagCursor));
            }
            //cursor on top right or low left border
            else if (QRectF(drawRect.x(), drawRect.y()+drawRect.height()-adjust, adjust, adjust).contains(position) ||
                    QRectF(drawRect.x() + drawRect.width() - adjust, drawRect.y() , adjust, adjust).contains(position))
            {
              myitem->setCursor(QCursor(Qt::SizeBDiagCursor));
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}


void Widget::resizeEvent(QResizeEvent *event)
{
    ui->graphicsView->setGeometry(0,0,this->width(),this->height()); //Устанавливаем размеры x,y - координаты верхнего левого угла
    scene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());//Устанавливаем размеры x,y - координаты верхнего левого угла
    emit signal_newSizeScene(this->size());
}

void Widget::setSceneBackground(QPixmap& pixmap)
{
    QPixmap px = pixmap.scaled(256, 256, Qt::IgnoreAspectRatio);
    QBrush br(Qt::TexturePattern);
    br.setTexture(px);
    scene->setBackgroundBrush(br);
    //scene->addPixmap(QPixmap("D:/show.jpg"));
    ui->graphicsView->setScene(scene);
}

