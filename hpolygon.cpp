﻿#include "hpolygon.h"
#include "hiconpolygonitem.h"

///////////////////////////////////////////////HPolygon//////////////////////////////////////
HPolygon::HPolygon()
{
    pylist.clear();
    ptOld = ptNew = QPointF(0,0);
}

HPolygon::~HPolygon()
{

}

void HPolygon::readData(QDataStream* data)
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

void HPolygon::writeData(QDataStream* data)
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

void HPolygon::readXml(QDomElement* dom)
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

void HPolygon::writeXml(QDomElement* dom)
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

QString HPolygon::TagName()
{
    return "Polygon";
}

//拷贝克隆
void HPolygon::copyTo(HBaseObj* obj)
{
    HPolygon* ob = (HPolygon*)obj;
    ob->pylist.clear();
    for(int i = 0;i<pylist.count();i++)
    {
        ob->pylist.append(pylist.at(i));
    }
}

void HPolygon::clone(HBaseObj *obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HPolygon::getShapeType()
{
    return enumPolygon;
}

void HPolygon::moveBy(qreal dx, qreal dy)
{
    for(int i = 0; i < pylist.count();i++)
    {
        QPointF pt = pylist[i];
        pt.setX(pt.x()+dx);
        pt.setY(pt.y()+dy);
        pylist[i] = pt;
    }
}

QRectF HPolygon::boundingRect()
{
   return shape().controlPointRect();
}

bool HPolygon::contains(const QPointF &point)
{
    return shape().contains(point);
}

QPainterPath HPolygon::shape()
{
    QPainterPath path;
    path.addPolygon(getPolyLists());
    QPainterPathStroker ps;
    int pen = 10;
    ps.setWidth(pen);
    QPainterPath p = ps.createStroke(path);
    if(nFillWay > 0 && nFillStyle > 0)
        p.addPath(path);
    return p;
}


void HPolygon::paint(QPainter* painter)
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

    if(pylist.count() == 0) return;
    QRectF rect = QPolygonF(pylist).boundingRect();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

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
        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
    }

     painter->restore();

}

void HPolygon::resize(double w,double h)
{
    for(int i = 0; i < pylist.count();i++)
    {
        QPointF pt = pylist[i];
        pt.setX(ptNew.x() + (pt.x() - ptOld.x())*w);
        pt.setY(ptNew.y() + (pt.y() - ptOld.y())*h);
        pylist[i] = pt;
    }
}

void HPolygon::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
    ptNew = pt1;
    ptOld = pt2;
}

QPolygonF HPolygon::getPolyLists()
{
    QPolygonF pyList(pylist);
    //pyList<<rectF.topLeft()<<rectF.topRight()<<rectF.bottomRight()<<rectF.bottomLeft();
    Maps(pyList,0);
    return pyList;
}

void HPolygon::drawSelect(QPainter* painter)
{
    //painter->save();
    QPen pen1 = QPen(Qt::green);
    pen1.setWidth(1);
    painter->setPen(pen1);
    qreal halfpw = 6.00;
    int nRect = pylist.size();
    QRectF *pRect = new QRectF[nRect];
    for(int i = 0 ; i < nRect; i++)
    {
        pRect[i].setSize(QSizeF(halfpw,halfpw));
        pRect[i].moveCenter(pylist.at(i));
        painter->drawRect(pRect[i]);
    }
    if(pRect)
    {
        delete[] pRect;
        pRect = NULL;
    }
    //painter->restore();
}

void HPolygon::drawMulSelect(QPainter *painter,bool benchmark)
{
    //多选有两种情况1.不是标杆，2.是标杆
    //是标杆的话 就要加持绘制
    //painter->save();
    QPen pen1;
    pen1.setWidth(2);
    if(benchmark)
    {
        pen1.setColor(QColor("#FF00FF"));
    }
    else
    {
        pen1.setColor(QColor("#EE82EE"));//粉色
    }
    painter->setPen(pen1);
    QPolygonF polygonF(pylist);
    painter->drawPolygon(polygonF);
    //painter->restore();
}

