#include "transfers_widget.h"
#include "ui_transfers_widget.h"
#include "transfer_model.h"
#include "peermodel.h"
#include "transferlist_delegate.h"
#include "ed2k_link_maker.h"
#include "qed2ksession.h"

#include <QDesktopServices>
#include <QUrl>
#include <QSortFilterProxyModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>

transfers_widget::transfers_widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    tmodel = new TransferModel(this);
    pmodel = new PeerModel(this);
    psort_model = new QSortFilterProxyModel(this);
    psort_model->setSourceModel(pmodel);
    psort_model->setSortRole(PeerModel::SortRole);
    psort_model->setDynamicSortFilter(true);

    tsort_model = new QSortFilterProxyModel(this);
    tsort_model->setSourceModel(tmodel);
    tsort_model->setDynamicSortFilter(true);
    tsort_model->setFilterRole(TransferModelItem::FilterRole);

    trView->setRootIsDecorated(false);
    trView->setItemDelegate(new TransferListDelegate(this));
    trView->setModel(tsort_model);
    trView->setSortingEnabled(true);

    peerView->setModel(psort_model);
    peerView->setRootIsDecorated(false);
    peerView->setSortingEnabled(true);

    connect(trView->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            this, SLOT(trSortChanged(int, Qt::SortOrder)));

    connect(peerView->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            this, SLOT(pSortChanged(int, Qt::SortOrder)));

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
    menu->addAction(actionLoad_ED2K_link);
    connect(trView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(displayListMenu(const QPoint&)));

    connect(&m_refreshTimer, SIGNAL(timeout()), SLOT(refreshModels()));
    m_refreshTimer.start(3000);
    Preferences pref;
    actionShow_all_transfers->setChecked(pref.showAllTransfers());
    on_actionShow_all_transfers_triggered();

    if(pref.contains("TransferListHeader")) {
        trView->header()->restoreState(pref.value("TransferListHeader").toByteArray());
    } else {
        trView->setColumnHidden(TransferModelItem::TM_AMOUNT_DOWNLOADED, true);
        trView->setColumnHidden(TransferModelItem::TM_AMOUNT_LEFT, true);
        trView->setColumnHidden(TransferModelItem::TM_TIME_ELAPSED, true);
        trView->setColumnHidden(TransferModelItem::TM_HASH, true);
        trView->setColumnHidden(TransferModelItem::TM_TYPE, true);
    }

    if (pref.contains("PeerListHeader")) {
        peerView->header()->restoreState(pref.value("PeerListHeader").toByteArray());
    }

    trView->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(trView->header(), SIGNAL(customContextMenuRequested(const QPoint&)),
                SLOT(displayHSMenu(const QPoint&)));

    trView->header()->setSortIndicator(TransferModelItem::TM_NAME, Qt::DescendingOrder);
}

transfers_widget::~transfers_widget() {
    Preferences pref;
    pref.setShowAllTransfers(actionShow_all_transfers->isChecked());
    pref.setValue("TransferListHeader", trView->header()->saveState());
    pref.setValue("PeerListHeader", peerView->header()->saveState());
}

void transfers_widget::displayListMenu(const QPoint&) {
    QModelIndexList selected = trView->selectionModel()->selectedRows();
    const QStringList hashes = getSelectedHashes();
    bool has_start = false;
    bool has_pause = false;
    bool has_preview = false;
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
    actionED2K_link->setDisabled(hashes.isEmpty());
    actionRemove->setDisabled(hashes.isEmpty());
    actionRename->setDisabled(hashes.size() != 1);

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

void transfers_widget::keyPressEvent( QKeyEvent * event)
{
    if (focusWidget() == trView)
    {
        qDebug() << event;
        if (event->key() == Qt::Key_L && (event->modifiers().testFlag(Qt::ControlModifier))) {
            on_actionLoad_ED2K_link_triggered();
        } else
        {
            const QStringList hashes = getSelectedHashes();
            foreach (const QString &hash, hashes) {
                if (event->key() == Qt::Key_Delete) {
                    tmodel->removeTransfer(hash);
                    Session::instance()->deleteTransfer(hash, true);
                }
                else if (event->key() == Qt::Key_Return) {
                    QED2KHandle h = Session::instance()->getTransfer(hash);
                    if (h.is_valid())
                    {
                        if (h.is_seed()) QDesktopServices::openUrl(QUrl(h.filepath()));
                        else if (h.is_paused()) h.resume();
                        else h.pause();
                    }
                }
            }
        }
    }
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

QModelIndex transfers_widget::mapToSource(const QModelIndex& index) const {
    Q_ASSERT(index.isValid());
    QModelIndex res = tsort_model->mapToSource(index);
    Q_ASSERT(res.isValid());
    return res;
}

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
    ed2k_link_maker dlg(this);
    const QStringList hashes = getSelectedHashes();
    foreach (const QString &hash, hashes) {
        QED2KHandle h = Session::instance()->getTransfer(hash);
        if (h.is_valid()) {
            dlg.addED2KLink(h.filename(), h.hash(), h.filesize());
        }
    }

    dlg.build();
    dlg.exec();
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

void transfers_widget::trSortChanged(int logicalIndex, Qt::SortOrder order) {
    tsort_model->sort(logicalIndex, order);
}

void transfers_widget::pSortChanged(int logicalIndex, Qt::SortOrder order) {
    psort_model->sort(logicalIndex, order);
}

void transfers_widget::on_actionShow_all_transfers_triggered()
{
    if (actionShow_all_transfers->isChecked()) {
        tsort_model->setFilterFixedString("");
    } else {
        tsort_model->setFilterFixedString("N");
    }
}

void transfers_widget::displayHSMenu(const QPoint&) {
    QMenu hideshowColumn(this);
    hideshowColumn.setTitle(tr("Column visibility"));
    QList<QAction*> actions;
    for (int i=0; i < tmodel->columnCount(); ++i) {
        QAction *myAct = hideshowColumn.addAction(
            tmodel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        myAct->setCheckable(true);
        myAct->setChecked(!trView->isColumnHidden(i));
        actions.append(myAct);
    }

    QAction *act = hideshowColumn.exec(QCursor::pos());
    if (act) {
        int col = actions.indexOf(act);
        Q_ASSERT(col >= 0);
        trView->setColumnHidden(col, !trView->isColumnHidden(col));
        if (!trView->isColumnHidden(col) && trView->columnWidth(col) <= 5)
            trView->setColumnWidth(col, 100);
    }
}

void transfers_widget::on_trView_doubleClicked(const QModelIndex &index)
{
    QString hash = getHashFromRow(mapToSource(index).row());
    QED2KHandle handle = Session::instance()->getTransfer(hash);
    if (handle.is_valid()) {
        if (handle.is_seed()) QDesktopServices::openUrl(QUrl(Session::instance()->getTransfer(hash).filepath()));
        else if (handle.is_paused()) handle.resume();
        else handle.pause();
    }
}

void transfers_widget::on_actionLoad_ED2K_link_triggered()
{
    bool ok;
    QString clipboardText = QApplication::clipboard()->text();
    QString link = clipboardText.startsWith("ed2k://", Qt::CaseInsensitive)?clipboardText : "";
    link = QInputDialog::getText(this, tr("Add ED2K link"), tr("ED2K link:"), QLineEdit::Normal, link, &ok);
    if (ok && !link.isEmpty())
    {
        if ((link.length() > 1024))
        {
            QMessageBox::critical(this, tr("Add ED2K link"), tr("Link too long"));
            return;
        }

        if (!link.endsWith("|/")) link += "|/";

        QPair<QED2KHandle,ErrorCode> res = Session::instance()->addLink(link.trimmed());
        if (!res.first.is_valid()) QMessageBox::critical(this, tr("Add ED2K link"), res.second? tr(res.second.message().c_str()) : tr("Incorrect link"));
    }
}
