#include "transfers_widget.h"
#include "ui_transfers_widget.h"
#include "transfer_model.h"
#include "peermodel.h"
#include "transferlist_delegate.h"
#include "ed2k_link_maker.h"
#include "qed2ksession.h"

#include <QDesktopServices>
#include <QUrl>

transfers_widget::transfers_widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    tmodel = new TransferModel(this);
    pmodel = new PeerModel(this);

    trView->setRootIsDecorated(false);
    trView->setItemDelegate(new TransferListDelegate(this));
    trView->setModel(tmodel);

    peerView->setModel(pmodel);
    peerView->setRootIsDecorated(false);
    menu = new QMenu(this);
    menu->addAction(actionShow_all_transfers);
    menu->addSeparator();
    menu->addAction(actionStart);
    menu->addAction(actionPause);
    menu->addSeparator();
    menu->addAction(actionPreview);
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

    connect(&m_refreshTimer, SIGNAL(timeout()), SLOT(refreshModels()));
    m_refreshTimer.start(3000);
    /*
    // Default hidden columns
    if (!column_loaded) {
        trView->setColumnHidden(TransferModelItem::TM_AMOUNT_DOWNLOADED, true);
        trView->setColumnHidden(TransferModelItem::TM_AMOUNT_LEFT, true);
        trView->setColumnHidden(TransferModelItem::TM_TIME_ELAPSED, true);
    }
    */
}

transfers_widget::~transfers_widget()
{
}

void transfers_widget::displayListMenu(const QPoint&) {
    QModelIndexList selected = trView->selectionModel()->selectedRows();
    const QStringList hashes = getSelectedHashes();
    bool has_start = false;
    bool has_pause = false;
    bool has_preview = false;
    bool has_link = (hashes.size() == 1);
    int all_same_sequential_download_mode = 0;
    int all_same_prio_firstlast = 0;
    int seeders = 0;
    int invalid = 0;

    foreach(const QString& hash, hashes) {
        QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid()) {
            if (h.is_seed()) ++seeders; else {
                if (h.is_sequential_download()) ++all_same_sequential_download_mode;
                if (h.extremity_pieces_first()) ++all_same_prio_firstlast;
            }
            if (h.is_paused()) has_start = true; else has_pause = true;
            if (misc::isPreviewable(misc::file_extension(h.filename()))) has_preview = true;
        } else {
            ++invalid;
        }
    }

    actionStart->setEnabled(has_start);
    actionPause->setEnabled(has_pause);
    actionPreview->setEnabled(has_preview);
    actionED2K_link->setEnabled(has_link);
    actionRemove->setDisabled(hashes.isEmpty());
    actionRename->setEnabled(has_link);

    actionSeries_download->setChecked(all_same_sequential_download_mode == (hashes.size() - seeders - invalid));
    actionFirst_and_last_pieces_first->setChecked(all_same_prio_firstlast == (hashes.size() - seeders- invalid));
    if (seeders == (hashes.size() - invalid)) {
        actionSeries_download->setDisabled(true);
        actionSeries_download->setChecked(false);
        actionFirst_and_last_pieces_first->setDisabled(true);
        actionFirst_and_last_pieces_first->setChecked(false);
    }

    menu->exec(QCursor::pos());
}

QString transfers_widget::getHashFromRow(int row) const {
    return tmodel->transferHash(row);
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
        Session::instance()->getTransfer(hash).pause();
    }
}

void transfers_widget::on_actionRemove_triggered() {
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        tmodel->removeTransfer(hash);
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

void transfers_widget::on_actionFirst_and_last_pieces_first_triggered() {
    QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid()) {
            h.prioritize_extremity_pieces(!h.extremity_pieces_first());
        }
    }
}

void transfers_widget::on_actionPreview_triggered() {
    int counter = 0;
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        const QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid() && misc::isPreviewable(misc::file_extension(h.filename())) && counter < 10) {
            Session::instance()->deferPlayMedia(h);
            ++counter;  // avoid run a lot of programs - max 10
        }
    }
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

void transfers_widget::refreshModels() {
    pmodel->populate();
}
