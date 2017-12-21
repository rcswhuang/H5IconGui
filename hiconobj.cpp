#include "H5IconGui/hiconobj.h"
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
    //quint8 nFillPercentage = getFillPercentage(); //填充比例
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
    //quint8 nFillPercentage = getFillPercentage(); //填充比例
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
