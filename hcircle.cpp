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
    QPainterPath path = getPath();
    //path.setFillRule(Qt::WindingFill);
    //path.addEllipse(rect);
    painter->drawPath(path);
    painter->restore();
    if(pItem && pItem->isSelected())
    {
        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
    }
}

