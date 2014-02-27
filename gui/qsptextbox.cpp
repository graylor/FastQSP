#include "qsptextbox.h"

QspTextBox::QspTextBox(QWidget* parent) :
    QTextEdit(parent)
{
    setReadOnly(true);

    viewport()->setMouseTracking(true);
}

QVariant QspTextBox::loadResource(int type, const QUrl &name)
{
    if (type == QTextDocument::ImageResource) {
        QFile file(srcUrl.resolved(name).toLocalFile());
        if (file.open(QIODevice::ReadOnly))
            return file.readAll();
    }
    return QTextEdit::loadResource(type, name);
}

void QspTextBox::SetContentUrl(QUrl srcUrl)
{
    this->srcUrl = srcUrl;
}

void QspTextBox::SetText(QString text)
{
    QString txt;
    if (m_isHtml)
        txt = text.replace("\n", "<br />");
    else
        txt = Qt::convertFromPlainText(text);
    QTextDocument *doc = this->document();
    //QString css = "";
    //doc->addResource(QTextDocument::StyleSheetResource, QUrl("SlipSummary.css"), css);
    //doc->setHtml(QString("<html><head><link rel='stylesheet' type='text/css' href='SlipSummary.css'></head><body>%1</body></html>").arg(txt));
    doc->setHtml(txt);
    this->setDocument(doc);
}

void QspTextBox::LoadBackImage(QString filename)
{
    if (QFile::exists(filename))
    {
        SetBackgroundImage(QPixmap(filename));
        return;
    }
    SetBackgroundImage(QPixmap());
}

void QspTextBox::SetBackgroundImage(QPixmap pic)
{
    _mPic = pic;
    CalcImageSize();
}

void QspTextBox::CalcImageSize()
{
    if (!_mPic.isNull())
    {
        int w, h;
        w = this->childrenRect().width();
        h = this->childrenRect().height();
        if (w < 1) w = 1;
        if (h < 1) h = 1;
        int srcW = _mPic.width(), srcH = _mPic.height();
        int destW = srcW * h / srcH, destH = srcH * w / srcW;
        if (destW > w)
            destW = w;
        else
            destH = h;
        m_posX = (w - destW) / 2;
        m_posY = (h - destH) / 2;
        if (destW > 0 && destH > 0)
            m_bmpRealBg = _mPic.scaled(destW, destH);
    }
}

void QspTextBox::mouseMoveEvent(QMouseEvent* e)
{
    QPoint p = e->pos();
    QCursor cursor = viewport()->cursor();

    if ( !anchorAt( p ).isEmpty() )
    {
        cursor.setShape(Qt::PointingHandCursor);
        viewport()->setCursor(cursor);
    }
    else
    {
        cursor.setShape(Qt::ArrowCursor);
        viewport()->setCursor(cursor);
    }
}

void QspTextBox::mousePressEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    if ( !anchorAt( p ).isEmpty() )
    {
        emit LinkClicked(anchorAt(p), e->globalPos());
    }
}

void QspTextBox::paintEvent(QPaintEvent *e)
{
    if (!_mPic.isNull())
    {
        QPainter painter(viewport());
        painter.drawImage(m_posX, m_posY, *(new QImage(m_bmpRealBg.toImage())));
    }

    QTextEdit::paintEvent(e);
}

void QspTextBox::resizeEvent(QResizeEvent *e)
{
    if (!_mPic.isNull())
    {
        CalcImageSize();
    }
    QTextEdit::resizeEvent(e);
}
