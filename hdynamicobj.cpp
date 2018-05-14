#include "hdynamicobj.h"

HDynamicObj::HDynamicObj()
{
    wStation = (ushort)-1;
    wPoint = (ushort)-1;
    wAttrib = (ushort)-1;
    btFieldType = (uchar)-1;
    wFormula = (ushort)-1;
}

HDynamicObj::~HDynamicObj()
{

}

//二进制读写
void HDynamicObj::readData(QDataStream* data)
{
    if(!data) return;
    ushort temp;
    *data>>temp;
    wStation = temp;
    *data>>temp;
    wPoint = temp;
    *data>>temp;
    wAttrib = temp;
    uchar btTemp;
    *data>>btTemp;
    btFieldType = btTemp;
    *data>>temp;
    wFormula = temp;
}

void HDynamicObj::writeData(QDataStream* data)
{
    if(!data) return;
    *data<<(ushort)wStation;
    *data<<(ushort)wPoint;
    *data<<(ushort)wAttrib;
    *data<<(uchar)btFieldType;
    *data<<(ushort)wFormula;
}

//xml文件读写
void HDynamicObj::readXml(QDomElement* dom)
{
    if(!dom)
        return;
    wStation = dom->attribute("Station").toUShort();
    wPoint = dom->attribute("Point").toUShort();
    wAttrib = dom->attribute("Attrib").toUShort();
    btFieldType = dom->attribute("FieldType").toUShort();
    wFormula = dom->attribute("Formula").toUShort();
}

void HDynamicObj::writeXml(QDomElement* dom)
{
    if(!dom)
        return;
    dom->setAttribute("Station",wStation);
    dom->setAttribute("Point",wPoint);
    dom->setAttribute("Attrib",wAttrib);
    dom->setAttribute("FieldType",btFieldType);
    dom->setAttribute("Formula",wFormula);
}

QString HDynamicObj::TagName()
{
    return "DynamicData";
}

//拷贝克隆
void HDynamicObj::copyTo(HDynamicObj* obj)
{

    obj->btFieldType = btFieldType;
    obj->wFormula = wFormula;
    obj->wStation = wStation;
    obj->wPoint = wPoint;
    obj->wAttrib = wAttrib;
}

void HDynamicObj::clone(HDynamicObj* obj)
{
    if(!obj)
        return;
    copyTo(obj);
}


void HDynamicObj::setDBStation(ushort wStation)
{
    this->wStation = wStation;
}

ushort HDynamicObj::getDBStation()
{
    return wStation;
}

void HDynamicObj::setDBPoint(ushort wPoint)
{
    this->wPoint = wPoint;
}

ushort HDynamicObj::getDBPoint()
{
    return wPoint;
}

void HDynamicObj::setDBAttr(ushort wAttr)
{
    wAttrib = wAttr;
}

ushort HDynamicObj::getDBAttr()
{
    return wAttrib;
}

void HDynamicObj::setValueType(uchar btFt)
{
    btFieldType = btFt;
}

uchar HDynamicObj::getValueType()
{
    return btFieldType;
}

void HDynamicObj::setFormula(ushort formula)
{
    wFormula = formula;
}

ushort HDynamicObj::getFormula()
{
    return wFormula;
}
