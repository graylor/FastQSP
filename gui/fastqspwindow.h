#ifndef QSPVIEW_H
#define QSPVIEW_H

#include "qsp_default.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>
#include <QWebView>
#include <QWebFrame>
#include <QString>
#include <QStringRef>
#include <QRegExp>
#include <QDebug>
#include <QGridLayout>
#include <QBoxLayout>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <QFontDatabase>

class FastQSPWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FastQSPWindow(QWidget *parent = 0);
    ~FastQSPWindow();
    void openFile(const QString &filename);
    void playAudio(QString filename, int vol);
    void stopAudio();
    void refreshView();
    void showMessage(const QString &text);
    void hideMessage();
signals:

public slots:

protected:
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject * obj, QEvent *e);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void openFileDialog();
    void showHtml();
    void showCss();
    void saveGame();
    void loadGame();
    void linkClicked(const QUrl & url);

private:
    QMenu* gameMenu;
    QWebView *mainView;
    QString gameDirectory;
    uint gameWidth;
    uint gameHeight;
    qreal scaleFactor;
    qreal aspectRatio;
    bool messageVisible;
    QString messageText;
    Phonon::MediaObject *media;
    Phonon::AudioOutput *audioOutput;
    void loadFonts();
    void loadPage();
    void loadStyle();
    void loadActions();
    void loadObjects(QString & objectsDesc, QString & objectsStyle);
    void loadMainHead(QString & mainHead, const QString & additionalStyle = "");
    void loadMainDesc(QString & mainDesc, const QString & additionalDesc = "");
    void loadMessage(QString & messageDesc, QString & messageStyle);
    void relPathsToURLs(QString & stylesheet);
    QString toUrlPath(QString &path);
};

#endif // QSPVIEW_H
