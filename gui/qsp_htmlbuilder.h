#ifndef QSP_HTMLBUILDER_H
#define QSP_HTMLBUILDER_H

#include "qsp_default.h"

#include <QStringBuilder>
#include <QString>
#include <QHash>
#include <QQueue>
#include <QRegExp>
#include <QDebug>

class QSP_HTMLBuilder
{
public:
    QSP_HTMLBuilder();
    QString getHTML();
    void setGameDir(const QString);
    void updateBaseStyle();
    void updateScripts();
    void updateMain();
    void updateObjects();
    void updateMessage();
    void updateActions();
    void updateStyle();
    void showMessage(const QString text);
    void hideMessage();
    void clear();

private:
    QString directory;
    QString baseStyle;
    QString stylesheet;
    QString scripts;
    QString main;
    QString objects;
    QString message;
    QQueue<QString> messageTexts;
    QString actions;
    QHash<QString, int> defaultIntegerValues;
    //Cached values
    QString mainDesc;

    const QString getStringVariable(const wchar_t *name) const;
    const QString getIntegerVariable(const wchar_t *name) const;
    void updateMainDesc();
    static QString removeQuotes(const QString &);
};

#endif // QSP_HTMLBUILDER_H
