#include "transfers_widget.h"
#include "ui_transfers_widget.h"
#include "transfer_model.h"
#include "transferlist_delegate.h"
#include "ed2k_link_maker.h"
#include "qed2ksession.h"

#include <QDesktopServices>
#include <QUrl>

transfers_widget::transfers_widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    model = new TransferModel(this);
    trView->setItemDelegate(new TransferListDelegate(this));
    trView->setModel(model);
    menu = new QMenu(this);
    menu->addAction(actionStart);
    menu->addAction(actionPause);
    menu->addSeparator();
    menu->addAction(actionRemove);
    menu->addAction(actionRename);
    menu->addSeparator();
    menu->addAction(actionED2K_link);
    menu->addAction(actionSeries_download);
    menu->addAction(actionFirst_and_last_pieces_first);
    menu->addSeparator();
    menu->addAction(actionOpen_destination_folder);
    connect(trView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(displayListMenu(const QPoint&)));
}

transfers_widget::~transfers_widget()
{
}

void transfers_widget::displayListMenu(const QPoint&) {
    QModelIndexList selected = trView->selectionModel()->selectedRows();
    //QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    //bool enabled =
    //    selected.size() == 1 &&
    //    misc::isMD4Hash(selected_data(treeResult, SearchModel::DC_HASH).toString());
    menu->exec(QCursor::pos());
}

QString transfers_widget::getHashFromRow(int row) const {
    return model->transferHash(row);
}

QStringList transfers_widget::getSelectedHashes() const {
    QStringList hashes;
    const QModelIndexList selectedIndexes = trView->selectionModel()->selectedRows();
    foreach (const QModelIndex &index, selectedIndexes) {
        hashes << getHashFromRow(mapToSource(index).row());
    }

    return hashes;
}

QModelIndex transfers_widget::mapToSource(const QModelIndex& index) const { return index; }

void transfers_widget::on_actionStart_triggered() {
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        Session::instance()->getTransfer(hash).resume();
    }
}

void transfers_widget::on_actionPause_triggered() {
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        qDebug() << "pause " << hash;
        Session::instance()->getTransfer(hash).pause();
    }
}

void transfers_widget::on_actionRemove_triggered() {
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        Session::instance()->deleteTransfer(hash, true);
    }
}

void transfers_widget::on_actionED2K_link_triggered() {
    const QModelIndexList selectedIndexes = trView->selectionModel()->selectedRows();
    const QString hash = getHashFromRow(mapToSource(selectedIndexes[0]).row());
    QED2KHandle h = Session::instance()->getTransfer(hash);
    if (h.is_valid()) {
        const QString file_name = h.filename();
        quint64 file_size = h.filesize();
        ed2k_link_maker dlg(file_name, hash, file_size, this);
        dlg.exec();
    }
}

void transfers_widget::on_actionSeries_download_triggered()
{
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid()) {
            bool was_sequential = h.is_sequential_download();
            h.set_sequential_download(!was_sequential);
            if (!was_sequential)  h.prioritize_extremity_pieces(true);
        }
    }
}

void transfers_widget::on_actionFirst_and_last_pieces_first_triggered()
{
    QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid()) {
            h.prioritize_extremity_pieces(!h.extremity_pieces_first());
        }
    }
}

void transfers_widget::on_actionPreview_triggered() {
    /*const QStringList hashes = getSelectedTorrentsHashes();
    foreach (const QString &hash, hashes) {
        const QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid()) {
          new PreviewSelect(this, h);
        }
      }
      */
}

void transfers_widget::on_actionOpen_destination_folder_triggered()
{
    QStringList pathsList;
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        const QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid()) {
            const QString savePath = h.save_path();
            qDebug("Opening path at %s", qPrintable(savePath));
            if (!pathsList.contains(savePath)) {
                pathsList.append(savePath);
                QDesktopServices::openUrl(QUrl::fromLocalFile(savePath));
            }
        }
    }

    // open default download folder when there's no selection
    if (hashes.size() == 0) {
        Preferences pref;
        QDesktopServices::openUrl(QUrl::fromLocalFile(pref.inputDir()));
    }
}
