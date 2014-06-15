#include "qsp_callback.h"
#ifdef __GNUC__
#include <unistd.h>
#elif _MSC_VER
#include <windows.h>
#endif

void QSPCallback::QSPCallback()
{
    qDebug() << QSP_CALL_SHOWMSGSTR;
    QSPSetCallBack(
                QSP_CALL_DEBUG,
                (QSP_CALLBACK)&QSPCallback::debug);
    QSPSetCallBack(
                QSP_CALL_ISPLAYINGFILE,
                (QSP_CALLBACK)&QSPCallback::isPlayingFile);
    QSPSetCallBack(
                QSP_CALL_PLAYFILE,
                (QSP_CALLBACK)&QSPCallback::playFile);
    QSPSetCallBack(
                QSP_CALL_CLOSEFILE,
                (QSP_CALLBACK)&QSPCallback::closeFile);
    QSPSetCallBack(
                QSP_CALL_SHOWIMAGE,
                (QSP_CALLBACK)&QSPCallback::showImage);
    QSPSetCallBack(
                QSP_CALL_SHOWWINDOW,
                (QSP_CALLBACK)&QSPCallback::showWindow);
    QSPSetCallBack(
                QSP_CALL_DELETEMENU,
                (QSP_CALLBACK)&QSPCallback::deleteMenu);
    QSPSetCallBack(
                QSP_CALL_ADDMENUITEM,
                (QSP_CALLBACK)&QSPCallback::addMenuItem);
    QSPSetCallBack(
                QSP_CALL_SHOWMENU,
                (QSP_CALLBACK)&QSPCallback::showMenu);
    QSPSetCallBack(
                QSP_CALL_SHOWMSGSTR,
                (QSP_CALLBACK)&QSPCallback::showMessage);
    QSPSetCallBack(
                QSP_CALL_REFRESHINT,
                (QSP_CALLBACK)&QSPCallback::refreshInt);
    QSPSetCallBack(
                QSP_CALL_SETTIMER,
                (QSP_CALLBACK)&QSPCallback::setTimer);
    QSPSetCallBack(
                QSP_CALL_SETINPUTSTRTEXT,
                (QSP_CALLBACK)&QSPCallback::setInputStrText);
    QSPSetCallBack(
                QSP_CALL_SYSTEM,
                (QSP_CALLBACK)&QSPCallback::system);
    QSPSetCallBack(
                QSP_CALL_OPENGAMESTATUS,
                (QSP_CALLBACK)&QSPCallback::openGameStatus);
    QSPSetCallBack(
                QSP_CALL_SAVEGAMESTATUS,
                (QSP_CALLBACK)&QSPCallback::saveGameStatus);
    QSPSetCallBack(
                QSP_CALL_SLEEP,
                (QSP_CALLBACK)&QSPCallback::sleep);
    QSPSetCallBack(
                QSP_CALL_GETMSCOUNT,
                (QSP_CALLBACK)&QSPCallback::getMsCount);
    QSPSetCallBack(
                QSP_CALL_INPUTBOX,
                (QSP_CALLBACK)&QSPCallback::callInputBox);
    QSPSetCallBack(
                QSP_CALL_DUMMY,
                (QSP_CALLBACK)&QSPCallback::dummy);
}

void QSPCallback::debug(const QSP_CHAR *str)
{
    qDebug() << "debug(), str: " << QString::fromWCharArray(str);
}

QSP_BOOL QSPCallback::isPlayingFile(const QSP_CHAR *file)
{
    qDebug() << "isPlayingFile(), file: " << QString::fromWCharArray(file);
    return true;
}

void QSPCallback::playFile(const QSP_CHAR *file, int volume)
{
    qspWin->playAudio(QString::fromWCharArray(file), volume);
    qDebug() << "playFile()"
             << ", file: "      << QString::fromWCharArray(file)
             << ", volume: "    << volume;
}

void QSPCallback::closeFile(const QSP_CHAR *file)
{
    qspWin->stopAudio();
    qDebug() << "closeFile(), file: " << QString::fromWCharArray(file);
}

void QSPCallback::showImage(const QSP_CHAR *file)
{
    qDebug() << "showImage(), file: " << QString::fromWCharArray(file);
}

void QSPCallback::showWindow(int type, QSP_BOOL isShow)
{
    qDebug() << "showWindow()"
             << ", type: "      << type
             << ", isShow: "    << isShow;
}

void QSPCallback::deleteMenu()
{
    qDebug() << "deleteMenu()";
}

void QSPCallback::addMenuItem(const QSP_CHAR *name, const QSP_CHAR *imgPath)
{
    qDebug() << "addMenuItem()"
             << ", name: "      << QString::fromWCharArray(name)
             << ", imgPath: "   << QString::fromWCharArray(imgPath);
}

void QSPCallback::showMenu()
{
    qDebug() << "showMenu()";
}

void QSPCallback::showMessage(const QSP_CHAR *str)
{
    qDebug() << "showMessage(), str: " << QString::fromWCharArray(str);
    qspWin->builder.showMessage(QString::fromWCharArray(str));
}

void QSPCallback::refreshInt(QSP_BOOL isRedraw)
{
    //qDebug() << "refreshInt(), isRedraw: " << isRedraw;
    if(isRedraw)
        qspWin->refreshView();
}

void QSPCallback::setTimer(int msec)
{
    qDebug() << "setTimer(), msec: " << msec;
    qspWin->startTimer(msec);
}

void QSPCallback::setInputStrText(const QSP_CHAR *text)
{
    qDebug() << "setInputStrText(), text: " << QString::fromWCharArray(text);
}

void QSPCallback::system(const QSP_CHAR *str)
{
    qDebug() << "system(), str: " << QString::fromWCharArray(str);
}

void QSPCallback::openGameStatus(const QSP_CHAR *file)
{
    qDebug() << "openGameStatus(), str: " << QString::fromWCharArray(file);
    QString filename(QString::fromWCharArray(file));
    if(filename.isEmpty())
        qspWin->openFileDialog();
    else
        qspWin->openFile(filename);
}

void QSPCallback::saveGameStatus(const QSP_CHAR *file)
{
    qDebug() << "saveGameStatus(), str: " << QString::fromWCharArray(file);
    QString filename(QString::fromWCharArray(file));
    if(filename.isEmpty())
        qspWin->saveGameDialog();
    else
        qspWin->saveGame(filename);
}

void QSPCallback::sleep(int msecs)
{
    qDebug() << "sleep(), msecs: " << msecs;
    #ifdef _MSC_VER
    Sleep(msecs);
    #elif __GNUC__
    usleep(1000 * msecs);
    #endif

}

int QSPCallback::getMsCount()
{
    qDebug() << "getMsCount()" << qspWin->getTimeFromStart();
    return qspWin->getTimeFromStart();
}

void QSPCallback::callInputBox(const QSP_CHAR *text, QSP_CHAR *buffer, int maxLen)
{
    qDebug() << "callInputBox()"
             << ", text: "   << QString::fromWCharArray(text)
             << ", buffer: " << QString::fromWCharArray(buffer)
             << ", maxLen: " << maxLen;
    QString input;
    input = QInputDialog::getText(qspWin,
                                  "",
                                  QString::fromWCharArray(text));
    input.truncate(maxLen);
    wcscpy(buffer, input.toStdWString().c_str());
}

void QSPCallback::dummy()
{
    qDebug() << "dummy()";
}
