#include "hellipse.h"
#include "hiconellipseitem.h"
///////////////////////////////////HEllipseObj//////////////////////////////////////////////////
HEllipse::HEllipse()
{

}

HEllipse::~HEllipse()
{

}

QString HEllipse::TagName()
{
    return "Ellipse";
}

//拷贝克隆
void HEllipse::copyTo(HBaseObj* obj)
{
    HEllipse* ob = (HEllipse*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
}

DRAWSHAPE HEllipse::getShapeType()
{
    return enumEllipse;
}

QRectF HEllipse::boundingRect()
{
    return shape().boundingRect();
}

bool HEllipse::contains(const QPointF &point)
{
    return shape().contains(point);
}

QPainterPath HEllipse::shape()
{
    QPainterPath path;
    bool bImage = isValidImagePath();
    QPolygonF pyList =getRectLists();
    QRectF rect = pyList.boundingRect();
    if((nFillWay > 0 && nFillStyle > 0) || bImage)
    {
        QRectF boundingRect = rect.adjusted(-5,-5,5,5);
        path.addEllipse(boundingRect);
        return path;
    }
    path.addPolygon(pyList);
    path.closeSubpath();
    QPainterPathStroker ps;
    ps.setWidth(10);
    return ps.createStroke(path);
}

void HEllipse::paint(QPainter* painter)
{
    painter->save();
    QRectF rect = getObjRect();
    setPainter(painter,rect);//设置Painter
    QPainterPath path = getPath();
    painter->drawPath(path);

    HIconEllipseItem* pItem = qgraphicsitem_cast<HIconEllipseItem*>(getIconGraphicsItem());
    if(pItem && pItem->isSelected())
    {
        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
    }
    painter->restore();
}

QPainterPath HEllipse::getPath()//只提供矩形，圆，椭圆，文字三种支持图片
{
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QPainterPath path;
    path.addEllipse(rect);
    return path;
}
