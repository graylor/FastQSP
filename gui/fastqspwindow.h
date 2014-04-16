#ifndef QSPVIEW_H
#define QSPVIEW_H

#include "qsp_default.h"
#include "qsp_htmlbuilder.h"

#include <qglobal.h>
#include <QApplication>
#include <QMainWindow>
#include <QResizeEvent>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>
#include <QWebView>
#include <QWebFrame>
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <QGridLayout>
#include <QBoxLayout>
#include <QFontDatabase>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsWebView>
#include <QGraphicsView>
#include <QShortcut>
#include <QDesktopServices>

#if QT_VERSION < 0x050000
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#else
#include <QMediaPlayer>
#endif


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
    QSP_HTMLBuilder builder;
signals:

public slots:

protected:
    void timerEvent(QTimerEvent * event);
    void resizeEvent(QResizeEvent * event);
    bool eventFilter(QObject * obj, QEvent *e);
    void closeEvent(QCloseEvent * event);

private slots:
    void openFileDialog();
    void about();
    void showHtml();
    void saveGameDialog();
    void autosave();
    void saveGame(const QString &filename);
    void loadGameDialog();
    void loadGame(const QString &filename);
    void restartGame();
    void linkClicked(const QUrl & url);
    void toggleFullscreen();
    void toggleUpdate();

private:
    QMenu* gameMenu;
    QAction *autosaveAction;
    QGraphicsWebView *webView;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QString gameDirectory;
    uint gameWidth;
    uint gameHeight;
    qreal aspectRatio;
    qreal scaleFactor;
    bool gameIsOpen;
    #if QT_VERSION < 0x050000
    Phonon::MediaObject *media;
    Phonon::AudioOutput *audioOutput;
    #else
    QMediaPlayer *player;
    #endif
    QDir saveDir;
    void loadFonts();
    void loadPage();
};

#endif // QSPVIEW_H
