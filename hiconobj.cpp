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
    pText = NULL;
    pDynamicObj = new HDynamicObj;
    bFrameSee = false;
    nGraphID = (int)-1;
    btGraphOperator = MODE_OPEN_GRAPH;
    btGraphComfirm = COMFIRM_MODE_GRAPH;
}

HIconObj::HIconObj(HIconTemplate* it)
    :pIconTemplate(it)
{
    pText = NULL;
    pDynamicObj = new HDynamicObj;
    pIconSymbol = new HIconSymbol(it);
    initIconTemplate();
    bFrameSee = false;
    nGraphID = (int)-1;
    btGraphOperator = MODE_OPEN_GRAPH;
    btGraphComfirm = COMFIRM_MODE_GRAPH;

}

HIconObj::~HIconObj()
{
    clearDynamicData();

    if(pIconSymbol)
    {
        delete pIconSymbol;
        pIconSymbol = NULL;
    }
    //if(pIconTemplate)
    //    delete pIconTemplate;
    if(pText)
    {
        delete pText;
        pText = NULL;
    }
    pIconTemplate = NULL;
}

void HIconObj::initIconTemplate()
{
    strCatalogName = pIconTemplate->getCatalogName();
    nCatalogType = pIconTemplate->getCatalogType();
    strUuid = pIconTemplate->getUuid().toString();
    pIconTemplate->getSymbol()->copyTo(pIconSymbol);
    if(TEMPLATE_TYPE_ANALOGUE == nCatalogType || TEMPLATE_TYPE_CONTROL == nCatalogType)
    {
        HText *pTemp = pIconSymbol->getFirstTextObj();
        if(!pText)
        {
            pText = new HText();
            pTemp->copyTo(pText);
        }
    }
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
   // *data>>s;
   // strSymbolName = s;
   // *data>>n;
  //  nSymbolType = n;
    *data>>n;
    nGraphID = n;
    uchar bt;
    *data>>bt;
    btGraphOperator = bt;
    *data>>bt;
    btGraphComfirm = bt;

    if(pIconSymbol)
    {
        pIconSymbol->readData(0,data);
        if(TEMPLATE_TYPE_ANALOGUE == nCatalogType || TEMPLATE_TYPE_CONTROL == nCatalogType)
        {
            HText *pText = pIconSymbol->getFirstTextObj();
            pText->readData(data);
        }
    }
    //动态数据
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
    //*data<<strSymbolName;
    //*data<<nSymbolType;
    *data<<nGraphID;
    *data<<btGraphOperator;
    *data<<btGraphComfirm;

    if(pIconSymbol)
    {
        pIconSymbol->writeData(0,data);
        if(TEMPLATE_TYPE_ANALOGUE == nCatalogType || TEMPLATE_TYPE_CONTROL == nCatalogType)
        {
            HText *pText = pIconSymbol->getFirstTextObj();
            pText->writeData(data);
        }
    }

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
    nGraphID = dom->attribute("graphID").toInt();
    btGraphOperator = dom->attribute("graphOperator").toUInt();
    btGraphComfirm = dom->attribute("graphComfirm").toUInt();

    /*
    QDomElement symbolDom = dom->namedItem("IconSymbol").toElement();
    if(!symbolDom.isNull())
    {
        pIconSymbol->readXml(&symbolDom);
    }*/
    QSizeF pt = pIconTemplate->getDefaultSize();
    double w1 = rectWidth/(pt.width()*20);
    double h1 = rectHeight/(pt.height()*20);
    resetRectPoint(topLeft,QPointF(-pt.width()*10,-pt.height()*10));
    resize(w1,h1);
    //如果是遥测类型或者控制点类型 还需要保存first text信息
    if(TEMPLATE_TYPE_ANALOGUE == nCatalogType || TEMPLATE_TYPE_CONTROL == nCatalogType)
    {
        pText = new HText();
        if(pText)
        {
            QDomElement textDom = dom->namedItem(pText->TagName()).toElement();
            if(!textDom.isNull())
            {
                pText->readXml(&textDom);
            }
            else
            {
                delete pText;
                pText = NULL;
            }
        }
    }

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
    //dom->setAttribute("SymbolName",strSymbolName);
    //dom->setAttribute("SymbolType",nSymbolType);
    dom->setAttribute("graphID",nGraphID);
    dom->setAttribute("graphOperator",btGraphOperator);
    dom->setAttribute("graphComfirm",btGraphComfirm);

    /*
    QDomElement symbolDom = dom->ownerDocument().createElement("IconSymbol");
    dom->appendChild(symbolDom);
    if(pIconSymbol)
    {
        pIconSymbol->writeXml(&symbolDom);
    }*/
    //如果是遥测类型或者控制点类型 还需要保存first text信息
    if(TEMPLATE_TYPE_ANALOGUE == nCatalogType || TEMPLATE_TYPE_CONTROL == nCatalogType)
    {
        if(pText)
        {
            QDomElement childEle = dom->ownerDocument().createElement(pText->TagName());
            dom->appendChild(childEle);
            pText->writeXml(&childEle);
        }
    }

    //动态数据
    QDomElement RelationDom = dom->ownerDocument().createElement("Relation");
    dom->appendChild(RelationDom);
    if(pDynamicObj)
        pDynamicObj->writeXml(&RelationDom);
}


QString HIconObj::TagName()
{
    return "WfPointObj";
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
    ob->nGraphID = nGraphID;
    ob->btGraphOperator = btGraphOperator;
    ob->btGraphComfirm = btGraphComfirm;

    if(pIconSymbol && ob->pIconSymbol)
    {
        ob->pIconSymbol->clear();
        pIconSymbol->copyTo(ob->pIconSymbol);
        HText *pText = pIconSymbol->getFirstTextObj();
        HText *pObText = ob->pIconSymbol->getFirstTextObj();
        if(pText && pObText)
        {
            pText->copyTo(pObText);
        }
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
        /*QPen pen1 = QPen(Qt::green);
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
        painter->drawRect(rect4);*/
        if(pItem->bMulSelect)
            drawMulSelect(painter,pItem->bBenchmark);
        else
            drawSelect(painter);
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
    //strSymbolName = symbolName;
    if(pIconSymbol)
        pIconSymbol->setSymbolName(symbolName);
}

QString HIconObj::getSymbolName()
{
    if(pIconSymbol)
        pIconSymbol->getSymolName();
    return "";
}

void HIconObj::setSymbolType(int symbolType)
{
    //if(pIconSymbol)
     //   pIconSymbol->setSymbolType(symbolType);
}

int HIconObj::getSymbolType()
{
    //if(pIconSymbol)
     //  return pIconSymbol->getSymbolType();
    return (int)-1;
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

void HIconObj::updateResize()
{
    if(!pIconTemplate)
        return;
    QSizeF pt = pIconTemplate->getDefaultSize();
    double w1 = rectWidth/(pt.width()*20);
    double h1 = rectHeight/(pt.height()*20);
    resetRectPoint(topLeft,QPointF(-pt.width()*10,-pt.height()*10));
    resize(w1,h1);
}
