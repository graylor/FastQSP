#ifndef QSPCALLBACK_H
#define QSPCALLBACK_H

#include "qsp_default.h"
#include "fastqspwindow.h"

#include <QInputDialog>

//extern FastQSPWindow *qspWin;

namespace QSPCallback
{
    void QSPCallback();
    void debug(const QSP_CHAR *str);
    QSP_BOOL isPlayingFile(const QSP_CHAR *file);
    void playFile(const QSP_CHAR *file, int volume);
    void closeFile(const QSP_CHAR *file);
    void showImage(const QSP_CHAR *file);
    void showWindow(int type, QSP_BOOL isShow);
    void deleteMenu();
    void addMenuItem(const QSP_CHAR *name, const QSP_CHAR *imgPath);
    void showMenu();
    void showMessage(const QSP_CHAR *str);
    void refreshInt(QSP_BOOL isRedraw);
    void setTimer(int msecs);
    void setInputStrText(const QSP_CHAR *text);
    void system(const QSP_CHAR *str);
    void openGameStatus(const QSP_CHAR *file);
    void saveGameStatus(const QSP_CHAR *file);
    void sleep(int msecs);
    int  getMsCount();
    void callInputBox(const QSP_CHAR *text, QSP_CHAR *buffer, int maxLen);
    void dummy();
}

#endif // QSPCALLBACK_H
