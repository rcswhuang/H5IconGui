#include "H5IconGui/hgroupobj.h"

HGroupObj::HGroupObj()
{

}

HGroupObj::~HGroupObj()
{

}

//二进制读写
void HGroupObj::readData(QDataStream* data)
{

}

void HGroupObj::writeData(QDataStream* data)
{

}

//xml文件读写
void HGroupObj::readXml(QDomElement* data)
{

}

void HGroupObj::writeXml(QDomElement* data)
{

}

QString HGroupObj::TagName()
{

}

//拷贝克隆
void HGroupObj::copyTo(HBaseObj* obj)
{

}

void HGroupObj::clone(HBaseObj* obj)
{

}

DRAWSHAPE HGroupObj::getShapeType()
{

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
