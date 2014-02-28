#include "qsp_callback.h"
#include "fastqspwindow.h"


FastQSPWindow::FastQSPWindow(QWidget *parent) :
    QMainWindow(parent),
    gameWidth(800),
    gameHeight(600),
    scaleFactor(1),
    messageVisible(false),
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
        hideMessage();
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

void FastQSPWindow::loadObjects(QString & objectsDesc, QString & objectsStyle)
{
    // Temp variables
    int numVal = 0;
    wchar_t *strVal = NULL;

    // Generate style
    objectsStyle = "#objects{";
    // Get object list size and position
    int left = 0;
    int top = 0;
    int width = 0;
    int height = 0;
    if(QSPGetVarValues(L"OBJECTS_X", 0, &numVal, &strVal))
        left = numVal;
    else
        left = 596;
    if(QSPGetVarValues(L"OBJECTS_Y", 0, &numVal, &strVal))
        top = numVal;
    else
        top = 4;
    if(QSPGetVarValues(L"OBJECTS_W", 0, &numVal, &strVal))
        width = numVal;
    else
        width = 200;
    if(QSPGetVarValues(L"OBJECTS_H", 0, &numVal, &strVal))
        height = numVal;
    else
        height = 389;
    objectsStyle += "width:" +
                QString::number(width) +
                ";\nheight:" +
                QString::number(height) +
                ";\ntop:" +
                QString::number(top) +
                ";\nleft:" +
                QString::number(left) +
                ";\nposition:fixed;}\n";

    // Generate description
    objectsDesc = "\n<div id = \"objects\">\n";
    uint numberOfObjects = 0;
    numberOfObjects = QSPGetObjectsCount();
    for(uint ind = 0; ind < numberOfObjects; ++ind)
    {
        // Tmp vars
        wchar_t *imgPath = NULL;
        wchar_t *desc = NULL;

        // TODO: imgPath stay unused right now
        QSPGetObjectData(ind, &imgPath, &desc);
        objectsDesc +=
                "<a href=\"obj:" +
                QString::number(ind) +
                "\">" +
                QString::fromWCharArray(desc) +
                "</a><br />\n";
    }
    objectsDesc += "\n</div>\n";
}

void FastQSPWindow::loadMainHead(QString & mainHead, const QString & additionalStyle)
{
    // Temp variables
    int numVal = 0;
    wchar_t *strVal = NULL;

    // Get background image
    QString backgroungPath;
    if(QSPGetVarValues(L"BACKIMAGE", 0, &numVal, &strVal))
    {
        backgroungPath = QString::fromWCharArray(strVal);
        backgroungPath = toUrlPath(backgroungPath);
    }

    // Get width and height
    uint width = 0;
    uint height = 0;
    if(QSPGetVarValues(L"MAINDESC_W", 0, &numVal, &strVal))
        width = numVal;
    else
        width = 589;
    if(QSPGetVarValues(L"MAINDESC_H", 0, &numVal, &strVal))
        height = numVal;
    else
        height = 389;

    // Get left and top
    uint left = 0;
    uint top = 0;
    if(QSPGetVarValues(L"MAINDESC_X", 0, &numVal, &strVal))
        left = numVal;
    else
        left = 4;
    if(QSPGetVarValues(L"MAINDESC_Y", 0, &numVal, &strVal))
        top = numVal;
    else
        top = 4;

    // Get font size and name
    uint fontsize = 0;
    QString fontName;
    if(QSPGetVarValues(L"FSIZE", 0, &numVal, &strVal))
        fontsize = numVal;
    else
        fontsize = 18;
    if(QSPGetVarValues(L"FNAME", 0, &numVal, &strVal))
        fontName = QString::fromWCharArray(strVal);
    else
        fontName = "Comic Sans MS";

    // Head for main description
    mainHead =
            "<head><style>\n*{padding: 0; margin: 0;}\na{text-decoration: none;outline:0;} \nbody{\nbackground: url('"
            + backgroungPath
            + "') no-repeat; font-family: \'"
            + fontName +
            + "\';width: 1280; height: 800;\n-webkit-user-select: none; \ncursor: default;"
            + "background-size: cover;}\n"
            + "#main{"
            + "\nposition: absolute;"
            + "\nwidth: "
            + QString::number(width)
            + ";\nheight: "
            + QString::number(height)
            + ";\nleft: "
            + QString::number(left)
            + ";\ntop: "
            + QString::number(top)
            + ";\nfont-size: "
            + QString::number(fontsize)
            + "\n}div{margin: 0 auto; display: table;}\n"
            + additionalStyle +
            "\n</style>\n<script type=\"text/javascript\">window.onload=function(){var msg = document.getElementById('message');var bgImage = new Image();bgImage.src = msg.style.backgroundImage.replace(/url\\((['\\\"])?(.*?)\\1\\)/gi, '$2').split(',')[0]; bgImage.onload=function(){msg.style.width = bgImage.width;msg.style.height = bgImage.height;}};\n</script></head>\n\n";
}

void FastQSPWindow::loadMessage(QString & messageDesc, QString & messageStyle)
{
    // Temp variables
    int numVal = 0;
    wchar_t *strVal = NULL;

    // Get background image
    QString backgroungPath;
    if(QSPGetVarValues(L"MSG_BACKIMAGE", 0, &numVal, &strVal))
    {
        backgroungPath = QString::fromWCharArray(strVal);
        //backgroungPath = toUrlPath(backgroungPath);
    }

    // Get left and top
    uint left = 0;
    uint top = 0;
    if(QSPGetVarValues(L"MSG_X", 0, &numVal, &strVal))
        left = numVal;
    else
        left = 200;
    if(QSPGetVarValues(L"MSG_Y", 0, &numVal, &strVal))
        top = numVal;
    else
        top = 165;

    // Get text width and height
    uint textWidth = 0;
    uint textHeight = 0;
    if(QSPGetVarValues(L"MSG_TEXT_W", 0, &numVal, &strVal))
        textWidth = numVal;
    else
        textWidth = 392;
    if(QSPGetVarValues(L"MSG_TEXT_H", 0, &numVal, &strVal))
        textHeight = numVal;
    else
        textHeight = 231;

    // Get text left and top
    uint textLeft = 0;
    uint textTop = 0;
    if(QSPGetVarValues(L"MSG_TEXT_X", 0, &numVal, &strVal))
        textLeft = numVal;
    else
        textLeft = 4;
    if(QSPGetVarValues(L"MSG_TEXT_Y", 0, &numVal, &strVal))
        textTop = numVal;
    else
        textTop = 4;

    // Get ok button left and top
    uint buttonLeft = 0;
    uint buttonTop = 0;
    if(QSPGetVarValues(L"MSG_OK_X", 0, &numVal, &strVal))
        buttonLeft = numVal;
    else
        buttonLeft = 186;
    if(QSPGetVarValues(L"MSG_OK_Y", 0, &numVal, &strVal))
        buttonTop = numVal;
    else
        buttonTop = 239;

    // Get ok button image
    QString buttonImagePath;
    if(QSPGetVarValues(L"MSG_OK_IMAGE", 0, &numVal, &strVal))
    {
        buttonImagePath = QString::fromWCharArray(strVal);
        //buttonImagePath = toUrlPath(buttonImagePath);
    }

    // Generate message style
    messageStyle =
            "#message{\nposition:fixed;\ntop:" +
            QString::number(top) +
            ";\nleft:" +
            QString::number(left) +
            ";\n}";

    // Get message format
    QString msgFormat;
    if(QSPGetVarValues(L"MSG_FORMAT", 0, &numVal, &strVal))
        msgFormat = QString::fromWCharArray(strVal);

    // Generate  message html
    messageDesc =
            "<div id = \"message\" style=\"background-image:url('" +
            backgroungPath +
            "');visibility:";
    if(messageVisible)
        messageDesc += "visible";
    else
        messageDesc += "hidden";
    messageDesc +=
            ";\" >\n" +
            msgFormat.replace("%TEXT%",
                              "<div style='top:" + QString::number(textTop) +
                              ";left:" + QString::number(textLeft) +
                              ";width:" + QString::number(textWidth) +
                              ";height:" + QString::number(textHeight) +
                              ";position:absolute;'>" + messageText + "</div>") +
            "\n<a href='msgclose'><img src='" +
            buttonImagePath +
            "' style='position:absolute;top:" +
            QString::number(buttonTop) +
            ";left:" +
            QString::number(buttonLeft) +
            ";'/></a></div>";
}

void FastQSPWindow::loadMainDesc(QString & mainDesc, const QString & additionalDesc)
{
    mainDesc = QString::fromWCharArray(QSPGetMainDesc()) + additionalDesc;

    // NOTE: all resources must be in content folder
    // TODO: make normal path replacement
    // Dirty hacks for AeroQsp compability
    mainDesc = mainDesc.replace('\\', '/');
    mainDesc = mainDesc.replace("content/", "file:///" + gameDirectory + "content/");
    mainDesc = mainDesc.replace("\r\n", "\r\n<br>");
    mainDesc = "<div id = \"main\">\n" + mainDesc + "</div>";
}

void FastQSPWindow::loadPage()
{
    QElapsedTimer myTimer;
    myTimer.start();
    QString res2 = builder.getHTML();
    int time2 = myTimer.nsecsElapsed();
    qDebug() << "new builder" << time2;

    myTimer.start();
    // Load actions
    //loadActions();

    // Load objects
    QString objectsDesc;
    QString objectsStyle;
    loadObjects(objectsDesc, objectsStyle);

    QString messageStyle;
    QString messageDesc;
    loadMessage(messageDesc, messageStyle);

    // Load page head
    QString mainHead;
    loadMainHead(mainHead, objectsStyle + messageStyle);

    // Load main view description
    QString mainDesc;
    loadMainDesc(mainDesc, objectsDesc + messageDesc);

    QString res1 = mainHead + "<body  ondragstart='return false;' ondrop='return false;'>" + mainDesc + "</body>";
    int time1 = myTimer.nsecsElapsed();
    qDebug() << "old builder" << time1;


    mainView->setHtml(res2);


    // Load external stylesheet
    // TODO: Must check that css unchanged. Maybe save stylesheet hash?
    loadStyle();
}

// TODO: find hidden meanig of actions
void FastQSPWindow::loadActions()
{
    uint count = QSPGetActionsCount();
    // Tmp variables
    wchar_t *imgPath = NULL;
    wchar_t *desc = NULL;
    for(uint ind = 0; ind < count; ++ind)
    {
        QSPGetActionData(ind, &imgPath, &desc);
    }
}

void FastQSPWindow::showMessage(const QString &text)
{
    messageVisible = true;
    messageText = text;
}

void FastQSPWindow::hideMessage()
{
    messageVisible = false;
    messageText = "";
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
