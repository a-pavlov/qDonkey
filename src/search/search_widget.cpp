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

UserDir::UserDir(Preferences& pref)
{
    bExpanded = pref.value("Expanded", false).toBool();
    bFilled   = pref.value("Filled", false).toBool();
    dirPath   = pref.value("DirPath", QString()).toString();

    int size =  pref.beginReadArray("SearchResults");
    vecFiles.reserve(size);

    for (int i = 0; i < size; ++i)
    {
        pref.setArrayIndex(i);
        vecFiles.push_back(QED2KSearchResultEntry(pref));
    }

    pref.endArray();
}

void UserDir::save(Preferences& pref) const
{
    pref.setValue("Expanded", bExpanded);
    pref.setValue("Filled", bFilled);
    pref.setValue("DirPath", dirPath);
    pref.beginWriteArray("SearchResults", vecFiles.size());

    int i = 0;
    foreach(const QED2KSearchResultEntry& sre, vecFiles)
    {
        pref.setArrayIndex(i);
        sre.save(pref);
        ++i;
    }

    pref.endArray();
}

SearchResult::SearchResult(Preferences& pref)
{
    strRequest  = pref.value("Request", QString()).toString();
    resultType  = (RESULT_TYPE)(pref.value("ResultType", QString()).toInt());

    int size = pref.beginReadArray("Results");

    for (int i = 0; i < size; ++i)
    {
        pref.setArrayIndex(i);
        vecResults.push_back(QED2KSearchResultEntry(pref));
    }

    pref.endArray();

    size = pref.beginReadArray("UserDir");

    for (int i = 0; i < size; ++i)
    {
        pref.setArrayIndex(i);
        vecUserDirs.push_back(UserDir(pref));
    }

    pref.endArray();

    netPoint.m_nIP  = pref.value("IP", 0).toUInt();
    netPoint.m_nPort= pref.value("Port", 0).toUInt();
}

void SearchResult::save(Preferences& pref) const
{
    pref.setValue("Request", strRequest);
    pref.setValue("ResultType", resultType);
    pref.beginWriteArray("Results", vecResults.size());

    int i = 0;
    foreach(const QED2KSearchResultEntry& e, vecResults)
    {
        pref.setArrayIndex(i);
        e.save(pref);
        ++i;
    }

    pref.endArray();

    pref.beginWriteArray("UserDir", vecUserDirs.size());
    i = 0;
    foreach(const UserDir& ud, vecUserDirs)
    {
        pref.setArrayIndex(i);
        ud.save(pref);
        ++i;
    }

    pref.endArray();

    pref.setValue("IP", netPoint.m_nIP);
    pref.setValue("Port", netPoint.m_nPort);
}

SWTabBar::SWTabBar(QWidget* parent): QTabBar(parent){}

void SWTabBar::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::MidButton) emit tabCloseRequested(tabAt(event->pos()));
    else QTabBar::mousePressEvent(event);
}

int selected_row(QAbstractItemView* view)
{
    QModelIndex index = view->currentIndex();
    return index.isValid() ? index.row() : -1;
}

QVariant selected_data(QAbstractItemView* view, int column, const QModelIndex& index)
{
    return view->model()->index(index.row(), column, index.parent()).data();
}

QVariant selected_data(QAbstractItemView* view, int column)
{
    return selected_data(view, column, view->currentIndex());
}

search_widget::search_widget(QWidget *parent)
    : QWidget(parent) , nCurTabSearch(-1), nSortedColumn(-1), nSearchesInProgress(0)
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
    
    closeAll = new QAction(this);
    closeAll->setObjectName(QString::fromUtf8("closeAll"));
    closeAll->setText(tr("Close all"));
    closeAll->setDisabled(true);

    defValue = new QAction(this);
    defValue->setObjectName(QString::fromUtf8("defValue"));
    defValue->setText(tr("Default"));
    defValue->setCheckable(true);
    defValue->setChecked(true);

    defKilos = new QAction(this);
    defKilos->setObjectName(QString::fromUtf8("defKilos"));
    defKilos->setText(tr("KiB"));
    defKilos->setCheckable(true);

    defMegas = new QAction(this);
    defMegas->setObjectName(QString::fromUtf8("defMegas"));
    defMegas->setText(tr("MiB"));
    defMegas->setCheckable(true);

    menuSubResults->addAction(defValue);
    menuSubResults->addAction(defKilos);
    menuSubResults->addAction(defMegas);

    menuResults->addAction(closeAll);
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

    model.reset(new SWItemModel(0, SWDelegate::SW_COLUMNS_NUM));
    model.data()->setHeaderData(SWDelegate::SW_NAME, Qt::Horizontal,           tr("File Name"));
    model.data()->setHeaderData(SWDelegate::SW_SIZE, Qt::Horizontal,           tr("File Size"));
    model.data()->setHeaderData(SWDelegate::SW_AVAILABILITY, Qt::Horizontal,   tr("Availability"));
    model.data()->setHeaderData(SWDelegate::SW_SOURCES, Qt::Horizontal,        tr("Sources"));
    model.data()->setHeaderData(SWDelegate::SW_TYPE, Qt::Horizontal,           tr("Type"));
    model.data()->setHeaderData(SWDelegate::SW_ID, Qt::Horizontal,             tr("ID"));
    model.data()->setHeaderData(SWDelegate::SW_DURATION, Qt::Horizontal,       tr("Duration"));
    model.data()->setHeaderData(SWDelegate::SW_BITRATE, Qt::Horizontal,        tr("Bitrate"));
    model.data()->setHeaderData(SWDelegate::SW_CODEC, Qt::Horizontal,          tr("Codec"));

    filterModel.reset(new SWSortFilterProxyModel());
    filterModel.data()->setDynamicSortFilter(true);
    filterModel.data()->setSourceModel(model.data());
    filterModel.data()->setFilterKeyColumn(SWDelegate::SW_NAME);
    filterModel.data()->setFilterRole(Qt::DisplayRole);
    filterModel.data()->setSortCaseSensitivity(Qt::CaseInsensitive);

    treeResult->setModel(filterModel.data());
    treeResult->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    itemDelegate = new SWDelegate(treeResult);
    treeResult->setItemDelegate(itemDelegate);

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    //sizePolicy.setHeightForWidth(searchFilter->sizePolicy().hasHeightForWidth());
    //searchFilter->setSizePolicy(sizePolicy);

    //searchFilter->hide();
    //horizontalLayoutTabs->addWidget(searchFilter);

    connect(tableCond, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(itemCondClicked(QTableWidgetItem*)));
    connect(btnStart, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(btnMore, SIGNAL(clicked()), this, SLOT(continueSearch()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancelSearch()));
    connect(btnClear, SIGNAL(clicked()), this, SLOT(clearSearch()));
    connect(checkOwn, SIGNAL(stateChanged(int)), this, SLOT(showOwn(int)));
    connect(Session::instance(),
            SIGNAL(searchResult(const libed2k::net_identifier&, const QString&,
                                const std::vector<QED2KSearchResultEntry>&, bool)),
    		this, SLOT(ed2kSearchFinished(const libed2k::net_identifier&, const QString&,
                                          const std::vector<QED2KSearchResultEntry>&, bool)));

    connect(Session::instance(), SIGNAL(addedTransfer(Transfer)),
            this, SLOT(addedTransfer(Transfer)));

    connect(Session::instance(), SIGNAL(deletedTransfer(QString)),
            this, SLOT(deletedTransfer(const QString&)));
    connect(tabSearch, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabSearch, SIGNAL(currentChanged (int)), this, SLOT(selectTab(int)));
    connect(closeAll, SIGNAL(triggered()),  this, SLOT(closeAllTabs()));
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
    connect(treeResult, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(download()));

    // sort by name ascending
    treeResult->header()->setSortIndicator(SWDelegate::SW_NAME, Qt::AscendingOrder);
    load();
}

void search_widget::load()
{
    Preferences pref;
    pref.beginGroup("SearchWidget");
    checkPlus->setChecked(pref.value("CheckPlus", true).toBool());
    checkOwn->setChecked(pref.value("CheckOwn", true).toBool());


    if(pref.contains("TreeResultHeader"))
    {
        treeResult->header()->restoreState(pref.value("TreeResultHeader").toByteArray());
    }

    nCurTabSearch = pref.value("CurrentTab", 0).toInt();    

    int size = pref.beginReadArray("SearchResults");

    for (int i = 0; i < size; ++i)
    {
        pref.setArrayIndex(i);
        QString title = pref.value("Title", QString()).toString();        
        searchItems.push_back(SearchResult(pref));
        if (searchItems[searchItems.size() - 1].resultType == RT_USER_DIRS)
            nCurTabSearch = tabSearch->addTab(iconUserFiles, title);
        else
            nCurTabSearch = tabSearch->addTab(iconSearchResult, title);
    }

    if (size > 0)
    {
        tabSearch->setCurrentIndex(nCurTabSearch);
        tabSearch->show();
        //searchFilter->show();
        closeAll->setEnabled(true);
    }

    pref.endArray();

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
}

void search_widget::save() const
{
    Preferences pref;
    pref.beginGroup("SearchWidget");
    pref.setValue("CheckPlus", checkPlus->isChecked());
    pref.setValue("CheckOwn", checkOwn->isChecked());
    pref.setValue("CurrentTab", tabSearch->currentIndex());
    pref.setValue("TreeResultHeader", treeResult->header()->saveState());
    pref.beginWriteArray("SearchResults", searchItems.size());
    int i = 0;

    foreach(const SearchResult& sr,  searchItems)
    {
        pref.setArrayIndex(i);
        pref.setValue("Title", tabSearch->tabText(i));
        sr.save(pref);
        ++i;
    }

    pref.endArray();

    // save comboName
    pref.beginWriteArray("ComboNames", comboName->count());

    for(int index = 0; index < comboName->count(); ++index)
    {
        pref.setArrayIndex(index);
        pref.setValue("CName", comboName->itemText(index));
    }

    pref.endArray();

    pref.endGroup();
}

search_widget::~search_widget()
{
    save();
}

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
    //tableCond->setCurrentItem(item0);
}

void search_widget::itemCondClicked(QTableWidgetItem* item)
{
    if (item->column() == 0)
    {
        tableCond->editItem(tableCond->item(item->row(), 1));
    }
}

void search_widget::sortChanged(int logicalIndex, Qt::SortOrder order)
{
    if (nSortedColumn != logicalIndex)
    {
        nSortedColumn = logicalIndex;
        if (logicalIndex == SWDelegate::SW_AVAILABILITY)
            treeResult->header()->setSortIndicator(SWDelegate::SW_AVAILABILITY, Qt::DescendingOrder);
        if (logicalIndex == SWDelegate::SW_SIZE)
            treeResult->header()->setSortIndicator(SWDelegate::SW_SIZE, Qt::DescendingOrder);
    }
    nSortedColumn = logicalIndex;
}

void search_widget::startSearch()
{
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
    RESULT_TYPE resultType = RT_FILES;
    switch(comboType->currentIndex())
    {
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
        case 9:
            fileType = ED2KFTSTR_FOLDER.c_str();
            resultType = RT_FOLDERS;
            reqType = tr("Folders: ");
            break;
        case 10:
            fileType = ED2KFTSTR_USER.c_str();
            resultType = RT_CLIENTS;
            reqType = tr("Clients: ");
            break;
    }

    m_lastSearchFileType = fileType;

    prepareNewSearch(reqType, comboName->currentText(), resultType, iconSerachActive);

    if ((checkPlus->checkState() == Qt::Checked) && (resultType != RT_CLIENTS))
        searchRequest += " NOT +++";

    if (Session::instance()->isServerConnected())
    {
        // size was will convert from Mb to bytes for generate search request
        Session::instance()->searchFiles(
            searchRequest, nMinSize*1024*1024, nMaxSize*1024*1024, nAvail, nSources,
            fileType, fileExt, mediaCodec, nDuration, nBitRate);
        nSearchesInProgress = 1;
    }
}

void search_widget::continueSearch()
{
    btnStart->setEnabled(false);
    btnCancel->setEnabled(true);
    btnMore->setEnabled(false);

    tabSearch->setTabIcon(nCurTabSearch, iconSerachActive);

    Session::instance()->searchMoreResults();

    nSearchesInProgress += 1;
}

void search_widget::cancelSearch()
{
    Session::instance()->cancelSearch();

    btnStart->setDisabled(comboName->currentText().isEmpty());
    btnCancel->setEnabled(false);
    btnMore->setEnabled(false);

    tabSearch->setTabIcon(nCurTabSearch, iconSearchResult);

    nSearchesInProgress = 0;

    nCurTabSearch = -1;
}

void search_widget::clearSearch()
{
    comboName->setEditText("");
    comboType->setCurrentIndex(0);

    checkOwn->setChecked(Qt::Checked);
    checkPlus->setChecked(Qt::Unchecked);

    for (int ii = 0; ii < 11; ii ++)
        tableCond->item(ii, 1)->setText("");
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
    QString hash = selected_data(treeResult, SWDelegate::SW_ID).toString();

    prepareNewSearch(reqType, hash, RT_FILES, iconSerachActive);

    if (Session::instance()->isServerConnected())
    {
        Session::instance()->searchRelatedFiles(hash);
        nSearchesInProgress = 1;
    }
}

bool typeFilter(const std::string strType, QED2KSearchResultEntry entry)
{
    return (libed2k::GetFileTypeByName(entry.m_strFilename.toStdString()) != strType);
}

void search_widget::processSearchResult(
    const std::vector<QED2KSearchResultEntry>& vRes, boost::optional<bool> obMoreResult)
{
    nSearchesInProgress -= 1;

    if (nCurTabSearch < 0)
        return;

    tabSearch->setTabIcon(nCurTabSearch, iconSearchResult);
    btnStart->setEnabled((nSearchesInProgress == 0) && (!comboName->currentText().isEmpty()));    
    btnCancel->setEnabled(nSearchesInProgress != 0);

    if (obMoreResult)
        btnMore->setEnabled(*obMoreResult);

    searchItems[nCurTabSearch].vecResults.insert(searchItems[nCurTabSearch].vecResults.end(), vRes.begin(), vRes.end());

    if (m_lastSearchFileType == QString::fromStdString(libed2k::ED2KFTSTR_CDIMAGE) ||
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

    quint64 overallSize = 0;
    QString strCaption;
    std::vector<QED2KSearchResultEntry>::iterator it;
    std::vector<QED2KSearchResultEntry>& vecResults = searchItems[nCurTabSearch].vecResults;
    if (vRes.size() > 0)
    {
        switch (searchItems[nCurTabSearch].resultType)
        {
            case RT_FILES:
            {
                for (it = vecResults.begin(); it != vecResults.end(); ++it)
                    overallSize += it->m_nFilesize;
                strCaption = tr("Files: ");
                break;
            }
            case RT_FOLDERS:
            {
                std::vector<UserDir>& userDirs = searchItems[nCurTabSearch].vecUserDirs;
                for (it = vecResults.begin(); it != vecResults.end(); ++it)
                {
                    QString folderName = it->m_strFilename;
                    int nPos = folderName.lastIndexOf("+++");
                    if (nPos >= 0)
                        folderName = folderName.right(folderName.length() - nPos - 3);
                    
                    nPos = folderName.lastIndexOf("ED2K--");
                    if (nPos >= 0)
                        folderName = "ED2K:\\" + folderName.right(folderName.length() - nPos - 6);

                    nPos = folderName.indexOf(".emulecollection");
                    int nQnty = 0;
                    if (nPos >= 0)
                    {
                        folderName = folderName.left(nPos);
                        nPos = folderName.lastIndexOf('-');
                        if (nPos < 0)
                            nPos = folderName.lastIndexOf('\\');
                        if (nPos >= 0)
                        {
                            nQnty = folderName.right(folderName.length() - nPos - 1).toInt();
                            folderName = folderName.left(nPos);
                        }
                    }
                    folderName.replace('-', '\\');
                    it->m_nSources = nQnty;

                    UserDir dir;
                    dir.dirPath = folderName;
                    userDirs.push_back(dir);
            
                    quint64 total_size = ((quint64)it->m_nMediaBitrate << 32) + (unsigned int)it->m_nMediaLength;
                    total_size = total_size ? total_size : it->m_nFilesize;
                    overallSize += total_size;
                }

                strCaption = tr("Folders: ");
                break;
            }
            case RT_CLIENTS:
            {
                for (it = vecResults.begin(); it != vecResults.end(); ++it)
                {
                    quint64 total_size = ((quint64)it->m_nMediaBitrate << 32) + (unsigned int)it->m_nMediaLength;
                    overallSize += total_size;
                }
                strCaption = tr("Clients: ");
                break;
            }
        }

        strCaption += searchItems[nCurTabSearch].strRequest + " (" + QString::number(qulonglong(vecResults.size())) + ") - " + misc::friendlyUnit(overallSize);
        tabSearch->setTabText(nCurTabSearch, strCaption);
    }

    if (tabSearch->currentIndex() == nCurTabSearch)
        selectTab(nCurTabSearch);
}

QED2KHandle search_widget::addTransfer(const QModelIndex& index) {
    QString hash = selected_data(treeResult, SWDelegate::SW_ID, index).toString();
    QString filename = selected_data(treeResult, SWDelegate::SW_NAME, index).toString();
    EED2KFileType fileType = GetED2KFileTypeID(filename.toStdString());
    if (fileType == ED2KFT_EMULECOLLECTION)
    {
        filename.replace('\\', '-');
        filename.replace('/', '-');
    }
    QString filepath = QDir(Preferences().getSavePath()).filePath(filename);

    libed2k::add_transfer_params params;
    params.file_hash = libed2k::md4_hash::fromString(hash.toStdString());
    params.file_path = filepath.toUtf8().constData();
    params.file_size = selected_data(treeResult, SWDelegate::SW_SIZE, index).toULongLong();
    params.seed_mode = false;
    params.num_complete_sources = selected_data(treeResult, SWDelegate::SW_SOURCES, index).toInt();
    params.num_incomplete_sources =
        selected_data(treeResult, SWDelegate::SW_AVAILABILITY, index).toInt() -
        params.num_complete_sources;
    return Session::instance()->addTransfer(params);
}

void search_widget::warnDisconnected()
{
    QMessageBox::warning(
        this, tr("Server connection closed"),
        tr("You can't search in ED2K network on closed server connection, "
           "set connection or check torrent combobox"));
}

void search_widget::prepareNewSearch(
    const QString& reqType, const QString& reqText, RESULT_TYPE resultType, const QIcon& icon)
{
    if (!tabSearch->count())
    {
        tabSearch->show();
        //searchFilter->show();
    }

    nCurTabSearch = tabSearch->addTab(icon, reqType + reqText);
    tabSearch->setCurrentIndex(nCurTabSearch);

    std::vector<QED2KSearchResultEntry> vec;
    SearchResult result(reqText, resultType, vec);
    searchItems.push_back(result);

    clearSearchTable();
    btnStart->setEnabled(false);
    btnCancel->setEnabled(true);
    btnMore->setEnabled(false);
    btnCloseAll->setEnabled(true);
}

void search_widget::closeTab(int index)
{
    if (tabSearch->currentIndex() == nCurTabSearch)
    {
        nCurTabSearch = -1;

        btnStart->setDisabled(comboName->currentText().isEmpty());
        btnCancel->setEnabled(false);
        btnMore->setEnabled(false);

        nSearchesInProgress = 0;
    }
    tabSearch->removeTab(index);
    
    if (searchItems.size() > index)
        searchItems.erase(searchItems.begin() + index);
        
    if (!tabSearch->count())
    {
        clearSearchTable();
        //searchFilter->hide();
        btnCloseAll->setDisabled(true);
    }
    else
        selectTab(tabSearch->currentIndex());
}

void search_widget::selectTab(int nTabNum)
{
    if (nTabNum >= searchItems.size() || nTabNum < 0)
        return;

    std::vector<QED2KSearchResultEntry> const& vRes = searchItems[nTabNum].vecResults;
    std::vector<QED2KSearchResultEntry>::const_iterator it;

    clearSearchTable();
    int row = 0;

    treeResult->setItemsExpandable(false);
    treeResult->setRootIsDecorated(false);
    treeResult->setSelectionMode(QAbstractItemView::ExtendedSelection);
    /*
    if (searchItems[nTabNum].resultType == RT_FILES)
    {
        for (it = vRes.begin(); it != vRes.end(); ++it)
        {
            model->insertRow(row);
            fillFileValues(row, *it);
            row++;
        }
    }
    else if (searchItems[nTabNum].resultType == RT_CLIENTS)
    {
        treeResult->setSelectionMode(QAbstractItemView::SingleSelection);
        QIcon user_icon(":/emule/common/client_red.ico");
        QIcon conn_icon(":/emule/common/User.ico");

        for (it = vRes.begin(); it != vRes.end(); ++it)
        {
            model->insertRow(row);
            QString user_name = it->m_strFilename;
            user_name.replace("+++USERNICK+++", "");
            // for users size calculated from
            // m_nMediaLength  - low part of real size
            // m_nMediaBitrate - high part of real size
            model->setData(model->index(row, SWDelegate::SW_NAME), user_name.trimmed());
            quint64 total_size = ((quint64)it->m_nMediaBitrate << 32) + (unsigned int)it->m_nMediaLength;
            model->setData(model->index(row, SWDelegate::SW_SIZE), total_size);
            model->setData(model->index(row, SWDelegate::SW_ID), it->m_hFile);

            std::vector<libed2k::net_identifier>::const_iterator con_ip;
            for (con_ip = connectedPeers.begin(); con_ip != connectedPeers.end(); ++con_ip)
                if (*con_ip == it->m_network_point)
                    break;

            if (con_ip == connectedPeers.end())
                model->item(row)->setIcon(user_icon);
            else
                model->item(row)->setIcon(conn_icon);
        }
    }
    else if (searchItems[nTabNum].resultType == RT_USER_DIRS)
    {
        treeResult->setSelectionMode(QAbstractItemView::SingleSelection);
        treeResult->setItemsExpandable(true);
        treeResult->setRootIsDecorated(true);
        std::vector<UserDir>& userDirs = searchItems[nTabNum].vecUserDirs;
        std::vector<UserDir>::iterator dir_iter;
        for (dir_iter = userDirs.begin(); dir_iter != userDirs.end(); ++dir_iter)
        {
            model->insertRow(row);
            model->setData(model->index(row, SWDelegate::SW_NAME), dir_iter->dirPath);
            model->item(row)->setIcon(iconFolder);
            QModelIndex index = model->index(row, 0);

            const std::vector<QED2KSearchResultEntry>& files = dir_iter->vecFiles;
            if (files.size() > 0)
            {
                model->setData(model->index(row, SWDelegate::SW_AVAILABILITY),
                               qulonglong(files.size()));
                quint64 size = 0;
                std::vector<QED2KSearchResultEntry>::const_iterator file_iter;
                for (file_iter = files.begin(); file_iter != files.end(); ++file_iter)
                    size += file_iter->m_nFilesize;
                model->setData(model->index(row, SWDelegate::SW_SIZE), size);

                model->insertRows(0, files.size(), index);
                model->insertColumns(0, SWDelegate::SW_COLUMNS_NUM, index);
            }

            dir_iter->bFilled = false;
            if (dir_iter->bExpanded)
                treeResult->setExpanded(filterModel->mapFromSource(index), true);
        }        
    }
    else if (searchItems[nTabNum].resultType == RT_FOLDERS)
    {
        treeResult->setSelectionMode(QAbstractItemView::SingleSelection);
        treeResult->setItemsExpandable(true);
        treeResult->setRootIsDecorated(true);

        std::vector<UserDir>& userDirs = searchItems[nTabNum].vecUserDirs;
        std::vector<UserDir>::iterator dir_iter = userDirs.begin();
        for (it = vRes.begin(); it != vRes.end(); ++it, ++dir_iter)
        {
            model->insertRow(row);
            model->setData(model->index(row, SWDelegate::SW_NAME), dir_iter->dirPath);
            model->item(row)->setIcon(iconFolder);
            model->setData(model->index(row, SWDelegate::SW_AVAILABILITY), it->m_nSources);
            quint64 total_size = ((quint64)it->m_nMediaBitrate << 32) + (unsigned int)it->m_nMediaLength;
            total_size = total_size ? total_size : it->m_nFilesize;
            model->setData(model->index(row, SWDelegate::SW_SIZE), total_size);
            model->setData(model->index(row, SWDelegate::SW_ID), it->m_hFile);

            QModelIndex index = model->index(row, 0);
            if (!dir_iter->bFilled)
            {
                if (!dir_iter->bExpanded)
                {
                    model->insertRows(0, 1, index);
                    model->insertColumns(0, 9, index);
                }
            }
            else
            {
                int sub_row = 0;
                std::vector<QED2KSearchResultEntry>::const_iterator file_iter;
                model->insertRows(0, dir_iter->vecFiles.size(), index);
                model->insertColumns(0, SWDelegate::SW_COLUMNS_NUM, index);
                for (file_iter = dir_iter->vecFiles.begin(); file_iter != dir_iter->vecFiles.end(); ++file_iter)
                {
                    fillFileValues(sub_row, *file_iter, index);
                    sub_row++;
                }

                if (dir_iter->bExpanded)
                    treeResult->setExpanded(filterModel->mapFromSource(index), true);
            }
        }
    }
*/
    QHeaderView* header = treeResult->header();
    model->sort(header->sortIndicatorSection(), header->sortIndicatorOrder());
}

void search_widget::fillFileValues(int row, const QED2KSearchResultEntry& fileEntry, const QModelIndex& parent)
{
    model->setData(model->index(row, SWDelegate::SW_NAME, parent), fileEntry.m_strFilename);
    model->setData(model->index(row, SWDelegate::SW_SIZE, parent), fileEntry.m_nFilesize);
    model->setData(model->index(row, SWDelegate::SW_SOURCES, parent), fileEntry.m_nCompleteSources);
    model->setData(model->index(row, SWDelegate::SW_AVAILABILITY, parent), fileEntry.m_nSources);

    EED2KFileType fileType = GetED2KFileTypeID(fileEntry.m_strFilename.toStdString());
    switch (fileType)
    {
        case ED2KFT_AUDIO:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("Audio"));
            //model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconAudio);
            break;
        case ED2KFT_VIDEO:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("Video"));
            ///model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconVideo);
            break;
        case ED2KFT_IMAGE:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("Picture"));
            //model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconPicture);
            break;
        case ED2KFT_PROGRAM:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("Program"));
            //model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconProgram);
            break;
        case ED2KFT_DOCUMENT:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("Document"));
            //model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconDocument);
            break;
        case ED2KFT_ARCHIVE:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("Archive"));
            //model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconArchive);
            break;
        case ED2KFT_CDIMAGE:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("CD Image"));
            //model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconCDImage);
            break;
        case ED2KFT_EMULECOLLECTION:
            model->setData(model->index(row, SWDelegate::SW_TYPE, parent), tr("Emule Collection"));
            //model->itemFromIndex(model->index(row, 0, parent))->setIcon(iconCollection);
            break;
        default:
            break;

    }

    model->setData(model->index(row, SWDelegate::SW_ID, parent), fileEntry.m_hFile);
    if (fileType == ED2KFT_AUDIO || fileType == ED2KFT_VIDEO)
    {
        model->setData(model->index(row, SWDelegate::SW_DURATION, parent), fileEntry.m_nMediaLength);
        model->setData(model->index(row, SWDelegate::SW_BITRATE, parent), fileEntry.m_nMediaBitrate);
    }
    if (fileType == ED2KFT_VIDEO)
        model->setData(model->index(row, SWDelegate::SW_CODEC, parent), fileEntry.m_strMediaCodec);
}

void search_widget::clearSearchTable()
{
    for (int ii = model->rowCount()-1; ii >= 0; --ii)
        model->removeRow(ii);
}

void search_widget::closeAllTabs()
{
    for (int indx = tabSearch->count() - 1; indx != -1; --indx)
    {
        qDebug() << "close tab " << indx;
        closeTab(indx);
    }

    searchItems.clear();
    clearSearchTable();
}

void search_widget::setSizeType()
{
    QObject* sender = QObject::sender();
    if (sender == defKilos)
    {
        defMegas->setChecked(false);
        defValue->setChecked(false);

        itemDelegate->setSizeType(misc::ST_KB);
    }
    else if (sender == defMegas)
    {
        defKilos->setChecked(false);
        defValue->setChecked(false);

        itemDelegate->setSizeType(misc::ST_MB);
    }
    else
    {
        defKilos->setChecked(false);
        defMegas->setChecked(false);

        itemDelegate->setSizeType(misc::ST_DEFAULT);
    }

    //tableResult->update(tableResult->rect());
    //qApp->processEvents();

    int width = treeResult->columnWidth(0);
    treeResult->setColumnWidth(0, width - 1);
    treeResult->setColumnWidth(0, width);
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

void search_widget::searchTextChanged(const QString text)
{
    if (!nSearchesInProgress)
        btnStart->setEnabled(text.length() > 0);
}

void search_widget::applyFilter(QString filter)
{
    //if (searchFilter->isFilterSet())
    //    filterModel->setFilterRegExp(QRegExp(filter, Qt::CaseInsensitive));
}

void search_widget::setFilterType(SWDelegate::Column column)
{
    filterModel->setFilterKeyColumn(column);
}

void search_widget::displayListMenu(const QPoint&) 
{
    if (tabSearch->currentIndex() < 0)
        return;

    RESULT_TYPE resultType = searchItems[tabSearch->currentIndex()].resultType;

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    bool enabled =
        selected.size() == 1 &&
        misc::isMD4Hash(selected_data(treeResult, SWDelegate::SW_ID).toString());

    actionSearch_related->setEnabled(enabled);
    actionED2K_link->setEnabled(enabled);
    fileMenu->exec(QCursor::pos());
}

void search_widget::resultSelectionChanged(const QItemSelection& sel, const QItemSelection& unsel)
{
    updateFileActions();
}

QList<QED2KHandle> search_widget::on_actionDownload()
{
    /*
    QList<Transfer> result;
    // Possible only with double click.
    if (searchItems[tabSearch->currentIndex()].resultType == RT_CLIENTS)
    {
        initPeer();
        return result;
    }

    if (!hasSelectedFiles())
    {
        qDebug("some files should be selected for downloading");
        return result;
    }

    bool bDirs =
        searchItems[tabSearch->currentIndex()].resultType == RT_USER_DIRS ||
        searchItems[tabSearch->currentIndex()].resultType == RT_FOLDERS;

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    QModelIndexList::const_iterator iter;

    for (iter = selected.begin(); iter != selected.end(); ++iter)
    {
        if (bDirs && iter->parent() == treeResult->rootIndex())
        {
            std::vector<QED2KSearchResultEntry> const& vRes =
                searchItems[tabSearch->currentIndex()].vecResults;
            std::vector<QED2KSearchResultEntry>::const_iterator it;
            std::vector<UserDir>& userDirs = searchItems[tabSearch->currentIndex()].vecUserDirs;
            std::vector<UserDir>::iterator dir_iter = userDirs.begin();

            if (searchItems[tabSearch->currentIndex()].resultType == RT_USER_DIRS)
            {
                QString dirName = selected_data(treeResult, SWDelegate::SW_NAME, *iter).toString();
                for (dir_iter = userDirs.begin(); dir_iter != userDirs.end(); ++dir_iter)
                {
                    if(dir_iter->dirPath == dirName && dir_iter->vecFiles.size() > 0)
                    {
                        break;
                    }
                }
            }
            if (searchItems[tabSearch->currentIndex()].resultType == RT_FOLDERS)
            {
                QString hash = selected_data(treeResult, SWDelegate::SW_ID, *iter).toString();

                for (it = vRes.begin(); it != vRes.end(); ++it, ++dir_iter)
                {
                    if (it->m_hFile == hash)        
                        break;
                }
            }
                
            if (dir_iter != userDirs.end())
            {
                QString name = dir_iter->dirPath;
                int nPos = name.lastIndexOf("\\");
                if (nPos >= 0)
                    name = name.right(name.length() - nPos - 1);
                nPos = name.lastIndexOf("/");
                if (nPos >= 0)
                    name = name.right(name.length() - nPos - 1);

                std::vector<FileData> file_data;
                std::vector<QED2KSearchResultEntry>::const_iterator file_iter;
                for (file_iter = dir_iter->vecFiles.begin(); file_iter != dir_iter->vecFiles.end(); ++file_iter)
                {
                    FileData file;
                    file.file_name = file_iter->m_strFilename;
                    file.file_hash = libed2k::md4_hash::fromString(file_iter->m_hFile.toStdString());
                    file.file_size = file_iter->m_nFilesize;
                    file_data.push_back(file);
                }

                collection_save_dlg dlg(this, file_data, name);
                dlg.exec();
            }

            continue;
        }

        result << addTransfer(*iter);
    }

    return result;
    */
    return QList<QED2KHandle>();
}

void search_widget::on_actionDownload_pause() {
    foreach(QED2KHandle h, on_actionDownload())
        h.pause();
}

void search_widget::on_actionPreview() {
    if (selected_row(treeResult) < 0) {
        qDebug("preview button should be disabled when result isn't selected");
        return;
    }

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    QModelIndexList::const_iterator iter;

    for (iter = selected.begin(); iter != selected.end(); ++iter)
    {
        QString filename = selected_data(treeResult, SWDelegate::SW_NAME, *iter).toString();
        if (misc::isPreviewable(misc::file_extension(filename))) {
            //Transfer t = addTransfer(*iter);
            //Session::instance()->deferPlayMedia(t, 0);
        }
    }
}

bool search_widget::hasSelectedMedia()
{
    if (tabSearch->currentIndex() < 0 ||
        searchItems[tabSearch->currentIndex()].resultType == RT_CLIENTS)
        return false;

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    QModelIndexList::const_iterator iter;
    for (iter = selected.begin(); iter != selected.end(); ++iter)
    {
        QString filename = selected_data(treeResult, SWDelegate::SW_NAME, *iter).toString();
        QString hash = selected_data(treeResult, SWDelegate::SW_ID, *iter).toString();
        if (misc::isPreviewable(misc::file_extension(filename)))
        {
            return true;
        }
    }

    return false;
}

bool search_widget::hasSelectedFiles()
{
    return true;
    /*
    if (tabSearch->currentIndex() < 0 ||
        searchItems[tabSearch->currentIndex()].resultType == RT_CLIENTS)
        return false;

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();
    if (selected.size() && searchItems[tabSearch->currentIndex()].resultType == RT_FOLDERS) {
        if (selected.first().parent() == treeResult->rootIndex())
        {
            QString hash = model->data(
                model->index(selected.first().row(), SWDelegate::SW_ID)).toString();

            std::vector<QED2KSearchResultEntry> const& vRes =
                searchItems[tabSearch->currentIndex()].vecResults;
            std::vector<QED2KSearchResultEntry>::const_iterator it;
            std::vector<UserDir>& userDirs = searchItems[tabSearch->currentIndex()].vecUserDirs;
            std::vector<UserDir>::iterator dir_iter = userDirs.begin();
            for (it = vRes.begin(); it != vRes.end(); ++it, ++dir_iter)
            {
                if (it->m_hFile == hash)
                    break;
            }

            if (dir_iter != userDirs.end())
            {
                if (!dir_iter->vecFiles.size())
                    return false;
            }
        }
    }

    foreach (const QModelIndex& index, selected)
    {
        QString filename = selected_data(treeResult, SWDelegate::SW_NAME, index).toString();
        QString hash = selected_data(treeResult, SWDelegate::SW_ID, index).toString();
    }

    return false;
    */
}

void search_widget::updateFileActions()
{
    bool hasSelFiles = hasSelectedFiles();
    actionDownload->setEnabled(hasSelFiles);
    actionDownload_pause->setEnabled(hasSelFiles);
    actionPreview->setEnabled(hasSelectedMedia());
}

void search_widget::displayHSMenu(const QPoint&)
{
    QMenu hideshowColumn(this);
    hideshowColumn.setTitle(tr("Column visibility"));
    QList<QAction*> actions;
    for (int i=0; i < model->columnCount(); ++i)
    {
        QAction *myAct = hideshowColumn.addAction(
            model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        myAct->setCheckable(true);
        myAct->setChecked(!treeResult->isColumnHidden(i));
        actions.append(myAct);
    }
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
    const std::vector<QED2KSearchResultEntry>& vRes, bool bMoreResult)
{
    processSearchResult(vRes, bMoreResult);
}

void search_widget::addedTransfer(QED2KHandle h) {
    updateFileActions();
    filterModel->showOwn(checkOwn->isChecked());
}

void search_widget::deletedTransfer(const QString& hash)
{
    updateFileActions();
    filterModel->showOwn(checkOwn->isChecked());
}

void search_widget::on_actionED2K_link()
{
    if (selected_row(treeResult) < 0) {
        return;
    }

    QModelIndexList selected = treeResult->selectionModel()->selectedRows();

    QString file_name = selected_data(treeResult, SWDelegate::SW_NAME, selected[0]).toString();
    QString hash = selected_data(treeResult, SWDelegate::SW_ID, selected[0]).toString();
    quint64 file_size = selected_data(treeResult, SWDelegate::SW_SIZE, selected[0]).toULongLong();

    ed2k_link_maker dlg(file_name, hash, file_size, this);
    dlg.exec();
}

QVariant SWItemModel::data(const QModelIndex& inx, int role) const
{
    QVariant res;

    if (role == Qt::ForegroundRole && inx.column() == SWDelegate::SW_NAME)
        res = QVariant(itemColor(inx));
    else
        res = QStandardItemModel::data(inx, role);

    return res;
}
