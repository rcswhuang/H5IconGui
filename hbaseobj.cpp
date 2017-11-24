#include "H5IconGui/hbaseobj.h"
#include <QVariant>

HBaseObj::HBaseObj(QObject *parent) : QObject(parent)
{
    init();
    pIconGraphicsItem = NULL;
}

HBaseObj::~HBaseObj()
{
}

QString HBaseObj::TagName()
{
    return QString("base");
}

void HBaseObj::init()
{
    strObjName = "baseobj"; //对象名称
    drawShape = enumNo;//图符类型(直线\圆)
    originX = 0;//原点位置（x,y)
    originY = 0;

    //线条方面
    strLineColor = "#00FF00";//线条颜色
    nLineWidth = 0;//线宽
    nLineStyle = (qint8)Qt::SolidLine;//线型
    nLineJoinStyle = (qint8)Qt::BevelJoin;//线连接处形状
    nLineCapStyle = (qint8)Qt::RoundCap;//线端形状


    //填充方面
    nFillWay = 0;//填充方式
    nFillStyle = (quint8)Qt::NoBrush;//填充风格
    strFillColor = "#00FF00"; //填充颜色
    nFillDirection = DIRECT_BOTTOM_TO_TOP;//填充方向
    nFillPercentage = 100;//填充比例

    //边框透明度
    bFrameSee = true;//边框可见
    nTransparency = 0; //透明度 0:不透明 100完全透明

    fRotateAngle = 0.0;//旋转角度
    bHorizonTurn = false;//水平翻转
    bVerticalTurn = false;//垂直翻转
    bVisible = true;//是否显示
    bDeleted = false;//是否删除
    nStackOrder = 0;

    bModify = false;
    //父图符
    m_pParent = NULL;

}

void HBaseObj::readData(QDataStream *data)
{
    if(!data) return;
    QString s;
    //*data>>s;
    //strObjName = s;
    //quint32 n32;
    //*data>>n32;
    //nObjectId = n32;
    quint8 n8;
    *data>>n8;
    drawShape = n8;
    double ff;
    *data>>ff;
    originX = ff;
    *data>>ff;
    originY = ff;
    *data>>s;
    strLineColor = s;
    *data>>n8;
    nLineWidth = n8;
    *data>>n8;
    nLineStyle = n8;
    *data>>n8;
    nLineJoinStyle = n8;
    *data>>n8;
    nLineCapStyle = n8;
    *data>>n8;
    nFillWay = n8;
    *data>>n8;
    nFillStyle = n8;
    *data>>s;
    strFillColor = s;
    *data>>n8;
    nFillDirection = n8;
    *data>>n8;
    nFillPercentage = n8;
    bool b;
    *data>>b;
    bFrameSee = b;
    *data>>n8;
    nTransparency = n8;
    float f;
    *data>>f;
    fRotateAngle = f;
    *data>>b;
    bHorizonTurn = b;
    *data>>b;
    bVerticalTurn = b;
    *data>>b;
    bVisible = n8;
    *data>>b;
    bDeleted = b;
    quint64 n64;
    *data>>n64;
    nStackOrder = n64;
    /**data>>n8;//showpattern num
    for(int i = 0; i < n8;i++)
    {
        quint8 nsp;
        *data>>nsp;
        nPattern.append(nsp);
    }*/
    *data>>n8;
    nPattern = n8;
    *data>>b;
    bModify = b;

}

void HBaseObj::writeData(QDataStream *data)
{
    if(!data) return;
    //*data<<strObjName;
    //*data<<(quint32)nObjectId;
    *data<<(quint8)drawShape;
    *data<<(double)originX;
    *data<<(double)originY;
    *data<<(QString)strLineColor;
    *data<<(quint8)nLineWidth;
    *data<<(quint8)nLineStyle;
    *data<<(quint8)nLineJoinStyle;
    *data<<(quint8)nLineCapStyle;
    *data<<(quint8)nFillWay;
    *data<<(quint8)nFillStyle;
    *data<<(QString)strFillColor;
    *data<<(quint8)nFillDirection;
    *data<<(quint8)nFillPercentage;
    *data<<(bool)bFrameSee;
    *data<<(quint8)nTransparency;
    *data<<(float)fRotateAngle;
    *data<<(bool)bHorizonTurn;
    *data<<(bool)bVerticalTurn;
    *data<<(bool)bVisible;
    *data<<(bool)bDeleted;
    *data<<(quint64)nStackOrder;
    *data<<(quint8)nPattern;
    *data<<(bool)bModify;
}

void HBaseObj::readXml(QDomElement* dom)
{
    if(!dom)
        return;

    strObjName = dom->attribute("ObjName");
    nObjectId = dom->attribute("ObjId").toUInt();
    drawShape = (quint8)dom->attribute("ObjType").toUInt();
    originX = dom->attribute("X").toDouble();
    originY = dom->attribute("Y").toDouble();
    strLineColor = dom->attribute("LineColor");
    nLineWidth = dom->attribute("LineWidth").toUInt();
    nLineStyle = dom->attribute("LineStyle").toUInt();
    nLineJoinStyle = dom->attribute("LineJoinStyle").toUInt();
    nLineCapStyle = dom->attribute("LineCapStyle").toUInt();
    nFillWay = dom->attribute("FillWay").toUInt();
    nFillStyle = dom->attribute("FillStyle").toUInt();
    strFillColor = dom->attribute("FillColor");
    nFillDirection = dom->attribute("FillDirection").toUInt();

    nFillPercentage = dom->attribute("nFillPercentage").toUInt();
    bFrameSee = (bool)dom->attribute("FrameSee").toUInt();
    nTransparency = dom->attribute("Transparency").toUInt();
    fRotateAngle = dom->attribute("RotateAngle").toDouble();
    bHorizonTurn = dom->attribute("HorizonTurn").toInt();
    bVerticalTurn = dom->attribute("VerticalTurn").toInt();
    bVisible = dom->attribute("Visible").toInt();
    nStackOrder = dom->attribute("StackOrder").toInt();
    nPattern = dom->attribute("nPattern").toUInt();
}

void HBaseObj::writeXml(QDomElement* dom)
{
    if(!dom)
        return;
    dom->setAttribute("ObjName",strObjName);
    dom->setAttribute("ObjId",nObjectId);
    dom->setAttribute("ObjType",drawShape); 
    dom->setAttribute("X",originX);
    dom->setAttribute("Y",originY);
    dom->setAttribute("LineColor",strLineColor);
    dom->setAttribute("LineWidth",nLineWidth);
    dom->setAttribute("LineStyle",nLineStyle);
    dom->setAttribute("LineJoinSytle",nLineJoinStyle);
    dom->setAttribute("LineCapSytle",nLineCapStyle);
    dom->setAttribute("FillWay",nFillWay);
    dom->setAttribute("FillStyle",nFillStyle);
    dom->setAttribute("FillColor",strFillColor);
    dom->setAttribute("FillDirection",nFillDirection);
    dom->setAttribute("FillPercentage",nFillPercentage);
    dom->setAttribute("FrameSee",bFrameSee);
    dom->setAttribute("Transparency",nTransparency);
    dom->setAttribute("RotateAngle",fRotateAngle);
    dom->setAttribute("HorizonTurn",bHorizonTurn);
    dom->setAttribute("VerticalTurn",bVerticalTurn);
    dom->setAttribute("Visible",bVisible);
    dom->setAttribute("StackOrder",nStackOrder);
    dom->setAttribute("nPattern",nPattern);
}

//拷贝克隆
void HBaseObj::copyTo(HBaseObj* obj)
{
    //id暂时不能复制 只能等到粘贴的时候才能赋值
    obj->drawShape = drawShape;
    obj->strObjName = strObjName;
    obj->nObjectId = nObjectId;
    obj->originX = originX;
    obj->originY = originY;
    obj->fRotateAngle = fRotateAngle;

    obj->strLineColor = strLineColor;
    obj->nLineWidth = nLineWidth;
    obj->nLineStyle = nLineStyle;
    obj->nLineJoinStyle = nLineJoinStyle;
    obj->nLineCapStyle = nLineCapStyle;

    obj->nFillWay = nFillWay;
    obj->nFillStyle = nFillStyle;
    obj->strFillColor = strFillColor;
    obj->nFillDirection = nFillDirection;
    obj->nFillPercentage = nFillPercentage;
    obj->bFrameSee = bFrameSee;
    obj->nTransparency = nTransparency;

    obj->fRotateAngle = fRotateAngle;
    obj->bHorizonTurn = bHorizonTurn;
    obj->bVerticalTurn = bVerticalTurn;
    obj->bVisible = bVisible;
    obj->bDeleted = bDeleted;
    obj->nStackOrder = nStackOrder;
    obj->nPattern = nPattern;
    obj->bModify = bModify;
}

void HBaseObj::clone(HBaseObj* ob)
{
    if(!ob) return;
    HBaseObj::copyTo(ob);
}

//设置属性值 By Name
void HBaseObj::setObjName(const QString strName)
{
   strObjName = strName;
}

QString HBaseObj::getObjName()
{
   return strObjName;
}

int HBaseObj::getObjID()
{
    return nObjectId;
}

void HBaseObj::setObjID(int nObjID)
{
    nObjectId = nObjID;
}

void HBaseObj::setOX(double x)
{
    originX = x;
}

void HBaseObj::setOY(double y)
{
    originY = y;
}

double HBaseObj::getOX()
{
    return originX;
}

double HBaseObj::getOY()
{
    return originY;
}

//形状类型
DRAWSHAPE HBaseObj::getShapeType()
{
    return (DRAWSHAPE)drawShape;
}

void HBaseObj::setShapeType(DRAWSHAPE t)
{
    drawShape = t;
}

//线条颜色
void HBaseObj::setLineColorName(QString clrName)
{
    strLineColor = clrName;
}

QString HBaseObj::getLineColorName()
{
   return strLineColor;
}

//线宽
void HBaseObj::setLineWidth(quint8 lineWidth)
{
    nLineWidth = lineWidth;
}

quint8 HBaseObj::getLineWidth()
{
    return nLineWidth;
}

void HBaseObj::setLineStyle(Qt::PenStyle style)
{
    nLineStyle = style;
}

Qt::PenStyle HBaseObj::getLineStyle()
{
    return (Qt::PenStyle)nLineStyle;
}

//线连接处形状
void HBaseObj::setLineJoinStyle(Qt::PenJoinStyle style)
{
    nLineJoinStyle = style;
}

Qt::PenJoinStyle HBaseObj::getLineJoinStyle()
{
    return (Qt::PenJoinStyle)nLineJoinStyle;
}

//线端形状
void HBaseObj::setLineCapStyle(Qt::PenCapStyle style)
{
    nLineCapStyle = style;
}

Qt::PenCapStyle HBaseObj::getLineCapStyle()
{
    return (Qt::PenCapStyle)nLineCapStyle;
}

//填充方式
void HBaseObj::setFillWay(quint8 fillWay)
{
    nFillWay = fillWay;
}

quint8 HBaseObj::getFillWay()
{
    return nFillWay;
}

//填充风格
void HBaseObj::setFillStyle(quint8 style)
{
    nFillStyle = style;
}

quint8 HBaseObj::getFillStyle()
{
    return nFillStyle;
}

//填充方向
void HBaseObj::setFillDirection(quint8 fillDirection)
{
    nFillDirection = fillDirection;
}

quint8 HBaseObj::getFillDirection()
{
    return nFillDirection;
}

//填充比例
void HBaseObj::setFillPercentage(quint8 fillPer)
{
    nFillPercentage = fillPer;
}

quint8 HBaseObj::getFillPercentage()
{
    return nFillPercentage;
}

//边框可见
void HBaseObj::setFrameSee(bool frameSee)
{
    bFrameSee = frameSee;
}

bool HBaseObj::getFrameSee()
{
    return bFrameSee;
}

//透明度
void HBaseObj::setTransparency(quint8 transparency)
{
    nTransparency = transparency;
}

quint8 HBaseObj::getTransparency()
{
    return nTransparency;
}


//填充色
void HBaseObj::setFillColorName(QString clrName)
{
    strFillColor = clrName;
}

QString HBaseObj::getFillColorName()
{
    return strFillColor;
}

//边框色
void HBaseObj::setBorderColor(const QColor* c)
{

}

QColor HBaseObj::getBorderColor()
{
    return QColor();
}

//文字色
void HBaseObj::setTextColor(const QColor* c)
{

}

QColor HBaseObj::getTextColor()
{
    return QColor();
}

//是否旋转
bool HBaseObj::isRotated(qint8 nFlag)
{
    return false;
}

//旋转角度
void HBaseObj::setRotateAngle(float rotate)
{
    fRotateAngle = rotate;
}

float HBaseObj::getRotateAngle()
{
    return fRotateAngle;
}

//增加一个角度
void HBaseObj::setRotateAdd(float fAngle)
{

}

//翻转
bool HBaseObj::isTurned(qint8 nFlag)
{
    return false;
}

//是否水平翻转
bool HBaseObj::isTurn(bool bHorizonTurn)
{
    return false;
}

void HBaseObj::setModify(bool modify)
{
    bModify = modify;
}

bool HBaseObj::getModify()
{
    return bModify;
}


//设置翻转
void HBaseObj::setTurn(bool bHorizon,bool bVertical)
{

}

//是否可见
void HBaseObj::setVisible(bool bvisible,int nFlag)
{
    bVisible = bvisible;
}

bool HBaseObj::isVisible()
{
    return bVisible;
}

//是否删除
void HBaseObj::setDeleted(bool bDel)
{
    bDeleted = bDel;
}

bool HBaseObj::isDeleted()
{
    return bDeleted;
}

//叠放次序
qint64 HBaseObj::getStackOrder()
{
    return nStackOrder;
}

void HBaseObj::setStackOrder(qint64 nStack)
{

}

bool HBaseObj::contains(int nPatternId)
{
    return nPattern == nPatternId;
}

void HBaseObj::moveBy(qreal dx, qreal dy)
{

}

void HBaseObj::paint(QPainter* painter)
{

}

//设置item对象
void HBaseObj::setIconGraphicsItem(HIconGraphicsItem* item)
{
    pIconGraphicsItem = item;
}

HIconGraphicsItem* HBaseObj::getIconGraphicsItem()
{
    return pIconGraphicsItem;
}

void HBaseObj::resize(double w,double h)
{

}

QRectF HBaseObj::boundingRect() const
{
    return QRectF();
}
bool HBaseObj::contains(const QPointF &point) const
{
    return false;
}

QPainterPath HBaseObj::shape() const
{
    return QPainterPath();

}


