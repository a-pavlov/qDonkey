/*
 * Bittorrent Client using Qt4 and libtorrent.
 * Copyright (C) 2006  Christophe Dumez
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * Contact : chris@qbittorrent.org
 */

#include <QStandardItemModel>
#include <QDesktopServices>
#include <QTimer>
#include <QClipboard>
#include <QInputDialog>
#include <QColor>
#include <QUrl>
#include <QMenu>
#include <QRegExp>
#include <QFileDialog>
#include <QMessageBox>

#include <libtorrent/version.hpp>
#include <vector>
#include <queue>

#include "transport/session.h"
#include "transferlistwidget.h"
#include "torrentpersistentdata.h"
#include "transferlistdelegate.h"
#include "previewselect.h"
#include "speedlimitdlg.h"
#include "updownratiodlg.h"
#include "options_imp.h"
#include "preferences.h"
#include "torrentmodel.h"
#include "deletionconfirmationdlg.h"
#include "iconprovider.h"
#include "torrent_properties.h"
#include "ed2k_link_maker.h"
#include "res.h"

using namespace libtorrent;

TransfersFilterModel::TransfersFilterModel(QObject *parent)
    :
      QSortFilterProxyModel(parent),
      m_filter(Filters::All)
{}

TransfersFilterModel::~TransfersFilterModel()
{}

bool TransfersFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_filter == Filters::All)
        return true;

    if (m_filter == Filters::Torrents)
    {
        const QModelIndex source_index = sourceModel()->index(source_row, TorrentModelItem::TR_TYPE, source_parent);

        if (!source_index.isValid())
            return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

        return sourceModel()->data(source_index).value<Transfer::Type>() == Transfer::Types::BITTORRENT;
    }

    // the rest is STATUS filters
    const QModelIndex source_index = sourceModel()->index(source_row, TorrentModelItem::TR_STATUS, source_parent);

    if (!source_index.isValid())
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

    const TorrentModelItem::State state = sourceModel()->data(source_index).value<TorrentModelItem::State>();

    switch(state)
    {
        case TorrentModelItem::States::DOWNLOADING:
            return (m_filter == Filters::Downloading);
        case TorrentModelItem::States::STALLED_UP:
        case TorrentModelItem::States::SEEDING:
            return (m_filter == Filters::Completed);
        case TorrentModelItem::States::PAUSED_DL:
        case TorrentModelItem::States::PAUSED_UP:
        case TorrentModelItem::States::STALLED_DL:
        case TorrentModelItem::States::CHECKING_DL:
        case TorrentModelItem::States::CHECKING_UP:
        case TorrentModelItem::States::QUEUED_DL:
        case TorrentModelItem::States::QUEUED_UP:
        case TorrentModelItem::States::INVALID:
            return (m_filter == Filters::Waiting);
        default:
            return false;
    }
}

void TransfersFilterModel::show(Filter filter)
{
    m_filter = filter;
    invalidateFilter();
}

//--------------------TransferListWidget----------------------------

TransferListWidget::TransferListWidget(QTreeView *ui, Session *_BTSession)
    :
      ui(ui),
      BTSession(_BTSession)
{
  Q_ASSERT(ui);

  ui->setIconSize(QSize(22, 22));

  // Load settings
  bool column_loaded = loadSettings();

  // Create and apply delegate
  listDelegate = new TransferListDelegate(ui);
  ui->setItemDelegate(listDelegate);

  // Create transfer list model
  listModel = new TorrentModel(ui);

  // Set Sort/Filter proxy
  transfersFilterModel = new TransfersFilterModel;
  transfersFilterModel->setDynamicSortFilter(true);
  transfersFilterModel->setSourceModel(listModel);

  labelFilterModel = new QSortFilterProxyModel();
  labelFilterModel->setDynamicSortFilter(true);
  labelFilterModel->setSourceModel(transfersFilterModel);
  labelFilterModel->setFilterKeyColumn(TorrentModelItem::TR_LABEL);
  labelFilterModel->setFilterRole(Qt::DisplayRole);

  statusFilterModel = new QSortFilterProxyModel();
  statusFilterModel->setDynamicSortFilter(true);
  statusFilterModel->setSourceModel(labelFilterModel);
  statusFilterModel->setFilterKeyColumn(TorrentModelItem::TR_STATUS);
  statusFilterModel->setFilterRole(Qt::DisplayRole);

  nameFilterModel = new QSortFilterProxyModel();
  nameFilterModel->setDynamicSortFilter(true);
  nameFilterModel->setSourceModel(statusFilterModel);
  nameFilterModel->setFilterKeyColumn(TorrentModelItem::TR_NAME);
  nameFilterModel->setFilterRole(Qt::DisplayRole);
  nameFilterModel->setSortCaseSensitivity(Qt::CaseInsensitive);

  ui->setModel(nameFilterModel);
  ui->setColumnWidth(0, 500);

  // Visual settings
  ui->setRootIsDecorated(false);
  ui->setAllColumnsShowFocus(true);
  ui->setSortingEnabled(true);
  ui->setSelectionMode(QAbstractItemView::ExtendedSelection);
  ui->setItemsExpandable(false);
  ui->setAutoScroll(true);
  ui->setDragDropMode(QAbstractItemView::DragOnly);

  // Default hidden columns
  if (!column_loaded) {
    ui->setColumnHidden(TorrentModelItem::TR_PRIORITY, true);
    ui->setColumnHidden(TorrentModelItem::TR_TYPE, true);
    ui->setColumnHidden(TorrentModelItem::TR_HASH, true);
    ui->setColumnHidden(TorrentModelItem::TR_STATUS, true);
    ui->setColumnHidden(TorrentModelItem::TR_ADD_DATE, true);
    ui->setColumnHidden(TorrentModelItem::TR_SEED_DATE, true);
    ui->setColumnHidden(TorrentModelItem::TR_UPLIMIT, true);
    ui->setColumnHidden(TorrentModelItem::TR_DLLIMIT, true);
    ui->setColumnHidden(TorrentModelItem::TR_TRACKER, true);
    ui->setColumnHidden(TorrentModelItem::TR_AMOUNT_DOWNLOADED, true);
    ui->setColumnHidden(TorrentModelItem::TR_AMOUNT_LEFT, true);
    ui->setColumnHidden(TorrentModelItem::TR_TIME_ELAPSED, true);
    ui->setColumnHidden(TorrentModelItem::TR_LABEL, true);
    ui->setColumnHidden(TorrentModelItem::TR_FILE_TYPE, true);
  }

  ui->setContextMenuPolicy(Qt::CustomContextMenu);

  // Listen for list events
  connect(ui, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(torrentDoubleClicked(QModelIndex)));
  connect(ui, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(displayListMenu(const QPoint&)));
  ui->header()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(displayDLHoSMenu(const QPoint&)));

  actionAddLink = new QAction(QIcon(res::itemED2KLink()), tr("Add link..."), ui);
  actionAddLink->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+V")));
  ui->addAction(actionAddLink);
  connect(actionAddLink, SIGNAL(triggered()), this, SLOT(addLinkDialog()));

  actionOpen = new QAction(IconProvider::instance()->getIcon("list-add"), tr("Add torrent file"), ui);
  actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
  actionOpen->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+O")));
  ui->addAction(actionOpen);
  connect(actionOpen, SIGNAL(triggered()), SIGNAL(sigOpenTorrent()));

  actionDelete = new QAction(IconProvider::instance()->getIcon("edit-delete"), tr("Delete", "Delete the torrent"), ui);
  actionDelete->setShortcut(QKeySequence(QString::fromUtf8("Del")));
  ui->addAction(actionDelete);
  connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteSelectedTorrents()));

  actionLaunch = new QAction(ui);
  actionLaunch->setShortcut(QKeySequence(QString::fromUtf8("Return")));
  ui->addAction(actionLaunch);
  connect(actionLaunch, SIGNAL(triggered()), this, SLOT(launchSelectedTorrents()));
}

TransferListWidget::~TransferListWidget() {
  qDebug() << Q_FUNC_INFO << "ENTER";

  // Save settings
  saveSettings();

  // Clean up
  delete labelFilterModel;
  delete statusFilterModel;
  delete nameFilterModel;
  delete listModel;
  delete listDelegate;
  qDebug() << Q_FUNC_INFO << "EXIT";
}

TorrentModel* TransferListWidget::getSourceModel() const {
  return listModel;
}

void TransferListWidget::previewFile(Transfer t, int fileIndex) {
    BTSession->deferPlayMedia(t, fileIndex);
}

void TransferListWidget::setRefreshInterval(int t) {
  qDebug("Settings transfer list refresh interval to %dms", t);
  listModel->setRefreshInterval(t);
}

int TransferListWidget::getRowFromHash(QString hash) const {
  return listModel->torrentRow(hash);
}

inline QString TransferListWidget::getHashFromRow(const QModelIndex &index) const {
  return index.model()->data(index.model()->index(index.row(), TorrentModelItem::TR_HASH)).toString();
}

inline QModelIndex TransferListWidget::mapToSource(const QModelIndex &index) const {
  Q_ASSERT(index.isValid());
  if (index.model() == nameFilterModel)
    return labelFilterModel->mapToSource(statusFilterModel->mapToSource(nameFilterModel->mapToSource(index)));
  if (index.model() == statusFilterModel)
    return labelFilterModel->mapToSource(statusFilterModel->mapToSource(index));
  return labelFilterModel->mapToSource(index);
}

inline QModelIndex TransferListWidget::mapFromSource(const QModelIndex &index) const {
  Q_ASSERT(index.isValid());
  Q_ASSERT(index.model() == labelFilterModel);
  return nameFilterModel->mapFromSource(statusFilterModel->mapFromSource(labelFilterModel->mapFromSource(index)));
}


QStringList TransferListWidget::getCustomLabels() const
{
    return Preferences().value("TransferListFilters/customLabels", QStringList()).toStringList();
}

void TransferListWidget::torrentDoubleClicked(const QModelIndex& index) {
  const QString hash = index.model()->data(index.model()->index(index.row(), TorrentModelItem::TR_HASH)).toString();//mapToSource(index).row();
  Transfer h = BTSession->getTransfer(hash);
  if (!h.is_valid()) return;
  int action;
  if (h.is_seed()) {
    action = Preferences().getActionOnDblClOnTorrentFn();
  } else {
    action = Preferences().getActionOnDblClOnTorrentDl();
  }

  switch(action) {
  case TOGGLE_PAUSE:
    if (h.is_paused()) {
      h.resume();
    } else {
      h.pause();
    }
    break;
  case OPEN_DEST:
    QString link = h.num_files() == 1 ? h.absolute_files_path().at(0) : h.save_path();
    QDesktopServices::openUrl(QUrl::fromLocalFile(link));
    break;
  }
}

QStringList TransferListWidget::getSelectedTorrentsHashes() const {
  QStringList hashes;
  const QModelIndexList selectedIndexes = ui->selectionModel()->selectedRows();
  foreach (const QModelIndex &index, selectedIndexes) {
    hashes << index.model()->data(index.model()->index(index.row(), TorrentModelItem::TR_HASH)).toString();
    //hashes << getHashFromRow(mapToSource(index).row());
  }
  return hashes;
}

void TransferListWidget::setSelectedTorrentsLocation() {
  const QStringList hashes = getSelectedTorrentsHashes();
  if (hashes.isEmpty()) return;
  QString dir;
  const QDir saveDir(TorrentPersistentData::getSavePath(hashes.first()));
  qDebug("Old save path is %s", qPrintable(saveDir.absolutePath()));
  dir = QFileDialog::getExistingDirectory(ui, tr("Choose save path"), saveDir.absolutePath(),
                                          QFileDialog::DontConfirmOverwrite|QFileDialog::ShowDirsOnly|QFileDialog::HideNameFilterDetails);
  if (!dir.isNull()) {
    qDebug("New path is %s", qPrintable(dir));
    // Check if savePath exists
    QDir savePath(misc::expandPath(dir));
    qDebug("New path after clean up is %s", qPrintable(savePath.absolutePath()));
    foreach (const QString & hash, hashes) {
      // Actually move storage
      Transfer h = BTSession->getTransfer(hash);
      if (!BTSession->useTemporaryFolder() || h.is_seed()) {
        if (!savePath.exists()) savePath.mkpath(savePath.absolutePath());
        h.move_storage(savePath.absolutePath());
      } else {
        TorrentPersistentData::saveSavePath(h.hash(), savePath.absolutePath());
//        main_window->getProperties()->updateSavePath(h);
      }
    }
  }
}

void TransferListWidget::startSelectedTorrents() {
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    BTSession->resumeTransfer(hash);
  }
}

void TransferListWidget::startVisibleTorrents() {
  QStringList hashes;
  for (int i=0; i<ui->model()->rowCount(); ++i) {
    hashes << getHashFromRow(ui->model()->index(i, 0));
  }
  foreach (const QString &hash, hashes) {
    BTSession->resumeTransfer(hash);
  }
}

void TransferListWidget::pauseSelectedTorrents() {
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    BTSession->pauseTransfer(hash);
  }
}

void TransferListWidget::pauseVisibleTorrents() {
  QStringList hashes;
  for (int i=0; i<ui->model()->rowCount(); ++i) {
    hashes << getHashFromRow(ui->model()->index(i, 0));
  }
  foreach (const QString &hash, hashes) {
    BTSession->pauseTransfer(hash);
  }
}

void TransferListWidget::deleteSelectedTorrents()
{
    //  if (main_window->getCurrentTabWidget() != this) return;
  const QStringList& hashes = getSelectedTorrentsHashes();
  if (hashes.empty()) return;
  bool delete_local_files = false;
  bool file_control_active = false;

  // search torrent or completed ed2k transfer and active file control
  foreach (const QString &hash, hashes)
  {
      Transfer t = BTSession->getTransfer(hash);
      if (t.type() == Transfer::Types::ED2K &&  !t.is_seed())
          continue;

      file_control_active = true;
      break;
  }

  if (Preferences().confirmTorrentDeletion() &&
      !DeletionConfirmationDlg::askForDeletionConfirmation(file_control_active, &delete_local_files))
    return;

    foreach (const QString &hash, hashes)
    {
        Transfer t = BTSession->getTransfer(hash);

        try
        {
            if (t.type() == Transfer::Types::ED2K && t.is_seed() && !delete_local_files)
            {
                // delete view only
                listModel->removeTorrent(hash);
            } // delete bt files by flag, delete ed2k files by flag or ed2k + always on not seed
            else
                BTSession->deleteTransfer(hash, delete_local_files || (!t.is_seed() && (t.type() == Transfer::Types::ED2K)));

        }
        catch(const libtorrent::libtorrent_exception& )
        {
        }
    }
}

void TransferListWidget::deleteVisibleTorrents()
{
  if (nameFilterModel->rowCount() <= 0) return;
  bool delete_local_files = false;
  if (Preferences().confirmTorrentDeletion() &&
      !DeletionConfirmationDlg::askForDeletionConfirmation(true, &delete_local_files)) // TODO - delete it?
    return;
  QStringList hashes;
  for (int i=0; i<ui->model()->rowCount(); ++i) {
    hashes << getHashFromRow(ui->model()->index(i, 0));
  }
  foreach (const QString &hash, hashes) {
    BTSession->deleteTransfer(hash, delete_local_files);
  }
}

void TransferListWidget::launchSelectedTorrents()
{
    const QModelIndexList selectedIndexes = ui->selectionModel()->selectedRows();
    if (selectedIndexes.size() == 1)
        torrentDoubleClicked(selectedIndexes.first());
}

void TransferListWidget::increasePrioSelectedTorrents() {
  qDebug() << Q_FUNC_INFO;
//  if (main_window->getCurrentTabWidget() != this) return;
  const QStringList hashes = getSelectedTorrentsHashes();
  std::priority_queue<QPair<int, Transfer>, std::vector<QPair<int, Transfer> >, 
                      std::greater<QPair<int, Transfer> > > torrent_queue;
  // Sort torrents by priority
  foreach (const QString &hash, hashes) {
    try {
      Transfer h = BTSession->getTransfer(hash);
      if (!h.is_seed()) {
        torrent_queue.push(qMakePair(h.queue_position(), h));
      }
    }catch(invalid_handle&) {}
  }
  // Increase torrents priority (starting with the ones with highest priority)
  while(!torrent_queue.empty()) {
    Transfer h = torrent_queue.top().second;
    try {
      h.queue_position_up();
    } catch(invalid_handle& h) {}
    torrent_queue.pop();
  }
}

void TransferListWidget::decreasePrioSelectedTorrents() {
  qDebug() << Q_FUNC_INFO;
//  if (main_window->getCurrentTabWidget() != this) return;
  const QStringList hashes = getSelectedTorrentsHashes();
  std::priority_queue<QPair<int, Transfer>, std::vector<QPair<int, Transfer> >,
                      std::less<QPair<int, Transfer> > > torrent_queue;
  // Sort torrents by priority
  foreach (const QString &hash, hashes) {
    try {
      Transfer h = BTSession->getTransfer(hash);
      if (!h.is_seed()) {
        torrent_queue.push(qMakePair(h.queue_position(), h));
      }
    }catch(invalid_handle&) {}
  }
  // Decrease torrents priority (starting with the ones with lowest priority)
  while(!torrent_queue.empty()) {
    Transfer h = torrent_queue.top().second;
    try {
      h.queue_position_down();
    } catch(invalid_handle& h) {}
    torrent_queue.pop();
  }
}

void TransferListWidget::topPrioSelectedTorrents() {
//  if (main_window->getCurrentTabWidget() != this) return;
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && !h.is_seed()) {
      h.queue_position_top();
    }
  }
}

void TransferListWidget::bottomPrioSelectedTorrents() {
//  if (main_window->getCurrentTabWidget() != this) return;
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && !h.is_seed()) {
      h.queue_position_bottom();
    }
  }
}

void TransferListWidget::copySelectedMagnetURIs() const {
  QStringList magnet_uris;
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    const Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && h.has_metadata())
      magnet_uris << misc::toQString(make_magnet_uri(h.get_info()));
  }
  qApp->clipboard()->setText(magnet_uris.join("\n"));
}

void TransferListWidget::hidePriorityColumn(bool hide) {
  qDebug("hidePriorityColumn(%d)", hide);
  ui->setColumnHidden(TorrentModelItem::TR_PRIORITY, hide);
}

void TransferListWidget::addLinkDialog() {
  Transfer t;
  bool ok;
  ErrorCode ec;
  QString clipboardText = QApplication::clipboard()->text();
  QString link =
      clipboardText.startsWith("ed2k://", Qt::CaseInsensitive) ||
      clipboardText.startsWith("magnet:", Qt::CaseInsensitive) ? clipboardText : "";

  do {
    link = QInputDialog::getText(
      ui, tr("Add link..."), tr("ED2K/magnet link:"), QLineEdit::Normal, link, &ok);
    if (ok && !link.isEmpty())
    {
        // Qt input dialog allow only 32K text and long link will be truncated, so we must normalize links end
        if (link.startsWith("ed2k://") && (link.length() > 30*1024) && !link.endsWith("|/"))
        {
            link += "|/";
        }

        QPair<Transfer,ErrorCode> res = BTSession->addLink(link.trimmed());
        t = res.first;
        ec = res.second;

        if (!t.is_valid())
            QMessageBox::critical(ui, tr("Error"), tr(ec ? ec.message().c_str() : "Incorrect link"));
    }
  } while(ok && !t.is_valid());
}

void TransferListWidget::openSelectedTorrentsFolder() const {
  QStringList pathsList;
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    const Transfer h = BTSession->getTransfer(hash);
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
    QDesktopServices::openUrl(QUrl::fromLocalFile(pref.getSavePathTorrent()));
  }
}

void TransferListWidget::previewSelectedTorrents() {
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    const Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && h.has_metadata()) {
      new PreviewSelect(ui, h);
    }
  }
}

void TransferListWidget::setDlLimitSelectedTorrents() {
  QList<Transfer> selected_torrents;
  bool first = true;
  bool all_same_limit = true;
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    const Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && !h.is_seed()) {
      selected_torrents << h;
      // Determine current limit for selected torrents
      if (first) {
        first = false;
      } else {
        if (all_same_limit && h.download_limit() != selected_torrents.first().download_limit())
          all_same_limit = false;
      }
    }
  }
  if (selected_torrents.empty()) return;

  bool ok=false;
  int default_limit = -1;
  if (all_same_limit)
    default_limit = selected_torrents.first().download_limit();
  const long new_limit = SpeedLimitDialog::askSpeedLimit(&ok, tr("Torrent Download Speed Limiting"), default_limit, Preferences().getGlobalDownloadLimit()*1024.);
  if (ok) {
    foreach (const Transfer &h, selected_torrents) {
      qDebug("Applying download speed limit of %ld Kb/s to torrent %s", (long)(new_limit/1024.), qPrintable(h.hash()));
      BTSession->setDownloadLimit(h.hash(), new_limit);
    }
  }
}

void TransferListWidget::setUpLimitSelectedTorrents() {
  QList<Transfer> selected_torrents;
  bool first = true;
  bool all_same_limit = true;
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    const Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid()) {
      selected_torrents << h;
      // Determine current limit for selected torrents
      if (first) {
        first = false;
      } else {
        if (all_same_limit && h.upload_limit() != selected_torrents.first().upload_limit())
          all_same_limit = false;
      }
    }
  }
  if (selected_torrents.empty()) return;

  bool ok=false;
  int default_limit = -1;
  if (all_same_limit)
    default_limit = selected_torrents.first().upload_limit();
  const long new_limit = SpeedLimitDialog::askSpeedLimit(&ok, tr("Torrent Upload Speed Limiting"), default_limit, Preferences().getGlobalUploadLimit()*1024.);
  if (ok) {
    foreach (const Transfer &h, selected_torrents) {
      qDebug("Applying upload speed limit of %ld Kb/s to torrent %s", (long)(new_limit/1024.), qPrintable(h.hash()));
      BTSession->setUploadLimit(h.hash(), new_limit);
    }
  }
}

void TransferListWidget::setMaxRatioSelectedTorrents() {
  const QStringList hashes = getSelectedTorrentsHashes();
  if (hashes.isEmpty())
    return;
  bool useGlobalValue;
  qreal currentMaxRatio;
  if (hashes.count() == 1) {
    currentMaxRatio = BTSession->getMaxRatioPerTransfer(hashes.first(), &useGlobalValue);
  } else {
    useGlobalValue = true;
    currentMaxRatio = BTSession->getGlobalMaxRatio();
  }
  UpDownRatioDlg dlg(useGlobalValue, currentMaxRatio, Session::MAX_RATIO, ui);
  if (dlg.exec() != QDialog::Accepted)
    return;
  foreach (const QString &hash, hashes) {
    if (dlg.useDefault())
      BTSession->removeRatioPerTransfer(hash);
    else
      BTSession->setMaxRatioPerTransfer(hash, dlg.ratio());
  }
}

void TransferListWidget::recheckSelectedTorrents() {
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    BTSession->recheckTransfer(hash);
  }
}

// hide/show columns menu
void TransferListWidget::displayDLHoSMenu(const QPoint&) {
  QMenu hideshowColumn(ui);
  hideshowColumn.setTitle(tr("Column visibility"));
  QList<QAction*> actions;

  for (int i=0; i < listModel->columnCount(); ++i) {
    if (!BTSession->isQueueingEnabled() && i == TorrentModelItem::TR_PRIORITY) {
      actions.append(0);
      continue;
    }

    QAction *myAct = hideshowColumn.addAction(listModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
    myAct->setCheckable(true);
    myAct->setChecked(!ui->isColumnHidden(i));
    actions.append(myAct);
  }
  // Call menu
  QAction *act = hideshowColumn.exec(QCursor::pos());
  if (act) {
    int col = actions.indexOf(act);
    Q_ASSERT(col >= 0);
    qDebug("Toggling column %d visibility", col);
    ui->setColumnHidden(col, !ui->isColumnHidden(col));
    if (!ui->isColumnHidden(col) && ui->columnWidth(col) <= 5)
      ui->setColumnWidth(col, 100);
  }
}

void TransferListWidget::toggleSelectedTorrentsSuperSeeding() const {
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && h.has_metadata()) {
      h.super_seeding(!h.super_seeding());
    }
  }
}

void TransferListWidget::toggleSelectedTorrentsSequentialDownload() const {
  const QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && h.has_metadata()) {
      bool was_sequential = h.is_sequential_download();
      h.set_sequential_download(!was_sequential);
      if (!was_sequential)
        h.prioritize_extremity_pieces(true);
    }
  }
}

void TransferListWidget::toggleSelectedFirstLastPiecePrio() const {
  QStringList hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    Transfer h = BTSession->getTransfer(hash);
    if (h.is_valid() && h.has_metadata()) {
      h.prioritize_extremity_pieces(!h.extremity_pieces_first());
    }
  }
}

void TransferListWidget::askNewLabelForSelection() {
  // Ask for label
  bool ok;
  bool invalid;
  do {
    invalid = false;
    const QString label = QInputDialog::getText(ui, tr("New Label"), tr("Label:"), QLineEdit::Normal, "", &ok);
    if (ok && !label.isEmpty()) {
      if (misc::isValidFileSystemName(label)) {
        setSelectionLabel(label);
      } else {
        QMessageBox::warning(ui, tr("Invalid label name"), tr("Please don't use any special characters in the label name."));
        invalid = true;
      }
    }
  }while(invalid);
}

void TransferListWidget::renameSelectedTorrent() {
  const QModelIndexList selectedIndexes = ui->selectionModel()->selectedRows();
  if (selectedIndexes.size() != 1) return;
  if (!selectedIndexes.first().isValid()) return;
  const QString hash = getHashFromRow(selectedIndexes.first());
  const Transfer h = BTSession->getTransfer(hash);
  if (!h.is_valid()) return;
  // Ask for a new Name
  bool ok;
  const QString name = QInputDialog::getText(ui, tr("Rename"), tr("New name:"), QLineEdit::Normal, h.name(), &ok);
  if (ok && !name.isEmpty()) {
    if (h.type() == Transfer::Types::ED2K) h.rename_file(0, name);
    // Rename the transfer
    nameFilterModel->setData(selectedIndexes.first(), name, Qt::DisplayRole);
  }
}

void TransferListWidget::setSelectionLabel(QString label) {
  const QStringList& hashes = getSelectedTorrentsHashes();
  foreach (const QString &hash, hashes) {
    Q_ASSERT(!hash.isEmpty());
    const int row = getRowFromHash(hash);
    const QString old_label = listModel->data(listModel->index(row, TorrentModelItem::TR_LABEL)).toString();
    listModel->setData(listModel->index(row, TorrentModelItem::TR_LABEL), QVariant(label), Qt::DisplayRole);
    // Update save path if necessary
    Transfer h = BTSession->getTransfer(hash);
    BTSession->changeLabelInSavePath(h, old_label, label);
  }
}

void TransferListWidget::removeLabelFromRows(QString label) {
  for (int i=0; i<listModel->rowCount(); ++i) {
    const QModelIndex index = listModel->index(i, TorrentModelItem::TR_LABEL);
    if (listModel->data(index) == label) {
      const QString hash = getHashFromRow(index);
      listModel->setData(index, "", Qt::DisplayRole);
      // Update save path if necessary
      Transfer h = BTSession->getTransfer(hash);
      BTSession->changeLabelInSavePath(h, label, "");
    }
  }
}

void TransferListWidget::displayListMenu(const QPoint&) {
  QModelIndexList selectedIndexes = ui->selectionModel()->selectedRows();

  // Create actions
  QAction actionStart(IconProvider::instance()->getIcon("media-playback-start"), tr("Resume", "Resume/start the torrent"), 0);
  connect(&actionStart, SIGNAL(triggered()), this, SLOT(startSelectedTorrents()));
  QAction actionPause(IconProvider::instance()->getIcon("media-playback-pause"), tr("Pause", "Pause the torrent"), 0);
  connect(&actionPause, SIGNAL(triggered()), this, SLOT(pauseSelectedTorrents()));
  QAction actionPreview_file(IconProvider::instance()->getIcon("view-preview"), tr("Preview file..."), 0);
  connect(&actionPreview_file, SIGNAL(triggered()), this, SLOT(previewSelectedTorrents()));
  QAction actionView_file(IconProvider::instance()->getIcon("view-preview"), tr("View file..."), 0);
  connect(&actionView_file, SIGNAL(triggered()), this, SLOT(previewSelectedTorrents()));
  QAction actionSet_max_ratio(QIcon(res::ratio()), tr("Limit share ratio..."), 0);
  connect(&actionSet_max_ratio, SIGNAL(triggered()), this, SLOT(setMaxRatioSelectedTorrents()));
  QAction actionSet_upload_limit(QIcon(res::upload()), tr("Limit upload rate..."), 0);
  connect(&actionSet_upload_limit, SIGNAL(triggered()), this, SLOT(setUpLimitSelectedTorrents()));
  QAction actionSet_download_limit(QIcon(res::download()), tr("Limit download rate..."), 0);
  connect(&actionSet_download_limit, SIGNAL(triggered()), this, SLOT(setDlLimitSelectedTorrents()));
  QAction actionOpen_destination_folder(IconProvider::instance()->getIcon("inode-directory"), tr("Open destination folder"), 0);
  connect(&actionOpen_destination_folder, SIGNAL(triggered()), this, SLOT(openSelectedTorrentsFolder()));
  QAction actionIncreasePriority(IconProvider::instance()->getIcon("go-up"), tr("Move up", "i.e. move up in the queue"), 0);
  connect(&actionIncreasePriority, SIGNAL(triggered()), this, SLOT(increasePrioSelectedTorrents()));
  QAction actionDecreasePriority(IconProvider::instance()->getIcon("go-down"), tr("Move down", "i.e. Move down in the queue"), 0);
  connect(&actionDecreasePriority, SIGNAL(triggered()), this, SLOT(decreasePrioSelectedTorrents()));
  QAction actionTopPriority(IconProvider::instance()->getIcon("go-top"), tr("Move to top", "i.e. Move to top of the queue"), 0);
  connect(&actionTopPriority, SIGNAL(triggered()), this, SLOT(topPrioSelectedTorrents()));
  QAction actionBottomPriority(IconProvider::instance()->getIcon("go-bottom"), tr("Move to bottom", "i.e. Move to bottom of the queue"), 0);
  connect(&actionBottomPriority, SIGNAL(triggered()), this, SLOT(bottomPrioSelectedTorrents()));
  QAction actionSetTorrentPath(IconProvider::instance()->getIcon("inode-directory"), tr("Set location..."), 0);
  connect(&actionSetTorrentPath, SIGNAL(triggered()), this, SLOT(setSelectedTorrentsLocation()));
  QAction actionForce_recheck(IconProvider::instance()->getIcon("document-edit-verify"), tr("Force recheck"), 0);
  connect(&actionForce_recheck, SIGNAL(triggered()), this, SLOT(recheckSelectedTorrents()));
  QAction actionCopy_magnet_link(QIcon(res::itemMagnetLink()), tr("Copy magnet link"), 0);
  connect(&actionCopy_magnet_link, SIGNAL(triggered()), this, SLOT(copySelectedMagnetURIs()));
  QAction actionSuper_seeding_mode(tr("Super seeding mode"), 0);
  actionSuper_seeding_mode.setCheckable(true);
  connect(&actionSuper_seeding_mode, SIGNAL(triggered()), this, SLOT(toggleSelectedTorrentsSuperSeeding()));
  QAction actionRename(IconProvider::instance()->getIcon("edit-rename"), tr("Rename..."), 0);
  connect(&actionRename, SIGNAL(triggered()), this, SLOT(renameSelectedTorrent()));
  QAction actionSequential_download(tr("Download in sequential order"), 0);
  actionSequential_download.setCheckable(true);
  connect(&actionSequential_download, SIGNAL(triggered()), this, SLOT(toggleSelectedTorrentsSequentialDownload()));
  QAction actionFirstLastPiece_prio(tr("Download first and last piece first"), 0);
  actionFirstLastPiece_prio.setCheckable(true);
  connect(&actionFirstLastPiece_prio, SIGNAL(triggered()), this, SLOT(toggleSelectedFirstLastPiecePrio()));
  QAction actionProperties(QIcon(res::fileInfo()), tr("Properties"), 0);
  connect(&actionProperties, SIGNAL(triggered()), this, SLOT(showProperties()));
  QAction actionED2KLink(QIcon(res::itemED2KLink()), tr("ED2K link"), 0);
  connect(&actionED2KLink, SIGNAL(triggered()), this, SLOT(createED2KLink()));

  // End of actions
  QMenu listMenu(ui);
  // Enable/disable pause/start action given the DL state
  bool has_pause = false, has_start = false, has_preview = false;
  bool all_same_super_seeding = true;
  bool super_seeding_mode = false;
  bool all_same_sequential_download_mode = true, all_same_prio_firstlast = true;
  bool sequential_download_mode = false, prioritize_first_last = false;
  bool one_has_metadata = false, one_seed = false, one_not_seed = false, one_is_bittorrent = false;
  bool first = true;
  bool has_view = true;
  Transfer h;
  qDebug("Displaying menu");
  foreach (const QModelIndex &index, selectedIndexes) {
    // Get the file name
    QString hash = getHashFromRow(index);
    // Get handle and pause the torrent
    h = BTSession->getTransfer(hash);
    if (!h.is_valid()) continue;
    if (h.has_metadata())
      one_has_metadata = true;
    if (h.type() == Transfer::Types::BITTORRENT)
      one_is_bittorrent = true;
    if (!h.is_seed()) {
      one_not_seed = true;
      if (h.has_metadata()) {
        if (first) {
          sequential_download_mode = h.is_sequential_download();
          prioritize_first_last = h.extremity_pieces_first();
        } else {
          if (sequential_download_mode != h.is_sequential_download()) {
            all_same_sequential_download_mode = false;
          }
          if (prioritize_first_last != h.extremity_pieces_first()) {
            all_same_prio_firstlast = false;
          }
        }
      }
    }
    else {
      one_seed = true;
      if (!one_not_seed && all_same_super_seeding && h.has_metadata()) {
        if (first) {
          super_seeding_mode = h.super_seeding();
        } else {
          if (super_seeding_mode != h.super_seeding()) {
            all_same_super_seeding = false;
          }
        }
      }
    }
    if (h.is_paused()) {
      if (!has_start) {
        listMenu.addAction(&actionStart);
        has_start = true;
      }
    }else{
      if (!has_pause) {
        listMenu.addAction(&actionPause);
        has_pause = true;
      }
    }
    if (h.has_metadata() && BTSession->isFilePreviewPossible(hash) && !has_preview) {
      has_preview = true;
    }
    if (has_view && !h.is_seed())
        has_view = false;
    first = false;
    if (has_pause && has_start && has_preview && one_not_seed) break;
  }
  if (selectedIndexes.size() > 0) {
    listMenu.addSeparator();
    listMenu.addAction(actionDelete);
    listMenu.addSeparator();
    if (!one_seed)
      listMenu.addAction(&actionSetTorrentPath);
  }
  if (selectedIndexes.size() == 1 && one_not_seed)
    listMenu.addAction(&actionRename);
  // Label Menu
  QStringList customLabels = getCustomLabels();
  customLabels.sort();
  QList<QAction*> labelActions;
  if (selectedIndexes.size() > 0) {
    QMenu *labelMenu = listMenu.addMenu(
        IconProvider::instance()->getIcon("view-categories"), tr("Label"));
    labelActions << labelMenu->addAction(
        IconProvider::instance()->getIcon("list-add"), tr("New...", "New label..."));
    labelActions << labelMenu->addAction(
        IconProvider::instance()->getIcon("edit-clear"), tr("Reset", "Reset label"));
    labelMenu->addSeparator();
    foreach (const QString &label, customLabels) {
      labelActions << labelMenu->addAction(
          IconProvider::instance()->getIcon("inode-directory"), label);
    }
    // temporary disable label actions
    foreach (QAction* a, labelActions)
      a->setEnabled(false);
  }

  if (one_is_bittorrent)
  {
    // use limits only for bittorrent transfers
    listMenu.addSeparator();
    if (one_not_seed)
      listMenu.addAction(&actionSet_download_limit);
    listMenu.addAction(&actionSet_max_ratio);
    listMenu.addAction(&actionSet_upload_limit);
    if (!one_not_seed && all_same_super_seeding && one_has_metadata) {
      actionSuper_seeding_mode.setChecked(super_seeding_mode);
      listMenu.addAction(&actionSuper_seeding_mode);
    }
  }

  listMenu.addSeparator();
  bool added_preview_action = false;
  if (has_view && has_preview)
  {
      listMenu.addAction(&actionView_file);
      added_preview_action = true;
  }
  else
  {
      if (has_preview) 
      {
        listMenu.addAction(&actionPreview_file);
        added_preview_action = true;
      }
  }
  if (!one_is_bittorrent && selectedIndexes.size() == 1)
  {
      listMenu.addAction(&actionED2KLink);
  }
  if (one_not_seed && one_has_metadata) {
    if (all_same_sequential_download_mode) {
      actionSequential_download.setChecked(sequential_download_mode);
      listMenu.addAction(&actionSequential_download);
      added_preview_action = true;
    }
    if (all_same_prio_firstlast) {
      actionFirstLastPiece_prio.setChecked(prioritize_first_last);
      listMenu.addAction(&actionFirstLastPiece_prio);
      added_preview_action = true;
    }
  }
  if (added_preview_action)
    listMenu.addSeparator();
  if (one_has_metadata && one_is_bittorrent) {
    listMenu.addAction(&actionForce_recheck);
    listMenu.addSeparator();
  }
  if (selectedIndexes.size() == 0) {
    listMenu.addAction(actionAddLink);
    listMenu.addAction(actionOpen);
    listMenu.addSeparator();
  }
  listMenu.addAction(&actionOpen_destination_folder);
  if (BTSession->isQueueingEnabled() && one_not_seed) {
    listMenu.addSeparator();
    QMenu *prioMenu = listMenu.addMenu(tr("Priority"));
    prioMenu->addAction(&actionTopPriority);
    prioMenu->addAction(&actionIncreasePriority);
    prioMenu->addAction(&actionDecreasePriority);
    prioMenu->addAction(&actionBottomPriority);
  }
  listMenu.addSeparator();
  if (one_has_metadata && one_is_bittorrent)
    listMenu.addAction(&actionCopy_magnet_link);
  if (one_is_bittorrent && selectedIndexes.size() == 1)
    listMenu.addAction(&actionProperties);
  // Call menu
  QAction *act = 0;
  act = listMenu.exec(QCursor::pos());
  if (act) {
    // Parse label actions only (others have slots assigned)
    int i = labelActions.indexOf(act);
    if (i >= 0) {
      // Label action
      if (i == 0) {
        // New Label
        askNewLabelForSelection();
      } else {
        QString label = "";
        if (i > 1)
          label = customLabels.at(i-2);
        // Update Label
        setSelectionLabel(label);
      }
    }
  }
}

void TransferListWidget::currentChanged(const QModelIndex& current, const QModelIndex&) {
  qDebug("CURRENT CHANGED");
  Transfer h;
  if (current.isValid()) {
    h = BTSession->getTransfer(getHashFromRow(current));
    // Scroll Fix
    ui->scrollTo(current);
  }
  emit currentTorrentChanged(h);
}

void TransferListWidget::applyLabelFilter(QString label) {
  if (label == "all") {
    labelFilterModel->setFilterRegExp(QRegExp());
    return;
  }
  if (label == "none") {
    labelFilterModel->setFilterRegExp(QRegExp("^$"));
    return;
  }
  qDebug("Applying Label filter: %s", qPrintable(label));
  labelFilterModel->setFilterRegExp(QRegExp("^"+label+"$", Qt::CaseSensitive));
}

void TransferListWidget::applyNameFilter(QString name) {
  nameFilterModel->setFilterRegExp(QRegExp(name, Qt::CaseInsensitive));
}

void TransferListWidget::applyStatusFilter(int f) {
  switch(f) {
  case FILTER_DOWNLOADING:
    statusFilterModel->setFilterRegExp(QRegExp(QString::number(TorrentModelItem::States::DOWNLOADING)+"|"+QString::number(TorrentModelItem::States::STALLED_DL)+"|"+
                                               QString::number(TorrentModelItem::States::PAUSED_DL)+"|"+QString::number(TorrentModelItem::States::CHECKING_DL)+"|"+
                                               QString::number(TorrentModelItem::States::QUEUED_DL), Qt::CaseSensitive));
    break;
  case FILTER_COMPLETED:
    statusFilterModel->setFilterRegExp(QRegExp(QString::number(TorrentModelItem::States::SEEDING)+"|"+QString::number(TorrentModelItem::States::STALLED_UP)+"|"+
                                               QString::number(TorrentModelItem::States::PAUSED_UP)+"|"+QString::number(TorrentModelItem::States::CHECKING_UP)+"|"+
                                               QString::number(TorrentModelItem::States::QUEUED_UP), Qt::CaseSensitive));
    break;
  case FILTER_ACTIVE:
    statusFilterModel->setFilterRegExp(QRegExp(QString::number(TorrentModelItem::States::DOWNLOADING)+"|"+QString::number(TorrentModelItem::States::SEEDING), Qt::CaseSensitive));
    break;
  case FILTER_INACTIVE:
    statusFilterModel->setFilterRegExp(QRegExp("[^"+QString::number(TorrentModelItem::States::DOWNLOADING)+QString::number(TorrentModelItem::States::SEEDING)+"]", Qt::CaseSensitive));
    break;
  case FILTER_PAUSED:
    statusFilterModel->setFilterRegExp(QRegExp(QString::number(TorrentModelItem::States::PAUSED_UP)+"|"+QString::number(TorrentModelItem::States::PAUSED_DL)));
    break;
  default:
    statusFilterModel->setFilterRegExp(QRegExp());
  }
  // Select first item if nothing is selected
  if (ui->selectionModel()->selectedRows(0).empty() && nameFilterModel->rowCount() > 0) {
    qDebug("Nothing is selected, selecting first row: %s", qPrintable(nameFilterModel->index(0, TorrentModelItem::TR_NAME).data().toString()));
    ui->selectionModel()->setCurrentIndex(nameFilterModel->index(0, TorrentModelItem::TR_NAME), QItemSelectionModel::SelectCurrent|QItemSelectionModel::Rows);
  }
}

void TransferListWidget::saveSettings()
{
    Preferences().setValue("TransferList/HeaderState", ui->header()->saveState());
}

bool TransferListWidget::loadSettings()
{
  Preferences settings;
  bool ok = ui->header()->restoreState(settings.value("TransferList/HeaderState").toByteArray());

  if (!ok) {
    ui->header()->resizeSection(0, 200); // Default
  }
  return ok;
}

void TransferListWidget::showProperties()
{
    const QModelIndexList selectedIndexes = ui->selectionModel()->selectedRows();
    const QString hash = getHashFromRow(selectedIndexes[0]);
    Transfer h = BTSession->getTransfer(hash);

    torrent_properties dlg(ui, h);
    dlg.exec();
}

void TransferListWidget::createED2KLink()
{
    const QModelIndexList selectedIndexes = ui->selectionModel()->selectedRows();
    const QString hash = getHashFromRow(selectedIndexes[0]);
    Transfer h = BTSession->getTransfer(hash);
    const QString file_name = h.filename_at(0);
    quint64 file_size = h.filesize_at(0);

    ed2k_link_maker dlg(file_name, hash, file_size, ui);
    dlg.exec();
}

void TransferListWidget::show(TransfersFilterModel::Filter filter)
{
    transfersFilterModel->show(filter);
}

void TransferListWidget::showWaiting()
{
    show(TransfersFilterModel::Filters::Waiting);
}

void TransferListWidget::showDownloading()
{
    show(TransfersFilterModel::Filters::Downloading);
}

void TransferListWidget::showCompleted()
{
    show(TransfersFilterModel::Filters::Completed);
}

void TransferListWidget::showAll()
{
    show(TransfersFilterModel::Filters::All);
}

void TransferListWidget::showTorrents()
{
    show(TransfersFilterModel::Filters::Torrents);
}
