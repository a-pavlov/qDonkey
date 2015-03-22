#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QMessageBox>

#include <libed2k/file.hpp>

#include "search/search_widget.h"
#include "preferences.h"
#include "ed2k_link_maker.h"
#include "collection_save_dlg.h"
#include "res.h"

#include "qtlibed2k/qed2kpeerhandle.h"

using namespace libed2k;

boost::optional<int> toInt(const QString& str)
{
    bool bOk = false;
    int res = str.toInt(&bOk);
    return bOk ? boost::optional<int>(res) : boost::optional<int>();
}

boost::optional<qulonglong> toULongLong(const QString& str)
{
    bool bOk = false;
    qulonglong res = str.toULongLong(&bOk);
    return bOk ? boost::optional<qulonglong>(res) : boost::optional<qulonglong>();
}

SWTabBar::SWTabBar(QWidget* parent): QTabBar(parent){}

void SWTabBar::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::MidButton) emit tabCloseRequested(tabAt(event->pos()));
    else QTabBar::mousePressEvent(event);
}

int selected_row(QAbstractItemView* view) {
    QModelIndex index = view->currentIndex();
    return index.isValid() ? index.row() : -1;
}

QVariant selected_data(QAbstractItemView* view, int column, const QModelIndex& index) {
    return view->model()->index(index.row(), column, index.parent()).data(SearchModel::NativeRole);
}

QVariant selected_data(QAbstractItemView* view, int column) {
    return selected_data(view, column, view->currentIndex());
}

search_widget::search_widget(QWidget *parent)
    : QWidget(parent) , nCurTabSearch(-1), nSortedColumn(-1)
{
    setupUi(this);

    tabSearch = new SWTabBar(this);
    tabSearch->setObjectName(QString::fromUtf8("tabSearch"));
    tabSearch->setTabsClosable(true);
    tabSearch->setShape(QTabBar::RoundedNorth);
    tabSearch->setExpanding(false);
    tabSearch->hide();
    horizontalLayoutTabs->addWidget(tabSearch);

    menuResults = new QMenu(this);
    menuResults->setObjectName(QString::fromUtf8("menuStatus"));
    menuSubResults = new QMenu(this);
    menuSubResults->setObjectName(QString::fromUtf8("menuSubResults"));
    menuSubResults->setTitle(tr("Size"));
    
    defValue->setChecked(true);
    menuSubResults->addAction(defValue);
    menuSubResults->addAction(defKilos);
    menuSubResults->addAction(defMegas);

    menuResults->addAction(actionClose_all);
    menuResults->addSeparator();
    menuResults->addMenu(menuSubResults);

    btnResults->setMenu(menuResults);

    comboType->addItem(QIcon(res::itemAny()), tr("Any"));
    comboType->addItem(QIcon(res::itemArchive()), tr("Archive"));
    comboType->addItem(QIcon(res::itemMusic()), tr("Audio"));
    comboType->addItem(QIcon(res::itemCDImage()), tr("CD Image"));
    comboType->addItem(QIcon(res::itemPicture()), tr("Picture"));
    comboType->addItem(QIcon(res::itemProgram()), tr("Program"));
    comboType->addItem(QIcon(res::itemVideo()), tr("Video"));
    comboType->addItem(QIcon(res::itemDocument()), tr("Document"));
    comboType->addItem(QIcon(res::itemEmuleCollection()), tr("Emule Collection"));

    comboType->setMaxVisibleItems(10);
    comboName->setMaxCount(50);
    comboName->setToolTip(tr("Press Ctrl-Delete for history cleanup"));

    tableCond->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableCond->setColumnWidth(0, 200);
    addCondRow();
    tableCond->item(0, 0)->setText(tr("Min. size [MiB]"));
    addCondRow();
    tableCond->item(1, 0)->setText(tr("Max. size [MiB]"));
    addCondRow();
    tableCond->item(2, 0)->setText(tr("Availability"));
    addCondRow();
    tableCond->item(3, 0)->setText(tr("Full sources"));
    addCondRow();
    tableCond->item(4, 0)->setText(tr("Extension"));
    addCondRow();
    tableCond->item(5, 0)->setText(tr("Codec"));
    addCondRow();
    tableCond->item(6, 0)->setText(tr("Min bitrait [kBit/sec]"));
    addCondRow();
    tableCond->item(7, 0)->setText(tr("Min duration [h:m:s]"));

    checkOwn->setChecked(Qt::Checked);

    model = new SearchModel(this);
    filterModel = new SWSortFilterProxyModel(this);
    filterModel->setDynamicSortFilter(false);
    filterModel->setSourceModel(model);

    treeResult->setModel(filterModel);
    treeResult->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    connect(tableCond, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(itemCondClicked(QTableWidgetItem*)));
    connect(btnStart, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(btnMore, SIGNAL(clicked()), this, SLOT(continueSearch()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancelSearch()));
    connect(btnClear, SIGNAL(clicked()), this, SLOT(clearSearch()));
    connect(checkOwn, SIGNAL(stateChanged(int)), this, SLOT(showOwn(int)));
    connect(Session::instance(),
            SIGNAL(searchResult(const libed2k::net_identifier&, const QString&,
                                const QList<QED2KSearchResultEntry>&, bool)),
    		this, SLOT(ed2kSearchFinished(const libed2k::net_identifier&, const QString&,
                                          const QList<QED2KSearchResultEntry>&, bool)));

    connect(Session::instance(), SIGNAL(transferAdded(QED2KHandle)),
            this, SLOT(addedTransfer(QED2KHandle)));

    connect(Session::instance(), SIGNAL(transferDeleted(QString)),
            this, SLOT(deletedTransfer(QString)));

    connect(model, SIGNAL(countChanged(int)), this, SLOT(modelCountChanged(int)));

    connect(tabSearch, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabSearch, SIGNAL(currentChanged (int)), this, SLOT(selectTab(int)));

    connect(defValue,  SIGNAL(triggered()), this, SLOT(setSizeType()));
    connect(defKilos,  SIGNAL(triggered()), this, SLOT(setSizeType()));
    connect(defMegas,  SIGNAL(triggered()), this, SLOT(setSizeType()));
    connect(comboName,  SIGNAL(editTextChanged(const QString)), this, SLOT(searchTextChanged(const QString)));
    connect(comboName->lineEdit(), SIGNAL(returnPressed()), this, SLOT(startSearch()));

    fileMenu = new QMenu(this);
    fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
    fileMenu->setTitle(tr("Files"));

    btnDownload->setDefaultAction(actionDownload);
    btnPreview->setDefaultAction(actionPreview);
    btnCloseAll->setDefaultAction(actionClose_all);

    btnDownload->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnPreview->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnCloseAll->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    fileMenu->addAction(actionDownload);
    fileMenu->addAction(actionDownload_pause);
    fileMenu->addAction(actionPreview);
    fileMenu->addAction(actionED2K_link);
    fileMenu->addSeparator();
    fileMenu->addAction(actionSearch_related);

    treeResult->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeResult, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(displayListMenu(const QPoint&)));

    connect(treeResult->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this, SLOT(resultSelectionChanged(const QItemSelection&, const QItemSelection&)));
    connect(treeResult->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            this, SLOT(sortChanged(int, Qt::SortOrder)));
    treeResult->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeResult->header(), SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(displayHSMenu(const QPoint&)));
    connect(treeResult, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_actionDownload_triggered()));

    load();

    // sort by name ascending
    treeResult->header()->setSortIndicator(SearchModel::DC_SOURCES, Qt::DescendingOrder);
}

void search_widget::load() {
    model->load();

    Preferences pref;
    pref.beginGroup("SearchWidget");
    checkPlus->setChecked(pref.value("CheckPlus", true).toBool());
    checkOwn->setChecked(pref.value("CheckOwn", true).toBool());

    if (pref.value("defMegas", false).toBool()) {
        //defMegas->setChecked(true);
    }

    if (pref.value("defValue", true).toBool()) {
        //defValue->setChecked(true);
    }

    if (pref.value("defKilos", false).toBool()) {
        //defKilos->setChecked(true);
    }

    if(pref.contains("TreeResultHeader")) {
        treeResult->header()->restoreState(pref.value("TreeResultHeader").toByteArray());
    }

    int size = pref.beginReadArray("Tabs");

    for (int i = 0; i < size; ++i) {
        pref.setArrayIndex(i);
        QString title = pref.value("Title", QString()).toString();
        qDebug() << "load " << title;
        tabSearch->addTab(QIcon(res::searchResult()), title);
    }

    pref.endArray();

    int currentTab = pref.value("CurrentTab", -1).toInt();
    if (currentTab < tabSearch->count()) {
        tabSearch->setCurrentIndex(currentTab);
    }

    // restore comboName
    size = pref.beginReadArray("ComboNames");
    for (int i = 0; i < size; ++i)
    {
        pref.setArrayIndex(i);
        comboName->addItem(pref.value("CName", QString()).toString());
    }

    pref.endArray();
    comboName->setCurrentIndex(-1);
    pref.endGroup();

    if (tabSearch->count() != 0) tabSearch->show();
}

void search_widget::save() const {
    model->save();
    Preferences pref;
    pref.beginGroup("SearchWidget");
    pref.setValue("CheckPlus", checkPlus->isChecked());
    pref.setValue("CheckOwn", checkOwn->isChecked());
    pref.setValue("CurrentTab", tabSearch->currentIndex());
    pref.setValue("TreeResultHeader", treeResult->header()->saveState());

    pref.setValue("defValue", defValue->isChecked());
    pref.setValue("defKilos", defKilos->isChecked());
    pref.setValue("defMegas", defMegas->isChecked());

    pref.beginWriteArray("Tabs", tabSearch->count());

    for (int i = 0; i < tabSearch->count(); ++i) {
        pref.setArrayIndex(i);
        pref.setValue("Title", tabSearch->tabText(i));
        qDebug() << "save " << tabSearch->tabText(i);
    }

    pref.endArray();

    // save comboName
    pref.beginWriteArray("ComboNames", comboName->count());

    for(int index = 0; index < comboName->count(); ++index) {
        pref.setArrayIndex(index);
        pref.setValue("CName", comboName->itemText(index));
    }

    pref.endArray();
    pref.endGroup();
}

search_widget::~search_widget() { save(); }

void search_widget::addCondRow() {
    int row = tableCond->rowCount();
    tableCond->insertRow(row);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item0->setFlags(Qt::ItemIsEnabled);
    tableCond->setItem(row, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    tableCond->setItem(row, 1, item1);
}

void search_widget::itemCondClicked(QTableWidgetItem* item)
{
    if (item->column() == 0) {
        tableCond->editItem(tableCond->item(item->row(), 1));
    }
}

void search_widget::sortChanged(int logicalIndex, Qt::SortOrder order) {
    filterModel->sort(logicalIndex, order);
}

void search_widget::startSearch() {
    // search in ed2k when server connection isn't online
    if (!Session::instance()->isServerConnected()) {
        warnDisconnected();
        return;
    }

    QString searchRequest = comboName->currentText();
    if (!searchRequest.length())
        return;

    bool bOk = false;
    quint64 nMinSize = tableCond->item(0, 1)->text().toLongLong(&bOk);
    if (!bOk && tableCond->item(0, 1)->text().length() > 0)
    {
        showErrorParamMsg(0);
        return;
    }

    quint64 nMaxSize = tableCond->item(1, 1)->text().toLongLong(&bOk);
    if (!bOk && tableCond->item(1, 1)->text().length() > 0)
    {
        showErrorParamMsg(1);
        return;
    }
    int nAvail = tableCond->item(2, 1)->text().toInt(&bOk);
    if (!bOk && tableCond->item(2, 1)->text().length() > 0)
    {
        showErrorParamMsg(2);
        return;
    }
    int nSources = tableCond->item(3, 1)->text().toInt(&bOk);
    if (!bOk && tableCond->item(3, 1)->text().length() > 0)
    {
        showErrorParamMsg(3);
        return;
    }
    QString fileExt = tableCond->item(4, 1)->text();
    QString mediaCodec = tableCond->item(5, 1)->text();
    int nBitRate = tableCond->item(6, 1)->text().toInt(&bOk);
    if (!bOk && tableCond->item(6, 1)->text().length() > 0)
    {
        showErrorParamMsg(6);
        return;
    }

    QString duration = tableCond->item(7, 1)->text();
    bOk = true;
    int nDuration = 0;
    if (duration.length())
    {
        int nPos, nNum, coef = 1;
        QString num;
        do
        {
            nPos = duration.lastIndexOf(':');
            if (nPos >= 0)
            {
                num = duration.right(duration.length() - nPos - 1);
                duration = duration.left(nPos);
            }
            else
            {
                num = duration;
                duration = "";
            }

            nNum = num.toInt(&bOk);
            nDuration += nNum * coef;
            coef *= 60;
        }
        while (bOk && duration.length());
    }
    if (!bOk)
    {
        showErrorParamMsg(7);
        return;
    }

    QString fileType = "";
    QString reqType = tr("Files: ");

    switch(comboType->currentIndex())  {
        case 1:
            fileType = ED2KFTSTR_ARCHIVE.c_str();
            break;
        case 2:
            fileType = ED2KFTSTR_AUDIO.c_str();
            break;
        case 3:
            fileType = ED2KFTSTR_CDIMAGE.c_str();
            break;
        case 4:
            fileType = ED2KFTSTR_IMAGE.c_str();
            break;
        case 5:
            fileType = ED2KFTSTR_PROGRAM.c_str();
            break;
        case 6:
            fileType = ED2KFTSTR_VIDEO.c_str();
            break;
        case 7:
            fileType = ED2KFTSTR_DOCUMENT.c_str();
            break;
        case 8:
            fileType = ED2KFTSTR_EMULECOLLECTION.c_str();
            break;
        default:
            break;
    }

    m_lastSearchFileType = fileType;

    prepareNewSearch(reqType + comboName->currentText());

    if ((checkPlus->checkState() == Qt::Checked))
        searchRequest += " NOT +++";

    if (Session::instance()->isServerConnected()) {
        // size was will convert from Mb to bytes for generate search request
        Session::instance()->searchFiles(
            searchRequest, nMinSize*1024*1024, nMaxSize*1024*1024, nAvail, nSources,
            fileType, fileExt, mediaCodec, nDuration, nBitRate);
    }
}

void search_widget::continueSearch() {
    Q_ASSERT(tabSearch->count() > 0);
    nCurTabSearch = tabSearch->count() - 1;
    btnStart->setEnabled(false);
    btnCancel->setEnabled(true);
    btnMore->setEnabled(false);
    tabSearch->setTabIcon(nCurTabSearch, QIcon(res::searchActive()));
    Session::instance()->searchMoreResults();
}

void search_widget::cancelSearch()
{
    Session::instance()->cancelSearch();

    btnStart->setDisabled(comboName->currentText().isEmpty());
    btnCancel->setEnabled(false);
    btnMore->setEnabled(false);

    tabSearch->setTabIcon(nCurTabSearch, QIcon(res::searchResult()));
    nCurTabSearch = -1;
    curSearchTitle.clear();
}

void search_widget::clearSearch()
{
    comboName->setEditText("");
    comboName->clear();
    comboType->setCurrentIndex(0);

    checkOwn->setChecked(Qt::Checked);
    checkPlus->setChecked(Qt::Checked);

    for (int ii = 0; ii < tableCond->rowCount(); ii ++) tableCond->item(ii, 1)->setText("");
}

void search_widget::showOwn(int state)
{
    filterModel->showOwn(state == Qt::Checked);
}

void search_widget::searchRelatedFiles()
{
    // search in ed2k when server connection isn't online
    if (!Session::instance()->isServerConnected())
    {
        warnDisconnected();
        return;
    }

    QString reqType = tr("Files: ");
    QString hash = selected_data(treeResult, SearchModel::DC_HASH).toString();

    prepareNewSearch(reqType + hash);

    if (Session::instance()->isServerConnected())  {
        Session::instance()->searchRelatedFiles(hash);
    }
}

bool typeFilter(const std::string strType, QED2KSearchResultEntry entry)
{
    return (libed2k::GetFileTypeByName(entry.m_strFilename.toStdString()) != strType);
}

void search_widget::processSearchResult(const QList<QED2KSearchResultEntry>& vRes, boost::optional<bool> obMoreResult) {

    // search was cancelled
    if (nCurTabSearch < 0) return;

    tabSearch->setTabIcon(nCurTabSearch, QIcon(res::searchResult()));
    btnStart->setEnabled(!comboName->currentText().isEmpty());
    btnCancel->setEnabled(false);

    if (obMoreResult)
        btnMore->setEnabled(*obMoreResult);

    int count = model->addDataTo(vRes, nCurTabSearch);

    /*if (m_lastSearchFileType == QString::fromStdString(libed2k::ED2KFTSTR_CDIMAGE) ||
        m_lastSearchFileType == QString::fromStdString(libed2k::ED2KFTSTR_ARCHIVE) ||
        m_lastSearchFileType == QString::fromStdString(libed2k::ED2KFTSTR_PROGRAM))
    {
        searchItems[nCurTabSearch].vecResults.erase(
            std::remove_if(searchItems[nCurTabSearch].vecResults.begin(),
                           searchItems[nCurTabSearch].vecResults.end(),
                           std::bind1st(std::ptr_fun(&typeFilter),
                                        m_lastSearchFileType.toStdString())),
            searchItems[nCurTabSearch].vecResults.end());
    }
*/

    if (count > 0) tabSearch->setTabText(nCurTabSearch, curSearchTitle + "(" + QString::number(count) + ")");
    if (tabSearch->currentIndex() == nCurTabSearch)
        selectTab(nCurTabSearch);
    nCurTabSearch = -1;
}

QED2KHandle search_widget::addTransfer(const QModelIndex& index) {
    QString hash = selected_data(treeResult, SearchModel::DC_HASH, index).toString();
    QString filename = selected_data(treeResult, SearchModel::DC_NAME, index).toString();
    qlonglong size = selected_data(treeResult, SearchModel::DC_FILESIZE, index).toULongLong();

    qDebug() << "download file " << filename << " with hash " << hash << " size " << size;

    EED2KFileType fileType = GetED2KFileTypeID(filename.toStdString());
    if (fileType == ED2KFT_EMULECOLLECTION) {
        filename.replace('\\', '-');
        filename.replace('/', '-');
    }

    QString filepath = QDir(Preferences().inputDir()).filePath(filename);

    libed2k::add_transfer_params params;
    params.file_hash = libed2k::md4_hash::fromString(hash.toStdString());
    params.file_path = filepath.toUtf8().constData();
    params.file_size = size;
    params.seed_mode = false;
    params.num_complete_sources = selected_data(treeResult, SearchModel::DC_SOURCES, index).toInt();
    //params.num_incomplete_sources =
   //     selected_data(treeResult, SearchModel::DC_, index).toInt() -
    //    params.num_complete_sources;
    return Session::instance()->addTransfer(params);
}

void search_widget::warnDisconnected() {
    QMessageBox::warning(
        this, tr("Server connection closed"),
        tr("You can't search in ED2K network on closed server connection, "
           "set connection or check torrent combobox"));
}

void search_widget::prepareNewSearch(const QString& title)
{
    if (!tabSearch->count()) tabSearch->show();

    model->appendData(QList<QED2KSearchResultEntry>());
    nCurTabSearch = tabSearch->addTab(QIcon(res::searchActive()), title);
    curSearchTitle = title;
    tabSearch->setCurrentIndex(nCurTabSearch);

    btnStart->setEnabled(false);
    btnCancel->setEnabled(!btnStart->isEnabled());
    btnMore->setEnabled(false);
}

void search_widget::closeTab(int index)
{
    qDebug() << "close tab " << index;
    if (index == nCurTabSearch) {
        cancelSearch();
    }

    if (nCurTabSearch > index) {
        nCurTabSearch--;
    }

    if (index == tabSearch->count() - 1)  btnMore->setEnabled(false);

    tabSearch->removeTab(index);
    model->removeIndex(index);
    if (tabSearch->count() != 0) selectTab(tabSearch->currentIndex());
}

void search_widget::selectTab(int nTabNum) {
    filterModel->setSourceModel(model);
    model->resetToIndex(nTabNum);
    filterModel->sort(treeResult->header()->sortIndicatorSection(), treeResult->header()->sortIndicatorOrder());
    updateDownloadControls();
}

void search_widget::setSizeType() {
    QObject* sender = QObject::sender();
    if (sender == defKilos) {
        defMegas->setChecked(false);
        defValue->setChecked(false);
        model->setSizeType(misc::ST_KB);
    }
    else if (sender == defMegas) {
        defKilos->setChecked(false);
        defValue->setChecked(false);
        model->setSizeType(misc::ST_MB);
    }
    else if (sender == defValue) {
        defKilos->setChecked(false);
        defMegas->setChecked(false);
        model->setSizeType(misc::ST_DEFAULT);
    }
}

void search_widget::showErrorParamMsg(int numParam)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("Search request parsing error"));
    QString errorText(tr("Syntax error in '"));
    errorText += tableCond->item(numParam, 0)->text();
    errorText += tr("' parameter");
    msgBox.setInformativeText(errorText);
    msgBox.exec();
}

void search_widget::searchTextChanged(const QString text) {
    btnStart->setEnabled(!text.isEmpty() && nCurTabSearch == -1);
}

void search_widget::displayListMenu(const QPoint&) 
{
    if (tabSearch->currentIndex() < 0)
        return;


    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    bool enabled =
        selected.size() == 1 &&
        misc::isMD4Hash(selected_data(treeResult, SearchModel::DC_HASH).toString());

    actionSearch_related->setEnabled(enabled);
    actionED2K_link->setEnabled(enabled);
    fileMenu->exec(QCursor::pos());
}

void search_widget::updateDownloadControls() {
    bool hasFiles = false;
    bool hasMedia = false;
    foreach(const QModelIndex& i, treeResult->selectionModel()->selectedRows()) {
        QModelIndex index = filterModel->mapToSource(i);
        if (index.isValid()) {
            // do not process my files
            if (Session::instance()->getTransfer(model->hash(index)).is_valid()) {
                continue;
            }

            hasFiles = true;
            hasMedia = misc::isPreviewable(misc::file_extension(model->filename(index)));
            if (hasMedia) break;
        }
    }

    btnDownload->setEnabled(hasFiles);
    btnPreview->setEnabled(hasMedia);
}

void search_widget::resultSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    updateDownloadControls();
}

QList<QED2KHandle> search_widget::on_actionDownload_triggered() {

    QList<QED2KHandle> result;

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    QModelIndexList::const_iterator iter;

    for (iter = selected.begin(); iter != selected.end(); ++iter) {
        result << addTransfer(*iter);
    }

    return result;
}

void search_widget::on_actionDownload_pause_triggered() {
    foreach(QED2KHandle h, on_actionDownload_triggered())
        h.pause();
}

void search_widget::on_actionPreview_triggered() {
    if (selected_row(treeResult) < 0) {
        qDebug("preview button should be disabled when result isn't selected");
        return;
    }

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    QModelIndexList::const_iterator iter;

    for (iter = selected.begin(); iter != selected.end(); ++iter) {
        QString filename = selected_data(treeResult, SearchModel::DC_NAME, *iter).toString();
        if (misc::isPreviewable(misc::file_extension(filename))) {
            QED2KHandle h = addTransfer(*iter);
            Session::instance()->deferPlayMedia(h);
        }
    }
}

void search_widget::on_actionClose_all_triggered() {
    qDebug() << "close all";
    for (int indx = tabSearch->count() - 1; indx != -1; --indx)  {
        closeTab(indx);
    }
}

void search_widget::on_actionED2K_link_triggered() {
    if (selected_row(treeResult) < 0) {
        return;
    }

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();

    QString file_name = selected_data(treeResult, SearchModel::DC_NAME, selected[0]).toString();
    QString hash = selected_data(treeResult, SearchModel::DC_HASH, selected[0]).toString();
    quint64 file_size = selected_data(treeResult, SearchModel::DC_FILESIZE, selected[0]).toULongLong();

    ed2k_link_maker dlg(file_name, hash, file_size, this);
    dlg.exec();
}

void search_widget::displayHSMenu(const QPoint&)
{
    QMenu hideshowColumn(this);
    hideshowColumn.setTitle(tr("Column visibility"));
    QList<QAction*> actions;
    //for (int i=0; i < model->columnCount(); ++i)
    //{
        //QAction *myAct = hideshowColumn.addAction(
        //    model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        //myAct->setCheckable(true);
        //myAct->setChecked(!treeResult->isColumnHidden(i));
        //actions.append(myAct);
    //}
    // Call menu
    QAction *act = hideshowColumn.exec(QCursor::pos());
    if (act)
    {
        int col = actions.indexOf(act);
        Q_ASSERT(col >= 0);
        qDebug("Toggling column %d visibility", col);
        treeResult->setColumnHidden(col, !treeResult->isColumnHidden(col));
        if (!treeResult->isColumnHidden(col) && treeResult->columnWidth(col) <= 5)
            treeResult->setColumnWidth(col, 100);
    }
}

// parse strings like: '500 MB', '1.6 GB'
// return 0 on fail
qulonglong parseSize(const QString& strSize)
{
    QStringList lst = strSize.split(" ", QString::SkipEmptyParts);
    float base = 0;
    qulonglong mes = 1;

    if (lst.size() == 2)
    {
        base = lst[0].toFloat();

        if (lst[1] == "KB") mes = 1000;
        else if (lst[1] == "MB") mes = 1000 * 1000;
        else if (lst[1] == "GB") mes = 1000 * 1000 * 1000;
        else mes = 0;
    }

    return base * mes;
}

void search_widget::ed2kSearchFinished(
    const libed2k::net_identifier& np,const QString& hash,
    const QList<QED2KSearchResultEntry>& vRes, bool bMoreResult)
{
    processSearchResult(vRes, bMoreResult);
}

void search_widget::addedTransfer(QED2KHandle h) {
    updateDownloadControls();
    filterModel->showOwn(checkOwn->isChecked());
}

void search_widget::deletedTransfer(const QString& hash) {
    updateDownloadControls();
    filterModel->showOwn(checkOwn->isChecked());
}

void search_widget::modelCountChanged(int count) {
    actionClose_all->setEnabled(count != 0);
}


void search_widget::on_actionSearch_related_triggered()
{
    searchRelatedFiles();
}
