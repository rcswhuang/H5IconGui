#include "H5IconGui/hiconrectobj.h"
#include "H5IconGui/hiconarcitem.h"
#include "H5IconGui/hiconcircleitem.h"
#include "H5IconGui/hiconellipseitem.h"
#include "H5IconGui/hiconpieitem.h"
#include "H5IconGui/hiconrectitem.h"
#include "H5IconGui/hicontextitem.h"
#include "H5IconGui/hiconcomplexitem.h"
#include "H5IconGui/hiconshowpattern.h"
/////////////////////////////////////HRectObj//////////////////////////////////////
HRectObj::HRectObj()
{
    rectHeight = 0;
    rectWidth = 0;
    ptOld = ptNew = QPointF(0,0);
}

HRectObj::~HRectObj()
{

}

//二进制读写
void HRectObj::readData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::readData(data);
    qreal qr;
    *data>>qr;
    topLeft.setX(qr);
    *data>>qr;
    topLeft.setY(qr);
    *data>>qr;
    rectWidth = qr;
    *data>>qr;
    rectHeight = qr;
}

void HRectObj::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;
}

void HRectObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
}

void HRectObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
}

QString HRectObj::TagName()
{
    return "Rectangle";
}

//拷贝克隆
void HRectObj::copyTo(HBaseObj* obj)
{
    HRectObj* ob = (HRectObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
}

void HRectObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HRectObj::getShapeType()
{
    return enumRectangle;
}

void HRectObj::moveBy(qreal dx, qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    bModify = true;
}

QRectF HRectObj::boundingRect() const
{
    return shape().controlPointRect();
}

bool HRectObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HRectObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth+20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);

    /*QPainterPath path;
    QRectF rectPath;
    rectPath.setX(rect().x() - 10);
    rectPath.setY(rect().y() - 10);
    rectPath.setWidth(rect().width() + 20);
    rectPath.setHeight(rect().height() + 20);
    path.addRect(rectPath);*/
    return path;
}

void HRectObj::setPainter(QPainter *painter,const QRectF& rect)
{
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度
    bool bFrameSee = getFrameSee();//边框可见
    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    //quint8 nFillPercentage = getFillPercentage(); //填充比例
    QBrush brush;//(Qt::NoBrush);
    if(nFillWay >= 1)
    {
        painter->setOpacity(1-(qreal)nTransparency/100.00);
        if(nFillStyle == Qt::LinearGradientPattern)
        {
            QPointF ps1,ps2;
            switch(nFillDir)
            {
                case DIRECT_BOTTOM_TO_TOP:
                {
                    ps2 = rect.topLeft();
                    ps1 = rect.bottomLeft();
                    break;
                }
                case DIRECT_TOP_TO_BOTTOM: //有顶到底
                {
                    ps1 = rect.topLeft();
                    ps2 = rect.bottomLeft();
                    break;
                }
                case DIRECT_LEFT_TO_RIGHT: //由左到右
                {
                    ps1 = rect.topLeft();
                    ps2 = rect.topRight();
                    break;
                }
                case DIRECT_RIGHT_TO_LEFT: //由右到左
                {
                    ps1 = rect.topRight();
                    ps2 = rect.topLeft();
                    break;
                }
                case DIRECT_VER_TO_OUT: //垂直到外
                {
                    ps1 = QPointF(rect.center().x(),rect.top());
                    ps2 = rect.topLeft();
                    break;
                }
                case DIRECT_HORi_TO_OUT: //水平向外
                {
                    ps1 = QPointF(rect.left(),rect.center().y());
                    ps2 = rect.topLeft();
                    break;
                }
                case DIRECT_VER_TO_IN: //垂直向里
                {
                    ps2 = QPointF(rect.center().x(),rect.top());
                    ps1 = rect.topLeft();
                    break;
                }
                case DIRECT_HORI_TO_IN: //垂直向里
                {
                    ps2 = QPointF(rect.left(),rect.center().y());
                    ps1 = rect.topLeft();
                    break;
                }
            }
            QLinearGradient lgrd(ps1,ps2);
            lgrd.setColorAt(0.0,fillClr);
            lgrd.setColorAt(0.5,fillClr.lighter(150));
            lgrd.setColorAt(1.0,fillClr.lighter(250));
            lgrd.setSpread(QGradient::ReflectSpread);
            QBrush brush2(lgrd);
            brush = brush2;
        }
        else if(nFillStyle == Qt::RadialGradientPattern)
        {
            QRadialGradient lgrd(rect.center(),qMin(rect.width(),rect.height())/2);
            lgrd.setColorAt(0.0,fillClr);
            lgrd.setColorAt(0.5,fillClr.dark(150));
            lgrd.setColorAt(1.0,fillClr.dark(250));
            lgrd.setSpread(QGradient::ReflectSpread);
            QBrush brush2(lgrd);
            brush = brush2;
        }
        else if(nFillStyle == Qt::ConicalGradientPattern)
        {
            QConicalGradient lgrd(rect.center(),270);
            lgrd.setColorAt(0.0,fillClr);
            lgrd.setColorAt(0.5,fillClr.lighter(150));
            lgrd.setColorAt(1.0,fillClr.lighter(250));
            lgrd.setSpread(QGradient::ReflectSpread);
            QBrush brush2(lgrd);
            brush = brush2;
        }
        else
        {
            Qt::BrushStyle bs = (Qt::BrushStyle)nFillStyle;
            QBrush brush1(fillClr,bs);
            brush = brush1;
        }
        painter->setBrush(brush);
    }
}

void HRectObj::paint(QPainter* painter)
{
    HIconRectItem* pItem = qgraphicsitem_cast<HIconRectItem*>(getIconGraphicsItem());
    qreal fRotateAngle = getRotateAngle();
    bool bRound = getRound();
    int nXAxis = getXAxis();
    int nYAxis = getYAxis();

    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    //QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        //pItem->setTransformOriginPoint(centerPoint);
        //QTransform transform;
        //transform.rotate(fRotateAngle);
        //pItem->setTransform(transform);
    }
    else
    {
        //painter->rotate(fRotateAngle);
    }


    setPainter(painter,rect);//设置Painter
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    if(!bRound)
    {
        path.addRect(rect);
    }
    else
    {
        path.addRoundedRect(rect,nXAxis,nYAxis);
    }
    painter->drawPath(path);


    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }
    painter->restore();
}

void HRectObj::resize(double w,double h)
{
    topLeft.setX(ptNew.x() + (topLeft.x() - ptOld.x())*w);
    topLeft.setY(ptNew.y() + (topLeft.y() - ptOld.y())*h);
    rectWidth = rectWidth*w;
    rectHeight = rectHeight*h;
}

void HRectObj::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
   ptNew = pt1;
   ptOld = pt2;
}


void HRectObj::setObjRect(const QRectF& rect)
{
    topLeft = rect.topLeft();
    rectWidth = rect.width();
    rectHeight = rect.height();
}

QRectF HRectObj::getObjRect()
{
    return QRectF(topLeft,QSize(rectWidth,rectHeight));
}

void HRectObj::setTopLeft(const QPointF& point)
{
    topLeft = point;
}

QPointF HRectObj::getTopLeft()
{
    return topLeft;
}

void HRectObj::setRectWidth(double width)
{
    rectWidth = width;
}

double HRectObj::getRectWidth()
{
    return rectWidth;
}

void HRectObj::setRectHeight(double height)
{
    rectHeight = height;
}

double HRectObj::getRectHeight()
{
    return rectHeight;
}

///////////////////////////////////HEllipseObj//////////////////////////////////////////////////
HEllipseObj::HEllipseObj()
{

}

HEllipseObj::~HEllipseObj()
{

}

QString HEllipseObj::TagName()
{
    return "Ellipse";
}

//拷贝克隆
void HEllipseObj::copyTo(HBaseObj* obj)
{
    HEllipseObj* ob = (HEllipseObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
}

DRAWSHAPE HEllipseObj::getShapeType()
{
    return enumEllipse;
}

void HEllipseObj::paint(QPainter* painter)
{
    HIconEllipseItem* pItem = qgraphicsitem_cast<HIconEllipseItem*>(getIconGraphicsItem());

    qreal fRotateAngle = getRotateAngle();
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        pItem->setTransformOriginPoint(centerPoint);
        QTransform transform;
        transform.rotate(fRotateAngle);
        pItem->setTransform(transform);
    }
    else
    {
        painter->rotate(fRotateAngle);
    }

    setPainter(painter,rect);//设置Painter

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(rect);
    painter->drawPath(path);

    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }
    painter->restore();
}

///////////////////////////////////HCircleObj//////////////////////////////////////////////////
HCircleObj::HCircleObj()
{

}

HCircleObj::~HCircleObj()
{

}

QString HCircleObj::TagName()
{
    return "Circle";
}

//拷贝克隆
void HCircleObj::copyTo(HBaseObj* obj)
{
    HCircleObj* ob = (HCircleObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
}

DRAWSHAPE HCircleObj::getShapeType()
{
    return enumCircle;
}

void HCircleObj::paint(QPainter* painter)
{
    HIconCircleItem* pItem = qgraphicsitem_cast<HIconCircleItem*>(getIconGraphicsItem());
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        pItem->setTransformOriginPoint(centerPoint);
        QTransform transform;
        transform.rotate(fRotateAngle);
        pItem->setTransform(transform);
    }
    else
    {
        painter->rotate(fRotateAngle);
    }

    setPainter(painter,rect);//设置Painter
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(rect);
    painter->drawPath(path);

    if(pItem && pItem->isSelected())
    {

        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);

    }
    painter->restore();
}

///////////////////////////////////////////////HArcObj//////////////////////////////////////
HArcObj::HArcObj()
{
    startAngle = 30;
    spanAngle = 240;
    bCloseCheck = false;
}

HArcObj::~HArcObj()
{

}

//二进制读写
void HArcObj::readData(QDataStream* data)
{
    if(!data) return;
    HRectObj::readData(data);
    int n;
    *data>>n;
    startAngle = n;
    *data>>n;
    spanAngle = n;
    bool b;
    *data>>b;
    bCloseCheck = b;
}

void HArcObj::writeData(QDataStream* data)
{
    if(!data) return;
    HRectObj::writeData(data);
    *data<<(int)startAngle;
    *data<<(int)spanAngle;
    *data<<(bool)bCloseCheck;
}

void HArcObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectObj::readXml(dom);
    startAngle = dom->attribute("startAngle").toInt();
    spanAngle = dom->attribute("spanAngle").toInt();
    bCloseCheck = dom->attribute("bCloseCheck").toUInt();
}

void HArcObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectObj::writeXml(dom);
    dom->setAttribute("startAngle",startAngle);
    dom->setAttribute("spanAngle",spanAngle);
    dom->setAttribute("bCloseCheck",bCloseCheck);
}

QString HArcObj::TagName()
{
    return "Arc";
}

//拷贝克隆
void HArcObj::copyTo(HBaseObj* obj)
{
    HArcObj* ob = (HArcObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->startAngle = startAngle;
    ob->spanAngle = spanAngle;
    ob->bCloseCheck = bCloseCheck;
}

DRAWSHAPE HArcObj::getShapeType()
{
    return enumArc;
}

void HArcObj::setStartAngle(int sAngle)
{
    startAngle = sAngle;
}

int HArcObj::getStartAngle()
{
    return startAngle;
}

void HArcObj::setSpanAngle(int spanangle)
{
    spanAngle = spanangle;
}

int HArcObj::getSpanAngle()
{
    return spanAngle;
}

void HArcObj::setCloseStatus(bool bcheck)
{
    bCloseCheck = bcheck;
}

bool HArcObj::getCloseStatus()
{
    return bCloseCheck;
}

void HArcObj::paint(QPainter* painter)
{
    HIconArcItem* pItem = qgraphicsitem_cast<HIconArcItem*>(getIconGraphicsItem());

    qreal fRotateAngle = getRotateAngle();
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        pItem->setTransformOriginPoint(centerPoint);
        QTransform transform;
        transform.rotate(fRotateAngle);
        pItem->setTransform(transform);
    }
    else
    {
        painter->rotate(fRotateAngle);
    }

    setPainter(painter,rect);//设置Painter
    int startAngle = getStartAngle()*16;
    int spanAngle = getSpanAngle()*16;
    bool bClose = getCloseStatus();
    if(bClose)
        painter->drawChord(rect,startAngle,spanAngle);
    else
        painter->drawArc(rect,startAngle,spanAngle);

    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }

    painter->restore();
}

///////////////////////////////////////////////HPieObj//////////////////////////////////////
HPieObj::HPieObj()
{
    startAngle = 30;
    spanAngle = 240;
}

HPieObj::~HPieObj()
{

}

//二进制读写
void HPieObj::readData(QDataStream* data)
{
    if(!data) return;
    HRectObj::readData(data);
    int n;
    *data>>n;
    startAngle = n;
    *data>>n;
    spanAngle = n;
}

void HPieObj::writeData(QDataStream* data)
{
    if(!data) return;
    HRectObj::writeData(data);
    *data<<(int)startAngle;
    *data<<(int)spanAngle;
}

void HPieObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectObj::readXml(dom);
    startAngle = dom->attribute("startAngle").toInt();
    spanAngle = dom->attribute("spanAngle").toInt();
}

void HPieObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectObj::writeXml(dom);
    dom->setAttribute("startAngle",startAngle);
    dom->setAttribute("spanAngle",spanAngle);
}

QString HPieObj::TagName()
{
    return "Pie";
}

//拷贝克隆
void HPieObj::copyTo(HBaseObj* obj)
{
    HPieObj* ob = (HPieObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->startAngle = startAngle;
    ob->spanAngle = spanAngle;
}

DRAWSHAPE HPieObj::getShapeType()
{
    return enumPie;
}

void HPieObj::setStartAngle(int sAngle)
{
    startAngle = sAngle;
}

int HPieObj::getStartAngle()
{
    return startAngle;
}

void HPieObj::setSpanAngle(int spanangle)
{
    spanAngle = spanangle;
}

int HPieObj::getSpanAngle()
{
    return spanAngle;
}

void HPieObj::paint(QPainter* painter)
{
    HIconPieItem* pItem = qgraphicsitem_cast<HIconPieItem*>(getIconGraphicsItem());

    qreal fRotateAngle = getRotateAngle();
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        pItem->setTransformOriginPoint(centerPoint);
        QTransform transform;
        transform.rotate(fRotateAngle);
        pItem->setTransform(transform);
    }
    else
    {
        painter->rotate(fRotateAngle);
    }

    setPainter(painter,rect);//设置Painter

    int startAngle = getStartAngle()*16;
    int spanAngle = getSpanAngle()*16;
    painter->drawPie(rect,startAngle,spanAngle);

    if(pItem && pItem->isSelected())
    {
        painter->save();
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }
    painter->restore();
}

///////////////////////////////////////////////////////////////////////
HTextObj::HTextObj()
{
    strTextContent = "icon text";

    textColorName = "#00FF00";//线条颜色
    textFontName = QStringLiteral("微软雅黑");
    layout = LAYOUT_TEXT_NULL;
    horizontalAlign = Qt::AlignHCenter;
    verticalAlign = Qt::AlignVCenter;
    pointSize = 10;//字体大小
    weight = QFont::Normal;//粗体
    italic = false;//斜体
    bFrameSee = false;//???
}

HTextObj::~HTextObj()
{

}

//二进制读写
void HTextObj::readData(QDataStream* data)
{
    if(!data) return;
    HRectObj::readData(data);
    QString s;
    *data>>s;
    strTextContent = s;
    *data>>s;
    textColorName = s;
    *data>>s;
    textFontName = s;
    quint8 n8;
    *data>>n8;
    layout = n8;
    int n;
    *data>>n;
    horizontalAlign = n;
    *data>>n;
    verticalAlign = n;
    *data>>n;
    pointSize = n;
    *data>>n;
    weight = n;
    *data>>n;
    italic = n;
}

void HTextObj::writeData(QDataStream* data)
{
    if(!data) return;
    HRectObj::writeData(data);
    *data<<(QString)strTextContent;
    *data<<(QString)textColorName;
    *data<<(QString)textFontName;
    *data<<(quint8)layout;
    *data<<(int)horizontalAlign;
    *data<<(int)verticalAlign;
    *data<<(int)pointSize;
    *data<<(int)weight;
    *data<<(int)italic;
}

//xml文件读写
void HTextObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectObj::readXml(dom);
    strTextContent = dom->attribute("textContent");
    textColorName = dom->attribute("textColorName");
    textFontName = dom->attribute("textFontName");
    layout = dom->attribute("layout").toInt();
    horizontalAlign = dom->attribute("horizontalAlign").toInt();
    verticalAlign = dom->attribute("verticalAlign").toInt();
    pointSize = dom->attribute("pointSize").toInt();
    weight = dom->attribute("weight").toInt();
    italic = dom->attribute("italic").toInt();

}

void HTextObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectObj::writeXml(dom);
    dom->setAttribute("textContent",strTextContent);
    dom->setAttribute("textColorName",textColorName);
    dom->setAttribute("textFontName",textFontName);
    dom->setAttribute("layout",layout);
    dom->setAttribute("horizontalAlign",horizontalAlign);
    dom->setAttribute("verticalAlign",verticalAlign);
    dom->setAttribute("pointSize",pointSize);
    dom->setAttribute("weight",weight);
    dom->setAttribute("italic",italic);
}

QString HTextObj::TagName()
{
    return "Text";
}

//拷贝克隆
void HTextObj::copyTo(HBaseObj* obj)
{
    HTextObj* ob = (HTextObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->strTextContent = strTextContent;
    ob->textColorName = textColorName;
    ob->textFontName = textFontName;
    ob->layout = layout;
    ob->horizontalAlign = horizontalAlign;
    ob->verticalAlign = verticalAlign;
    ob->pointSize = pointSize;
    ob->weight = weight;
    ob->italic = italic;
}

DRAWSHAPE HTextObj::getShapeType()
{
    return enumText;
}

void HTextObj::setTextColorName(QString strClrName)
{
    textColorName = strClrName;
}

QString HTextObj::getTextColorName()
{
    return textColorName;
}

void HTextObj::setLayout(ushort layout)
{
    this->layout = layout;
}

ushort HTextObj::getLayout()
{
    return layout;
}

void HTextObj::setHorizontalAlign(int hAlign)
{
    horizontalAlign = hAlign;
}

int HTextObj::getHorizontalAlign()
{
    return horizontalAlign;
}

void HTextObj::setVerticalAlign(int vAlign)
{
    verticalAlign = vAlign;
}

int HTextObj::getVerticalAlign()
{
    return verticalAlign;
}

void HTextObj::setTextFontName(QString strFontName)
{
    textFontName = strFontName;
}

QString HTextObj::getTextFontName()
{
    return textFontName;
}

void HTextObj::setPointSize(int ptSize)
{
    pointSize = ptSize;
}

int HTextObj::getPointSize()
{
    return pointSize;
}

void HTextObj::setWeight(int weight)
{
    this->weight = weight;
}

int HTextObj::getWeigth()
{
    return weight;
}

void HTextObj::setItalic(bool bitalic)
{
    italic = bitalic;
}

bool HTextObj::getItalic()
{
    return italic;
}

void HTextObj::setTextContent(QString strContent)
{
    strTextContent = strContent;
}

QString HTextObj::getTextContent()
{
    return strTextContent;
}


void HTextObj::setTopLeftPoint(QPointF topLeft)
{
    this->topLeft = topLeft;
}

QPointF HTextObj::getTopLeftPoint()
{
    return topLeft;
}

void HTextObj::setRectWidth(double width)
{
    rectWidth = width;
}

double HTextObj::getRectWidth()
{
    return rectWidth;
}

void HTextObj::setRectHeight(double height)
{
    rectHeight = height;
}

double HTextObj::getRectHeight()
{
    return rectHeight;
}

void HTextObj::paint(QPainter* painter)
{
    //if(!painter) return;
    HIconTextItem* pItem = qgraphicsitem_cast<HIconTextItem*>(getIconGraphicsItem());

    ushort nLayout = getLayout();
    QString strTextContent = getTextContent();
    qreal fRotateAngle = getRotateAngle();
    bool bRound = getRound();
    int nXAxis = getXAxis();
    int nYAxis = getYAxis();

    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QRectF mainRectF = rect;
    QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        pItem->setTransformOriginPoint(centerPoint);
        QTransform transform;
        transform.rotate(fRotateAngle);
        pItem->setTransform(transform);
    }
    else
    {
        painter->rotate(fRotateAngle);
    }

    setPainter(painter,rect);
    QPainterPath path;
    if(!bRound)
    {
        path.addRect(mainRectF);
    }
    else
    {
        path.addRoundedRect(mainRectF,nXAxis,nYAxis);
    }
    painter->drawPath(path);

    //设置字体部分
    QString strFontName = getTextFontName();
    int pointSize = getPointSize();
    int weight = getWeigth();
    bool italic = getItalic();
    QFont font(strFontName,pointSize,weight,italic);

    QPen textPen = QPen(QColor(getTextColorName()));
    painter->setPen(textPen);
    painter->setFont(font);

    int nAlign = getHorizontalAlign()|getVerticalAlign();
    QFontMetricsF fm(font);
    if(nLayout == LAYOUT_TEXT_FULL)
    {
        double fw = fm.width(strTextContent);
        double fh = fm.height();
        fh = mainRectF.height()/fh;
        fw = (mainRectF.width()+fh)/fw;
        painter->scale(fw,fh);
        mainRectF = QRectF(mainRectF.x()/fw,mainRectF.y()/fh,mainRectF.width()/fw,mainRectF.height()/fh);
        nAlign = Qt::AlignCenter | Qt::TextSingleLine;
    }
    painter->drawText(mainRectF,nAlign,strTextContent);
    painter->restore();


    if(pItem && pItem->isSelected())
    {
        painter->save();
        QPen pen1 = QPen(Qt::red);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
        painter->restore();
    }
}


/*************************************图符类**********************************/
/*
 * 图符类只用于drawgrph和在线系统上面，所以保存的信息和基本图元是不一致的。
*/
HIconComplexObj::HIconComplexObj()
{
    //去掉本身所有的边框颜色设置，这些都是不需要的
    pDynamicObj = new HDynamicObj;
    bFrameSee = false;
}

HIconComplexObj::HIconComplexObj(HIconTemplate* it)
    :pIconTemplate(it)
{
    pDynamicObj = new HDynamicObj;
    pIconSymbol = new HIconSymbol(it);
    initIconTemplate();

    bFrameSee = false;
}

HIconComplexObj::~HIconComplexObj()
{
    if(pDynamicObj)
        delete pDynamicObj;
    if(pIconSymbol)
        delete pIconSymbol;
    //if(pIconTemplate)
    //    delete pIconTemplate;
}

void HIconComplexObj::initIconTemplate()
{
    catalogName = pIconTemplate->getCatalogName();
    catalogType = pIconTemplate->getCatalogType();
    uuid = pIconTemplate->getUuid().toString();
    pIconTemplate->getSymbol()->copyTo(pIconSymbol);
}

//二进制读写
void HIconComplexObj::readData(QDataStream* data)
{
    if(!data) return;
    HRectObj::readData(data);
    QString s;
    *data>>s;
    catalogName = s;
    int n;
    *data>>n;
    catalogType = n;
    *data>>s;
    uuid = s;
    *data>>s;
    symbolName = s;
    *data>>n;
    symbolType = n;

    //还有动态数据
    if(pDynamicObj)
        pDynamicObj->readData(data);
}

void HIconComplexObj::writeData(QDataStream* data)
{
    if(!data) return;
    HRectObj::writeData(data);
    *data<<catalogName;
    *data<<catalogType;
    *data<<uuid;
    *data<<symbolName;
    *data<<symbolType;
    //动态数据
    if(pDynamicObj)
        pDynamicObj->writeData(data);
}

//xml文件读写
void HIconComplexObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectObj::readXml(dom);
    catalogName = dom->attribute("CatalogName");
    catalogType = dom->attribute("CatalogType").toInt();
    uuid = dom->attribute("Uuid");
    symbolName = dom->attribute("SymbolName");
    symbolType = dom->attribute("SymbolType").toInt();
    //topLeft.setX(dom->attribute("topLeftx").toDouble());
    //topLeft.setY(dom->attribute("topLefty").toDouble());
    //rectWidth = dom->attribute("rectWidth").toInt();
    //rectHeight = dom->attribute("rectHeight").toInt();

    //这里读取symbol信息不能用symbol对象来读取
    //动态数据
    QDomElement RelationDom = dom->namedItem("Relation").toElement();
    if(pDynamicObj)
        pDynamicObj->readXml(&RelationDom);

}

void HIconComplexObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectObj::writeXml(dom);
    dom->setAttribute("CatalogName",catalogName);
    dom->setAttribute("CatalogType",catalogType);
    dom->setAttribute("Uuid",uuid);
    dom->setAttribute("SymbolName",symbolName);
    dom->setAttribute("SymbolType",symbolType);
    //dom->setAttribute("topLeftx",topLeft.x());
    //dom->setAttribute("topLefty",topLeft.y());
    //dom->setAttribute("rectWidth",rectWidth);
    //dom->setAttribute("rectHeight",rectHeight);

    //动态数据
    QDomElement RelationDom = dom->ownerDocument().createElement("Relation");
    dom->appendChild(RelationDom);
    if(pDynamicObj)
        pDynamicObj->writeXml(&RelationDom);
}


QString HIconComplexObj::TagName()
{
    return "ComplexObj";
}

//拷贝克隆
void HIconComplexObj::copyTo(HBaseObj* obj)
{
    HIconComplexObj* ob = (HIconComplexObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->catalogName = catalogName;
    ob->catalogType = catalogType;
    ob->symbolName = symbolName;
    ob->symbolType = symbolType;

    //模板数据
    if(pIconSymbol && ob->pIconSymbol)
    {
        pIconSymbol->copyTo(ob->pIconSymbol);
    }

    //动态数据
    if(pDynamicObj && ob->pDynamicObj)
    {
        pDynamicObj->copyTo(ob->pDynamicObj);
    }
}

void HIconComplexObj::clone(HBaseObj* obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HIconComplexObj::getShapeType()
{
    return enumComplex;
}

void HIconComplexObj::moveBy(qreal dx,qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    if(!getIconSymbol() && !getIconSymbol()->getCurrentPatternPtr())
        return;
    HIconShowPattern* pattern = getIconSymbol()->getCurrentPatternPtr();
    for(int i = 0; i < pattern->pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)(pattern->pObjList[i]);
        if(pObj)
        {
           pObj->moveBy(dx,dy);
        }
    }
    bModify = true;
}

void HIconComplexObj::paint(QPainter* painter)
{
    HIconComplexItem* pItem = qgraphicsitem_cast<HIconComplexItem*>(getIconGraphicsItem());
    painter->save();
    if(!getIconSymbol() && !getIconSymbol()->findPatternById(0))
        return;
    HIconShowPattern* pattern = getIconSymbol()->getCurrentPatternPtr();
    for(int i = 0; i < pattern->pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)(pattern->pObjList[i]);
        if(pObj)
        {
           pObj->paint(painter);
        }
    }
    QPen pen(Qt::red);
    painter->setPen(pen);
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    //painter->drawRect(rect);

    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 8.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }
    painter->restore();
}

void HIconComplexObj::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
    if(pIconSymbol)
        pIconSymbol->resetRectPoint(pt1,pt2);
}

void HIconComplexObj::resize(qreal w,qreal h)
{
    if(pIconSymbol)
        pIconSymbol->resize(w,h);
}

QRectF HIconComplexObj::boundingRect() const
{
   return shape().controlPointRect();
}

bool HIconComplexObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HIconComplexObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth +20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);


   // QRectF shapeRect = boundingRect.adjusted(10,10,-10,-10);
  //  path.addRect(shapeRect);
    return path;
}

void HIconComplexObj::setUuid(const QString& uuid)
{
    this->uuid = uuid;
}

QString HIconComplexObj::getUuid()
{
    return uuid;
}

void HIconComplexObj::setIconTemplate(HIconTemplate* t)
{
    pIconTemplate = t;
    if(!pIconSymbol)
    {
        pIconSymbol = new HIconSymbol(t);
        initIconTemplate();
    }
}

HIconTemplate* HIconComplexObj::iconTemplate()
{
    return pIconTemplate;
}

HIconSymbol* HIconComplexObj::getIconSymbol()
{
    return pIconSymbol;
}

void HIconComplexObj::initDynamicData()
{
    clearDynamicData();
}

void HIconComplexObj::clearDynamicData()
{
    if(pDynamicObj)
    {
        delete pDynamicObj;
        pDynamicObj = NULL;
    }
}
