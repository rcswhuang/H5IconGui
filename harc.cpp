#include "harc.h"
#include "hiconarcitem.h"
///////////////////////////////////////////////HArcObj//////////////////////////////////////
HArc::HArc()
{
    startAngle = 30;
    spanAngle = 240;
    bCloseCheck = false;
}

HArc::~HArc()
{

}

//二进制读写
void HArc::readData(QDataStream* data)
{
    if(!data) return;
    HRectangle::readData(data);
    int n;
    *data>>n;
    startAngle = n;
    *data>>n;
    spanAngle = n;
    bool b;
    *data>>b;
    bCloseCheck = b;
}

void HArc::writeData(QDataStream* data)
{
    if(!data) return;
    HRectangle::writeData(data);
    *data<<(int)startAngle;
    *data<<(int)spanAngle;
    *data<<(bool)bCloseCheck;
}

void HArc::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectangle::readXml(dom);
    startAngle = dom->attribute("startAngle").toInt();
    spanAngle = dom->attribute("spanAngle").toInt();
    bCloseCheck = dom->attribute("bCloseCheck").toUInt();
}

void HArc::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectangle::writeXml(dom);
    dom->setAttribute("startAngle",startAngle);
    dom->setAttribute("spanAngle",spanAngle);
    dom->setAttribute("bCloseCheck",bCloseCheck);
}

QString HArc::TagName()
{
    return "Arc";
}

//拷贝克隆
void HArc::copyTo(HBaseObj* obj)
{
    HArc* ob = (HArc*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->startAngle = startAngle;
    ob->spanAngle = spanAngle;
    ob->bCloseCheck = bCloseCheck;
}

QRectF HArc::boundingRect()
{
    return shape().controlPointRect();
}

bool HArc::contains(const QPointF &point)
{
    return shape().contains(point);
}

QPainterPath HArc::shape()
{
    QPainterPath path;
    QPolygonF pyList = getRectLists();
    QRectF rect = pyList.boundingRect();
    if((nFillWay > 0 && nFillStyle > 0))
    {
        QRectF boundingRect = rect.adjusted(-5,-5,5,5);
        path.addRect(boundingRect);
        return path;
    }
    path.addPolygon(pyList);
    path.closeSubpath();
    QPainterPathStroker ps;
    ps.setWidth(10);
    return ps.createStroke(path);
}

DRAWSHAPE HArc::getShapeType()
{
    return enumArc;
}

void HArc::setStartAngle(int sAngle)
{
    startAngle = sAngle;
}

int HArc::getStartAngle()
{
    return startAngle;
}

void HArc::setSpanAngle(int spanangle)
{
    spanAngle = spanangle;
}

int HArc::getSpanAngle()
{
    return spanAngle;
}

void HArc::setCloseStatus(bool bcheck)
{
    bCloseCheck = bcheck;
}

bool HArc::getCloseStatus()
{
    return bCloseCheck;
}

void HArc::paint(QPainter* painter)
{
    painter->save();
    QRectF rect = getObjRect();
    setPainter(painter,rect);//设置Painter
    int startAngle = getStartAngle()*16;
    int spanAngle = getSpanAngle()*16;
    bool bClose = getCloseStatus();
    if(bClose)
        painter->drawChord(rect,startAngle,spanAngle);
    else
        painter->drawArc(rect,startAngle,spanAngle);

    HIconArcItem* pItem = qgraphicsitem_cast<HIconArcItem*>(getIconGraphicsItem());
    if(pItem && pItem->isSelected())
    {
        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
    }
    painter->restore();
}
