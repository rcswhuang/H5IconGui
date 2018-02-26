#include "H5IconGui/hiconcomplexobj.h"
#include "H5IconGui/hiconcomplexitem.h"
#include "H5IconGui/hiconshowpattern.h"
/*************************************图符类**********************************/
/*
 * 图符类只用于drawgrph和在线系统上面，所以保存的信息和基本图元是不一致的。
*/
HIconComplexObj::HIconComplexObj()
{
    //去掉本身所有的边框颜色设置，这些都是不需要的
    pDynamicObj = new HDynamicObj;
    bFrameSee = false;
}

HIconComplexObj::HIconComplexObj(HIconTemplate* it)
    :pIconTemplate(it)
{
    pDynamicObj = new HDynamicObj;
    pIconSymbol = new HIconSymbol(it);
    initIconTemplate();

    bFrameSee = false;
}

HIconComplexObj::~HIconComplexObj()
{
    if(pDynamicObj)
        delete pDynamicObj;
    if(pIconSymbol)
        delete pIconSymbol;
    //if(pIconTemplate)
    //    delete pIconTemplate;
    pIconTemplate = NULL;
}

void HIconComplexObj::initIconTemplate()
{
    catalogName = pIconTemplate->getCatalogName();
    catalogType = pIconTemplate->getCatalogType();
    uuid = pIconTemplate->getUuid().toString();
    pIconTemplate->getSymbol()->copyTo(pIconSymbol);
}

//二进制读写
void HIconComplexObj::readData(QDataStream* data)
{
    if(!data) return;
    HRectObj::readData(data);
    QString s;
    *data>>s;
    catalogName = s;
    int n;
    *data>>n;
    catalogType = n;
    *data>>s;
    uuid = s;
    *data>>s;
    symbolName = s;
    *data>>n;
    symbolType = n;

    //还有动态数据
    if(pDynamicObj)
        pDynamicObj->readData(data);
}

void HIconComplexObj::writeData(QDataStream* data)
{
    if(!data) return;
    HRectObj::writeData(data);
    *data<<catalogName;
    *data<<catalogType;
    *data<<uuid;
    *data<<symbolName;
    *data<<symbolType;
    //动态数据
    if(pDynamicObj)
        pDynamicObj->writeData(data);
}

//xml文件读写
void HIconComplexObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectObj::readXml(dom);
    catalogName = dom->attribute("CatalogName");
    catalogType = dom->attribute("CatalogType").toInt();
    uuid = dom->attribute("Uuid");
    symbolName = dom->attribute("SymbolName");
    symbolType = dom->attribute("SymbolType").toInt();

    //pIconSymbol对象需不需要保存?不需要！每次新建complexObj就可以从templates里面导入进来copy过来一个pSymbol对象即可
    //动态数据
    QDomElement RelationDom = dom->namedItem("Relation").toElement();
    if(pDynamicObj)
        pDynamicObj->readXml(&RelationDom);
}

void HIconComplexObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectObj::writeXml(dom);
    dom->setAttribute("CatalogName",catalogName);
    dom->setAttribute("CatalogType",catalogType);
    dom->setAttribute("Uuid",uuid);
    dom->setAttribute("SymbolName",symbolName);
    dom->setAttribute("SymbolType",symbolType);
    //动态数据
    QDomElement RelationDom = dom->ownerDocument().createElement("Relation");
    dom->appendChild(RelationDom);
    if(pDynamicObj)
        pDynamicObj->writeXml(&RelationDom);
}


QString HIconComplexObj::TagName()
{
    return "ComplexObj";
}

//拷贝克隆
void HIconComplexObj::copyTo(HBaseObj* obj)
{
    HIconComplexObj* ob = (HIconComplexObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->catalogName = catalogName;
    ob->catalogType = catalogType;
    ob->symbolName = symbolName;
    ob->symbolType = symbolType;

    if(pIconSymbol && ob->pIconSymbol)
    {
        pIconSymbol->copyTo(ob->pIconSymbol);
    }

    //动态数据
    if(pDynamicObj && ob->pDynamicObj)
    {
        pDynamicObj->copyTo(ob->pDynamicObj);
    }
}

void HIconComplexObj::clone(HBaseObj* obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HIconComplexObj::getShapeType()
{
    return enumComplex;
}

void HIconComplexObj::moveBy(qreal dx,qreal dy)
{
    topLeft.setX(topLeft.x() + dx);
    topLeft.setY(topLeft.y() + dy);
    if(!getIconSymbol() && !getIconSymbol()->getCurrentPatternPtr())
        return;
    HIconShowPattern* pattern = getIconSymbol()->getCurrentPatternPtr();
    for(int i = 0; i < pattern->pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)(pattern->pObjList[i]);
        if(pObj)
        {
           pObj->moveBy(dx,dy);
        }
    }
    bModify = true;
}

void HIconComplexObj::paint(QPainter* painter)
{
    HIconComplexItem* pItem = qgraphicsitem_cast<HIconComplexItem*>(getIconGraphicsItem());
    painter->save();
    if(!getIconSymbol() && !getIconSymbol()->getCurrentPatternPtr())
        return;
    HIconShowPattern* pattern = getIconSymbol()->getCurrentPatternPtr();
    for(int i = 0; i < pattern->pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)(pattern->pObjList[i]);
        if(pObj)
        {
           pObj->paint(painter);
        }
    }
    QPen pen(Qt::red);
    painter->setPen(pen);
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    painter->drawRect(rect);

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

void HIconComplexObj::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
    if(pIconSymbol)
        pIconSymbol->resetRectPoint(pt1,pt2);
}

void HIconComplexObj::resize(qreal w,qreal h)
{
    if(pIconSymbol)
        pIconSymbol->resize(w,h);
}

QRectF HIconComplexObj::boundingRect() const
{
   return shape().controlPointRect();
}

bool HIconComplexObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HIconComplexObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-10);
    boundingRect.setY(topLeft.y()-10);
    boundingRect.setWidth(rectWidth +20);
    boundingRect.setHeight(rectHeight+20);
    path.addRect(boundingRect);


   // QRectF shapeRect = boundingRect.adjusted(10,10,-10,-10);
  //  path.addRect(shapeRect);
    return path;
}

void HIconComplexObj::setUuid(const QString& uuid)
{
    this->uuid = uuid;
}

QString HIconComplexObj::getUuid()
{
    return uuid;
}

void HIconComplexObj::setCatalogName(const QString& strCatalogName)
{
    catalogName = strCatalogName;
}

QString HIconComplexObj::getCatalogName()
{
    return catalogName;
}

void HIconComplexObj::setIconTemplate(HIconTemplate* t)
{
    pIconTemplate = t;
    if(!pIconSymbol)
    {
        pIconSymbol = new HIconSymbol(t);
        initIconTemplate();
    }
}

HIconTemplate* HIconComplexObj::iconTemplate()
{
    return pIconTemplate;
}

HIconSymbol* HIconComplexObj::getIconSymbol()
{
    return pIconSymbol;
}

HDynamicObj* HIconComplexObj::getDynamicObj()
{
    return pDynamicObj;
}

void HIconComplexObj::initDynamicData()
{
    clearDynamicData();
}

void HIconComplexObj::clearDynamicData()
{
    if(pDynamicObj)
    {
        delete pDynamicObj;
        pDynamicObj = NULL;
    }
}
