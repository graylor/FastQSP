#include "fastqspwindow.h"
#include <crash-report.h>
#include <QApplication>
#include <QStandardPaths>

FastQSPWindow *qspWin;

#define STR_(x) #x
#define STR(x) STR_(x)

int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("FastQSP");
  QCoreApplication::setApplicationVersion(STR(GIT_VERSION));

  QApplication a(argc, argv);
  Breakpad::CrashHandler::instance()->Init(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

  qspWin = new FastQSPWindow();
  qspWin->resize(975, 630);

  qspWin->show();

  if (argc > 1 && QFile(argv[1]).exists())
    qspWin->openFile(argv[1]);

  return a.exec();
}
