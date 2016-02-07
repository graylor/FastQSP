#include "fastqspwindow.h"
#include <QApplication>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

FastQSPWindow *qspWin;

int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("FastQSP");
  QCoreApplication::setApplicationVersion(STRINGIZE(GIT_VERSION));

  QApplication a(argc, argv);
  qspWin = new FastQSPWindow();
  qspWin->resize(975, 630);

  qspWin->show();

  if (argc > 1 && QFile(argv[1]).exists())
    qspWin->openFile(argv[1]);

  return a.exec();
}
