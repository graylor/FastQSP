#include "fastqspwindow.h"
#include "qsp_default.h"
#include <QApplication>

FastQSPWindow* qspWin;

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("FastQSP");

    QApplication a(argc, argv);
    qspWin = new FastQSPWindow();
    qspWin->resize(975, 630);

    //qspWin->openFile("R:\\jack053_lite\\jack.qsp");

    qspWin->show();


    return a.exec();
}
