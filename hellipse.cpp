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

QRectF HEllipse::boundingRect() const
{
    return shape().boundingRect();
}

bool HEllipse::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HEllipse::shape() const
{
    QPainterPath path;
    QRectF rect = QRectF(topLeft,QSizeF(rectWidth,rectHeight));
    bool bImage = isValidImagePath();
    if((nFillWay > 0 && nFillStyle > 0) || bImage)
    {
        QRectF boundingRect = rect.adjusted(-5,-5,5,5);
        path.addRect(boundingRect);
        return path;
    }
    path.addRect(rect);
    path.addEllipse(rect);
    QPainterPathStroker ps;
    ps.setWidth(10);
    return ps.createStroke(path);
}

void HEllipse::paint(QPainter* painter)
{
    HIconEllipseItem* pItem = qgraphicsitem_cast<HIconEllipseItem*>(getIconGraphicsItem());

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
    QPainterPath path = getPath();
    painter->drawPath(path);
    painter->restore();
    if(pItem && pItem->isSelected())
    {

        painter->save();
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
        painter->restore();
    }
}

QPainterPath HEllipse::getPath()//只提供矩形，圆，椭圆，文字三种支持图片
{
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QPainterPath path;
    path.addEllipse(rect);
    return path;
}
