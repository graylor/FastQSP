#include "fastqspwindow.h"
#include <QApplication>

FastQSPWindow* qspWin;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qspWin = new FastQSPWindow();

    qspWin->resize(975, 630);


    qspWin->show();


    return a.exec();
}
