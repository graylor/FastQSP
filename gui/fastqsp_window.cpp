#include "fastqsp_window.h"
#include "qsp_callback.h"


FQWindow::FQWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *wgt = new QWidget(this);
    btn1 = new QPushButton(wgt);
    btn2 = new QPushButton(wgt);
    btn1->setText("Read object");
    btn2->setText("action 1");
    QObject::connect(
                btn1,
                SIGNAL(clicked()),
                this,
                SLOT(getObject()));
    QObject::connect(
                btn2,
                SIGNAL(clicked()),
                this,
                SLOT(act1()));
    btn2->setGeometry(0, 20, 60, 20);
    setCentralWidget(wgt);

    QMenu* pmnu = new QMenu("&File");
    pmnu->addAction("Open file",
                    this,
                    SLOT(openFile()),
                    Qt::CTRL + Qt::Key_O);
    menuBar()->addMenu(pmnu);

    /*timer = new QTimer(this);
    timer->start(1500);
    QObject::connect(
                timer,
                SIGNAL(timeout()),
                this,
                SLOT(execCounter()));*/

    QSPInit();
    /*QSPSetCallBack(
                QSP_CALL_SETTIMER,
                (QSP_CALLBACK)&FQWindow::setTimer);*/
    QSPCallback::QSPCallback();
}

FQWindow::~FQWindow()
{

}


void FQWindow::openFile()
{
    /*QFileDialog dlg;
    QString filename = dlg.getOpenFileName(
                this,
                "Open File",
                NULL,
                "QSP game (*.qsp)");
    QSPLoadGameWorld(filename.toStdString().c_str());*/
    const wchar_t * fname = L"C:\\Users\\Sergey\\Desktop\\Jack\\jack051_lite\\jack.qsp";
    if(!QSPLoadGameWorld(fname))
        qDebug() << QString("Could not open file: ") << fname;
    QFileInfo fi(QString::fromWCharArray(fname));
    gameDirectory = fi.absolutePath() + "/";
    //QSPEnableDebugMode(QSP_TRUE);
    QSPRestartGame(QSP_TRUE);    
}

void FQWindow::act1()
{
    qDebug() << "Action 1";
    QSPSetSelActionIndex(0, true);
    QSPExecuteSelActionCode(true);
}

void FQWindow::getObject()
{
    qDebug() << "Variables description: " << QString::fromWCharArray(QSPGetVarsDesc());

    int objectsCount = QSPGetObjectsCount();
    qDebug() << "Number of objects: " << objectsCount;
    for(int i = 0; i < objectsCount; ++i)
    {
        wchar_t *imgPath;
        wchar_t *desk;
        QSPGetObjectData(i, &imgPath, &desk);
        qDebug() << i + 1 << ")\tImage Path:" << QString::fromWCharArray(imgPath);
        qDebug() << "\tDescription:" << QString::fromWCharArray(desk);
    }
    int actionsNumber = QSPGetActionsCount();

    qDebug() << "Number of actions: " << actionsNumber;
    for(int i = 0; i < actionsNumber; ++i)
    {
        wchar_t *imgPath;
        wchar_t *desk;
        QSPGetActionData(i, &imgPath, &desk);
        qDebug() << i + 1 << ")\tImage Path:" << QString::fromWCharArray(imgPath);
        qDebug() << "\tDescription:" << QString::fromWCharArray(desk);
    }

    qDebug() << "Current location: " << QString::fromWCharArray(QSPGetCurLoc());
    qDebug() << "Quest path: " << QString::fromWCharArray(QSPGetQstFullPath());

    int numVal, numVal2;
    wchar_t *strVal;
    QSPGetVarValues(L"BACKIMAGE", 0, &numVal, &strVal);
    qDebug() << "Backimage: numVal=" << numVal
             << "strVal="          << QString::fromWCharArray(strVal);

    /*qDebug() << "STYLESHEET: numVal=" << numVal
             << "strVal="          << QString::fromWCharArray(strVal);*/

    QString mainDesc = QString::fromWCharArray(QSPGetMainDesc());
    mainDesc = mainDesc.replace("content\\", "file:///" + gameDirectory + "content/");

    qDebug() << mainDesc;

    // Show QLabel with background
    QLabel* lbl = new QLabel();

    lbl->resize(640, 480);

    QDir dir(QString::fromWCharArray(strVal));
    QString bg = gameDirectory + dir.path();
    QPalette pal;
    QImage img(bg);
    QBrush brush(img.scaled(lbl->size()));
    pal.setBrush(lbl->backgroundRole(), brush);

    lbl->setPalette(pal);
    lbl->setText(mainDesc);
    lbl->setTextFormat(Qt::RichText);
    lbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    lbl->show();


    QWebView *wv = new QWebView();
    wv->resize(640, 480);
    //wv->setStyleSheet("background-image:url(" +  bg+ ")");
    wv->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(wv, SIGNAL(linkClicked(QUrl)), SLOT(linkClick(const QUrl &)));

    //QSPGetVarValues(L"STYLESHEET", 0, &numVal, &strVal);
    //QString styleSheet = "data:text/css;charset=UTF-8;" + QString::fromUtf8(QString::fromWCharArray(strVal).toUtf8());

    //wv->settings()->setUserStyleSheetUrl(styleSheet);


    wv->setHtml(mainDesc);

    //QByteArray css = QString::fromWCharArray(strVal).toUtf8();
    //wv->setStyleSheet(QString::fromWCharArray(strVal));
    //wv->setStyleSheet(css);//->settings()->setCSSMediaType(QString::fromWCharArray(L"#under {\n\tcolor:#FFD700;\n\tfont-size:40%;\n\tfont-family: Victoriana;t\n\text-align:center;}"));
    //qDebug() << css;

    QUrl url;
    url.setUrl("file:///" + gameDirectory + "style.css");

    //qDebug() << "file:///" + gameDirectory + "style.css";

    //QString styleSheet = "base:text/css;charset=UTF-8," + QString::fromWCharArray(strVal).toUtf8();
    //qDebug() << QString::fromWCharArray(strVal);
    //qDebug() << styleSheet;


    wv->settings()->setUserStyleSheetUrl(url);

    //QSPGetVarValues(L"FSIZE", 0, &numVal, &strVal);
    //qDebug() << "FSIZE" << numVal << QString::fromWCharArray(strVal);
    //wv->settings()->setFontSize(QWebSettings::DefaultFontSize, 40);

    QWebElement webElem = wv->page()->mainFrame()->findFirstElement("a");
    webElem.setAttribute("href", "1");
    qDebug() << webElem.attribute("href");


    wv->show();

    //QSPGetExprValue()


    /*QspTextBox *tb = new QspTextBox();
    tb->SetIsHtml(true);
    tb->LoadBackImage(bg);
    tb->setText(QString((const QChar *)QSPGetMainDesc()));
    tb->show();*/

    // Unused in sample projects
    /*for(int ind = 0; ind < 10; ++ind)
    {
        QSPGetVarNameByIndex(ind, &strVal);
        qDebug() << "Index: " << ind
                 << "Name: " << QString::fromWCharArray(strVal);
    }*/

    /*if(num < QSPGetObjectsCount())
    {
        qDebug() << "IMG: " << imgLst[num];
        qDebug() << "Desc: " << descLst[num];
    }*/
}

void FQWindow::linkClick(const QUrl & url)
{
    qDebug() << url.fileName();
}

void FQWindow::timerEvent(QTimerEvent *event)
{
    //qDebug() << "execCounter()";
    QSPExecCounter(true);
}
