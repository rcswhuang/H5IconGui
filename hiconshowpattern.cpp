#include "H5IconGui/hiconshowpattern.h"
#include "H5IconGui/hiconobj.h"
HIconShowPattern::HIconShowPattern(HIconSymbol* symbol):pSymbol(symbol)
{

}

HIconShowPattern::~HIconShowPattern()
{
    clear();
}

void HIconShowPattern::readXml(QDomElement* dom)
{
    if(!dom) return;
    QDomElement objEle = dom->namedItem("ShowPatterns").toElement();
    //构建下面的元素对象
    QDomNode n = objEle.firstChild();
    int i = 0;
    for(;!n.isNull();n=n.nextSibling(),i++)
    {
        QDomElement e = n.toElement();
        int patternID = e.attribute("PatternID").toInt();
        HIconShowPattern* pattern = pSymbol->newPattern(patternID);
        if(!pattern)
        {
            delete pattern;
            pattern = NULL;
            continue;
        }
        pattern->readXml(&e);
        pSymbol->pShowPatternVector.append(pattern);
    }
}

void HIconShowPattern::writeXml(QDomElement *dom)
{
    if(!dom) return;
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

void HIconShowPattern::addObj(HBaseObj* pObj,bool badd)
{
    if(!pObj)
        return;
    if(badd)
        pObj->nPattern = nPattern;
    pObjList.append(pObj);
}

void HIconShowPattern::delObj(HBaseObj* pObj)
{
    if(!pObj)
        return;
    pObjList.removeOne(pObj);
    delete pObj;
    pObj = NULL;
}

void HIconShowPattern::takeObj(HBaseObj* pObj)
{
    if(!pObj)
        return;
    pObjList.removeOne(pObj);
}

void HIconShowPattern::copyTo(HIconShowPattern* sp)
{
    if(!sp)
        return;
    sp->strName = strName;
    sp->nPattern = nPattern;
    sp->strAlias = strAlias;
    sp->pSymbol = pSymbol;
    //sp->clear();
    for(int i = 0; i < pObjList.size();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        if(pObj->getShapeType() == enumLine)
        {
            HLineObj* pLineObj = new HLineObj;
            pObj->clone(pLineObj);
            sp->addObj(pLineObj);
        }
        else if(pObj->getShapeType() == enumRectangle)
        {
            HRectObj* pRectObj = new HRectObj;
            pObj->clone(pRectObj);
            sp->addObj(pRectObj);
        }
        else if(pObj->getShapeType() == enumEllipse)
        {
            HEllipseObj* pEllipseObj = new HEllipseObj;
            pObj->clone(pEllipseObj);
            sp->addObj(pEllipseObj);
        }
        else if(pObj->getShapeType() == enumCircle)
        {
            HCircleObj* pCircleObj = new HCircleObj;
            pObj->clone(pCircleObj);
            sp->addObj(pCircleObj);
        }
        else if(pObj->getShapeType() == enumPolyline)
        {
            HPolylineObj* pPolylineObj = new HPolylineObj;
            pObj->clone(pPolylineObj);
            sp->addObj(pPolylineObj);
        }
        else if(pObj->getShapeType() == enumPolygon)
        {
            HPolygonObj* pPolygonObj = new HPolygonObj;
            pObj->clone(pPolygonObj);
            sp->addObj(pPolygonObj);
        }
        else if(pObj->getShapeType() == enumPie)
        {
            HPieObj* pPieObj = new HPieObj;
            pObj->clone(pPieObj);
            sp->addObj(pPieObj);
        }
        else if(pObj->getShapeType() == enumArc)
        {
            HArcObj* pArcObj = new HArcObj;
            pObj->clone(pArcObj);
            sp->addObj(pArcObj);
        }
        else if(pObj->getShapeType() == enumText)
        {
            HTextObj* pTextObj = new HTextObj;
            pObj->clone(pTextObj);
            sp->addObj(pTextObj);
        }
    }
}

void HIconShowPattern::clear()
{
    if(pObjList.isEmpty()) return;
    while(!pObjList.isEmpty())
    {
        HBaseObj* pObj = pObjList.takeFirst();
        if(pObj)
        {
            delete pObj;
            pObj = NULL;
        }
    }
    pObjList.clear();
}

void HIconShowPattern::resize(double w,double h)
{
    for(int i = 0; i < pObjList.size();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->resize(w,h);
    }
}



