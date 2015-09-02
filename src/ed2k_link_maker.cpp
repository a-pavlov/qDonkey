#include "misc.h"
#include "ed2k_link_maker.h"

#include <QClipboard>
#include <QUrl>


ed2k_link_maker::ed2k_link_maker(QWidget *parent /* = 0*/)
    : QDialog(parent)
{
    setupUi(this);
    connect(checkForum, SIGNAL(stateChanged(int)), this, SLOT(checkChanged(int)));
    connect(checkSize, SIGNAL(stateChanged(int)), this, SLOT(checkChanged(int)));
    connect(btnCopy, SIGNAL(clicked()), this, SLOT(putToClipboard()));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
}

ed2k_link_maker::~ed2k_link_maker()
{
}

void ed2k_link_maker::addED2KLink(const QString& fileName, const QString& hash, quint64 fileSize)
{
    m_links.push_back(qMakePair(qMakePair(fileName, hash), fileSize));
}

void ed2k_link_maker::build()
{
    editLink->clear();
    checkSize->setDisabled(true);

    foreach(const LINK& l, m_links)
    {
        QString size = QString::number(l.second);

        QString link = "ed2k://|file|" + QString(QUrl::toPercentEncoding(l.first.first)) + "|" +
            size + "|" + l.first.second + "|/";

        if (checkForum->checkState() == Qt::Checked)
        {
            link = "[u][b][url=" + link + "]" + l.first.first + "[/url][/b][/u]";
            checkSize->setEnabled(true);
            if (checkSize->checkState() == Qt::Checked)
                link += " " + misc::friendlyUnit(l.second);
        }

        editLink->appendPlainText(link);
    }
}

void ed2k_link_maker::checkChanged(int state)
{
    build();
}

void ed2k_link_maker::putToClipboard()
{
    QString text = editLink->toPlainText();
    if (text.length())
    {
        QClipboard *cb = QApplication::clipboard();
        cb->setText(text);
    }
}

void ed2k_link_maker::close()
{
    accept();
}
