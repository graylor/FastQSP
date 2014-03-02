#ifndef QSPVIEW_H
#define QSPVIEW_H

#include "qsp_default.h"
#include "qsp_htmlbuilder.h"

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
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <QFontDatabase>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsWebView>
#include <QGraphicsView>

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
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject * obj, QEvent *e);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void openFileDialog();
    void about();
    void showHtml();
    void saveGame();
    void loadGame();
    void linkClicked(const QUrl & url);
    void toggleFullscreen();
    void updateView();

private:
    QMenu* gameMenu;
    QGraphicsWebView *webView;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QString gameDirectory;
    uint gameWidth;
    uint gameHeight;
    qreal aspectRatio;
    qreal scaleFactor;
    Phonon::MediaObject *media;
    Phonon::AudioOutput *audioOutput;
    void loadFonts();
    void loadPage();
};

#endif // QSPVIEW_H
