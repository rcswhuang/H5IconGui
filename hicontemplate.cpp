#include "H5IconGui/hicontemplate.h"
#include <QFile>
#include <QTextStream>
#include <QDomProcessingInstruction>
#include <QTextCodec>
HIconTemplate::HIconTemplate(const QUuid& uuid,QObject* parent)
    :uUid(uuid)
{
    if(uUid.isNull())
    {
        uUid = QUuid::createUuid();
    }
    pIconSymbol = new HIconSymbol;
    sDefaultSize = QSizeF(50,50);
    nCatalogType = 0;
    nVersion = 1;
}

HIconTemplate::~HIconTemplate()
{
    if(pIconSymbol)
    {
        delete pIconSymbol;
        pIconSymbol = NULL;
    }
}

void HIconTemplate::clear()
{
    if(pIconSymbol)
    {
       pIconSymbol->clear();
    }
}

void HIconTemplate::copyTo(HIconTemplate *temp)
{
    temp->nVersion = nVersion;
    temp->strCatalogName = strCatalogName;//图元属性名称 遥测 遥信等
    temp->nCatalogType = nCatalogType;//图元类型 遥测 遥信等
    temp->sDefaultSize = sDefaultSize;
    temp->uUid = uUid;
    pIconSymbol->copyTo(temp->pIconSymbol);
}

void HIconTemplate::readXml(const QString &strXmlFile)
{
    QFile file(strXmlFile);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream dsm(&file);
    QDomDocument doc;
    dsm.setCodec("GB2312");
    QString errorString;
    int errorLine;
    int errorColumn;
    if(!doc.setContent(&file,false,&errorString,&errorLine,&errorColumn))
    {
        file.close();
        return;
    }
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return;
    readXml(&root);
    file.close();
}

void HIconTemplate::readXml(QDomElement* dom)
{
    if(!dom)
        return;
    strCatalogName = dom->attribute("TypeName");
    nCatalogType = dom->attribute("TypeId").toInt();
    uUid = QUuid(dom->attribute("UUID"));
    double w = dom->attribute("DefaultWidth").toDouble();
    double h = dom->attribute("DefaultHeight").toDouble();
    sDefaultSize = QSizeF(w,h);
    QDomElement symbolDom = dom->namedItem("IconSymbol").toElement();
    if(!symbolDom.isNull())
    {
        pIconSymbol->readXml(&symbolDom);
    }
}

void HIconTemplate::writeXml(const QString &strXmlFile)
{
    QFile file(strXmlFile);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QTextStream dsm(&file);
    QDomDocument doc;
    QTextCodec* c = QTextCodec::codecForLocale();
    QString strLocal = QString("version=\"1.0\" encoding=\"GB2312\"");
    QDomProcessingInstruction instruct = doc.createProcessingInstruction("xml",strLocal);
    doc.appendChild(instruct);
    QDomElement root = doc.createElement("IconTemplate");
    if(root.isNull())
        return;
    doc.appendChild(root);
    writeXml(&root);
    dsm.setCodec("GB2312");
    doc.save(dsm,4);
    file.close();
}

void HIconTemplate::writeXml(QDomElement *dom)
{
    if(!dom)
        return;

    dom->setAttribute("TypeName",strCatalogName);
    dom->setAttribute("TypeId",nCatalogType);
    dom->setAttribute("UUID",uUid.toString());
    dom->setAttribute("DefaultWidth",sDefaultSize.width());
    dom->setAttribute("DefaultHeight",sDefaultSize.height());

    QDomElement symbolDom = dom->ownerDocument().createElement("IconSymbol");
    dom->appendChild(symbolDom);
    if(pIconSymbol)
    {
        pIconSymbol->writeXml(&symbolDom);
    }
}

void HIconTemplate::setVersion(int version)
{
    nVersion = version;
}

int HIconTemplate::getVersion()
{
    return nVersion;
}

void HIconTemplate::setCatalogName(QString name)
{
    strCatalogName = name;
}

QString HIconTemplate::getCatalogName()
{
    return strCatalogName;
}

void HIconTemplate::setCatalogType(int type)
{
    nCatalogType = type;
}

int HIconTemplate::getCatalogType()
{
    return nCatalogType;
}

void HIconTemplate::setDefaultSize(const QSizeF& size)
{
    sDefaultSize = size;
}

QSizeF HIconTemplate::getDefaultSize()
{
    return sDefaultSize;
}

QUuid& HIconTemplate::getUuid()
{
    return uUid;
}

void HIconTemplate::setUuid(const QUuid &id)
{
    uUid = id;
}

HIconSymbol* HIconTemplate::getSymbol()
{
    if(pIconSymbol)
        return pIconSymbol;
    return NULL;
}

void HIconTemplate::setModify(bool modify)
{
    pIconSymbol->setModify(modify);
}

bool HIconTemplate::getModify()
{
    if(!pIconSymbol)
        return false;
    return pIconSymbol->getModify();
}


