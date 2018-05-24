#include "hgroupobj.h"
#include "hiconsymbol.h"
#include "hiconitemgroup.h"
#include "hgraph.h"
HGroupObj::HGroupObj()
{
    m_pGraph = NULL;
    m_strUuid = "";
    clear();
}

HGroupObj::~HGroupObj()
{
    clear();
}

void HGroupObj::clear()
{
    while(!pObjList.isEmpty())
        delete pObjList.takeFirst();
    pObjList.clear();
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
        if(strTagName == "WfPointObj")
            m_strUuid = e.attribute("Uuid");
        HBaseObj* pObj = newObj(strTagName);
        if(!pObj) continue;
        pObj->readXml(&e);
        pObjList.append(pObj);
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
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        if(pObj->getShapeType() == enumLine)
        {
            HLine* lineObj = new HLine;
            pObj->copyTo(lineObj);
            ob->pObjList.append(lineObj);
        }
        else if(pObj->getShapeType() == enumRectangle)
        {
            HRectangle* pRectObj = new HRectangle;
            pObj->clone(pRectObj);
            ob->pObjList.append(pRectObj);
        }
        else if(pObj->getShapeType() == enumEllipse)
        {
            HEllipse* pEllipseObj = new HEllipse;
            pObj->clone(pEllipseObj);
            ob->pObjList.append(pEllipseObj);
        }
        else if(pObj->getShapeType() == enumCircle)
        {
            HCircle* pCircleObj = new HCircle;
            pObj->clone(pCircleObj);
            ob->pObjList.append(pCircleObj);
        }
        else if(pObj->getShapeType() == enumPolyline)
        {
            HPolyline* pPolylineObj = new HPolyline;
            pObj->clone(pPolylineObj);
            ob->pObjList.append(pPolylineObj);
        }
        else if(pObj->getShapeType() == enumPolygon)
        {
            HPolygon* pPolygonObj = new HPolygon;
            pObj->clone(pPolygonObj);
            ob->pObjList.append(pPolygonObj);
        }
        else if(pObj->getShapeType() == enumPie)
        {
            HPie* pPieObj = new HPie;
            pObj->clone(pPieObj);
            ob->pObjList.append(pPieObj);
        }
        else if(pObj->getShapeType() == enumArc)
        {
            HArc* pArcObj = new HArc;
            pObj->clone(pArcObj);
            ob->pObjList.append(pArcObj);
        }
        else if(pObj->getShapeType() == enumText)
        {
            HText* pTextObj = new HText;
            pObj->clone(pTextObj);
            ob->pObjList.append(pTextObj);
        }
        else if(pObj->getShapeType() == enumComplex)
        {
            HIconObj* pObj1 = (HIconObj*)pObj;
            HIconObj* pIconObj = new HIconObj(pObj1->iconTemplate());
            pObj->clone(pIconObj);
            ob->pObjList.append(pIconObj);
        }
    }
}


HBaseObj* HGroupObj::newObj(QString tagName)
{
    HBaseObj* pObj = NULL;
    quint8 nObjType = enumNo;
    if(tagName == "Line")
    {
        pObj = new HLine();
        nObjType = enumLine;
    }
    else if(tagName == "Rectangle")
    {
        pObj = new HRectangle();
        nObjType = enumRectangle;
    }
    else if(tagName == "Ellipse")
    {
        pObj = new HEllipse();
        nObjType = enumEllipse;
    }
    else if(tagName == "Circle")
    {
        pObj = new HCircle();
        nObjType = enumCircle;
    }
    else if(tagName == "Polygon")
    {
        pObj = new HPolygon();
        nObjType = enumPolygon;
    }
    else if(tagName == "Arc")
    {
        pObj = new HArc();
        nObjType = enumArc;
    }
    else if(tagName == "Pie")
    {
        pObj = new HPie();
        nObjType = enumPie;
    }
    else if(tagName == "Text")
    {
        pObj = new HText();
        nObjType = enumText;
    }
    else if(tagName == "Polyline")
    {
        pObj = new HPolyline();
        nObjType = enumPolyline;
    }
    else if(tagName == "WfPointObj")
    {
        //先获取icontemplate
        if(!m_pGraph)
            return NULL;
        HIconTemplate* icontemp = m_pGraph->findIconTemplate(QUuid(m_strUuid));
        if(!icontemp)
            return NULL;
        pObj = new HIconObj(icontemp);
        nObjType = enumComplex;
    }
    pObj->setShapeType((DRAWSHAPE)nObjType);
    return pObj;
}

void HGroupObj::clone(HBaseObj* obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

void HGroupObj::setGraph(HGraph* graph)
{
    m_pGraph = graph;
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
void HGroupObj::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->resetRectPoint(pt1,pt2);
    }
}

void HGroupObj::resize(double w,double h)
{
   // rectWidth = rectWidth*w;
   // rectHeight = rectHeight*h;
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

HBaseObj* HGroupObj::takeFirst()
{
    return pObjList.takeFirst();
}

bool HGroupObj::isEmpty()
{
    return pObjList.isEmpty();
}


void HGroupObj::paint(QPainter* painter)
{
    //还需要判断
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
    painter->restore();
    if(pItem && pItem->isSelected())
    {

        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
    }

}

void HGroupObj::drawSelect(QPainter* painter)
{
    painter->save();
    QPen pen1 = QPen(Qt::red);
    pen1.setWidth(1);
    QBrush brush;
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    painter->setPen(pen1);
    painter->setBrush(brush);

    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);

    qreal halfpw = 6.00;
    QRectF rect1,rect2,rect3,rect4;
    rect1.setSize(QSizeF(halfpw,halfpw));
    rect1.moveCenter(rect.topLeft());
    rect2.setSize(QSizeF(halfpw,halfpw));
    rect2.moveCenter(rect.topRight());
    rect3.setSize(QSizeF(halfpw,halfpw));
    rect3.moveCenter(rect.bottomLeft());
    rect4.setSize(QSizeF(halfpw,halfpw));
    rect4.moveCenter(rect.bottomRight());


    QRectF rectC1,rectC2,rectC3,rectC4; //上中，下中，左中，右中
    rectC1.setSize(QSizeF(halfpw,halfpw));
    rectC2.setSize(QSizeF(halfpw,halfpw));
    rectC3.setSize(QSizeF(halfpw,halfpw));
    rectC4.setSize(QSizeF(halfpw,halfpw));
    QPointF ptC1 = QPointF((rect.topLeft().x()+rect.topRight().x())/2,rect.top());
    QPointF ptC2 = QPointF((rect.bottomLeft().x()+rect.bottomRight().x())/2,rect.bottom());
    QPointF ptC3 = QPointF(rect.left(),(rect.topLeft().y()+rect.bottomLeft().y())/2);
    QPointF ptC4 = QPointF(rect.right(),(rect.topRight().y()+rect.bottomRight().y())/2);

    rectC1.moveCenter(ptC1);
    rectC2.moveCenter(ptC2);
    rectC3.moveCenter(ptC3);
    rectC4.moveCenter(ptC4);

    painter->drawRect(rect1);
    painter->drawRect(rect2);
    painter->drawRect(rect3);
    painter->drawRect(rect4);

    painter->drawRect(rectC1);
    painter->drawRect(rectC2);
    painter->drawRect(rectC3);
    painter->drawRect(rectC4);

    painter->restore();
}

void HGroupObj::drawMulSelect(QPainter *painter,bool benchmark)
{
    //多选有两种情况1.不是标杆，2.是标杆
    //是标杆的话 就要加持绘制
    painter->save();
    QPen pen1;
    pen1.setWidth(2);
    if(benchmark)
    {
        pen1.setColor(QColor("#FF00FF"));
    }
    else
    {
        pen1.setColor(QColor("#EE82EE"));//粉色
    }
    painter->setPen(pen1);
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    painter->drawRect(rect);
    painter->restore();
}
