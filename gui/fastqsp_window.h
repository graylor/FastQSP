#ifndef FASTQSP_WINDOW_H
#define FASTQSP_WINDOW_H

#include "qsp_default.h"
#include "qsptextbox.h"

#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
//#include <QtWebKit/QtWebKit>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QByteArray>


//TODO: must be singleton
class FQWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FQWindow(QWidget *parent = 0);
    ~FQWindow();

public slots:
    void openFile();
    void getObject();
    void act1();
    void linkClick(const QUrl & url);

protected:
    void timerEvent(QTimerEvent *event);

private:
    QMenuBar* mnuBar;
    QPushButton* btn1;
    QPushButton* btn2;
    QString gameDirectory;
};

#endif // FASTQSP_WINDOW_H
