#include "htext.h"
#include "hicontextitem.h"
///////////////////////////////////////////////////////////////////////
HText::HText()
{
    strTextContent = "icon text";

    textColorName = "#00FF00";//线条颜色
    textFontName = QStringLiteral("微软雅黑");
    layout = LAYOUT_TEXT_NULL;
    horizontalAlign = Qt::AlignHCenter;
    verticalAlign = Qt::AlignVCenter;
    pointSize = 10;//字体大小
    weight = QFont::Normal;//粗体
    italic = false;//斜体
    bFrameSee = false;//???
    strPrefix = "";//前缀
    strSuffix = "";//后缀
    btFormat = TEXT_TWO_POINT;//格式
}

HText::~HText()
{

}

//二进制读写
void HText::readData(QDataStream* data)
{
    if(!data) return;
    HRectangle::readData(data);
    QString s;
    *data>>s;
    strTextContent = s;
    *data>>s;
    textColorName = s;
    *data>>s;
    textFontName = s;
    quint8 n8;
    *data>>n8;
    layout = n8;
    int n;
    *data>>n;
    horizontalAlign = n;
    *data>>n;
    verticalAlign = n;
    *data>>n;
    pointSize = n;
    *data>>n;
    weight = n;
    *data>>n;
    italic = n;
    *data>>s;
    strPrefix = s;
    *data>>s;
    strSuffix = s;
    uchar bt;
    *data>>bt;
    btFormat = bt;

}

void HText::writeData(QDataStream* data)
{
    if(!data) return;
    HRectangle::writeData(data);
    *data<<(QString)strTextContent;
    *data<<(QString)textColorName;
    *data<<(QString)textFontName;
    *data<<(quint8)layout;
    *data<<(int)horizontalAlign;
    *data<<(int)verticalAlign;
    *data<<(int)pointSize;
    *data<<(int)weight;
    *data<<(int)italic;
    *data<<(QString)strPrefix;
    *data<<(QString)strSuffix;
    *data<<(uchar)btFormat;
}

//xml文件读写
void HText::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectangle::readXml(dom);
    strTextContent = dom->attribute("textContent");
    textColorName = dom->attribute("textColorName");
    textFontName = dom->attribute("textFontName");
    layout = dom->attribute("layout").toInt();
    horizontalAlign = dom->attribute("horizontalAlign").toInt();
    verticalAlign = dom->attribute("verticalAlign").toInt();
    pointSize = dom->attribute("pointSize").toInt();
    weight = dom->attribute("weight").toInt();
    italic = dom->attribute("italic").toInt();
    strPrefix = dom->attribute("prefix");
    strSuffix = dom->attribute("suffix");
    btFormat = dom->attribute("format").toUInt();

}

void HText::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectangle::writeXml(dom);
    dom->setAttribute("textContent",strTextContent);
    dom->setAttribute("textColorName",textColorName);
    dom->setAttribute("textFontName",textFontName);
    dom->setAttribute("layout",layout);
    dom->setAttribute("horizontalAlign",horizontalAlign);
    dom->setAttribute("verticalAlign",verticalAlign);
    dom->setAttribute("pointSize",pointSize);
    dom->setAttribute("weight",weight);
    dom->setAttribute("italic",italic);
    dom->setAttribute("prefix",strPrefix);
    dom->setAttribute("suffix",strSuffix);
    dom->setAttribute("format",btFormat);
}

QString HText::TagName()
{
    return "Text";
}

//拷贝克隆
void HText::copyTo(HBaseObj* obj)
{
    HText* ob = (HText*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->strTextContent = strTextContent;
    ob->textColorName = textColorName;
    ob->textFontName = textFontName;
    ob->layout = layout;
    ob->horizontalAlign = horizontalAlign;
    ob->verticalAlign = verticalAlign;
    ob->pointSize = pointSize;
    ob->weight = weight;
    ob->italic = italic;
    ob->strPrefix = strPrefix;
    ob->strSuffix = strSuffix;
    ob->btFormat = btFormat;
}

DRAWSHAPE HText::getShapeType()
{
    return enumText;
}

void HText::setTextColorName(QString strClrName)
{
    textColorName = strClrName;
}

QString HText::getTextColorName()
{
    return textColorName;
}

void HText::setLayout(ushort layout)
{
    this->layout = layout;
}

ushort HText::getLayout()
{
    return layout;
}

void HText::setHorizontalAlign(int hAlign)
{
    horizontalAlign = hAlign;
}

int HText::getHorizontalAlign()
{
    return horizontalAlign;
}

void HText::setVerticalAlign(int vAlign)
{
    verticalAlign = vAlign;
}

int HText::getVerticalAlign()
{
    return verticalAlign;
}

void HText::setTextFontName(QString strFontName)
{
    textFontName = strFontName;
}

QString HText::getTextFontName()
{
    return textFontName;
}

void HText::setPointSize(int ptSize)
{
    pointSize = ptSize;
}

int HText::getPointSize()
{
    return pointSize;
}

void HText::setWeight(int weight)
{
    this->weight = weight;
}

int HText::getWeigth()
{
    return weight;
}

void HText::setItalic(bool bitalic)
{
    italic = bitalic;
}

bool HText::getItalic()
{
    return italic;
}

void HText::setTextContent(QString strContent)
{
    strTextContent = strContent;
}

QString HText::getTextContent()
{
    return strTextContent;
}


void HText::setTopLeftPoint(QPointF topLeft)
{
    this->topLeft = topLeft;
}

QPointF HText::getTopLeftPoint()
{
    return topLeft;
}

void HText::setRectWidth(double width)
{
    rectWidth = width;
}

double HText::getRectWidth()
{
    return rectWidth;
}

void HText::setRectHeight(double height)
{
    rectHeight = height;
}

double HText::getRectHeight()
{
    return rectHeight;
}

void HText::setTextFormat(uchar btType)
{
    btFormat = btType;
}

uchar HText::getTextFormat()
{
    return btFormat;
}

void HText::setTextPrefix(const QString& strPrefix)
{
    this->strPrefix = strPrefix;
}

QString HText::getTextPrefix()
{
    return this->strPrefix;
}

void HText::setTextSuffix(const QString& strSuffix)
{
    this->strSuffix = strSuffix;
}

QString HText::getTextSuffix()
{
    return this->strSuffix;
}

/*
QRectF HText::boundingRect() const
{
    return shape().controlPointRect();
}

bool HText::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HText::shape() const
{
    QPainterPath path;
    QRectF boundingRect = QRectF(topLeft,QSizeF(rectWidth,rectHeight)).adjusted(-5,-5,5,5);
    path.addRect(boundingRect);
    return path;
}*/

void HText::paint(QPainter* painter)
{
    //if(!painter) return;
    HIconTextItem* pItem = qgraphicsitem_cast<HIconTextItem*>(getIconGraphicsItem());

    ushort nLayout = getLayout();
    QString strTextContent = getTextContent();
    QString strSuffix = getTextSuffix();
    QString strPrefix = getTextPrefix();
    qreal fRotateAngle = getRotateAngle();
    bool bRound = getRound();
    int nXAxis = getXAxis();
    int nYAxis = getYAxis();

    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QRectF mainRectF = rect;
    QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        pItem->setTransformOriginPoint(centerPoint);
        QTransform transform;
        transform.rotate(fRotateAngle);
        pItem->setTransform(transform);
    }
    else
    {
        painter->rotate(fRotateAngle);
    }

    setPainter(painter,rect);
    QPainterPath path = getPath();
    /*if(!bRound)
    {
        path.addRect(mainRectF);
    }
    else
    {
        path.addRoundedRect(mainRectF,nXAxis,nYAxis);
    }*/
    painter->drawPath(path);

    //设置字体部分
    QString strFontName = getTextFontName();
    int pointSize = getPointSize();
    int weight = getWeigth();
    bool italic = getItalic();
    QFont font(strFontName,pointSize,weight,italic);

    QPen textPen = QPen(QColor(getTextColorName()));
    painter->setPen(textPen);
    painter->setFont(font);

    int nAlign = getHorizontalAlign()|getVerticalAlign();
    QFontMetricsF fm(font);
    if(nLayout == LAYOUT_TEXT_FULL)
    {
        double fw = fm.width(strTextContent);
        double fh = fm.height();
        fh = mainRectF.height()/fh;
        fw = (mainRectF.width()+fh)/fw;
        painter->scale(fw,fh);
        mainRectF = QRectF(mainRectF.x()/fw,mainRectF.y()/fh,mainRectF.width()/fw,mainRectF.height()/fh);
        nAlign = Qt::AlignCenter | Qt::TextSingleLine;
    }
    painter->drawText(mainRectF,nAlign,strPrefix+strTextContent+strSuffix);
    painter->restore();


    if(pItem && pItem->isSelected())
    {
        painter->save();
        QPen pen1 = QPen(Qt::red);
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
        painter->restore();
    }
}
