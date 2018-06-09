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
    painter->save();
    QRectF rect = getObjRect();
    setPainter(painter,rect);//设置Painter
    QPainterPath path = getPath();
    //path.setFillRule(Qt::WindingFill);
    //path.addEllipse(rect);
    painter->drawPath(path);

    HIconCircleItem* pItem = qgraphicsitem_cast<HIconCircleItem*>(getIconGraphicsItem());
    if(pItem && pItem->isSelected())
    {
        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
    }
    painter->restore();
}

