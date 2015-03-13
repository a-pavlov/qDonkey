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

#ifndef TRANSFERLISTWIDGET_H
#define TRANSFERLISTWIDGET_H

#include <QTreeView>
#include <QSortFilterProxyModel>
#include <libtorrent/version.hpp>
#include "transport/transfer.h"

class Session;
class TransferListDelegate;
class TorrentModel;

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

/** To show only records with the specified status
 */
class TransfersFilterModel : public QSortFilterProxyModel
{
public:
    struct Filters
    {
        enum Filter
        {
            Waiting,
            Downloading,
            Completed,
            Torrents,
            All
        };
    };

    typedef Filters::Filter Filter;

private:
    Filter  m_filter;

private:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

public:
    TransfersFilterModel(QObject *parent = 0);
    virtual ~TransfersFilterModel();

    void show(Filter filter);
};

enum TorrentFilter
{
    FILTER_ALL,
    FILTER_DOWNLOADING,
    FILTER_COMPLETED,
    FILTER_PAUSED,
    FILTER_ACTIVE,
    FILTER_INACTIVE
};

class TransferListWidget : public QObject
{
  Q_OBJECT
public:
  TransferListWidget(QTreeView *ui, Session* BTSession);
  ~TransferListWidget();
  TorrentModel* getSourceModel() const;

public slots:
  void setSelectionLabel(QString label);
  void setRefreshInterval(int t);
  void setSelectedTorrentsLocation();
  void startSelectedTorrents();
  void startVisibleTorrents();
  void pauseSelectedTorrents();
  void pauseVisibleTorrents();
  void deleteSelectedTorrents();
  void deleteVisibleTorrents();
  void launchSelectedTorrents();
  void increasePrioSelectedTorrents();
  void decreasePrioSelectedTorrents();
  void topPrioSelectedTorrents();
  void bottomPrioSelectedTorrents();
  void copySelectedMagnetURIs() const;
  void addLinkDialog();
  void openSelectedTorrentsFolder() const;
  void recheckSelectedTorrents();
  void setDlLimitSelectedTorrents();
  void setUpLimitSelectedTorrents();
  void setMaxRatioSelectedTorrents();
  void previewSelectedTorrents();
  void hidePriorityColumn(bool hide);
  void displayDLHoSMenu(const QPoint&);
  void applyNameFilter(QString name);
  void applyStatusFilter(int f);
  void applyLabelFilter(QString label);
  void previewFile(Transfer t, int fileIndex);
  void removeLabelFromRows(QString label);
  void renameSelectedTorrent();
  void show(TransfersFilterModel::Filter filter);
  void showWaiting();
  void showDownloading();
  void showCompleted();
  void showAll();
  void showTorrents();

protected:
  int getRowFromHash(QString hash) const;
  QString getHashFromRow(const QModelIndex &index) const;
  QModelIndex mapToSource(const QModelIndex &index) const;
  QModelIndex mapFromSource(const QModelIndex &index) const;
  QStringList getCustomLabels() const;
  void saveSettings();
  bool loadSettings();
  QStringList getSelectedTorrentsHashes() const;

protected slots:
  void torrentDoubleClicked(const QModelIndex& index);
  void displayListMenu(const QPoint&);
  void currentChanged(const QModelIndex& current, const QModelIndex&);
  void toggleSelectedTorrentsSuperSeeding() const;
  void toggleSelectedTorrentsSequentialDownload() const;
  void toggleSelectedFirstLastPiecePrio() const;
  void askNewLabelForSelection();
  void showProperties();
  void createED2KLink();

signals:
  void currentTorrentChanged(const Transfer &h);
  void sigOpenTorrent();

private:
  QTreeView *ui;
  TransferListDelegate *listDelegate;
  TorrentModel *listModel;
  TransfersFilterModel *transfersFilterModel;
  QSortFilterProxyModel *nameFilterModel;
  QSortFilterProxyModel *statusFilterModel;
  QSortFilterProxyModel *labelFilterModel;
  Session* BTSession;
  QAction* actionAddLink;
  QAction* actionOpen;
  QAction* actionDelete;
  QAction* actionLaunch;
};

#endif // TRANSFERLISTWIDGET_H
