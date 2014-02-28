#ifndef QSP_HTMLBUILDER_H
#define QSP_HTMLBUILDER_H

#include "qsp_default.h"

#include <QStringBuilder>
#include <QString>
#include <QHash>

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
    void showMessage();
    void hideMessage();

private:
    QString directory;
    QString baseStyle;
    QString scripts;
    QString main;
    QString objects;
    QString message;
    bool messageVisible;
    QString actions;
    QHash<QString, int> defaultIntegerValues;

    const QString getStringVariable(const wchar_t *name) const;
    const QString getIntegerVariable(const wchar_t *name) const;
    const QString getMainDesc() const;
};

#endif // QSP_HTMLBUILDER_H
