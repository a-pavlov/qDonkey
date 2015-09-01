#ifndef ED2K_LINK_MAKER_H
#define ED2K_LINK_MAKER_H

#include <QDialog>
#include "ui_ed2k_link_maker.h"

class ed2k_link_maker : public QDialog, public Ui::ed2k_link_maker
{
    Q_OBJECT
public:
    ed2k_link_maker(QWidget *parent = 0);
    ~ed2k_link_maker();
    void addED2KLink(const QString& fileName, const QString& hash, quint64 fileSize);
    void build();
private:
    typedef QPair<QPair<QString, QString>, quint64> LINK;
    QList<LINK> m_links;
private slots:
    void checkChanged(int state);
    void putToClipboard();
    void close();
};

#endif // ED2K_LINK_MAKER_H
