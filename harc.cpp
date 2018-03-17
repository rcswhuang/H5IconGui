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

QRectF HArc::boundingRect() const
{
    return shape().controlPointRect();
}

bool HArc::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HArc::shape() const
{
    QPainterPath path;
    if(nFillWay > 0 && nFillStyle > 0)
    {
        QRectF boundingRect = QRectF(topLeft,QSizeF(rectWidth,rectHeight)).adjusted(-5,-5,5,5);
        path.addRect(boundingRect);
        return path;
    }

    QRectF pathRect = QRectF(topLeft,QSizeF(rectWidth,rectHeight));
    QPainterPath p;
    p.arcTo(pathRect,startAngle,spanAngle);
    if(bCloseCheck)
        p.closeSubpath();
    path.addPath(p);
    path.addRect(pathRect);
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
