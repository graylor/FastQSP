#include "qsp_callback.h"
#include "qsp_default.h"
#include "fastqspwindow.h"
#include <QIcon>

FastQSPWindow::FastQSPWindow(QWidget *parent)
    : QMainWindow(parent), gameWidth(800), gameHeight(600),
      aspectRatio(qreal(gameWidth) / qreal(gameHeight)), scaleFactor(1),
      gameIsOpen(false), netManager() {
// Init audio
#if QT_VERSION < 0x050000
  media = new Phonon::MediaObject(this);
  audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
  Phonon::createPath(media, audioOutput);
#else
  player = new QMediaPlayer();
#endif

  // Start timer
  timer.start();

  // Init view
  scene = new QGraphicsScene(this);
  graphicsView = new QGraphicsView(scene, this);
  graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphicsView->setUpdatesEnabled(true);

  webView = new QGraphicsWebView();
  webView->page()->setNetworkAccessManager(&netManager);
  webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  scene->addItem(webView);
  scene->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
  webView->setRenderHints(
      QPainter::Antialiasing | QPainter::HighQualityAntialiasing |
      QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform |
      QPainter::NonCosmeticDefaultPen);
  webView->settings()->setAttribute(QWebSettings::AutoLoadImages, true);
  webView->setAutoFillBackground(false);

  // Filter context menu event
  graphicsView->viewport()->installEventFilter(this);

  // Creating menu
  //-File menu--------------------------------------------------------
  QMenu *fileMenu = new QMenu("File");
  fileMenu->addAction("Open file\tCtrl+O", this, SLOT(openFileDialog()));
  QShortcut *openFile = new QShortcut(QKeySequence("Ctrl+O"), this);
  connect(openFile, SIGNAL(activated()), SLOT(openFileDialog()));

  fileMenu->addAction("Exit\tCtrl+Q", this, SLOT(close()));
  QShortcut *exit = new QShortcut(QKeySequence("Ctrl+Q"), this);
  connect(exit, SIGNAL(activated()), SLOT(close()));

  menuBar()->addMenu(fileMenu);

  //-Game menu--------------------------------------------------------
  gameMenu = new QMenu("Game");
  gameMenu->addAction("Save\tCtrl+S", this, SLOT(saveGameDialog()));
  QShortcut *save = new QShortcut(QKeySequence("Ctrl+S"), this);
  connect(save, SIGNAL(activated()), SLOT(saveGameDialog()));

  gameMenu->addAction("Load\tCtrl+L", this, SLOT(loadGameDialog()));
  QShortcut *load = new QShortcut(QKeySequence("Ctrl+L"), this);
  connect(load, SIGNAL(activated()), SLOT(loadGameDialog()));

  gameMenu->addAction("Restart\tCtrl+R", this, SLOT(restartGame()));
  QShortcut *restart = new QShortcut(QKeySequence("Ctrl+R"), this);
  connect(restart, SIGNAL(activated()), SLOT(restartGame()));

  // TODO: slows the game, move saving to diffrent thread
  autosaveAction = new QAction("Autosave", this);
  autosaveAction->setCheckable(true);
  autosaveAction->setChecked(false);
  // gameMenu->addAction(autosave);

  menuBar()->addMenu(gameMenu);
  gameMenu->setDisabled(true);

  //-Other menu-------------------------------------------------------
  QMenu *otherMenu = new QMenu("Other");
  otherMenu->addAction("Fullscreen\tAlt+Enter", this, SLOT(toggleFullscreen()));
  QShortcut *fullscreen =
      new QShortcut(QKeySequence(Qt::Key_Return + Qt::AltModifier), this);
  otherMenu->addAction("Show html", this, SLOT(showHtml()));
  connect(fullscreen, SIGNAL(activated()), SLOT(toggleFullscreen()));

  menuBar()->addMenu(otherMenu);

  //-Help menu--------------------------------------------------------
  QMenu *helpMenu = new QMenu("Help");
  helpMenu->addAction("About", this, SLOT(about()));
  menuBar()->addMenu(helpMenu);
  //------------------------------------------------------------------

  connect(webView, SIGNAL(linkClicked(const QUrl &)),
          SLOT(linkClicked(const QUrl &)), Qt::DirectConnection);
  connect(webView, SIGNAL(loadFinished(bool)), SLOT(toggleUpdate()),
          Qt::DirectConnection);
  connect(webView, SIGNAL(loadStarted()), SLOT(toggleUpdate()),
          Qt::DirectConnection);

  setCentralWidget(graphicsView);

  // Initializing QSP
  QSPInit();
  QSPCallback::QSPCallback();

  qDebug() << "QSP init finished";
}

void FastQSPWindow::loadFonts() {
  QDir fontsDir(gameDirectory + "fonts");
  fontsDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
  fontsDir.setSorting(QDir::Size | QDir::Reversed);
  QFileInfoList fontList = fontsDir.entryInfoList();
  for (int i = 0; i < fontList.size(); ++i) {
    QFileInfo fileInfo = fontList.at(i);
    QFontDatabase::addApplicationFont(fileInfo.absoluteFilePath());
  }
}

FastQSPWindow::~FastQSPWindow() { QSPDeInit(); }

bool FastQSPWindow::eventFilter(QObject * /*obj*/, QEvent *e) {
  if (e->type() == QEvent::ContextMenu)
    return true;
  return false;
}

void FastQSPWindow::toggleUpdate() {
  graphicsView->setUpdatesEnabled(!graphicsView->updatesEnabled());
}

int FastQSPWindow::getTimeFromStart() { return timer.elapsed(); }

void FastQSPWindow::toggleFullscreen() {
  if (isFullScreen()) {
    menuBar()->show();
    showNormal();
    qDebug() << "fullscreen mode off";
  } else {
    menuBar()->hide();
    showFullScreen();
    qDebug() << "fullscreen mode on";
  }
}

void FastQSPWindow::about() {
  QLabel *about = new QLabel;
  about->setText(
      "<h2>FastQSP player " % QCoreApplication::applicationVersion() %
      "</h2><table><tr><td>Author:</td> "
      "<td>Graylor[graylor@yandex.ru]</td></tr><tr><td>Page:</td> <td><a "
      "href='https://github.com/graylor/FastQSP'>https://github.com/graylor/"
      "FastQSP</a></td></tr><tr><td>License:</td> <td>GPL v3</td></tr>");
  about->setFixedSize(300, 90);
  about->show();
}

void FastQSPWindow::openFileDialog() {
  QString filename =
      QFileDialog::getOpenFileName(this, "Open File", NULL, "QSP game (*.qsp)");
  if (!filename.isEmpty())
    openFile(filename);
}

void FastQSPWindow::saveGameDialog() {
  QString filename = QFileDialog::getSaveFileName(
      this, "Save Game", saveDir.absolutePath(), "QSP save-game (*.sav)");
  if (!filename.isEmpty() &&
      !filename.endsWith(QLatin1String(".sav"), Qt::CaseInsensitive))
    filename += QLatin1String(".sav");
  saveGame(filename);
}

void FastQSPWindow::saveGame(const QString &filename) {
  qDebug() << "Saving game to" << filename;
  if (!filename.isEmpty())
    QSPSaveGame(filename.toStdWString().c_str(), true);
}

void FastQSPWindow::loadGameDialog() {
  QFileDialog dlg;
  QString filename = dlg.getOpenFileName(
      this, "Load Game", saveDir.absolutePath(), "QSP save-game (*.sav)");
  if (!filename.isEmpty())
    loadGame(filename);
}

void FastQSPWindow::loadGame(const QString &filename) {
  qDebug() << "Loading game from" << filename;
  builder.clear();
  if (!filename.isEmpty() &&
      QSPOpenSavedGame(filename.toStdWString().c_str(), true)) {
    loadPage();
  }
}

void FastQSPWindow::restartGame() {
  QSPRestartGame(true);
  builder.clear();
  loadPage();
}

void FastQSPWindow::showHtml() {
  static QTextEdit *htmlText;
  if (!htmlText)
    htmlText = new QTextEdit();
  htmlText->setPlainText(webView->page()->mainFrame()->toHtml());
  htmlText->show();
}

void FastQSPWindow::linkClicked(const QUrl &url) {
  qDebug() << "Link clicked" << url.toString();
  if (url.toString().startsWith(QLatin1String("exec:"), Qt::CaseInsensitive)) {
    QString execStr;
    execStr = url.toString().remove(0, 5).trimmed().replace("%22", "\"");
    QSPExecString(execStr.toStdWString().c_str(), true);
  } else if (url.toString().startsWith(QLatin1String("obj:"),
                                       Qt::CaseInsensitive)) {
    bool ok = false;
    int number = 0;
    number = url.toString().remove(0, 4).trimmed().toInt(&ok);
    if (!ok) {
      qCritical() << "Unknown link format" << url.toString();
      return;
    }
    QSPSetSelObjectIndex(number, true);
  } else if (url.path() == QLatin1String("/api/closemsg")) {
    builder.hideMessage();
  } else if (url.toString().toLower() == QLatin1String("restart")) {
    restartGame();
  } else if (url.host() == QLatin1String("qspgame.local")) {
    QString path = url.path();
    path.remove(0, 1);
    qDebug() << path;
    bool ok = false;
    int number;
    number = path.toUInt(&ok);
    if (ok) {
      QSPSetSelActionIndex(number - 1, true);
      QSPExecuteSelActionCode(true);
    } else {
      qWarning() << QLatin1String("Bad link path:") << path;
    }
  } else if (url.scheme() == QLatin1String("http") ||
             url.scheme() == QLatin1String("mailto")) {
    QDesktopServices::openUrl(url);
  } else {
    qWarning() << QLatin1String("Bad link:") << url.toString();
  }
  loadPage();
}

void FastQSPWindow::playAudio(QString filename, int vol) {
  filename = filename.replace('\\', '/');
#if QT_VERSION < 0x050000
  if (QFile(filename).exists() && media->state() != Phonon::PlayingState) {
    qDebug() << "playing:" << QFileInfo(filename).filePath() << vol;
    audioOutput->setVolume(qreal(vol) / qreal(100));
    media->setCurrentSource(
        QUrl::fromLocalFile(QFileInfo(filename).filePath()));
    media->play();
  }
#else
  if (QFile(filename).exists() &&
      player->state() != QMediaPlayer::PlayingState) {
    qDebug() << "playing:" << QFileInfo(filename).filePath() << vol;
    player->setMedia(QUrl::fromLocalFile(QFileInfo(filename).filePath()));
    player->setVolume(vol);
    player->play();
  }
#endif
}

void FastQSPWindow::stopAudio() {
#if QT_VERSION < 0x050000
  media->stop();
#else
  player->stop();
#endif
}

void FastQSPWindow::openFile(const QString &filename) {
  builder.clear();
  if (gameIsOpen)
    autosave();
  if (!QSPLoadGameWorld(filename.toStdWString().c_str()))
    qCritical() << QString("Could not open file: ") << filename;
  if (QSPRestartGame(QSP_TRUE)) {
    gameMenu->setEnabled(true);
    gameDirectory = QFileInfo(filename).absolutePath() + "/";
    builder.setGameDir(gameDirectory);
    netManager.setGameDirectory(gameDirectory);
    loadFonts();
    QFile configFile(gameDirectory + QLatin1String("config.xml"));
    if (configFile.open(QFile::ReadOnly)) {
      QTextStream stream(&configFile);
      QString config = stream.readLine();
      configFile.close();

      QRegExp re;
      re.setMinimal(true);

      re.setPattern("width=\"(\\d+)\"");
      if (re.indexIn(config))
        gameWidth = re.cap(1).toInt();
      else
        gameWidth = 800;
      re.setPattern("height=\"(\\d+)\"");
      if (re.indexIn(config) > 0)
        gameHeight = re.cap(1).toInt();
      else
        gameHeight = 600;
      aspectRatio = qreal(gameWidth) / qreal(gameHeight);

      re.setPattern("title=\"(.+)\"");
      if (re.indexIn(config) >= 0)
        setWindowTitle(re.cap(1));
      else
        setWindowTitle("FastQSP");

      re.setPattern("icon=\"(.+)\"");
      if (re.indexIn(config) >= 0)
        QApplication::setWindowIcon(QIcon(gameDirectory + re.cap(1)));
    }
    aspectRatio = qreal(gameWidth) / qreal(gameHeight);
    loadPage();
    webView->resize(gameWidth, gameHeight);
    resize(gameWidth, gameHeight);
    gameIsOpen = true;
    saveDir = gameDirectory + "save/";
    if (!saveDir.exists()) {
      saveDir.mkpath(".");
    }
    timer.restart();
  }
}

// That function is called by callback if isRefsresh == true
// according to the debug log that never happens. maybe I should remove it?
void FastQSPWindow::refreshView() { qDebug() << "refreshView()"; }

void FastQSPWindow::loadPage() {
  webView->setHtml(builder.getHTML(), QUrl("http://qspgame.local"));
  if (autosaveAction->isChecked())
    autosave();
}

void FastQSPWindow::autosave() {
  qDebug() << "autosave:" << saveDir.absolutePath() + "/auto.sav";
  saveGame(saveDir.absolutePath() + "/auto.sav");
}

void FastQSPWindow::resizeEvent(QResizeEvent * /*event*/) {
  graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void FastQSPWindow::closeEvent(QCloseEvent * /*event*/) {
  if (gameIsOpen)
    autosave();
}

void FastQSPWindow::timerEvent(QTimerEvent *event) {
  QMainWindow::timerEvent(event);
  QSPExecCounter(true);
}
