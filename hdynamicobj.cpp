#include "H5IconGui/hdynamicobj.h"

HDynamicObj::HDynamicObj()
{

}

HDynamicObj::~HDynamicObj()
{

}

//二进制读写
void HDynamicObj::readData(QDataStream* data)
{

}

void HDynamicObj::writeData(QDataStream* data)
{

}

//xml文件读写
void HDynamicObj::readXml(QDomElement* data)
{

}

void HDynamicObj::writeXml(QDomElement* data)
{

}

QString HDynamicObj::TagName()
{

}

//拷贝克隆
void HDynamicObj::copyTo(HDynamicObj* obj)
{

}

void HDynamicObj::clone(HDynamicObj* obj)
{

}


void HDynamicObj::setDBStation(ushort wStation)
{

}

ushort HDynamicObj::getDBStation()
{

}

void HDynamicObj::setDBPoint(ushort wPoint)
{

}

ushort HDynamicObj::getDBPoint()
{

}

void HDynamicObj::setDBAttr(ushort wAttr)
{

}

ushort HDynamicObj::getDBAttr()
{

}

void HDynamicObj::setValueType(uchar btFt)
{

}

uchar HDynamicObj::getValueType()
{

}

void HDynamicObj::setFormula(ushort formula)
{

}

ushort HDynamicObj::getFormula()
{

}
