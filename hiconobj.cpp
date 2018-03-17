#include "hiconobj.h"
#include "hiconcomplexitem.h"
#include "hiconshowpattern.h"
/*************************************图符类**********************************/
/*
 * 图符类只用于drawgrph和在线系统上面，所以保存的信息和基本图元是不一致的。
*/
HIconObj::HIconObj()
{
    //去掉本身所有的边框颜色设置，这些都是不需要的
    pDynamicObj = new HDynamicObj;
    bFrameSee = false;
    nGraphID = (int)-1;
    btGraphOperator = MODE_OPEN_GRAPH;
    btGraphComfirm = COMFIRM_MODE_GRAPH;
}

HIconObj::HIconObj(HIconTemplate* it)
    :pIconTemplate(it)
{
    pDynamicObj = new HDynamicObj;
    pIconSymbol = new HIconSymbol(it);
    initIconTemplate();

    bFrameSee = false;
}

HIconObj::~HIconObj()
{
    if(pDynamicObj)
        delete pDynamicObj;
    if(pIconSymbol)
        delete pIconSymbol;
    //if(pIconTemplate)
    //    delete pIconTemplate;
    pIconTemplate = NULL;
}

void HIconObj::initIconTemplate()
{
    strCatalogName = pIconTemplate->getCatalogName();
    nCatalogType = pIconTemplate->getCatalogType();
    strUuid = pIconTemplate->getUuid().toString();
    pIconTemplate->getSymbol()->copyTo(pIconSymbol);
}

//二进制读写
void HIconObj::readData(QDataStream* data)
{
    if(!data) return;
    HRectangle::readData(data);
    QString s;
    *data>>s;
    strCatalogName = s;
    int n;
    *data>>n;
    nCatalogType = n;
    *data>>s;
    strUuid = s;
    *data>>s;
    strSymbolName = s;
    *data>>n;
    nSymbolType = n;
    *data>>n;
    nGraphID = n;
    uchar bt;
    *data>>bt;
    btGraphOperator = bt;
    *data>>bt;
    btGraphComfirm = bt;
    //还有动态数据
    if(pDynamicObj)
        pDynamicObj->readData(data);
}

void HIconObj::writeData(QDataStream* data)
{
    if(!data) return;
    HRectangle::writeData(data);
    *data<<strCatalogName;
    *data<<nCatalogType;
    *data<<strUuid;
    *data<<strSymbolName;
    *data<<nSymbolType;
    *data<<nGraphID;
    *data<<btGraphOperator;
    *data<<btGraphComfirm;
    //动态数据
    if(pDynamicObj)
        pDynamicObj->writeData(data);
}

//xml文件读写
void HIconObj::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectangle::readXml(dom);
    strCatalogName = dom->attribute("CatalogName");
    nCatalogType = dom->attribute("CatalogType").toInt();
    strUuid = dom->attribute("Uuid");
    strSymbolName = dom->attribute("SymbolName");
    nSymbolType = dom->attribute("SymbolType").toInt();
    nGraphID = dom->attribute("graphID").toInt();
    btGraphOperator = dom->attribute("graphOperator").toUInt();
    btGraphComfirm = dom->attribute("graphComfirm").toUInt();

    //pIconSymbol对象需不需要保存?不需要！每次新建complexObj就可以从templates里面导入进来copy过来一个pSymbol对象即可
    //动态数据
    QDomElement RelationDom = dom->namedItem("Relation").toElement();
    if(pDynamicObj)
        pDynamicObj->readXml(&RelationDom);
}

void HIconObj::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectangle::writeXml(dom);
    dom->setAttribute("CatalogName",strCatalogName);
    dom->setAttribute("CatalogType",nCatalogType);
    dom->setAttribute("Uuid",strUuid);
    dom->setAttribute("SymbolName",strSymbolName);
    dom->setAttribute("SymbolType",nSymbolType);
    dom->setAttribute("graphID",nGraphID);
    dom->setAttribute("graphOperator",btGraphOperator);
    dom->setAttribute("graphComfirm",btGraphComfirm);
    //动态数据
    QDomElement RelationDom = dom->ownerDocument().createElement("Relation");
    dom->appendChild(RelationDom);
    if(pDynamicObj)
        pDynamicObj->writeXml(&RelationDom);
}


QString HIconObj::TagName()
{
    return "ComplexObj";
}

//拷贝克隆
void HIconObj::copyTo(HBaseObj* obj)
{
    HIconObj* ob = (HIconObj*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->strCatalogName = strCatalogName;
    ob->nCatalogType = nCatalogType;
    ob->strSymbolName = strSymbolName;
    ob->nSymbolType = nSymbolType;
    ob->nGraphID = nGraphID;
    ob->btGraphOperator = btGraphOperator;
    ob->btGraphComfirm = btGraphComfirm;

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

void HIconObj::clone(HBaseObj* obj)
{
    if(!obj) return;
    HBaseObj::clone(obj);
    copyTo(obj);
}

DRAWSHAPE HIconObj::getShapeType()
{
    return enumComplex;
}

void HIconObj::moveBy(qreal dx,qreal dy)
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

void HIconObj::paint(QPainter* painter)
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
    //painter->drawRect(rect);

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

void HIconObj::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
    if(pIconSymbol)
        pIconSymbol->resetRectPoint(pt1,pt2);
}

void HIconObj::resize(qreal w,qreal h)
{
    if(pIconSymbol)
        pIconSymbol->resize(w,h);
}

QRectF HIconObj::boundingRect() const
{
   return shape().controlPointRect();
}

bool HIconObj::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HIconObj::shape() const
{
    QPainterPath path;
    QRectF boundingRect;
    boundingRect.setX(topLeft.x()-5);
    boundingRect.setY(topLeft.y()-5);
    boundingRect.setWidth(rectWidth +10);
    boundingRect.setHeight(rectHeight+10);
    path.addRect(boundingRect);


   // QRectF shapeRect = boundingRect.adjusted(10,10,-10,-10);
  //  path.addRect(shapeRect);
    return path;
}

void HIconObj::setUuid(const QString& uuid)
{
    this->strUuid = uuid;
}

QString HIconObj::getUuid()
{
    return strUuid;
}

void HIconObj::setCatalogName(const QString& catalogName)
{
    strCatalogName = catalogName;
}

QString HIconObj::getCatalogName()
{
    return strCatalogName;
}

void HIconObj::setObjType( uchar catalogType)
{
    nCatalogType = catalogType;
}

int HIconObj::getObjType()
{
    return nCatalogType;
}

void HIconObj::setSymbolName(const QString& symbolName)
{
    strSymbolName = symbolName;
}

QString HIconObj::getSymbolName()
{
    return strSymbolName;
}

void HIconObj::setSymbolType(int symbolType)
{
    nSymbolType = symbolType;
}

int HIconObj::getSymbolType()
{
    return nSymbolType;
}

void HIconObj::setGraphID(int graphID)
{
    nGraphID = graphID;
}

int HIconObj::getGraphID()
{
    return nGraphID;
}

void HIconObj::setGraphOperator(uchar graphOperator)
{
    btGraphOperator = graphOperator;
}

uchar HIconObj::getGraphOpeartor()
{
    return btGraphOperator;
}

void HIconObj::setGraphComfirm(uchar graphComfirm)
{
    btGraphComfirm = graphComfirm;
}

uchar HIconObj::getGraphComfirm()
{
    return btGraphComfirm;
}

void HIconObj::setIconTemplate(HIconTemplate* t)
{
    pIconTemplate = t;
    if(!pIconSymbol)
    {
        pIconSymbol = new HIconSymbol(t);
        initIconTemplate();
    }
}

HIconTemplate* HIconObj::iconTemplate()
{
    return pIconTemplate;
}

HIconSymbol* HIconObj::getIconSymbol()
{
    return pIconSymbol;
}

HDynamicObj* HIconObj::getDynamicObj()
{
    return pDynamicObj;
}

void HIconObj::initDynamicData()
{
    clearDynamicData();
}

void HIconObj::clearDynamicData()
{
    if(pDynamicObj)
    {
        delete pDynamicObj;
        pDynamicObj = NULL;
    }
}
