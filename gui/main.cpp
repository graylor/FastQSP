#include "fastqspwindow.h"
#include "qsp_default.h"
#include "qsp_htmlbuilder.h"
#include "qsp_callback.h"
#include <QApplication>

FastQSPWindow* qspWin;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qspWin = new FastQSPWindow();
    qspWin->resize(975, 630);

    //qspWin->openFile("R:\\jack053_lite\\jack.qsp");

    qspWin->show();


    return a.exec();
}
