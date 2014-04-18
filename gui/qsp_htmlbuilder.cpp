#include "qsp_htmlbuilder.h"

QSP_HTMLBuilder::QSP_HTMLBuilder()
{
    defaultIntegerValues["FSIZE"] = 18;

    defaultIntegerValues["MAINDESC_X"] = 4;
    defaultIntegerValues["MAINDESC_Y"] = 4;
    defaultIntegerValues["MAINDESC_W"] = 589;
    defaultIntegerValues["MAINDESC_H"] = 389;

    defaultIntegerValues["OBJECTS_X"] = 596;
    defaultIntegerValues["OBJECTS_Y"] = 4;
    defaultIntegerValues["OBJECTS_W"] = 200;
    defaultIntegerValues["OBJECTS_H"] = 389;

    defaultIntegerValues["MSG_X"] = 200;
    defaultIntegerValues["MSG_Y"] = 165;
    defaultIntegerValues["MSG_TEXT_X"] = 4;
    defaultIntegerValues["MSG_TEXT_Y"] = 4;
    defaultIntegerValues["MSG_TEXT_W"] = 392;
    defaultIntegerValues["MSG_TEXT_H"] = 231;
    defaultIntegerValues["MSG_OK_X"] = 186;
    defaultIntegerValues["MSG_OK_Y"] = 239;
}

void QSP_HTMLBuilder::setGameDir(const QString dir)
{
    directory = dir;
}

QString QSP_HTMLBuilder::getHTML()
{
    updateBaseStyle();
    updateStyle();
    updateScripts();
    updateMain();
    updateMessage();
    updateObjects();
    updateActions();
    return QLatin1String("<head><script type='text/javascript'>") %
            scripts %
            QLatin1String("</script>") %
            stylesheet %
            QLatin1String("<style>") %
            baseStyle %
            QLatin1String("</style></head>") %
            QLatin1String("<body onmousedown='return false'>") %
            main %
            objects %
            (messageTexts.isEmpty()?"":message.replace(QLatin1String("%TEXT%"), messageTexts.first())) %
            actions %
            QLatin1String("</body>") ;
}

void QSP_HTMLBuilder::updateBaseStyle()
{
    baseStyle =
            QLatin1String("*{padding:0;margin:0;} a{text-decoration:none;outline:0;") %
            (messageTexts.empty()?QLatin1String(""):QLatin1String("pointer-events: none;cursor: default;")) %
            ("} body{white-space: pre-wrap;-webkit-user-select:none;cursor:default;background-size:cover;background: url('") %
            getStringVariable(L"BACKIMAGE") %
            QLatin1String("')no-repeat;font-family:") %
            getStringVariable(L"FNAME") %
            QLatin1String(";font-size:") %
            getIntegerVariable(L"FSIZE") %
            QLatin1String(";}div{margin: 0 auto; display: table;}");
}

void QSP_HTMLBuilder::updateScripts()
{
    if(scripts.isEmpty())
    {
        scripts = QLatin1String("window.onload=function(){var msg = document.getElementById('__message__');var bgImage = new Image();bgImage.src = msg.style.backgroundImage.replace(/url\\((['\\\"])?(.*?)\\1\\)/gi, '$2').split(',')[0]; bgImage.onload=function(){msg.style.width = bgImage.width;msg.style.height = bgImage.height;}};");
        // onDomReady from https://github.com/tubalmartin/ondomready
        scripts += "\n<!--onDomReady from https://github.com/tubalmartin/ondomready-->\n";
        scripts += ";(function(a){\"function\"==typeof define&&define.amd?define(a):window['onDomReady']=a()}(function(){\"use strict\";function s(a){if(!q){if(!b.body)return v(s);for(q=!0;a=r.shift();)v(a)}}function t(a){(o||a.type===d||b[h]===g)&&(u(),s())}function u(){o?(b[n](l,t,e),a[n](d,t,e)):(b[j](m,t),a[j](f,t))}function v(a,b){setTimeout(a,+b>=0?b:1)}function x(a){q?v(a):r.push(a)}var a=window,b=a.document,c=b.documentElement,d=\"load\",e=!1,f=\"on\"+d,g=\"complete\",h=\"readyState\",i=\"attachEvent\",j=\"detachEvent\",k=\"addEventListener\",l=\"DOMContentLoaded\",m=\"onreadystatechange\",n=\"removeEventListener\",o=k in b,p=e,q=e,r=[];if(b[h]===g)v(s);else if(o)b[k](l,t,e),a[k](d,t,e);else{b[i](m,t),a[i](f,t);try{p=null==a.frameElement&&c}catch(w){}p&&p.doScroll&&function y(){if(!q){try{p.doScroll(\"left\")}catch(a){return v(y,50)}u(),s()}}()}return x.version=\"1.4.0\",x.isReady=function(){return q},x}));";
        scripts += "onDomReady(function() {images = document.getElementsByTagName('img');for(i=0; images[i]; i++){images[i].onerror = function() {this.onerror = null;this.style.visibility = 'hidden';}}});";
    }
}

void QSP_HTMLBuilder::updateMain()
{
    updateMainDesc();
    main = QLatin1Literal("<div style='position:fixed;top:") %
            getIntegerVariable(L"MAINDESC_Y") %
            QLatin1Literal(";left:") %
            getIntegerVariable(L"MAINDESC_X") %
            QLatin1Literal(";width:") %
            getIntegerVariable(L"MAINDESC_W") %
            QLatin1Literal(";height:") %
            getIntegerVariable(L"MAINDESC_H") %
            QLatin1Literal(";'>") %
            mainDesc %
            QLatin1Literal("</div>");
}

void QSP_HTMLBuilder::updateMessage()
{
    QString vis = QLatin1String("visibility:") +
            (messageTexts.empty()?QLatin1String("hidden"):QLatin1String("visible"));
    message = QLatin1String("<div id='__message__' style='") %
            vis %
            QLatin1String(";background-image:url(\"") %
            getStringVariable(L"MSG_BACKIMAGE") %
            QLatin1String("\");position:fixed;top:") %
            getIntegerVariable(L"MSG_Y") %
            QLatin1String(";left:") %
            getIntegerVariable(L"MSG_X") %
            QLatin1String(";'><div style='white-space:pre-wrap;position:absolute;top:") %
            getIntegerVariable(L"MSG_TEXT_Y") %
            QLatin1String(";left:") %
            getIntegerVariable(L"MSG_TEXT_X") %
            QLatin1String(";width:") %
            getIntegerVariable(L"MSG_TEXT_W") %
            QLatin1String(";height:") %
            getIntegerVariable(L"MSG_TEXT_H") %
            QLatin1String(";'>") %
            getStringVariable(L"MSG_FORMAT") %
            QLatin1String("</div><a href='msgclose' style='pointer-events:auto;cursor:pointer;'><img src='") %
            getStringVariable(L"MSG_OK_IMAGE") %
            QLatin1String("' style='position:absolute;top:") %
            getIntegerVariable(L"MSG_OK_Y") %
            QLatin1String(";left:") %
            getIntegerVariable(L"MSG_OK_X") %
            QLatin1String(";'></a>");
}

void QSP_HTMLBuilder::updateObjects()
{
    if(!QSPIsObjectsChanged())
        return;
    objects = QLatin1String("<div style='top:") %
            getIntegerVariable(L"OBJECTS_Y") %
            QLatin1String(";left:") %
            getIntegerVariable(L"OBJECTS_X") %
            QLatin1String(";width:") %
            getIntegerVariable(L"OBJECTS_W") %
            QLatin1String(";height:") %
            getIntegerVariable(L"OBJECTS_H") %
            QLatin1String(";position:fixed;'>");

    int numberOfObjects = 0;
    numberOfObjects = QSPGetObjectsCount();
    for(int ind = 0; ind < numberOfObjects; ++ind)
    {
        // Tmp vars
        wchar_t *imgPath = NULL;
        wchar_t *desc = NULL;

        // TODO: imgPath stay unused right now
        QSPGetObjectData(ind, &imgPath, &desc);
        objects =
                objects %
                QLatin1String("<a href=\"obj:") %
                QString::number(ind) %
                QLatin1String("\">") %
                QString::fromWCharArray(desc)
                .replace("content", QLatin1String("file:///") %
                         directory %
                         QLatin1String("content")) %
                QLatin1String("</a><br />\n");
    }
    objects = objects + QLatin1String("</div>");
}

//Do nothing, actions probably empty
void QSP_HTMLBuilder::updateActions()
{
}

const QString QSP_HTMLBuilder::getStringVariable(const wchar_t *name) const
{
    // Temp variables
    int numVal = 0;
    wchar_t *strVal = NULL;

    QSPGetVarValues(name, 0, &numVal, &strVal);
    QString str = QString::fromWCharArray(strVal);
    // TODO: TEMP: add search in list of posible path variables
    if(str.startsWith("content"))
    {
        return "file:///" + directory + str.replace('\\', '/').trimmed();
    }
    return str;
}

const QString QSP_HTMLBuilder::getIntegerVariable(const wchar_t *name) const
{
    // Temp variables
    int numVal = 0;
    wchar_t *strVal = NULL;

    if(QSPGetVarValues(name, 0, &numVal, &strVal))
        return QString::number(numVal);
    else
        return QString::number(defaultIntegerValues[QString::fromWCharArray(name)]);
}

void QSP_HTMLBuilder::updateStyle()
{
    stylesheet = getStringVariable(L"STYLESHEET");
    QRegExp *re = new QRegExp("background-image:(.*);",
                              Qt::CaseInsensitive);
    QRegExp *validUrl = new QRegExp("url\\(.*\\)",
                                    Qt::CaseInsensitive);
    re->setMinimal(true);
    int pos = 0;
    while((pos = re->indexIn(stylesheet, pos)) > 0)
    {
        QString url = re->cap(1).replace('\\', '/').replace("content",
                                                            QLatin1String("file:///") %
                                                            directory %
                                                            QLatin1String("content"));
        if(!validUrl->exactMatch(url.trimmed()))
            stylesheet = stylesheet.replace(pos, re->matchedLength(),
                                            QLatin1String("background-image:url('") %
                                            url % "');");
        else
            stylesheet = stylesheet.replace(pos, re->matchedLength(),
                                            QLatin1String("background-image:") %
                                            url % ";");
        pos += re->matchedLength();
    }
    delete validUrl;
    delete re;

    stylesheet = QLatin1String("<link rel='stylesheet' type='text/css' href='data:text/css;charset=utf-8;base64,") +
            stylesheet.toUtf8().toBase64() %
            QLatin1String("'>");
}

void QSP_HTMLBuilder::updateMainDesc()
{
    if(QSPIsMainDescChanged())
        mainDesc = QString::fromWCharArray(QSPGetMainDesc())
                .replace("\r\n", "<br>")
                .replace("content", QLatin1String("file:///") %
                         directory %
                         QLatin1String("content"));
}

void QSP_HTMLBuilder::showMessage(const QString text)
{
    messageTexts.enqueue(text);
}

void QSP_HTMLBuilder::hideMessage()
{
    messageTexts.dequeue();
}
