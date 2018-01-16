﻿#include "H5IconGui/hgroupobj.h"
#include "H5IconGui/hiconsymbol.h"
#include "H5IconGui/hiconitemgroup.h"
HGroupObj::HGroupObj(HIconSymbol* symbol)
    :pIconSymbol(symbol)
{

}

HGroupObj::~HGroupObj()
{

}

//二进制读写
void HGroupObj::readData(QDataStream* data)
{
    HBaseObj::readData(data);
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->readData(data);
    }
}

void HGroupObj::writeData(QDataStream* data)
{
    HBaseObj::writeData(data);
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->writeData(data);
    }
}

//xml文件读写 <Group ......>
void HGroupObj::readXml(QDomElement* dom)
{
    if(!dom)
        return;
    HBaseObj::readXml(dom);
    topLeft.setX(dom->attribute("topLeftx").toDouble());
    topLeft.setY(dom->attribute("topLefty").toDouble());
    rectWidth = dom->attribute("rectWidth").toDouble();
    rectHeight = dom->attribute("rectHeight").toDouble();
    //读出Group
    QDomNode n = dom->firstChild();
    for(int i = 0;!n.isNull();n=n.nextSibling(),i++)
    {
        QDomElement e = n.toElement();
        QString strTagName = e.tagName();
        HBaseObj* pObj = pIconSymbol->newObj(strTagName);
        if(!pObj) continue;
        pObjList.append(pObj);
        pObj->readXml(&e);
    }
}

//<Group ......>
void HGroupObj::writeXml(QDomElement* dom)
{
    if(!dom) return;
    HBaseObj::writeXml(dom);
    dom->setAttribute("topLeftx",topLeft.x());
    dom->setAttribute("topLefty",topLeft.y());
    dom->setAttribute("rectWidth",rectWidth);
    dom->setAttribute("rectHeight",rectHeight);
    //写入group里面的数据
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        if(pObj->isDeleted())
        {
            pObjList.removeAt(i);
            delete pObj;
            i--;
            continue;
        }
        QDomElement childEle = dom->ownerDocument().createElement(pObj->TagName());
        dom->appendChild(childEle);
        pObj->writeXml(&childEle);
    }
}

QString HGroupObj::TagName()
{
    return "Group";
}

//拷贝克隆
void HGroupObj::copyTo(HBaseObj* obj)
{
    HGroupObj* ob = (HGroupObj*)obj;
    ob->setTopLeft(topLeft);
    ob->setGroupWidth(rectWidth);
    ob->setGroupHeight(rectHeight);
}

void HGroupObj::clone(HBaseObj* obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HGroupObj::getShapeType()
{
    return enumGroup;
}

void HGroupObj::moveBy(qreal dx,qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->moveBy(dx,dy);
    }
    bModify = true;
}

//需要设置4个点的位置
void HGroupObj::resetRectPoint(qreal dx, qreal dy)
{
    //topLeft.setX(topLeft.x()+dx);
    //topLeft.setY(topLeft.y()+dy);
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->resetRectPoint(dx,dy);
    }
}

void HGroupObj::resize(double w,double h)
{
    rectWidth = rectWidth*w;
    rectHeight = rectHeight*h;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->resize(w,h);
    }
/*
    topLeft.setX(topLeft.x()*w);
    topLeft.setY(topLeft.y()*h);*/

}

QRectF HGroupObj::boundingRect() const
{
    return shape().controlPointRect();
}

bool HGroupObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HGroupObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth+20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);
    return path;
}

void HGroupObj::setObjRect(const QRectF& rect)
{
    topLeft = rect.topLeft();
    rectWidth = rect.width();
    rectHeight = rect.height();
}

QRectF HGroupObj::getObjRect()
{
    return QRectF(topLeft,QSize(rectWidth,rectHeight));
}

void HGroupObj::setSubObjRect(qreal dx,qreal dy)
{

}

void HGroupObj::setTopLeft(const QPointF &pointF)
{
    topLeft = pointF;
}

QPointF HGroupObj::getTopLeft()
{
    return topLeft;
}

void HGroupObj::setGroupWidth(double width)
{
    rectWidth = width;
}

double HGroupObj::getGroupWidth()
{
    return rectWidth;
}

void HGroupObj::setGroupHeight(double height)
{
    rectHeight = height;
}

double HGroupObj::getGroupHeight()
{
    return rectHeight;
}

void HGroupObj::addObj(HBaseObj* pObj)
{
    if(!pObj)
        return;
    pObjList.append(pObj);
}

void HGroupObj::delObj(HBaseObj* pObj)
{
    if(!pObj)
        return;
    pObjList.removeOne(pObj);
    delete pObj;
    pObj = NULL;
}

void HGroupObj::paint(QPainter* painter)
{
    HIconItemGroup* pItem = qgraphicsitem_cast<HIconItemGroup*>(getIconGraphicsItem());
    painter->save(); 
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)(pObjList[i]);
        if(pObj)
        {
           pObj->paint(painter);
        }
    }
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    painter->drawRect(rect);

    //QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    if(pItem && pItem->isSelected())
    {
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
        qreal halfpw = 14.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        QPointF pt21,pt22,pt23,pt24;
        pt21 = rect.topLeft();
        pt22 = rect.topRight();
        pt23 = rect.bottomLeft();
        pt24 = rect.bottomRight();
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
