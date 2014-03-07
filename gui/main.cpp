#include "fastqspwindow.h"
#include "qsp_default.h"
#include <QApplication>

FastQSPWindow* qspWin;

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("FastQSP");
    QCoreApplication::setApplicationVersion("v0.4.2");

    QApplication a(argc, argv);
    qspWin = new FastQSPWindow();
    qspWin->resize(975, 630);

    qspWin->show();

    if(argc > 1, QFile(argv[1]).exists())
        qspWin->openFile(argv[1]);

    return a.exec();
}
