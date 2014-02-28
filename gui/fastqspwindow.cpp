#include "qsp_callback.h"
#include "fastqspwindow.h"


FastQSPWindow::FastQSPWindow(QWidget *parent) :
    QMainWindow(parent),
    gameWidth(800),
    gameHeight(600),
    scaleFactor(1),
    media(new Phonon::MediaObject(this))
{
    // Init audio
    audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory, this);
    Phonon::createPath(media, audioOutput);

    // Creating main view
    //mainView = new FastQSPView(this);
    //QGraphicsWidget
    mainView = new QWebView(this);
    mainView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    mainView->setContextMenuPolicy(Qt::NoContextMenu);
    mainView->installEventFilter(this);
    mainView->setRenderHints(
                QPainter::Antialiasing |
                QPainter::HighQualityAntialiasing |
                QPainter::NonCosmeticDefaultPen |
                QPainter::SmoothPixmapTransform |
                QPainter::TextAntialiasing);

    connect(mainView,
            SIGNAL(linkClicked(const QUrl&)),
            SLOT(linkClicked(const QUrl&)));
    setCentralWidget(mainView);
    //mainView->setLayout(new QGridLayout());

    gameWidth = 1280;
    gameHeight = 800;

    aspectRatio = qreal(gameWidth) / qreal(gameHeight);

    //setWindowFlags(Qt::Window);


    // TODO: That must be optional
    mainView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    mainView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    //mainView->show();

    // Creating menu
    QMenu* fileMenu = new QMenu("File");
    fileMenu->addAction("Open file",
                    this,
                    SLOT(openFileDialog()),
                    Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("Exit",
                    this,
                    SLOT(close()),
                    Qt::CTRL + Qt::Key_Q);
    menuBar()->addMenu(fileMenu);

    gameMenu = new QMenu("Game");
    gameMenu->addAction("Save",
                    this,
                    SLOT(saveGame()),
                    Qt::CTRL + Qt::Key_S);
    gameMenu->addAction("Load",
                    this,
                    SLOT(loadGame()),
                    Qt::CTRL + Qt::Key_L);
    menuBar()->addMenu(gameMenu);
    gameMenu->setDisabled(true);


    QMenu* debugMenu = new QMenu("Debug");
    debugMenu->addAction("Show html",
                    this,
                    SLOT(showHtml()));
    debugMenu->addAction("Show css",
                    this,
                    SLOT(showCss()));

    menuBar()->addMenu(debugMenu);

    // Initializing QSP
    QSPInit();
    QSPCallback::QSPCallback();
}

void FastQSPWindow::loadFonts()
{
    QDir fontsDir(gameDirectory + "fonts");
    fontsDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    fontsDir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList fontList = fontsDir.entryInfoList();
    for (int i = 0; i < fontList.size(); ++i) {
        QFileInfo fileInfo = fontList.at(i);
        QFontDatabase::addApplicationFont(fileInfo.absoluteFilePath());
    }
}

FastQSPWindow::~FastQSPWindow()
{
    QSPDeInit();
}

bool FastQSPWindow::eventFilter(QObject * obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent *)e)->button() != Qt::LeftButton)
            return true;
    }
    if(e->type() == QEvent::DragEnter ||
       e->type() == QEvent::DragMove ||
       e->type() == QEvent::DragLeave ||
       e->type() == QEvent::DragResponse ||
       e->type() == QEvent::Drop)
    {
        return true;
    }
    return false;
}

void FastQSPWindow::keyPressEvent(QKeyEvent *e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {

        if(isFullScreen())
        {
            menuBar()->show();
            showNormal();
            qDebug() << "normal mode";
        }
        else
        {
            menuBar()->hide();
            showFullScreen();
            qDebug() << "fullscreen";
        }
    }
}

void FastQSPWindow::openFileDialog()
{
    QFileDialog dlg;
    QString filename = dlg.getOpenFileName(
                this,
                "Open File",
                NULL,
                "QSP game (*.qsp)");
    openFile(filename);
}

void FastQSPWindow::saveGame()
{
    QFileDialog dlg;
    QString filename = dlg.getSaveFileName(
                this,
                "Save Game",
                gameDirectory,
                "QSP save-game (*.sav)");
    if(!filename.isEmpty())
        QSPSaveGame(filename.toStdWString().c_str(), true);
}

void FastQSPWindow::loadGame()
{
    QFileDialog dlg;
    QString filename = dlg.getOpenFileName(
                this,
                "Load Game",
                gameDirectory,
                "QSP save-game (*.sav)");
    if(!filename.isEmpty() &&
        QSPOpenSavedGame(filename.toStdWString().c_str(), true))
    {
        loadPage();
        //mainView->resize(gameWidth, gameHeight);
    }
}

void FastQSPWindow::showHtml()
{
    static QTextEdit *htmlText;
    if(!htmlText)
        htmlText = new QTextEdit();
    htmlText->setPlainText(mainView->page()->mainFrame()->toHtml());
    htmlText->show();
}

void FastQSPWindow::showCss()
{
    static QTextEdit *cssText;
    if(!cssText)
        cssText = new QTextEdit();
    int numVal;
    wchar_t *strVal;
    QSPGetVarValues(L"STYLESHEET", 0, &numVal, &strVal);
    cssText->setPlainText(QString::fromWCharArray(strVal));
    cssText->show();
}

void FastQSPWindow::linkClicked(const QUrl & url)
{
    if(url.toString().startsWith("exec:", Qt::CaseInsensitive))
    {
        QString execStr;
        execStr = url.toString().remove(0, 5).trimmed().replace("%22", "\"");
        QSPExecString(execStr.toStdWString().c_str(), true);
    }
    else
    if(url.toString().startsWith("obj:", Qt::CaseInsensitive))
    {
        bool ok = false;
        int number = 0;
        number = url.toString().remove(0, 4).trimmed().toInt(&ok);
        if(!ok)
        {
            qCritical() << "Unknown link format" << url.toString();
            return;
        }
        QSPSetSelObjectIndex(number, true);
    }
    else
    if(url.toString() == "msgclose")
    {
        builder.hideMessage();
    }
    else
    {
        bool ok = false;
        int number;
        number = url.toString().toInt(&ok);
        if(!ok)
        {
            qCritical() << "Unknown link format" << url.toString();
            return;
        }
        QSPSetSelActionIndex(number - 1, true);
        QSPExecuteSelActionCode(true);
    }
    loadPage();
    qDebug() << "Link clicked" << url.toString();
}

void FastQSPWindow::playAudio(QString filename, int vol)
{
    if(QFile(filename).exists())
    {
        qDebug() << "playing:" << filename << vol;
        audioOutput->setVolume(qreal(vol) / qreal(100));
        media->setCurrentSource(QUrl(filename));
        media->play();
    }
}

void FastQSPWindow::stopAudio()
{
    media->stop();
}

void FastQSPWindow::openFile(const QString &filename)
{
    if(!QSPLoadGameWorld(filename.toStdWString().c_str()))
        qCritical() << QString("Could not open file: ") << filename;
    if(QSPRestartGame(QSP_TRUE))
    {
        gameMenu->setEnabled(true);
        gameDirectory = QFileInfo(filename).absolutePath() + "/";
        builder.setGameDir(gameDirectory);
        loadFonts();
        loadPage();
    }
}

// That function is called by callback if isRefsresh == true
void FastQSPWindow::refreshView()
{
    qDebug() << "refreshView()";
}

void FastQSPWindow::relPathsToURLs(QString & stylesheet)
{
    QRegExp *re = new QRegExp(
                "background-image:(.*);",
                Qt::CaseInsensitive);
    re->setMinimal(true);
    int pos = 0;
    while((pos = re->indexIn(stylesheet, pos)) > 0)
    {
        QString url = re->cap(1);
        stylesheet = stylesheet.replace(pos, re->matchedLength(), "background-image:url('" + toUrlPath(url) + "');");
        pos += re->matchedLength();
    }
    delete re;
}

// Load page stylesheet
void FastQSPWindow::loadStyle()
{
    // Temp variables
    int numVal = 0;
    wchar_t *strVal = NULL;

    QString stylesheet;
    QSPGetVarValues(L"STYLESHEET", 0, &numVal, &strVal);
    stylesheet = QString::fromWCharArray(strVal);

    // NOTE: only background-image property supportedre
    relPathsToURLs(stylesheet);

    // HACK: Because AeroQSP ignore '%'
    // TODO: Add settings option
    // stylesheet.replace("%", "");

    mainView->page()->settings()->setUserStyleSheetUrl(QString("data:text/css;charset=utf-8;base64," + stylesheet.toUtf8().toBase64()));

    /*if(QSPGetVarValues(L"STYLESHEET", 0, &numVal, &strVal))
    {
        stylesheet = QString::fromWCharArray(strVal);




        pageCSS->write(stylesheet.toUtf8());
        pageCSS->close();
        //mainView->page()->settings()->setUserStyleSheetUrl("file:///" + pageCSS->fileName());
        mainView->page()->settings()->setUserStyleSheetUrl(QString("data:text/css;charset=utf-8;base64," + stylesheet.toUtf8().toBase64()));
        qDebug() << "css file is: file:///" + pageCSS->fileName();
    }*/
}

void FastQSPWindow::loadPage()
{
    mainView->setHtml(builder.getHTML());

    // Load external stylesheet
    // TODO: Must check that css unchanged. Maybe save stylesheet hash?
    loadStyle();
}

// TODO: maximize doesn't work properly
void FastQSPWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QSize newSize;
    newSize = mainView->size();

    if(newSize.isValid())
    {
        int viewWidth, viewHeight;
        viewWidth = newSize.width();
        viewHeight = viewWidth / aspectRatio;
        if(viewHeight > newSize.height())
        {
            viewHeight = newSize.height();
            viewWidth = viewHeight * aspectRatio;
            //if(isMaximized() || isFullScreen())
        }
        mainView->page()->setViewportSize(QSize(viewWidth, viewHeight));
        scaleFactor = qreal(viewWidth) / qreal(gameWidth);
        mainView->setZoomFactor(scaleFactor);
    }
}

QString FastQSPWindow::toUrlPath(QString &path)
{
    return "file:///" + gameDirectory + path.replace('\\', '/').trimmed();
}

void FastQSPWindow::timerEvent(QTimerEvent *event)
{
    QSPExecCounter(true);
}
