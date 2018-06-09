#include "hpie.h"
#include "hiconpieitem.h"
///////////////////////////////////////////////HPie//////////////////////////////////////
HPie::HPie()
{
    startAngle = 30;
    spanAngle = 240;
}

HPie::~HPie()
{

}

//二进制读写
void HPie::readData(QDataStream* data)
{
    if(!data) return;
    HRectangle::readData(data);
    int n;
    *data>>n;
    startAngle = n;
    *data>>n;
    spanAngle = n;
}

void HPie::writeData(QDataStream* data)
{
    if(!data) return;
    HRectangle::writeData(data);
    *data<<(int)startAngle;
    *data<<(int)spanAngle;
}

void HPie::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectangle::readXml(dom);
    startAngle = dom->attribute("startAngle").toInt();
    spanAngle = dom->attribute("spanAngle").toInt();
}

void HPie::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectangle::writeXml(dom);
    dom->setAttribute("startAngle",startAngle);
    dom->setAttribute("spanAngle",spanAngle);
}

QString HPie::TagName()
{
    return "Pie";
}

//拷贝克隆
void HPie::copyTo(HBaseObj* obj)
{
    HPie* ob = (HPie*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->startAngle = startAngle;
    ob->spanAngle = spanAngle;
}

QRectF HPie::boundingRect()
{
    return shape().controlPointRect();
}

bool HPie::contains(const QPointF &point)
{
    return shape().contains(point);
}

QPainterPath HPie::shape()
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

DRAWSHAPE HPie::getShapeType()
{
    return enumPie;
}

void HPie::setStartAngle(int sAngle)
{
    startAngle = sAngle;
}

int HPie::getStartAngle()
{
    return startAngle;
}

void HPie::setSpanAngle(int spanangle)
{
    spanAngle = spanangle;
}

int HPie::getSpanAngle()
{
    return spanAngle;
}

void HPie::paint(QPainter* painter)
{
    painter->save();
    QRectF rect = getObjRect();
    setPainter(painter,rect);
    int startAngle = getStartAngle()*16;
    int spanAngle = getSpanAngle()*16;
    painter->drawPie(rect,startAngle,spanAngle);

    HIconPieItem* pItem = qgraphicsitem_cast<HIconPieItem*>(getIconGraphicsItem());
    if(pItem && pItem->isSelected())
    {
        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
    }
    painter->restore();
}
