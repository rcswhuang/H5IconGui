#include "hcircle.h"
#include "hiconcircleitem.h"

///////////////////////////////////HCircleObj//////////////////////////////////////////////////
HCircle::HCircle()
{

}

HCircle::~HCircle()
{

}

QString HCircle::TagName()
{
    return "Circle";
}

//拷贝克隆
void HCircle::copyTo(HBaseObj* obj)
{
    HCircle* ob = (HCircle*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
}

DRAWSHAPE HCircle::getShapeType()
{
    return enumCircle;
}

void HCircle::paint(QPainter* painter)
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
