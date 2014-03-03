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


    return a.exec();
}
