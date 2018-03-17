﻿#include "H5IconGui/hrectangle.h"
#include "H5IconGui/hiconrectitem.h"
/////////////////////////////////////HRectObj//////////////////////////////////////
HRectangle::HRectangle()
{
    rectHeight = 0;
    rectWidth = 0;
    ptOld = ptNew = QPointF(0,0);
}

HRectangle::~HRectangle()
{

}

//二进制读写
void HRectangle::readData(QDataStream* data)
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

void HRectangle::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;
}

void HRectangle::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
}

void HRectangle::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
}

QString HRectangle::TagName()
{
    return "Rectangle";
}

//拷贝克隆
void HRectangle::copyTo(HBaseObj* obj)
{
    HRectangle* ob = (HRectangle*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
}

void HRectangle::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HRectangle::getShapeType()
{
    return enumRectangle;
}

void HRectangle::moveBy(qreal dx, qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    bModify = true;
}

QRectF HRectangle::boundingRect() const
{
    return shape().controlPointRect();
}

bool HRectangle::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HRectangle::shape() const
{
    QPainterPath path;
    if(nFillWay > 0 && nFillStyle > 0)
    {
        QRectF boundingRect = QRectF(topLeft,QSizeF(rectWidth,rectHeight)).adjusted(-5,-5,5,5);
        path.addRect(boundingRect);
        return path;
    }

    QRectF pathRect = QRectF(topLeft,QSizeF(rectWidth,rectHeight));
    QPolygonF polygon;
    polygon<<pathRect.topLeft()<<pathRect.topRight()<<pathRect.bottomRight()<<pathRect.bottomLeft();
    path.addPolygon(polygon);
    path.closeSubpath();
    QPainterPathStroker ps;
    ps.setWidth(10);
    return ps.createStroke(path);
}

void HRectangle::setPainter(QPainter *painter,const QRectF& rect)
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

void HRectangle::paint(QPainter* painter)
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

void HRectangle::resize(double w,double h)
{
    topLeft.setX(ptNew.x() + (topLeft.x() - ptOld.x())*w);
    topLeft.setY(ptNew.y() + (topLeft.y() - ptOld.y())*h);
    rectWidth = rectWidth*w;
    rectHeight = rectHeight*h;
}

void HRectangle::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
   ptNew = pt1;
   ptOld = pt2;
}


void HRectangle::setObjRect(const QRectF& rect)
{
    topLeft = rect.topLeft();
    rectWidth = rect.width();
    rectHeight = rect.height();
}

QRectF HRectangle::getObjRect()
{
    return QRectF(topLeft,QSize(rectWidth,rectHeight));
}

void HRectangle::setTopLeft(const QPointF& point)
{
    topLeft = point;
}

QPointF HRectangle::getTopLeft()
{
    return topLeft;
}

void HRectangle::setRectWidth(double width)
{
    rectWidth = width;
}

double HRectangle::getRectWidth()
{
    return rectWidth;
}

void HRectangle::setRectHeight(double height)
{
    rectHeight = height;
}

double HRectangle::getRectHeight()
{
    return rectHeight;
}