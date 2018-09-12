#include "hline.h"
#include "hiconlineitem.h"

HLine::HLine()
{
    arrowHeight = 0;
    arrowWidth = 0;
    arrowStart = 0;
    arrowEnd = 0;
    ptOld = ptNew = QPointF(0,0);
}

HLine::~HLine()
{
  // pLineItem = NULL;
}

void HLine::readData(QDataStream *data)
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

void HLine::writeData(QDataStream *data)
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

void HLine::readXml(QDomElement* dom)
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

void HLine::writeXml(QDomElement* dom)
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

QString HLine::TagName()
{
    return "Line";
}

//拷贝克隆
void HLine::copyTo(HBaseObj* obj)
{
    HLine* ob = (HLine*)obj;
    ob->setArrowStart(arrowStart);
    ob->setArrowEnd(arrowEnd);
    ob->setArrowWidth(arrowWidth);
    ob->setArrowHeight(arrowHeight);
    ob->pfHeadPoint = pfHeadPoint;
    ob->pfTailPoint = pfTailPoint;
}

void HLine::clone(HBaseObj* obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HLine::getShapeType()
{
    return enumLine;
}

void HLine::moveBy(qreal dx, qreal dy)
{
    pfHeadPoint.setX(pfHeadPoint.x() + dx);
    pfHeadPoint.setY(pfHeadPoint.y() + dy);
    pfTailPoint.setX(pfTailPoint.x() + dx);
    pfTailPoint.setY(pfTailPoint.y() + dy);
    bModify = true;
}

void HLine::setArrowStart(quint8 start)
{
    arrowStart = start;
}

quint8 HLine::getArrowStart()
{
    return arrowStart;
}

void HLine::setArrowEnd(quint8 end)
{
    arrowEnd = end;
}

quint8 HLine::getArrowEnd()
{
    return arrowEnd;
}

void HLine::setArrowWidth(quint16 width)
{
    arrowWidth = width;
}

quint16 HLine::getArrowWidth()
{
    return arrowWidth;
}

void HLine::setArrowHeight(quint16 height)
{
    arrowHeight = height;
}

quint16 HLine::getArrowHeight()
{
    return arrowHeight;
}

void HLine::resize(double w,double h)
{
    pfHeadPoint.setX(ptNew.x() + (pfHeadPoint.x() - ptOld.x())*w);
    pfHeadPoint.setY(ptNew.y() + (pfHeadPoint.y() - ptOld.y())*h);
    pfTailPoint.setX(ptNew.x() + (pfTailPoint.x() - ptOld.x())*w);
    pfTailPoint.setY(ptNew.y() + (pfTailPoint.y() - ptOld.y())*h);

    if(arrowWidth > 0 && arrowHeight > 0)
    {
        arrowWidth = arrowWidth*w;
        arrowHeight = arrowHeight*h;
    }
}

QRectF HLine::boundingRect()
{
   return shape().controlPointRect();
}

bool HLine::contains(const QPointF &point)
{
    return shape().contains(point);
}

QPainterPath HLine::shape()
{
    QPainterPath path;
    path.addPolygon(getPointLists());
    QPainterPathStroker ps;
    int w = arrowWidth;
    int h = arrowHeight;
    quint16 arrowLength = sqrt(w*w+h*h);
    int pen = (int)(arrowLength*sin(PI/3))*2+1;
    if(pen <= 20)
        pen = 20;
    ps.setWidth(pen);
    return ps.createStroke(path);
}

void HLine::paint(QPainter* painter)
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
        double angle = ::acos(line.dx()/line.length());
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
        if(pItem && pItem->isSelected())
        {
            if(pItem->bMulSelect)
                drawMulSelect(painter,pItem->bBenchmark);
            else
                drawSelect(painter);
        }
    }
    painter->restore();
}

void HLine::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
    ptNew = pt1;
    ptOld = pt2;
}

QLineF HLine::getObjLine()
{
    return QLineF(pfHeadPoint,pfTailPoint);
}

QPolygonF HLine::getPointLists()
{
    QPolygonF pyList;
    pyList<<pfHeadPoint<<pfTailPoint;
    //line 本身就可以自由旋转所以不需要
    Maps(pyList,0);
    return pyList;
}

void HLine::drawSelect(QPainter* painter)
{
    //painter->save();
    QPen pen1 = QPen(Qt::green);
    pen1.setWidth(1);
    painter->setPen(pen1);

    qreal halfpw = 6.00;
    QPointF p1 = pfHeadPoint;
    QPointF p2 = pfTailPoint;
    pen1.setStyle(Qt::SolidLine);
    painter->setPen(pen1);
    QRectF rectF1;
    rectF1.setSize(QSizeF(halfpw,halfpw));
    rectF1.moveCenter(p1);
    QRectF rectF2;
    rectF2.setSize(QSize(halfpw,halfpw));
    rectF2.moveCenter(p2);
    painter->drawRect(rectF1);
    painter->drawRect(rectF2);
    //painter->restore();
}


void HLine::drawMulSelect(QPainter *painter,bool benchmark)
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
    QLineF lineF(pfHeadPoint,pfTailPoint);
    painter->drawLine(lineF);
    //painter->restore();
}
