#ifndef SEARCH_WIDGET_H
#define SEARCH_WIDGET_H

#include "ui_search_widget.h"
#include "search/search_model.h"
#include "search_widget_fp_model.h"
#include "qtlibed2k/qed2ksession.h"

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QStandardItemModel;
QT_END_NAMESPACE

class search_filter;

enum RESULT_TYPE
{
    RT_FILES,
    RT_CLIENTS,
    RT_FOLDERS,
    RT_USER_DIRS
};

class SWTabBar : public QTabBar
{
public:
    SWTabBar(QWidget* parent);
protected:
    void mousePressEvent(QMouseEvent* event);
};

class search_widget : public QWidget , private Ui::search_widget {
    Q_OBJECT

private:
    QMenu* menuResults;
    QAction* closeAll;
    QMenu* menuSubResults;
    QAction* defValue;
    QAction* defKilos;
    QAction* defMegas;
    SWTabBar* tabSearch;

    int nCurTabSearch;
    QString curSearchTitle;
    int nSortedColumn;

    QIcon iconSerachActive;
    QIcon iconSearchResult;
    QIcon iconUserFiles;
    SearchModel* model;
    QScopedPointer<SWSortFilterProxyModel> filterModel;
    QString     m_lastSearchFileType;
    QMenu* fileMenu;
public:
    search_widget(QWidget *parent = 0);
    void load();
    void save() const;
    ~search_widget();

private:
    void addCondRow();
    void showErrorParamMsg(int numParam);
    bool hasSelectedMedia();
    bool hasSelectedFiles();
    void updateFileActions();

    void processSearchResult(
        const QList<QED2KSearchResultEntry>& vRes, boost::optional<bool> obMoreResult);
    QED2KHandle addTransfer(const QModelIndex& index);

    void warnDisconnected();
    void prepareNewSearch(const QString& title);

    QModelIndex proxy2source(const QModelIndex& index) {
        if (index.isValid()) {
            return filterModel->mapToSource(index);
        }

        return QModelIndex();
    }

private slots:
    void itemCondClicked(QTableWidgetItem* item);
    void sortChanged(int logicalIndex, Qt::SortOrder order);
    void startSearch();
    void continueSearch();
    void cancelSearch();
    void clearSearch();
    void showOwn(int state);
    void searchRelatedFiles();
    void closeTab(int index);
    void selectTab(int nTabNum);
    void closeAllTabs();
    void setSizeType();
    void searchTextChanged(const QString text);
    void applyFilter(QString filter);
    void displayListMenu(const QPoint&);

    void resultSelectionChanged(const QItemSelection& sel, const QItemSelection& unsel);

    QList<QED2KHandle> on_actionDownload();
    void on_actionDownload_pause();
    void on_actionPreview();
    void on_actionED2K_link();

    void displayHSMenu(const QPoint&);

    void ed2kSearchFinished(const libed2k::net_identifier& np,const QString& hash,
                            const QList<QED2KSearchResultEntry>& vRes, bool bMoreResult);
    void addedTransfer(QED2KHandle h);
    void deletedTransfer(const QString& hash);
};

#endif // SEARCH_WIDGET_H
