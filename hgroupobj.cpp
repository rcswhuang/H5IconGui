#include "H5IconGui/hgroupobj.h"
#include "H5IconGui/hiconsymbol.h"
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

}

void HGroupObj::resize(double w,double h)
{

}

void HGroupObj::paint(QPainter* painter)
{

}

QRectF HGroupObj::boundingRect() const
{

}

bool HGroupObj::contains(const QPointF &point) const
{

}

QPainterPath HGroupObj::shape() const
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
