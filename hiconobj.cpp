﻿#include "H5IconGui/hiconobj.h"
#include <QVariant>
#include <math.h>
#include "H5IconGui/hiconshowpattern.h"
#include "H5IconGui/hiconlineitem.h"
#include "H5IconGui/hiconrectitem.h"
#include "H5IconGui/hiconellipseitem.h"
#include "H5IconGui/hiconcircleitem.h"
#include "H5IconGui/hiconarcitem.h"
#include "H5IconGui/hiconpieitem.h"
#include "H5IconGui/hiconpolygonitem.h"
#include "H5IconGui/hiconpolylineitem.h"
#include "H5IconGui/hicontextitem.h"
#include "H5IconGui/hiconcomplexitem.h"
#include <QDebug>
HLineObj::HLineObj()
{
    arrowHeight = 0;
    arrowWidth = 0;
    arrowStart = 0;
    arrowEnd = 0;
}

HLineObj::~HLineObj()
{
  // pLineItem = NULL;
}

void HLineObj::readData(QDataStream *data)
{
    if(!data)return;
    HBaseObj::readData(data);
    qreal qr;
    *data>>qr;
    pfHeadPoint.setX(qr);
    *data>>qr;
    pfHeadPoint.setY(qr);
    *data>>qr;
    pfTailPoint.setX(qr);
    *data>>qr;
    pfTailPoint.setY(qr);
    quint8 n8;
    *data>>n8;
    arrowStart = n8;
    *data>>n8;
    arrowEnd = n8;
    quint16 n16;
    *data>>n16;
    arrowWidth = n16;
    *data>>n16;
    arrowHeight = n16;
}

void HLineObj::writeData(QDataStream *data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(qreal)pfHeadPoint.x();
    *data<<(qreal)pfHeadPoint.y();
    *data<<(qreal)pfTailPoint.x();
    *data<<(qreal)pfTailPoint.y();
    *data<<(quint8)arrowStart;
    *data<<(quint8)arrowEnd;
    *data<<(quint16)arrowWidth;
    *data<<(quint16)arrowHeight;
}

void HLineObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    pfHeadPoint.setX(dom->attribute("pfHeadPointx").toDouble());
    pfHeadPoint.setY(dom->attribute("pfHeadPointy").toDouble());
    pfTailPoint.setX(dom->attribute("pfTailPointx").toDouble());
    pfTailPoint.setY(dom->attribute("pfTailPointy").toDouble());
    arrowStart = dom->attribute("arrowStart").toUInt();
    arrowEnd = dom->attribute("arrowEnd").toUInt();
    arrowWidth = dom->attribute("arrowWidth").toDouble();
    arrowHeight = dom->attribute("arrowHeight").toDouble();

}

void HLineObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("pfHeadPointx",pfHeadPoint.x());
    dom->setAttribute("pfHeadPointy",pfHeadPoint.y());
    dom->setAttribute("pfTailPointx",pfTailPoint.x());
    dom->setAttribute("pfTailPointy",pfTailPoint.y());
    dom->setAttribute("arrowStart",arrowStart);
    dom->setAttribute("arrowEnd",arrowEnd);
    dom->setAttribute("arrowWidth",arrowWidth);
    dom->setAttribute("arrowHeight",arrowHeight);
}

QString HLineObj::TagName()
{
    return "Line";
}

//拷贝克隆
void HLineObj::copyTo(HBaseObj* obj)
{
    HLineObj* ob = (HLineObj*)obj;
    ob->setArrowStart(arrowStart);
    ob->setArrowEnd(arrowEnd);
    ob->setArrowWidth(arrowWidth);
    ob->setArrowHeight(arrowHeight);
    ob->pfHeadPoint = pfHeadPoint;
    ob->pfTailPoint = pfTailPoint;
}

void HLineObj::clone(HBaseObj* obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HLineObj::getShapeType()
{
    return enumLine;
}

void HLineObj::moveBy(qreal dx, qreal dy)
{
    pfHeadPoint.setX(pfHeadPoint.x() + dx);
    pfHeadPoint.setY(pfHeadPoint.y() + dy);
    pfTailPoint.setX(pfTailPoint.x() + dx);
    pfTailPoint.setY(pfTailPoint.y() + dy);
    bModify = true;
}

void HLineObj::setArrowStart(quint8 start)
{
    arrowStart = start;
}

quint8 HLineObj::getArrowStart()
{
    return arrowStart;
}

void HLineObj::setArrowEnd(quint8 end)
{
    arrowEnd = end;
}

quint8 HLineObj::getArrowEnd()
{
    return arrowEnd;
}

void HLineObj::setArrowWidth(quint16 width)
{
    arrowWidth = width;
}

quint16 HLineObj::getArrowWidth()
{
    return arrowWidth;
}

void HLineObj::setArrowHeight(quint16 height)
{
    arrowHeight = height;
}

quint16 HLineObj::getArrowHeight()
{
    return arrowHeight;
}

void HLineObj::resize(double w,double h)
{
    pfHeadPoint.setX(pfHeadPoint.x()*w);
    pfHeadPoint.setY(pfHeadPoint.y()*h);
    pfTailPoint.setX(pfTailPoint.x()*w);
    pfTailPoint.setY(pfTailPoint.y()*h);

    if(arrowWidth > 0 && arrowHeight > 0)
    {
        arrowWidth = arrowWidth*w;
        arrowHeight = arrowHeight*h;
    }
}

QRectF HLineObj::boundingRect() const
{
   return shape().controlPointRect();
}

bool HLineObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HLineObj::shape() const
{
    QPainterPath path;
    QPainterPathStroker ps;
    int w = arrowWidth;
    int h = arrowHeight;
    quint16 arrowLength = sqrt(w*w+h*h);
    int pen = (int)(arrowLength*sin(PI/3))*2+1;
    if(pen <= 20)
        pen = 20;
    ps.setWidth(pen);
    path.moveTo(pfHeadPoint);
    path.lineTo(pfTailPoint);
    return ps.createStroke(path);
}

void HLineObj::paint(QPainter* painter)
{
    HIconLineItem* pItem = qgraphicsitem_cast<HIconLineItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName());
    int penWidth = getLineWidth();
    Qt::PenStyle penStyle = getLineStyle();
    Qt::PenCapStyle capStyle = getLineCapStyle();

    painter->save();
    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    painter->setPen(pen);
    QPointF ptS = pfHeadPoint;
    QPointF ptE = pfTailPoint;
    QLineF line = QLineF(ptS,ptE);
    //画箭头
    if(getArrowWidth() > 0 && getArrowHeight() > 0)
    {
        double angle = ::acos(line.dx() / line.length());
        if(line.dy() >= 0)
            angle = (PI*2) - angle;
        int w = getArrowWidth();
        int h = getArrowHeight();
        quint16 arrowLength = sqrt(w*w+h*h);
        quint8 arrowS = getArrowStart();
        quint8 arrowE = getArrowEnd();
        QPointF arrowP1;
        QPointF arrowP2;
        if(arrowS == 1)
        {
            arrowP1 = ptS + QPointF(sin(angle+PI/3)*arrowLength,cos(angle+PI/3)*arrowLength);
            arrowP2 = ptS + QPointF(sin(angle+PI - PI/3)*arrowLength,cos(angle+PI-PI/3)*arrowLength);
            painter->drawLine(arrowP1,ptS);
            painter->drawLine(arrowP2,ptS);
        }
        else if(arrowS == 2)
        {
            arrowP1 = ptS + QPointF(sin(angle+PI/3)*arrowLength,cos(angle+PI/3)*arrowLength);
            arrowP2 = ptS + QPointF(sin(angle+PI - PI/3)*arrowLength,cos(angle+PI-PI/3)*arrowLength);
            QPolygonF arrowHead;
            arrowHead<<arrowP1<<ptS<<arrowP2;
            QPainterPath path;
            path.addPolygon(arrowHead);
            path.closeSubpath();
            painter->drawPath(path);

            double fh = sin(PI/3)*arrowLength/line.length();
            QPointF pt = line.pointAt(fh);
            ptS = pt;

        }
        else if(arrowS == 3)
        {
            arrowP1 = ptS + QPointF(sin(angle+PI/3)*arrowLength,cos(angle+PI/3)*arrowLength);
            arrowP2 = ptS + QPointF(sin(angle+PI - PI/3)*arrowLength,cos(angle+PI-PI/3)*arrowLength);
            QPolygonF arrowHead;
            arrowHead<<ptS<<arrowP1<<arrowP2;
            painter->save();
            painter->setBrush(QColor(penClr));
            painter->drawPolygon(arrowHead);
            painter->restore();
        }

        if(arrowE == 1)
        {
            arrowP1 = ptE + QPointF(sin(angle-PI/3)*arrowLength,cos(angle-PI/3)*arrowLength);
            arrowP2 = ptE + QPointF(sin(angle-PI + PI/3)*arrowLength,cos(angle-PI+PI/3)*arrowLength);
            painter->drawLine(arrowP1,ptE);
            painter->drawLine(arrowP2,ptE);
        }
        else if(arrowE == 2)
        {
            arrowP1 = ptE + QPointF(sin(angle-PI/3)*arrowLength,cos(angle-PI/3)*arrowLength);
            arrowP2 = ptE + QPointF(sin(angle-PI + PI/3)*arrowLength,cos(angle-PI+PI/3)*arrowLength);
            QPolygonF arrowHead;
            arrowHead<<arrowP1<<ptE<<arrowP2;
            QPainterPath path;
            path.addPolygon(arrowHead);
            path.closeSubpath();
            painter->drawPath(path);

            double fh = sin(PI/3)*arrowLength/line.length();
            QPointF pt = QLineF(ptE,ptS).pointAt(fh);
            ptE = pt;
        }
        else if(arrowE == 3)
        {
            arrowP1 = ptE + QPointF(sin(angle-PI/3)*arrowLength,cos(angle-PI/3)*arrowLength);
            arrowP2 = ptE + QPointF(sin(angle-PI + PI/3)*arrowLength,cos(angle-PI+PI/3)*arrowLength);
            QPolygonF arrowHead;
            arrowHead<<ptE<<arrowP1<<arrowP2;
            painter->save();
            painter->setBrush(QColor(penClr));
            painter->drawPolygon(arrowHead);
            painter->restore();
        }
    }

    painter->drawLine(QLineF(ptS,ptE));


    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(penClr,penWidth,penStyle);
        painter->setPen(pen1);
        QPointF p1 = line.p1();
        QPointF p2 = line.p2();
        pen1.setStyle(Qt::SolidLine);
        painter->setPen(pen1);
        QRectF rectF1;
        rectF1.setSize(QSizeF(14,14));
        rectF1.moveCenter(p1);
        QRectF rectF2;
        rectF2.setSize(QSize(14,14));
        rectF2.moveCenter(p2);
        painter->drawRect(rectF1);
        painter->drawRect(rectF2);
    }
    painter->restore();

}
/////////////////////////////////////HRectObj//////////////////////////////////////
HRectObj::HRectObj()
{
    rectHeight = 0;
    rectWidth = 0;
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

void HRectObj::paint(QPainter* painter)
{
    HIconRectItem* pItem = qgraphicsitem_cast<HIconRectItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    quint8 nFillPercentage = getFillPercentage(); //填充比例
    qreal fRotateAngle = getRotateAngle();
    bool bRound = getRound();
    int nXAxis = getXAxis();
    int nYAxis = getYAxis();

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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    if(!bRound)
    {
        painter->drawRect(rect);
    }
    else
    {
        painter->drawRoundRect(rect,nXAxis,nYAxis);
    }
    //需要判断nFillStyle 如果是linear的模式 就要考虑填充方向了
    //
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
    }
    QPainterPath path;
    if(!bRound)
    {
        path.addRect(rect);
    }
    else
    {
        path.addRoundedRect(rect,nXAxis,nYAxis);
    }
    painter->fillPath(path,brush);


    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        QPointF pt21,pt22,pt23,pt24;
        pt21 = rect.topLeft();
        pt22 = rect.topRight();
        pt23 = rect.bottomLeft();
        pt24 = rect.bottomRight();
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
    topLeft.setX(topLeft.x()*w);
    topLeft.setY(topLeft.y()*h);
    rectWidth = rectWidth * w;
    rectHeight = rectHeight *h;
}

///////////////////////////////////HEllipseObj//////////////////////////////////////////////////
HEllipseObj::HEllipseObj()
{
    rectHeight = 0;
    rectWidth = 0;
}

HEllipseObj::~HEllipseObj()
{

}

//二进制读写
void HEllipseObj::readData(QDataStream* data)
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

void HEllipseObj::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;
}

void HEllipseObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
}

void HEllipseObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
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

void HEllipseObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HEllipseObj::getShapeType()
{
    return enumEllipse;
}

void HEllipseObj::moveBy(qreal dx, qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    bModify = true;
}

QRectF HEllipseObj::boundingRect() const
{
    return shape().controlPointRect();
}

bool HEllipseObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HEllipseObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth+20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);
    return path;
}

void HEllipseObj::paint(QPainter* painter)
{
    HIconEllipseItem* pItem = qgraphicsitem_cast<HIconEllipseItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    //quint8 nFillPercentage = pEllipseObj->getFillPercentage(); //填充比例
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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(rect);
    painter->drawPath(path);


    QBrush brush;
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
        path.setFillRule(Qt::WindingFill);
        painter->setBrush(brush);
        painter->drawPath(path);

    }


    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(pItem->rect().topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(pItem->rect().topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(pItem->rect().bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(pItem->rect().bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }
    painter->restore();
}

void HEllipseObj::resize(double w,double h)
{
    topLeft.setX(topLeft.x()*w);
    topLeft.setY(topLeft.y()*h);
    rectWidth = rectWidth * w;
    rectHeight = rectHeight *h;
}

///////////////////////////////////HCircleObj//////////////////////////////////////////////////
HCircleObj::HCircleObj()
{
    rectHeight = 0;
    rectWidth = 0;
}

HCircleObj::~HCircleObj()
{

}

//二进制读写
void HCircleObj::readData(QDataStream* data)
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

void HCircleObj::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;
}

void HCircleObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
}

void HCircleObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
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

void HCircleObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HCircleObj::getShapeType()
{
    return enumCircle;
}

void HCircleObj::moveBy(qreal dx, qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    bModify = true;
}

QRectF HCircleObj::boundingRect() const
{
    return shape().controlPointRect();
}

bool HCircleObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HCircleObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth+20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);
    return path;
}

void HCircleObj::paint(QPainter* painter)
{
    HIconCircleItem* pItem = qgraphicsitem_cast<HIconCircleItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    //quint8 nFillPercentage = pEllipseObj->getFillPercentage(); //填充比例
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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(rect);
    painter->drawPath(path);


    QBrush brush;
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
        path.setFillRule(Qt::WindingFill);
        painter->setBrush(brush);
        painter->drawPath(path);

    }


    if(pItem && pItem->isSelected())
    {

        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(pItem->rect().topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(pItem->rect().topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(pItem->rect().bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(pItem->rect().bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);

    }
    painter->restore();
}

void HCircleObj::resize(double w,double h)
{
    topLeft.setX(topLeft.x()*w);
    topLeft.setY(topLeft.y()*h);
    rectWidth = rectWidth * w;
    rectHeight = rectHeight *h;
}


///////////////////////////////////////////////HPolygonObj//////////////////////////////////////
HPolygonObj::HPolygonObj()
{
    pylist.clear();
}

HPolygonObj::~HPolygonObj()
{

}

void HPolygonObj::readData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::readData(data);
    quint8 n = (quint8)pylist.count();
    *data>>n;
    qreal delta = 0.0;
    for(int i = 0; i < n;i++)
    {
        QPointF pt;
        *data>>delta;
        pt.setX(delta);
        *data>>delta;
        pt.setY(delta);
        pylist.append(pt);
    }
}

void HPolygonObj::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(quint8)pylist.count();
    qreal delta = 0.0;
    for(int i = 0; i < pylist.count();i++)
    {
        QPointF pt = pylist.at(i);
        delta = pt.x();
        *data<<delta;
        delta = pt.y();
        *data<<delta;
    }
}

void HPolygonObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    for(QDomNode n = dom->firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement pointDom = n.toElement();
        QPointF pt;
        pt.setX(pointDom.attribute("PosX").toDouble());
        pt.setY(pointDom.attribute("PosY").toDouble());
        pylist.append(pt);
    }
}

void HPolygonObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    for(int i = 0; i < pylist.count();i++)
    {
        QDomElement Pointdom = dom->ownerDocument().createElement("Point");
        dom->appendChild(Pointdom);
        QPointF pt = pylist.at(i);
        Pointdom.setAttribute("PosX",pt.x());
        Pointdom.setAttribute("PosY",pt.y());
    }
}

QString HPolygonObj::TagName()
{
    return "Polygon";
}

//拷贝克隆
void HPolygonObj::copyTo(HBaseObj* obj)
{
    HPolygonObj* ob = (HPolygonObj*)obj;
    ob->pylist.clear();
    for(int i = 0;i<pylist.count();i++)
    {
        ob->pylist.append(pylist.at(i));
    }
}

void HPolygonObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HPolygonObj::getShapeType()
{
    return enumPolygon;
}

void HPolygonObj::moveBy(qreal dx, qreal dy)
{
    for(int i = 0; i < pylist.count();i++)
    {
        QPointF pt = pylist[i];
        pt.setX(pt.x()+dx);
        pt.setY(pt.y()+dy);
        pylist[i] = pt;
    }
}

QRectF HPolygonObj::boundingRect() const
{
   return shape().controlPointRect();
}

bool HPolygonObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HPolygonObj::shape() const
{
    QPainterPath path;
    path.addPolygon(pylist);
    QPainterPathStroker ps;
    int pen = 10;
    ps.setWidth(pen);
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}


void HPolygonObj::paint(QPainter* painter)
{
    HIconPolygonItem* pItem = qgraphicsitem_cast<HIconPolygonItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    quint8 nFillPercentage = getFillPercentage(); //填充比例
    qreal fRotateAngle = getRotateAngle();

    if(pylist.count() == 0) return;
    QRectF rect = QPolygonF(pylist).boundingRect();
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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    QPolygonF polygon(pylist);
    QPainterPath path;
    if(polygon.isClosed())
    {
        path.setFillRule(Qt::WindingFill);
        path.addPolygon(polygon);
        painter->drawPath(path);
    }
    else
    {
        path.moveTo(polygon.at(0));
        for(int i = 1; i < polygon.size();i++)
            path.lineTo(polygon.at(i));
        painter->drawPath(path);
    }

    QBrush brush;
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
    }
    if(polygon.isClosed())
    {
        painter->setBrush(brush);
        path.setFillRule(Qt::WindingFill);
        painter->drawPath(path);
    }

    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        int nRect = pItem->polygon().size();
        QRectF *pRect = new QRectF[nRect];
        for(int i = 0 ; i < nRect; i++)
        {
            pRect[i].setSize(QSizeF(halfpw,halfpw));
            pRect[i].moveCenter(pItem->polygon().at(i));
            painter->drawRect(pRect[i]);
        }
        if(pRect)
        {
            delete[] pRect;
            pRect = NULL;
        }
    }
    painter->restore();
}

void HPolygonObj::resize(double w,double h)
{
    for(int i = 0; i < pylist.count();i++)
    {
        QPointF pt = pylist[i];
        pt.setX(pt.x()*w);
        pt.setY(pt.y()*h);
        pylist[i] = pt;
    }
}

///////////////////////////////////////////////HPolylineObj//////////////////////////////////////
HPolylineObj::HPolylineObj()
{
    pylist.clear();
}

HPolylineObj::~HPolylineObj()
{

}

void HPolylineObj::readData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::readData(data);
    quint8 n = pylist.count();
    *data>>n;
    qreal delta = 0.0;
    for(int i = 0; i < n;i++)
    {
        QPointF pt;
        *data>>delta;
        pt.setX(delta);
        *data>>delta;
        pt.setY(delta);
        pylist.append(pt);
    }
}

void HPolylineObj::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(quint8)pylist.count();
    qreal delta = 0.0;
    for(int i = 0; i < pylist.count();i++)
    {
        QPointF pt = pylist.at(i);
        delta = pt.x();
        *data<<delta;
        delta = pt.y();
        *data<<delta;
    }
}

void HPolylineObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    for(QDomNode n = dom->firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement pointDom = n.toElement();
        QPointF pt;
        pt.setX(pointDom.attribute("PosX").toDouble());
        pt.setY(pointDom.attribute("PosY").toDouble());
        pylist.append(pt);
    }
}

void HPolylineObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    for(int i = 0; i < pylist.count();i++)
    {
        QDomElement Pointdom = dom->ownerDocument().createElement("Point");
        dom->appendChild(Pointdom);
        QPointF pt = pylist.at(i);
        Pointdom.setAttribute("PosX",pt.x());
        Pointdom.setAttribute("PosY",pt.y());
    }
}

QString HPolylineObj::TagName()
{
    return "Polyline";
}

//拷贝克隆
void HPolylineObj::copyTo(HBaseObj* obj)
{
    HPolylineObj* ob = (HPolylineObj*)obj;
    ob->pylist.clear();
    for(int i = 0;i<pylist.count();i++)
    {
        ob->pylist.append(pylist.at(i));
    }
}

void HPolylineObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HPolylineObj::getShapeType()
{
    return enumPolyline;
}

void HPolylineObj::moveBy(qreal dx, qreal dy)
{
   for(int i = 0; i < pylist.count();i++)
   {
       QPointF pt = pylist[i];
       pt.setX(pt.x()+dx);
       pt.setY(pt.y()+dy);
       pylist[i] = pt;
   }
}

QRectF HPolylineObj::boundingRect() const
{
   return shape().controlPointRect();
}

bool HPolylineObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HPolylineObj::shape() const
{
    QPainterPath path;
    path.addPolygon(pylist);
    QPainterPathStroker ps;
    int pen = 10;
    ps.setWidth(pen);
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}


void HPolylineObj::paint(QPainter* painter)
{
    HIconPolylineItem* pItem = qgraphicsitem_cast<HIconPolylineItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    quint8 nFillPercentage = getFillPercentage(); //填充比例
    qreal fRotateAngle = getRotateAngle();

    if(pylist.count() == 0) return;
    QRectF rect = QPolygonF(pylist).boundingRect();
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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    QPolygonF polygon(pylist);
    QPainterPath path;
    path.moveTo(polygon.at(0));
    for(int i = 1; i < polygon.size();i++)
        path.lineTo(polygon.at(i));
    painter->drawPath(path);


    QBrush brush;
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
    }

    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        int nRect = pItem->polygon().size();
        QRectF *pRect = new QRectF[nRect];
        for(int i = 0 ; i < nRect; i++)
        {
            pRect[i].setSize(QSizeF(halfpw,halfpw));
            pRect[i].moveCenter(pItem->polygon().at(i));
            painter->drawRect(pRect[i]);
        }
        if(pRect)
        {
            delete[] pRect;
            pRect = NULL;
        }
    }
    painter->restore();
}

void HPolylineObj::resize(double w,double h)
{
    for(int i = 0; i < pylist.count();i++)
    {
        QPointF pt = pylist[i];
        pt.setX(pt.x()*w);
        pt.setY(pt.y()*h);
        pylist[i] = pt;
    }
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
    HBaseObj::writeData(data);
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;
    *data<<(int)startAngle;
    *data<<(int)spanAngle;
    *data<<(bool)bCloseCheck;
}

void HArcObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
    startAngle = dom->attribute("startAngle").toInt();
    spanAngle = dom->attribute("spanAngle").toInt();
    bCloseCheck = dom->attribute("bCloseCheck").toUInt();
}

void HArcObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
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

void HArcObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
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

void HArcObj::moveBy(qreal dx, qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    bModify = true;
}

QRectF HArcObj::boundingRect() const
{
   return shape().controlPointRect();
}

bool HArcObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HArcObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth+20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);
    return path;
}

void HArcObj::paint(QPainter* painter)
{
    HIconArcItem* pItem = qgraphicsitem_cast<HIconArcItem*>(getIconGraphicsItem());
    HIconArcItem* pItem1 = (HIconArcItem*)getIconGraphicsItem();
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    //quint8 nFillPercentage = pRectObj->getFillPercentage(); //填充比例
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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    int startAngle = getStartAngle()*16;
    int spanAngle = getSpanAngle()*16;
    bool bClose = getCloseStatus();

    QBrush brush = QBrush(Qt::NoBrush);
    if(bClose)
        painter->drawChord(rect,startAngle,spanAngle);
    else
        painter->drawArc(rect,startAngle,spanAngle);

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
        if(bClose)
            painter->drawChord(rect,startAngle,spanAngle);
        else
            painter->drawArc(rect,startAngle,spanAngle);

    }

    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(pItem->rect().topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(pItem->rect().topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(pItem->rect().bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(pItem->rect().bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }

    painter->restore();
}

void HArcObj::resize(double w,double h)
{
    topLeft.setX(topLeft.x()*w);
    topLeft.setY(topLeft.y()*h);
    rectWidth = rectWidth * w;
    rectHeight = rectHeight *h;
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
    int n;
    *data>>n;
    startAngle = n;
    *data>>n;
    spanAngle = n;
}

void HPieObj::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;
    *data<<(int)startAngle;
    *data<<(int)spanAngle;
}

void HPieObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
    startAngle = dom->attribute("startAngle").toInt();
    spanAngle = dom->attribute("spanAngle").toInt();
}

void HPieObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
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

void HPieObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
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

void HPieObj::moveBy(qreal dx, qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    bModify = true;
}

QRectF HPieObj::boundingRect() const
{
    return shape().controlPointRect();
}

bool HPieObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HPieObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth+20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);
    return path;
}

void HPieObj::paint(QPainter* painter)
{
    HIconPieItem* pItem = qgraphicsitem_cast<HIconPieItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    //quint8 nFillDir = pRectObj->getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    //quint8 nFillPercentage = pRectObj->getFillPercentage(); //填充比例
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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);

    int startAngle = getStartAngle()*16;
    int spanAngle = getSpanAngle()*16;

    QBrush brush = QBrush(Qt::NoBrush);
    if(nFillWay >= 1)
    {
        painter->setOpacity(1-(qreal)nTransparency/100.00);
        Qt::BrushStyle bs = (Qt::BrushStyle)nFillStyle;
        brush.setColor(fillClr);
        brush.setStyle(bs);
    }
    painter->setBrush(brush);

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
        rect1.moveCenter(pItem->rect().topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(pItem->rect().topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(pItem->rect().bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(pItem->rect().bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }
    painter->restore();
}

void HPieObj::resize(double w,double h)
{
    topLeft.setX(topLeft.x()*w);
    topLeft.setY(topLeft.y()*h);
    rectWidth = rectWidth * w;
    rectHeight = rectHeight *h;
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
    HBaseObj::writeData(data);
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;
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
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
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
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
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

void HTextObj::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
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

void HTextObj::moveBy(qreal dx, qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    bModify = true;
}

QRectF HTextObj::boundingRect() const
{
    return shape().controlPointRect();
}

bool HTextObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HTextObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth+20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);
    return path;
}

void HTextObj::paint(QPainter* painter)
{
    //if(!painter) return;
    HIconTextItem* pItem = qgraphicsitem_cast<HIconTextItem*>(getIconGraphicsItem());
    QColor penClr = QColor(getLineColorName()); //线条颜色
    int penWidth = getLineWidth();//线条宽度
    Qt::PenStyle penStyle = getLineStyle(); //线条形状
    Qt::PenCapStyle capStyle = getLineCapStyle(); //线条角度

    bool bFrameSee = getFrameSee();//边框可见
    quint8 nFillWay = getFillWay();//填充选择
    quint8 nFillStyle = getFillStyle(); //填充风格
    quint8 nTransparency = getTransparency(); //透明度
    quint8 nFillDir = getFillDirection();//填充方向
    QColor fillClr = QColor(getFillColorName());//填充颜色
    quint8 nFillPercentage = getFillPercentage(); //填充比例
    ushort nLayout = getLayout();
    QString strTextContent = getTextContent();
    qreal fRotateAngle = getRotateAngle();
    bool bRound = getRound();
    int nXAxis = getXAxis();
    int nYAxis = getYAxis();

    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QRectF mainRectF = rect;
    QRectF drawRectF = mainRectF;
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

    QPen pen = QPen(penClr);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    pen.setCapStyle(capStyle);
    if(bFrameSee)
        painter->setPen(pen);
    else
        painter->setPen(Qt::NoPen);
    if(!bRound)
    {
        painter->drawRect(mainRectF);
    }
    else
    {
        painter->drawRoundRect(mainRectF,nXAxis,nYAxis);
    }
    //painter->scale(2,2);
    QBrush brush;
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
                    ps2 = mainRectF.topLeft();
                    ps1 = mainRectF.bottomLeft();
                    break;
                }
                case DIRECT_TOP_TO_BOTTOM: //有顶到底
                {
                    ps1 = mainRectF.topLeft();
                    ps2 = mainRectF.bottomLeft();
                    break;
                }
                case DIRECT_LEFT_TO_RIGHT: //由左到右
                {
                    ps1 = mainRectF.topLeft();
                    ps2 = mainRectF.topRight();
                    break;
                }
                case DIRECT_RIGHT_TO_LEFT: //由右到左
                {
                    ps1 = mainRectF.topRight();
                    ps2 = mainRectF.topLeft();
                    break;
                }
                case DIRECT_VER_TO_OUT: //垂直到外
                {
                    ps1 = QPointF(mainRectF.center().x(),mainRectF.top());
                    ps2 = mainRectF.topLeft();
                    break;
                }
                case DIRECT_HORi_TO_OUT: //水平向外
                {
                    ps1 = QPointF(mainRectF.left(),mainRectF.center().y());
                    ps2 = mainRectF.topLeft();
                    break;
                }
                case DIRECT_VER_TO_IN: //垂直向里
                {
                    ps2 = QPointF(mainRectF.center().x(),mainRectF.top());
                    ps1 = mainRectF.topLeft();
                    break;
                }
                case DIRECT_HORI_TO_IN: //垂直向里
                {
                    ps2 = QPointF(mainRectF.left(),mainRectF.center().y());
                    ps1 = mainRectF.topLeft();
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
            QRadialGradient lgrd(mainRectF.center(),qMin(mainRectF.width(),mainRectF.height())/2);
            lgrd.setColorAt(0.0,fillClr);
            lgrd.setColorAt(0.5,fillClr.dark(150));
            lgrd.setColorAt(1.0,fillClr.dark(250));
            lgrd.setSpread(QGradient::ReflectSpread);
            QBrush brush2(lgrd);
            brush = brush2;
        }
        else if(nFillStyle == Qt::ConicalGradientPattern)
        {
            QConicalGradient lgrd(mainRectF.center(),270);
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
       // qreal top = mainRectF.top()*(float)(nFillPercentage/100.00);
        //drawRectF.setTop(top);
    }
    QPainterPath path;
    if(!bRound)
    {
        path.addRect(mainRectF);
    }
    else
    {
        path.addRoundedRect(mainRectF,nXAxis,nYAxis);
    }
    painter->fillPath(path,brush);

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
        rect1.moveCenter(pItem->rect().topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(pItem->rect().topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(pItem->rect().bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(pItem->rect().bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
        painter->restore();
    }
}

void HTextObj::resize(double w,double h)
{
    topLeft.setX(topLeft.x()*w);
    topLeft.setY(topLeft.y()*h);
    rectWidth = rectWidth * w;
    rectHeight = rectHeight *h;
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
    if(pIconTemplate)
        delete pIconTemplate;
}

//二进制读写
void HIconComplexObj::readData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::readData(data);
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
    qreal qr;
    *data>>qr;
    topLeft.setX(qr);
    *data>>qr;
    topLeft.setY(qr);
    *data>>qr;
    rectWidth = qr;
    *data>>qr;
    rectHeight = qr;

    //还有动态数据
    if(pDynamicObj)
        pDynamicObj->readData(data);
}

void HIconComplexObj::writeData(QDataStream* data)
{
    if(!data) return;
    HBaseObj::writeData(data);
    *data<<catalogName;
    *data<<catalogType;
    *data<<uuid;
    *data<<symbolName;
    *data<<symbolType;
    *data<<(qreal)topLeft.x();
    *data<<(qreal)topLeft.y();
    *data<<(double)rectWidth;
    *data<<(double)rectHeight;

    //动态数据
    if(pDynamicObj)
        pDynamicObj->writeData(data);
}

//xml文件读写
void HIconComplexObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::readXml(dom);
    catalogName = dom->attribute("CatalogName");
    catalogType = dom->attribute("CatalogType").toInt();
    uuid = dom->attribute("Uuid");
    symbolName = dom->attribute("SymbolName");
    symbolType = dom->attribute("SymbolType").toInt();
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toInt();
    rectHeight = dom->attribute("rectHeight").toInt();

    //动态数据
    QDomElement RelationDom = dom->namedItem("Relation").toElement();
    if(pDynamicObj)
        pDynamicObj->readXml(&RelationDom);

}

void HIconComplexObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("CatalogName",catalogName);
    dom->setAttribute("CatalogType",catalogType);
    dom->setAttribute("Uuid",uuid);
    dom->setAttribute("SymbolName",symbolName);
    dom->setAttribute("SymbolType",symbolType);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);

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

    if(pIconSymbol && ob->pIconSymbol)
    {
        pIconSymbol->copyTo(ob->pIconSymbol);
    }

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
    //HIconTextItem* pItem = qgraphicsitem_cast<HIconTextItem*>(getIconGraphicsItem());
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
    //painter->drawRect(boundingRect());

    /*
    if(pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        QPointF pt21,pt22,pt23,pt24;
        pt21 = pItem->rect().topLeft();
        pt22 = pItem->rect().topRight();
        pt23 = pItem->rect().bottomLeft();
        pt24 = pItem->rect().bottomRight();
        rect1.moveCenter(pItem->rect().topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(pItem->rect().topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(pItem->rect().bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(pItem->rect().bottomRight());

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }*/
    painter->restore();
}

void HIconComplexObj::resize(qreal w,qreal h)
{
    if(pIconSymbol)
        pIconSymbol->resize(w,h);
}

QRectF HIconComplexObj::boundingRect() const
{
    QRectF boundingRect;
    boundingRect.setX(topLeft.x());
    boundingRect.setY(topLeft.y());
    boundingRect.setWidth(rectWidth );
    boundingRect.setHeight(rectHeight);
    return boundingRect;
}

bool HIconComplexObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HIconComplexObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x());
    boundingRect.setY(topLeft.y());
    boundingRect.setWidth(rectWidth );
    boundingRect.setHeight(rectHeight);
    path.addRect(boundingRect);

    QRectF shapeRect = boundingRect.adjusted(10,10,-10,-10);
    path.addRect(shapeRect);
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

void HIconComplexObj::setTopLeft(const QPointF& pointF)
{
    topLeft = pointF;
}

QPointF HIconComplexObj::getTopLeft()
{
    return topLeft;
}

void HIconComplexObj::setRectWidth(double width)
{
    rectWidth = width;
}

double HIconComplexObj::getRectWidth()
{
    return rectWidth;
}

void HIconComplexObj::setRectHeight(double height)
{
    rectHeight = height;
}

double HIconComplexObj::getRectHeight()
{
    return rectHeight;
}

void HIconComplexObj::initIconTemplate()
{
    pIconTemplate->getSymbol()->copyTo(pIconSymbol);
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
