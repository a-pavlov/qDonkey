/********************************************************************************
** Form generated from reading UI file 'options.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableView>
#include <QtGui/QTimeEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Preferences
{
public:
    QVBoxLayout *verticalLayout_3;
    QSplitter *hsplitter;
    QListWidget *tabSelection;
    QStackedWidget *tabOption;
    QWidget *tabOptionPage1;
    QVBoxLayout *verticalLayout_10;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *UISettingsBox;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *hboxLayout;
    QLabel *label_9;
    QComboBox *comboI18n;
    QLabel *lbl_i18n_info_2;
    QSpacerItem *spacerItem;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *checkAltRowColors;
    QGroupBox *groupBox_7;
    QFormLayout *formLayout_3;
    QLabel *lblDlList_2;
    QComboBox *actionTorrentDlOnDblClBox;
    QLabel *lblUploadList;
    QComboBox *actionTorrentFnOnDblClBox;
    QGroupBox *systrayBox;
    QVBoxLayout *verticalLayout_12;
    QCheckBox *checkShowSplash;
    QCheckBox *checkStartMinimized;
    QCheckBox *checkProgramExitConfirm;
    QGroupBox *checkShowSystray;
    QVBoxLayout *verticalLayout_22;
    QCheckBox *checkMinimizeToSysTray;
    QCheckBox *checkCloseToSystray;
    QFormLayout *formLayout_4;
    QLabel *label_trayIconStyle;
    QComboBox *comboTrayIcon;
    QGroupBox *groupFileAssociation;
    QVBoxLayout *verticalLayout_28;
    QCheckBox *checkAssociateTorrents;
    QCheckBox *checkAssociateMagnetLinks;
    QCheckBox *checkAssociateecolls;
    QCheckBox *checkAssociateED2KLinks;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_16;
    QCheckBox *checkPreventFromSuspend;
    QCheckBox *checkUpdates;
    QSpacerItem *verticalSpacer_4;
    QWidget *tabOptionPage2;
    QVBoxLayout *verticalLayout_13;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *torrentAdditionBox;
    QGridLayout *gridLayout_5;
    QCheckBox *checkAdditionDialog;
    QCheckBox *checkStartPaused;
    QGroupBox *torrentAdditionBox_2;
    QVBoxLayout *verticalLayout_21;
    QGroupBox *groupBox_13;
    QVBoxLayout *verticalLayout_46;
    QHBoxLayout *horizontalLayout_20;
    QLineEdit *textSavePathMule;
    QToolButton *browseSavePathMule;
    QGroupBox *groupBox_14;
    QVBoxLayout *verticalLayout_47;
    QHBoxLayout *horizontalLayout_21;
    QLineEdit *textTempPathMule;
    QToolButton *browseTempPathMule;
    QGroupBox *fileSystemBox;
    QVBoxLayout *verticalLayout_25;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_26;
    QHBoxLayout *horizontalLayout_10;
    QLineEdit *textSavePath;
    QToolButton *browseSaveDirButton;
    QCheckBox *checkAppendLabel;
    QCheckBox *checkPreallocateAll;
    QGroupBox *checkTempFolder;
    QGridLayout *gridLayout_3;
    QLineEdit *textTempPath;
    QToolButton *browseTempDirButton;
    QCheckBox *checkAppendqB;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_38;
    QHBoxLayout *horizontalLayout_16;
    QTableView *scanFoldersView;
    QVBoxLayout *verticalLayout_37;
    QPushButton *addScanFolderButton;
    QPushButton *removeScanFolderButton;
    QSpacerItem *verticalSpacer_6;
    QGroupBox *checkExportDir;
    QGridLayout *gridLayout_4;
    QLineEdit *textExportDir;
    QToolButton *browseExportDirButton;
    QSpacerItem *verticalSpacer_5;
    QGroupBox *autoRunBox;
    QVBoxLayout *verticalLayout_19;
    QLineEdit *autoRun_txt;
    QLabel *label_4;
    QWidget *tabOptionPage3;
    QVBoxLayout *verticalLayout_6;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QVBoxLayout *verticalLayout_20;
    QHBoxLayout *hboxLayout1;
    QGroupBox *ListeningPortBox;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout2;
    QLabel *lbl_ports;
    QSpinBox *spinPort;
    QPushButton *randomButton;
    QSpacerItem *spacerItem1;
    QCheckBox *checkUPnP;
    QGroupBox *nbConnecBox;
    QGridLayout *gridLayout;
    QCheckBox *checkMaxConnecs;
    QSpinBox *spinMaxConnec;
    QCheckBox *checkMaxConnecsPerTorrent;
    QSpinBox *spinMaxConnecPerTorrent;
    QCheckBox *checkMaxUploadsPerTorrent;
    QSpinBox *spinMaxUploadsPerTorrent;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupProxy;
    QVBoxLayout *verticalLayout_29;
    QHBoxLayout *_2;
    QLabel *ProxyType_lbl;
    QComboBox *comboProxyType;
    QLabel *lblProxyIP;
    QLineEdit *textProxyIP;
    QLabel *lblProxyPort;
    QSpinBox *spinProxyPort;
    QCheckBox *checkProxyPeerConnecs;
    QGroupBox *checkProxyAuth;
    QVBoxLayout *verticalLayout_24;
    QGridLayout *gridLayout_12;
    QLabel *lblProxyUsername;
    QLineEdit *textProxyUsername;
    QLabel *lblProxyPassword;
    QLineEdit *textProxyPassword;
    QGroupBox *checkIPFilter;
    QGridLayout *gridLayout_14;
    QLabel *lblFilterPath;
    QLineEdit *textFilterPath;
    QToolButton *browseFilterButton;
    QToolButton *IpFilterRefreshBtn;
    QVBoxLayout *vboxLayout1;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_34;
    QScrollArea *scrollArea_9;
    QWidget *scrollAreaWidgetContents_9;
    QVBoxLayout *verticalLayout_33;
    QGroupBox *groupBox_8;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_7;
    QGridLayout *gridLayout_13;
    QCheckBox *ed2k_checkUploadLimit;
    QSpinBox *ed2k_spinUploadLimit;
    QLabel *label_23;
    QCheckBox *ed2k_checkDownloadLimit;
    QSpinBox *ed2k_spinDownloadLimit;
    QLabel *label_24;
    QSpacerItem *spacer;
    QGroupBox *bandwidthBox;
    QGridLayout *gridLayout_7;
    QLabel *label_5;
    QGridLayout *gridLayout_6;
    QCheckBox *checkUploadLimit;
    QSpinBox *spinUploadLimit;
    QLabel *label_10;
    QCheckBox *checkDownloadLimit;
    QSpinBox *spinDownloadLimit;
    QLabel *label_13;
    QSpacerItem *spacerItem2;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_27;
    QCheckBox *checkuTP;
    QCheckBox *checkLimituTPConnections;
    QCheckBox *checkLimitTransportOverhead;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_11;
    QGridLayout *gridLayout_9;
    QLabel *label_16;
    QGridLayout *gridLayout_8;
    QLabel *label_11;
    QSpinBox *spinUploadLimitAlt;
    QLabel *label_14;
    QLabel *label_12;
    QSpinBox *spinDownloadLimitAlt;
    QLabel *label_15;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *check_schedule;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_6;
    QTimeEdit *schedule_from;
    QLabel *label_17;
    QTimeEdit *schedule_to;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_18;
    QComboBox *schedule_days;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer_2;
    QWidget *tabEmuleOption;
    QVBoxLayout *verticalLayout_30;
    QScrollArea *scrollArea_5;
    QWidget *scrollAreaWidgetContents_5;
    QVBoxLayout *verticalLayout_39;
    QHBoxLayout *horizontalLayout_9;
    QGroupBox *Username;
    QVBoxLayout *verticalLayout_31;
    QLineEdit *editUserName;
    QHBoxLayout *horizontalLayout_12;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_15;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_25;
    QSpinBox *emuleSpinPort;
    QPushButton *emuleRandomButton;
    QSpacerItem *horizontalSpacer_15;
    QCheckBox *checkEmuleSD;
    QCheckBox *checkRandomServers;
    QCheckBox *checkSaveSearchKeywords;
    QSpacerItem *verticalSpacer_3;
    QWidget *tabOptionPage4;
    QVBoxLayout *verticalLayout_15;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_4;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *AddBTFeaturesBox;
    QVBoxLayout *verticalLayout_14;
    QGroupBox *checkDHT;
    QVBoxLayout *verticalLayout_18;
    QGroupBox *checkDifferentDHTPort;
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *horizontalLayout_8;
    QLabel *dh_port_lbl;
    QSpinBox *spinDHTPort;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *checkPeX;
    QCheckBox *checkLSD;
    QHBoxLayout *hboxLayout3;
    QLabel *lbl_encryption;
    QComboBox *comboEncryption;
    QSpacerItem *spacerItem3;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *checkAnonymousMode;
    QSpacerItem *horizontalSpacer_14;
    QGroupBox *checkEnableQueueing;
    QGridLayout *gridLayout_2;
    QLabel *label_max_active_dl;
    QSpinBox *spinMaxActiveDownloads;
    QLabel *label_max_active_up;
    QSpinBox *spinMaxActiveUploads;
    QLabel *maxActiveTorrents_lbl;
    QSpinBox *spinMaxActiveTorrents;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_11;
    QCheckBox *checkIgnoreSlowTorrentsForQueueing;
    QGroupBox *ratioBox;
    QVBoxLayout *vboxLayout2;
    QHBoxLayout *hboxLayout4;
    QCheckBox *checkMaxRatio;
    QDoubleSpinBox *spinMaxRatio;
    QLabel *label;
    QComboBox *comboRatioLimitAct;
    QSpacerItem *verticalSpacer_8;
    QWidget *page;
    QVBoxLayout *verticalLayout_32;
    QScrollArea *scrollArea_7;
    QWidget *scrollAreaWidgetContents_7;
    QVBoxLayout *verticalLayout_23;
    QGroupBox *checkWebUi;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblWebUiPort;
    QSpinBox *spinWebUiPort;
    QSpacerItem *spacerItem4;
    QCheckBox *checkWebUIUPnP;
    QGroupBox *checkWebUiHttps;
    QGridLayout *gridLayout_11;
    QLabel *lblSslCertStatus;
    QLabel *lblWebUiCrt;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnWebUiCrt;
    QSpacerItem *horizontalSpacer_12;
    QLabel *lblSslKeyStatus;
    QLabel *lblWebUiKey;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnWebUiKey;
    QSpacerItem *horizontalSpacer_13;
    QLabel *lblWebUIInfo;
    QGroupBox *groupWebUiAuth;
    QGridLayout *gridLayout_10;
    QLineEdit *textWebUiUsername;
    QLineEdit *textWebUiPassword;
    QCheckBox *checkBypassLocalAuth;
    QLabel *lblWebUiUsername;
    QLabel *lblWebUiPassword;
    QGroupBox *checkDynDNS;
    QFormLayout *formLayout_5;
    QLabel *label_19;
    QHBoxLayout *horizontalLayout_11;
    QComboBox *comboDNSService;
    QPushButton *registerDNSBtn;
    QLabel *label_20;
    QLineEdit *domainNameTxt;
    QLabel *label_21;
    QLineEdit *DNSUsernameTxt;
    QLabel *label_22;
    QLineEdit *DNSPasswordTxt;
    QSpacerItem *verticalSpacer;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_35;
    QScrollArea *scrollArea_advanced;
    QWidget *scrollAreaWidgetContents_10;
    QVBoxLayout *verticalLayout_36;
    QHBoxLayout *horizontalLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Preferences)
    {
        if (Preferences->objectName().isEmpty())
            Preferences->setObjectName(QString::fromUtf8("Preferences"));
        Preferences->resize(789, 500);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Preferences->sizePolicy().hasHeightForWidth());
        Preferences->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(Preferences);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        hsplitter = new QSplitter(Preferences);
        hsplitter->setObjectName(QString::fromUtf8("hsplitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(hsplitter->sizePolicy().hasHeightForWidth());
        hsplitter->setSizePolicy(sizePolicy1);
        hsplitter->setOrientation(Qt::Horizontal);
        tabSelection = new QListWidget(hsplitter);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(tabSelection);
        __qlistwidgetitem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(tabSelection);
        __qlistwidgetitem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(tabSelection);
        __qlistwidgetitem2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(tabSelection);
        __qlistwidgetitem3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(tabSelection);
        __qlistwidgetitem4->setText(QString::fromUtf8("eMule"));
        __qlistwidgetitem4->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
#ifndef QT_NO_TOOLTIP
        __qlistwidgetitem4->setToolTip(QString::fromUtf8("eMule"));
#endif // QT_NO_TOOLTIP
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(tabSelection);
        __qlistwidgetitem5->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        new QListWidgetItem(tabSelection);
        QListWidgetItem *__qlistwidgetitem6 = new QListWidgetItem(tabSelection);
        __qlistwidgetitem6->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tabSelection->setObjectName(QString::fromUtf8("tabSelection"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabSelection->sizePolicy().hasHeightForWidth());
        tabSelection->setSizePolicy(sizePolicy2);
        tabSelection->setStyleSheet(QString::fromUtf8("QListWidget::item {height: 48px;}"));
        tabSelection->setAutoScroll(false);
        tabSelection->setSelectionBehavior(QAbstractItemView::SelectRows);
        tabSelection->setIconSize(QSize(32, 32));
        tabSelection->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tabSelection->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tabSelection->setResizeMode(QListView::Fixed);
        tabSelection->setUniformItemSizes(true);
        hsplitter->addWidget(tabSelection);
        tabOption = new QStackedWidget(hsplitter);
        tabOption->setObjectName(QString::fromUtf8("tabOption"));
        tabOptionPage1 = new QWidget();
        tabOptionPage1->setObjectName(QString::fromUtf8("tabOptionPage1"));
        verticalLayout_10 = new QVBoxLayout(tabOptionPage1);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        scrollArea = new QScrollArea(tabOptionPage1);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy3);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 480, 713));
        verticalLayout_9 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        UISettingsBox = new QGroupBox(scrollAreaWidgetContents);
        UISettingsBox->setObjectName(QString::fromUtf8("UISettingsBox"));
        verticalLayout_8 = new QVBoxLayout(UISettingsBox);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label_9 = new QLabel(UISettingsBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        hboxLayout->addWidget(label_9);

        comboI18n = new QComboBox(UISettingsBox);
        comboI18n->setObjectName(QString::fromUtf8("comboI18n"));
        comboI18n->setMinimumSize(QSize(0, 0));
        comboI18n->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        comboI18n->setModelColumn(0);

        hboxLayout->addWidget(comboI18n);

        lbl_i18n_info_2 = new QLabel(UISettingsBox);
        lbl_i18n_info_2->setObjectName(QString::fromUtf8("lbl_i18n_info_2"));
        QFont font;
        font.setPointSize(8);
        font.setItalic(true);
        lbl_i18n_info_2->setFont(font);
        lbl_i18n_info_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        hboxLayout->addWidget(lbl_i18n_info_2);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);


        verticalLayout_8->addLayout(hboxLayout);


        verticalLayout_9->addWidget(UISettingsBox);

        groupBox_4 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        checkAltRowColors = new QCheckBox(groupBox_4);
        checkAltRowColors->setObjectName(QString::fromUtf8("checkAltRowColors"));
        checkAltRowColors->setChecked(true);

        verticalLayout_4->addWidget(checkAltRowColors);

        groupBox_7 = new QGroupBox(groupBox_4);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        groupBox_7->setFont(font1);
        groupBox_7->setFlat(true);
        formLayout_3 = new QFormLayout(groupBox_7);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout_3->setContentsMargins(-1, -1, -1, 0);
        lblDlList_2 = new QLabel(groupBox_7);
        lblDlList_2->setObjectName(QString::fromUtf8("lblDlList_2"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, lblDlList_2);

        actionTorrentDlOnDblClBox = new QComboBox(groupBox_7);
        actionTorrentDlOnDblClBox->setObjectName(QString::fromUtf8("actionTorrentDlOnDblClBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(actionTorrentDlOnDblClBox->sizePolicy().hasHeightForWidth());
        actionTorrentDlOnDblClBox->setSizePolicy(sizePolicy4);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, actionTorrentDlOnDblClBox);

        lblUploadList = new QLabel(groupBox_7);
        lblUploadList->setObjectName(QString::fromUtf8("lblUploadList"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, lblUploadList);

        actionTorrentFnOnDblClBox = new QComboBox(groupBox_7);
        actionTorrentFnOnDblClBox->setObjectName(QString::fromUtf8("actionTorrentFnOnDblClBox"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, actionTorrentFnOnDblClBox);


        verticalLayout_4->addWidget(groupBox_7);


        verticalLayout_9->addWidget(groupBox_4);

        systrayBox = new QGroupBox(scrollAreaWidgetContents);
        systrayBox->setObjectName(QString::fromUtf8("systrayBox"));
        verticalLayout_12 = new QVBoxLayout(systrayBox);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        checkShowSplash = new QCheckBox(systrayBox);
        checkShowSplash->setObjectName(QString::fromUtf8("checkShowSplash"));
        checkShowSplash->setChecked(true);

        verticalLayout_12->addWidget(checkShowSplash);

        checkStartMinimized = new QCheckBox(systrayBox);
        checkStartMinimized->setObjectName(QString::fromUtf8("checkStartMinimized"));

        verticalLayout_12->addWidget(checkStartMinimized);

        checkProgramExitConfirm = new QCheckBox(systrayBox);
        checkProgramExitConfirm->setObjectName(QString::fromUtf8("checkProgramExitConfirm"));
        checkProgramExitConfirm->setChecked(true);

        verticalLayout_12->addWidget(checkProgramExitConfirm);

        checkShowSystray = new QGroupBox(systrayBox);
        checkShowSystray->setObjectName(QString::fromUtf8("checkShowSystray"));
        checkShowSystray->setCheckable(true);
        verticalLayout_22 = new QVBoxLayout(checkShowSystray);
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        checkMinimizeToSysTray = new QCheckBox(checkShowSystray);
        checkMinimizeToSysTray->setObjectName(QString::fromUtf8("checkMinimizeToSysTray"));
        checkMinimizeToSysTray->setChecked(false);

        verticalLayout_22->addWidget(checkMinimizeToSysTray);

        checkCloseToSystray = new QCheckBox(checkShowSystray);
        checkCloseToSystray->setObjectName(QString::fromUtf8("checkCloseToSystray"));

        verticalLayout_22->addWidget(checkCloseToSystray);

        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        label_trayIconStyle = new QLabel(checkShowSystray);
        label_trayIconStyle->setObjectName(QString::fromUtf8("label_trayIconStyle"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_trayIconStyle);

        comboTrayIcon = new QComboBox(checkShowSystray);
        comboTrayIcon->setObjectName(QString::fromUtf8("comboTrayIcon"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, comboTrayIcon);


        verticalLayout_22->addLayout(formLayout_4);

        groupFileAssociation = new QGroupBox(checkShowSystray);
        groupFileAssociation->setObjectName(QString::fromUtf8("groupFileAssociation"));
        verticalLayout_28 = new QVBoxLayout(groupFileAssociation);
        verticalLayout_28->setObjectName(QString::fromUtf8("verticalLayout_28"));
        checkAssociateTorrents = new QCheckBox(groupFileAssociation);
        checkAssociateTorrents->setObjectName(QString::fromUtf8("checkAssociateTorrents"));

        verticalLayout_28->addWidget(checkAssociateTorrents);

        checkAssociateMagnetLinks = new QCheckBox(groupFileAssociation);
        checkAssociateMagnetLinks->setObjectName(QString::fromUtf8("checkAssociateMagnetLinks"));

        verticalLayout_28->addWidget(checkAssociateMagnetLinks);

        checkAssociateecolls = new QCheckBox(groupFileAssociation);
        checkAssociateecolls->setObjectName(QString::fromUtf8("checkAssociateecolls"));

        verticalLayout_28->addWidget(checkAssociateecolls);

        checkAssociateED2KLinks = new QCheckBox(groupFileAssociation);
        checkAssociateED2KLinks->setObjectName(QString::fromUtf8("checkAssociateED2KLinks"));

        verticalLayout_28->addWidget(checkAssociateED2KLinks);


        verticalLayout_22->addWidget(groupFileAssociation);


        verticalLayout_12->addWidget(checkShowSystray);


        verticalLayout_9->addWidget(systrayBox);

        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_16 = new QVBoxLayout(groupBox);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        checkPreventFromSuspend = new QCheckBox(groupBox);
        checkPreventFromSuspend->setObjectName(QString::fromUtf8("checkPreventFromSuspend"));

        verticalLayout_16->addWidget(checkPreventFromSuspend);


        verticalLayout_9->addWidget(groupBox);

        checkUpdates = new QCheckBox(scrollAreaWidgetContents);
        checkUpdates->setObjectName(QString::fromUtf8("checkUpdates"));
        checkUpdates->setChecked(true);

        verticalLayout_9->addWidget(checkUpdates);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_4);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_10->addWidget(scrollArea);

        tabOption->addWidget(tabOptionPage1);
        tabOptionPage2 = new QWidget();
        tabOptionPage2->setObjectName(QString::fromUtf8("tabOptionPage2"));
        verticalLayout_13 = new QVBoxLayout(tabOptionPage2);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        scrollArea_2 = new QScrollArea(tabOptionPage2);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 435, 851));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        torrentAdditionBox = new QGroupBox(scrollAreaWidgetContents_2);
        torrentAdditionBox->setObjectName(QString::fromUtf8("torrentAdditionBox"));
        sizePolicy3.setHeightForWidth(torrentAdditionBox->sizePolicy().hasHeightForWidth());
        torrentAdditionBox->setSizePolicy(sizePolicy3);
        gridLayout_5 = new QGridLayout(torrentAdditionBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(-1, -1, -1, 9);
        checkAdditionDialog = new QCheckBox(torrentAdditionBox);
        checkAdditionDialog->setObjectName(QString::fromUtf8("checkAdditionDialog"));
        checkAdditionDialog->setChecked(false);

        gridLayout_5->addWidget(checkAdditionDialog, 0, 0, 1, 2);

        checkStartPaused = new QCheckBox(torrentAdditionBox);
        checkStartPaused->setObjectName(QString::fromUtf8("checkStartPaused"));

        gridLayout_5->addWidget(checkStartPaused, 1, 0, 1, 1);


        verticalLayout->addWidget(torrentAdditionBox);

        torrentAdditionBox_2 = new QGroupBox(scrollAreaWidgetContents_2);
        torrentAdditionBox_2->setObjectName(QString::fromUtf8("torrentAdditionBox_2"));
        sizePolicy3.setHeightForWidth(torrentAdditionBox_2->sizePolicy().hasHeightForWidth());
        torrentAdditionBox_2->setSizePolicy(sizePolicy3);
        verticalLayout_21 = new QVBoxLayout(torrentAdditionBox_2);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        groupBox_13 = new QGroupBox(torrentAdditionBox_2);
        groupBox_13->setObjectName(QString::fromUtf8("groupBox_13"));
        verticalLayout_46 = new QVBoxLayout(groupBox_13);
        verticalLayout_46->setObjectName(QString::fromUtf8("verticalLayout_46"));
        verticalLayout_46->setContentsMargins(-1, -1, -1, 9);
        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        textSavePathMule = new QLineEdit(groupBox_13);
        textSavePathMule->setObjectName(QString::fromUtf8("textSavePathMule"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(textSavePathMule->sizePolicy().hasHeightForWidth());
        textSavePathMule->setSizePolicy(sizePolicy5);

        horizontalLayout_20->addWidget(textSavePathMule);

        browseSavePathMule = new QToolButton(groupBox_13);
        browseSavePathMule->setObjectName(QString::fromUtf8("browseSavePathMule"));
        browseSavePathMule->setEnabled(true);
        browseSavePathMule->setMinimumSize(QSize(22, 22));
        browseSavePathMule->setMaximumSize(QSize(25, 27));
        browseSavePathMule->setText(QString::fromUtf8("..."));

        horizontalLayout_20->addWidget(browseSavePathMule);


        verticalLayout_46->addLayout(horizontalLayout_20);


        verticalLayout_21->addWidget(groupBox_13);

        groupBox_14 = new QGroupBox(torrentAdditionBox_2);
        groupBox_14->setObjectName(QString::fromUtf8("groupBox_14"));
        groupBox_14->setCheckable(true);
        groupBox_14->setChecked(false);
        verticalLayout_47 = new QVBoxLayout(groupBox_14);
        verticalLayout_47->setObjectName(QString::fromUtf8("verticalLayout_47"));
        verticalLayout_47->setContentsMargins(-1, -1, -1, 9);
        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        textTempPathMule = new QLineEdit(groupBox_14);
        textTempPathMule->setObjectName(QString::fromUtf8("textTempPathMule"));
        sizePolicy5.setHeightForWidth(textTempPathMule->sizePolicy().hasHeightForWidth());
        textTempPathMule->setSizePolicy(sizePolicy5);

        horizontalLayout_21->addWidget(textTempPathMule);

        browseTempPathMule = new QToolButton(groupBox_14);
        browseTempPathMule->setObjectName(QString::fromUtf8("browseTempPathMule"));
        browseTempPathMule->setEnabled(false);
        browseTempPathMule->setMinimumSize(QSize(22, 22));
        browseTempPathMule->setMaximumSize(QSize(25, 27));
        browseTempPathMule->setText(QString::fromUtf8("..."));

        horizontalLayout_21->addWidget(browseTempPathMule);


        verticalLayout_47->addLayout(horizontalLayout_21);


        verticalLayout_21->addWidget(groupBox_14);


        verticalLayout->addWidget(torrentAdditionBox_2);

        fileSystemBox = new QGroupBox(scrollAreaWidgetContents_2);
        fileSystemBox->setObjectName(QString::fromUtf8("fileSystemBox"));
        sizePolicy3.setHeightForWidth(fileSystemBox->sizePolicy().hasHeightForWidth());
        fileSystemBox->setSizePolicy(sizePolicy3);
        verticalLayout_25 = new QVBoxLayout(fileSystemBox);
        verticalLayout_25->setObjectName(QString::fromUtf8("verticalLayout_25"));
        verticalLayout_25->setContentsMargins(-1, -1, -1, 9);
        groupBox_3 = new QGroupBox(fileSystemBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_26 = new QVBoxLayout(groupBox_3);
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        verticalLayout_26->setContentsMargins(-1, -1, -1, 9);
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        textSavePath = new QLineEdit(groupBox_3);
        textSavePath->setObjectName(QString::fromUtf8("textSavePath"));
        sizePolicy5.setHeightForWidth(textSavePath->sizePolicy().hasHeightForWidth());
        textSavePath->setSizePolicy(sizePolicy5);

        horizontalLayout_10->addWidget(textSavePath);

        browseSaveDirButton = new QToolButton(groupBox_3);
        browseSaveDirButton->setObjectName(QString::fromUtf8("browseSaveDirButton"));
        browseSaveDirButton->setEnabled(true);
        browseSaveDirButton->setMinimumSize(QSize(22, 22));
        browseSaveDirButton->setMaximumSize(QSize(25, 27));
        browseSaveDirButton->setText(QString::fromUtf8("..."));

        horizontalLayout_10->addWidget(browseSaveDirButton);


        verticalLayout_26->addLayout(horizontalLayout_10);

        checkAppendLabel = new QCheckBox(groupBox_3);
        checkAppendLabel->setObjectName(QString::fromUtf8("checkAppendLabel"));

        verticalLayout_26->addWidget(checkAppendLabel);


        verticalLayout_25->addWidget(groupBox_3);

        checkPreallocateAll = new QCheckBox(fileSystemBox);
        checkPreallocateAll->setObjectName(QString::fromUtf8("checkPreallocateAll"));

        verticalLayout_25->addWidget(checkPreallocateAll);

        checkTempFolder = new QGroupBox(fileSystemBox);
        checkTempFolder->setObjectName(QString::fromUtf8("checkTempFolder"));
        checkTempFolder->setCheckable(true);
        checkTempFolder->setChecked(false);
        gridLayout_3 = new QGridLayout(checkTempFolder);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(-1, -1, -1, 9);
        textTempPath = new QLineEdit(checkTempFolder);
        textTempPath->setObjectName(QString::fromUtf8("textTempPath"));

        gridLayout_3->addWidget(textTempPath, 0, 0, 1, 1);

        browseTempDirButton = new QToolButton(checkTempFolder);
        browseTempDirButton->setObjectName(QString::fromUtf8("browseTempDirButton"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(25);
        sizePolicy6.setVerticalStretch(27);
        sizePolicy6.setHeightForWidth(browseTempDirButton->sizePolicy().hasHeightForWidth());
        browseTempDirButton->setSizePolicy(sizePolicy6);
        browseTempDirButton->setMinimumSize(QSize(22, 22));
        browseTempDirButton->setMaximumSize(QSize(25, 27));
        browseTempDirButton->setText(QString::fromUtf8("..."));

        gridLayout_3->addWidget(browseTempDirButton, 0, 1, 1, 1);


        verticalLayout_25->addWidget(checkTempFolder);

        checkAppendqB = new QCheckBox(fileSystemBox);
        checkAppendqB->setObjectName(QString::fromUtf8("checkAppendqB"));

        verticalLayout_25->addWidget(checkAppendqB);

        groupBox_2 = new QGroupBox(fileSystemBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_38 = new QVBoxLayout(groupBox_2);
        verticalLayout_38->setObjectName(QString::fromUtf8("verticalLayout_38"));
        verticalLayout_38->setContentsMargins(-1, -1, -1, 9);
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        scanFoldersView = new QTableView(groupBox_2);
        scanFoldersView->setObjectName(QString::fromUtf8("scanFoldersView"));
        QSizePolicy sizePolicy7(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(1);
        sizePolicy7.setHeightForWidth(scanFoldersView->sizePolicy().hasHeightForWidth());
        scanFoldersView->setSizePolicy(sizePolicy7);
        scanFoldersView->setMinimumSize(QSize(250, 75));
        scanFoldersView->setSelectionMode(QAbstractItemView::SingleSelection);
        scanFoldersView->setSelectionBehavior(QAbstractItemView::SelectRows);
        scanFoldersView->setShowGrid(false);
        scanFoldersView->setSortingEnabled(true);
        scanFoldersView->horizontalHeader()->setDefaultSectionSize(80);
        scanFoldersView->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        scanFoldersView->horizontalHeader()->setStretchLastSection(true);
        scanFoldersView->verticalHeader()->setVisible(false);

        horizontalLayout_16->addWidget(scanFoldersView);

        verticalLayout_37 = new QVBoxLayout();
        verticalLayout_37->setObjectName(QString::fromUtf8("verticalLayout_37"));
        addScanFolderButton = new QPushButton(groupBox_2);
        addScanFolderButton->setObjectName(QString::fromUtf8("addScanFolderButton"));

        verticalLayout_37->addWidget(addScanFolderButton);

        removeScanFolderButton = new QPushButton(groupBox_2);
        removeScanFolderButton->setObjectName(QString::fromUtf8("removeScanFolderButton"));
        removeScanFolderButton->setEnabled(false);

        verticalLayout_37->addWidget(removeScanFolderButton);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_37->addItem(verticalSpacer_6);


        horizontalLayout_16->addLayout(verticalLayout_37);


        verticalLayout_38->addLayout(horizontalLayout_16);


        verticalLayout_25->addWidget(groupBox_2);

        checkExportDir = new QGroupBox(fileSystemBox);
        checkExportDir->setObjectName(QString::fromUtf8("checkExportDir"));
        sizePolicy2.setHeightForWidth(checkExportDir->sizePolicy().hasHeightForWidth());
        checkExportDir->setSizePolicy(sizePolicy2);
        checkExportDir->setCheckable(true);
        checkExportDir->setChecked(false);
        gridLayout_4 = new QGridLayout(checkExportDir);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(-1, -1, -1, 9);
        textExportDir = new QLineEdit(checkExportDir);
        textExportDir->setObjectName(QString::fromUtf8("textExportDir"));

        gridLayout_4->addWidget(textExportDir, 0, 0, 1, 1);

        browseExportDirButton = new QToolButton(checkExportDir);
        browseExportDirButton->setObjectName(QString::fromUtf8("browseExportDirButton"));
        browseExportDirButton->setMinimumSize(QSize(22, 22));
        browseExportDirButton->setMaximumSize(QSize(25, 27));
        browseExportDirButton->setText(QString::fromUtf8("..."));

        gridLayout_4->addWidget(browseExportDirButton, 0, 1, 1, 1);


        verticalLayout_25->addWidget(checkExportDir);


        verticalLayout->addWidget(fileSystemBox);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        autoRunBox = new QGroupBox(scrollAreaWidgetContents_2);
        autoRunBox->setObjectName(QString::fromUtf8("autoRunBox"));
        autoRunBox->setCheckable(true);
        autoRunBox->setChecked(false);
        verticalLayout_19 = new QVBoxLayout(autoRunBox);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        autoRun_txt = new QLineEdit(autoRunBox);
        autoRun_txt->setObjectName(QString::fromUtf8("autoRun_txt"));

        verticalLayout_19->addWidget(autoRun_txt);

        label_4 = new QLabel(autoRunBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setTextFormat(Qt::RichText);

        verticalLayout_19->addWidget(label_4);


        verticalLayout->addWidget(autoRunBox);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_13->addWidget(scrollArea_2);

        tabOption->addWidget(tabOptionPage2);
        tabOptionPage3 = new QWidget();
        tabOptionPage3->setObjectName(QString::fromUtf8("tabOptionPage3"));
        verticalLayout_6 = new QVBoxLayout(tabOptionPage3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        scrollArea_3 = new QScrollArea(tabOptionPage3);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 420, 506));
        verticalLayout_20 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        ListeningPortBox = new QGroupBox(scrollAreaWidgetContents_3);
        ListeningPortBox->setObjectName(QString::fromUtf8("ListeningPortBox"));
        vboxLayout = new QVBoxLayout(ListeningPortBox);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        lbl_ports = new QLabel(ListeningPortBox);
        lbl_ports->setObjectName(QString::fromUtf8("lbl_ports"));

        hboxLayout2->addWidget(lbl_ports);

        spinPort = new QSpinBox(ListeningPortBox);
        spinPort->setObjectName(QString::fromUtf8("spinPort"));
        spinPort->setMinimum(1);
        spinPort->setMaximum(65535);
        spinPort->setValue(6881);

        hboxLayout2->addWidget(spinPort);

        randomButton = new QPushButton(ListeningPortBox);
        randomButton->setObjectName(QString::fromUtf8("randomButton"));

        hboxLayout2->addWidget(randomButton);

        spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem1);


        vboxLayout->addLayout(hboxLayout2);

        checkUPnP = new QCheckBox(ListeningPortBox);
        checkUPnP->setObjectName(QString::fromUtf8("checkUPnP"));
        checkUPnP->setChecked(true);

        vboxLayout->addWidget(checkUPnP);


        hboxLayout1->addWidget(ListeningPortBox);


        verticalLayout_20->addLayout(hboxLayout1);

        nbConnecBox = new QGroupBox(scrollAreaWidgetContents_3);
        nbConnecBox->setObjectName(QString::fromUtf8("nbConnecBox"));
        gridLayout = new QGridLayout(nbConnecBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkMaxConnecs = new QCheckBox(nbConnecBox);
        checkMaxConnecs->setObjectName(QString::fromUtf8("checkMaxConnecs"));
        checkMaxConnecs->setChecked(true);

        gridLayout->addWidget(checkMaxConnecs, 0, 0, 1, 1);

        spinMaxConnec = new QSpinBox(nbConnecBox);
        spinMaxConnec->setObjectName(QString::fromUtf8("spinMaxConnec"));
        spinMaxConnec->setEnabled(true);
        spinMaxConnec->setMinimum(2);
        spinMaxConnec->setMaximum(2000);
        spinMaxConnec->setValue(500);

        gridLayout->addWidget(spinMaxConnec, 0, 1, 1, 1);

        checkMaxConnecsPerTorrent = new QCheckBox(nbConnecBox);
        checkMaxConnecsPerTorrent->setObjectName(QString::fromUtf8("checkMaxConnecsPerTorrent"));
        checkMaxConnecsPerTorrent->setChecked(true);

        gridLayout->addWidget(checkMaxConnecsPerTorrent, 1, 0, 1, 1);

        spinMaxConnecPerTorrent = new QSpinBox(nbConnecBox);
        spinMaxConnecPerTorrent->setObjectName(QString::fromUtf8("spinMaxConnecPerTorrent"));
        spinMaxConnecPerTorrent->setMinimum(2);
        spinMaxConnecPerTorrent->setMaximum(2000);
        spinMaxConnecPerTorrent->setValue(100);

        gridLayout->addWidget(spinMaxConnecPerTorrent, 1, 1, 1, 1);

        checkMaxUploadsPerTorrent = new QCheckBox(nbConnecBox);
        checkMaxUploadsPerTorrent->setObjectName(QString::fromUtf8("checkMaxUploadsPerTorrent"));
        checkMaxUploadsPerTorrent->setChecked(true);

        gridLayout->addWidget(checkMaxUploadsPerTorrent, 2, 0, 1, 1);

        spinMaxUploadsPerTorrent = new QSpinBox(nbConnecBox);
        spinMaxUploadsPerTorrent->setObjectName(QString::fromUtf8("spinMaxUploadsPerTorrent"));
        spinMaxUploadsPerTorrent->setMaximum(500);
        spinMaxUploadsPerTorrent->setValue(4);

        gridLayout->addWidget(spinMaxUploadsPerTorrent, 2, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 2, 2, 1, 1);


        verticalLayout_20->addWidget(nbConnecBox);

        groupProxy = new QGroupBox(scrollAreaWidgetContents_3);
        groupProxy->setObjectName(QString::fromUtf8("groupProxy"));
        groupProxy->setEnabled(true);
        verticalLayout_29 = new QVBoxLayout(groupProxy);
        verticalLayout_29->setObjectName(QString::fromUtf8("verticalLayout_29"));
        _2 = new QHBoxLayout();
        _2->setObjectName(QString::fromUtf8("_2"));
        ProxyType_lbl = new QLabel(groupProxy);
        ProxyType_lbl->setObjectName(QString::fromUtf8("ProxyType_lbl"));

        _2->addWidget(ProxyType_lbl);

        comboProxyType = new QComboBox(groupProxy);
        comboProxyType->setObjectName(QString::fromUtf8("comboProxyType"));

        _2->addWidget(comboProxyType);

        lblProxyIP = new QLabel(groupProxy);
        lblProxyIP->setObjectName(QString::fromUtf8("lblProxyIP"));
        lblProxyIP->setEnabled(false);

        _2->addWidget(lblProxyIP);

        textProxyIP = new QLineEdit(groupProxy);
        textProxyIP->setObjectName(QString::fromUtf8("textProxyIP"));
        textProxyIP->setEnabled(false);
        textProxyIP->setMaxLength(75);
        textProxyIP->setEchoMode(QLineEdit::Normal);

        _2->addWidget(textProxyIP);

        lblProxyPort = new QLabel(groupProxy);
        lblProxyPort->setObjectName(QString::fromUtf8("lblProxyPort"));
        lblProxyPort->setEnabled(false);

        _2->addWidget(lblProxyPort);

        spinProxyPort = new QSpinBox(groupProxy);
        spinProxyPort->setObjectName(QString::fromUtf8("spinProxyPort"));
        spinProxyPort->setEnabled(false);
        spinProxyPort->setMinimum(1);
        spinProxyPort->setMaximum(65535);
        spinProxyPort->setValue(8080);

        _2->addWidget(spinProxyPort);


        verticalLayout_29->addLayout(_2);

        checkProxyPeerConnecs = new QCheckBox(groupProxy);
        checkProxyPeerConnecs->setObjectName(QString::fromUtf8("checkProxyPeerConnecs"));
        checkProxyPeerConnecs->setEnabled(false);

        verticalLayout_29->addWidget(checkProxyPeerConnecs);

        checkProxyAuth = new QGroupBox(groupProxy);
        checkProxyAuth->setObjectName(QString::fromUtf8("checkProxyAuth"));
        checkProxyAuth->setEnabled(false);
        checkProxyAuth->setCheckable(true);
        checkProxyAuth->setChecked(false);
        verticalLayout_24 = new QVBoxLayout(checkProxyAuth);
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        lblProxyUsername = new QLabel(checkProxyAuth);
        lblProxyUsername->setObjectName(QString::fromUtf8("lblProxyUsername"));

        gridLayout_12->addWidget(lblProxyUsername, 0, 0, 1, 1);

        textProxyUsername = new QLineEdit(checkProxyAuth);
        textProxyUsername->setObjectName(QString::fromUtf8("textProxyUsername"));
        textProxyUsername->setMaxLength(1000);
        textProxyUsername->setEchoMode(QLineEdit::Normal);

        gridLayout_12->addWidget(textProxyUsername, 0, 1, 1, 1);

        lblProxyPassword = new QLabel(checkProxyAuth);
        lblProxyPassword->setObjectName(QString::fromUtf8("lblProxyPassword"));

        gridLayout_12->addWidget(lblProxyPassword, 1, 0, 1, 1);

        textProxyPassword = new QLineEdit(checkProxyAuth);
        textProxyPassword->setObjectName(QString::fromUtf8("textProxyPassword"));
        textProxyPassword->setMaxLength(1000);
        textProxyPassword->setEchoMode(QLineEdit::Password);

        gridLayout_12->addWidget(textProxyPassword, 1, 1, 1, 1);


        verticalLayout_24->addLayout(gridLayout_12);


        verticalLayout_29->addWidget(checkProxyAuth);


        verticalLayout_20->addWidget(groupProxy);

        checkIPFilter = new QGroupBox(scrollAreaWidgetContents_3);
        checkIPFilter->setObjectName(QString::fromUtf8("checkIPFilter"));
        checkIPFilter->setCheckable(true);
        checkIPFilter->setChecked(false);
        gridLayout_14 = new QGridLayout(checkIPFilter);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        lblFilterPath = new QLabel(checkIPFilter);
        lblFilterPath->setObjectName(QString::fromUtf8("lblFilterPath"));

        gridLayout_14->addWidget(lblFilterPath, 0, 0, 1, 1);

        textFilterPath = new QLineEdit(checkIPFilter);
        textFilterPath->setObjectName(QString::fromUtf8("textFilterPath"));

        gridLayout_14->addWidget(textFilterPath, 0, 1, 1, 1);

        browseFilterButton = new QToolButton(checkIPFilter);
        browseFilterButton->setObjectName(QString::fromUtf8("browseFilterButton"));
        browseFilterButton->setMinimumSize(QSize(28, 27));
        browseFilterButton->setText(QString::fromUtf8("..."));

        gridLayout_14->addWidget(browseFilterButton, 0, 2, 1, 1);

        IpFilterRefreshBtn = new QToolButton(checkIPFilter);
        IpFilterRefreshBtn->setObjectName(QString::fromUtf8("IpFilterRefreshBtn"));
        IpFilterRefreshBtn->setMinimumSize(QSize(28, 27));
        IpFilterRefreshBtn->setIconSize(QSize(16, 16));

        gridLayout_14->addWidget(IpFilterRefreshBtn, 0, 3, 1, 1);


        verticalLayout_20->addWidget(checkIPFilter);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_6->addWidget(scrollArea_3);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));

        verticalLayout_6->addLayout(vboxLayout1);

        tabOption->addWidget(tabOptionPage3);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_34 = new QVBoxLayout(page_2);
        verticalLayout_34->setObjectName(QString::fromUtf8("verticalLayout_34"));
        scrollArea_9 = new QScrollArea(page_2);
        scrollArea_9->setObjectName(QString::fromUtf8("scrollArea_9"));
        scrollArea_9->setWidgetResizable(true);
        scrollAreaWidgetContents_9 = new QWidget();
        scrollAreaWidgetContents_9->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_9"));
        scrollAreaWidgetContents_9->setGeometry(QRect(0, 0, 346, 540));
        verticalLayout_33 = new QVBoxLayout(scrollAreaWidgetContents_9);
        verticalLayout_33->setObjectName(QString::fromUtf8("verticalLayout_33"));
        groupBox_8 = new QGroupBox(scrollAreaWidgetContents_9);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_8);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_7 = new QLabel(groupBox_8);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setText(QString::fromUtf8(""));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/Icons/actions/slow_off.png")));

        horizontalLayout_2->addWidget(label_7);

        gridLayout_13 = new QGridLayout();
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        ed2k_checkUploadLimit = new QCheckBox(groupBox_8);
        ed2k_checkUploadLimit->setObjectName(QString::fromUtf8("ed2k_checkUploadLimit"));
        ed2k_checkUploadLimit->setChecked(true);

        gridLayout_13->addWidget(ed2k_checkUploadLimit, 0, 0, 1, 1);

        ed2k_spinUploadLimit = new QSpinBox(groupBox_8);
        ed2k_spinUploadLimit->setObjectName(QString::fromUtf8("ed2k_spinUploadLimit"));
        ed2k_spinUploadLimit->setEnabled(true);
        ed2k_spinUploadLimit->setMinimum(25);
        ed2k_spinUploadLimit->setMaximum(1000000);
        ed2k_spinUploadLimit->setValue(8000);

        gridLayout_13->addWidget(ed2k_spinUploadLimit, 0, 1, 1, 1);

        label_23 = new QLabel(groupBox_8);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_13->addWidget(label_23, 0, 2, 1, 1);

        ed2k_checkDownloadLimit = new QCheckBox(groupBox_8);
        ed2k_checkDownloadLimit->setObjectName(QString::fromUtf8("ed2k_checkDownloadLimit"));

        gridLayout_13->addWidget(ed2k_checkDownloadLimit, 1, 0, 1, 1);

        ed2k_spinDownloadLimit = new QSpinBox(groupBox_8);
        ed2k_spinDownloadLimit->setObjectName(QString::fromUtf8("ed2k_spinDownloadLimit"));
        ed2k_spinDownloadLimit->setEnabled(false);
        ed2k_spinDownloadLimit->setMinimum(1);
        ed2k_spinDownloadLimit->setMaximum(1000000);
        ed2k_spinDownloadLimit->setValue(10000);

        gridLayout_13->addWidget(ed2k_spinDownloadLimit, 1, 1, 1, 1);

        label_24 = new QLabel(groupBox_8);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_13->addWidget(label_24, 1, 2, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_13);

        spacer = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(spacer);


        verticalLayout_33->addWidget(groupBox_8);

        bandwidthBox = new QGroupBox(scrollAreaWidgetContents_9);
        bandwidthBox->setObjectName(QString::fromUtf8("bandwidthBox"));
        gridLayout_7 = new QGridLayout(bandwidthBox);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_5 = new QLabel(bandwidthBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setText(QString::fromUtf8(""));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/Icons/actions/slow_off.png")));

        gridLayout_7->addWidget(label_5, 0, 0, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        checkUploadLimit = new QCheckBox(bandwidthBox);
        checkUploadLimit->setObjectName(QString::fromUtf8("checkUploadLimit"));
        checkUploadLimit->setChecked(true);

        gridLayout_6->addWidget(checkUploadLimit, 0, 0, 1, 1);

        spinUploadLimit = new QSpinBox(bandwidthBox);
        spinUploadLimit->setObjectName(QString::fromUtf8("spinUploadLimit"));
        spinUploadLimit->setEnabled(true);
        spinUploadLimit->setMinimum(25);
        spinUploadLimit->setMaximum(1000000);
        spinUploadLimit->setValue(300);

        gridLayout_6->addWidget(spinUploadLimit, 0, 1, 1, 1);

        label_10 = new QLabel(bandwidthBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_6->addWidget(label_10, 0, 2, 1, 1);

        checkDownloadLimit = new QCheckBox(bandwidthBox);
        checkDownloadLimit->setObjectName(QString::fromUtf8("checkDownloadLimit"));

        gridLayout_6->addWidget(checkDownloadLimit, 1, 0, 1, 1);

        spinDownloadLimit = new QSpinBox(bandwidthBox);
        spinDownloadLimit->setObjectName(QString::fromUtf8("spinDownloadLimit"));
        spinDownloadLimit->setEnabled(false);
        spinDownloadLimit->setMinimum(1);
        spinDownloadLimit->setMaximum(1000000);
        spinDownloadLimit->setValue(100);

        gridLayout_6->addWidget(spinDownloadLimit, 1, 1, 1, 1);

        label_13 = new QLabel(bandwidthBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_6->addWidget(label_13, 1, 2, 1, 1);


        gridLayout_7->addLayout(gridLayout_6, 0, 1, 1, 1);

        spacerItem2 = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(spacerItem2, 0, 2, 1, 1);

        groupBox_5 = new QGroupBox(bandwidthBox);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_27 = new QVBoxLayout(groupBox_5);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        checkuTP = new QCheckBox(groupBox_5);
        checkuTP->setObjectName(QString::fromUtf8("checkuTP"));
        checkuTP->setChecked(true);

        verticalLayout_27->addWidget(checkuTP);

        checkLimituTPConnections = new QCheckBox(groupBox_5);
        checkLimituTPConnections->setObjectName(QString::fromUtf8("checkLimituTPConnections"));

        verticalLayout_27->addWidget(checkLimituTPConnections);

        checkLimitTransportOverhead = new QCheckBox(groupBox_5);
        checkLimitTransportOverhead->setObjectName(QString::fromUtf8("checkLimitTransportOverhead"));

        verticalLayout_27->addWidget(checkLimitTransportOverhead);


        gridLayout_7->addWidget(groupBox_5, 1, 1, 1, 2);


        verticalLayout_33->addWidget(bandwidthBox);

        groupBox_6 = new QGroupBox(scrollAreaWidgetContents_9);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_11 = new QVBoxLayout(groupBox_6);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_16 = new QLabel(groupBox_6);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(48, 48));
        label_16->setMaximumSize(QSize(48, 48));
        label_16->setPixmap(QPixmap(QString::fromUtf8(":/Icons/actions/slow.png")));
        label_16->setScaledContents(false);

        gridLayout_9->addWidget(label_16, 0, 0, 1, 1);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label_11 = new QLabel(groupBox_6);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_8->addWidget(label_11, 0, 0, 1, 1);

        spinUploadLimitAlt = new QSpinBox(groupBox_6);
        spinUploadLimitAlt->setObjectName(QString::fromUtf8("spinUploadLimitAlt"));
        spinUploadLimitAlt->setMinimum(1);
        spinUploadLimitAlt->setMaximum(1000000);
        spinUploadLimitAlt->setValue(10);

        gridLayout_8->addWidget(spinUploadLimitAlt, 0, 1, 1, 1);

        label_14 = new QLabel(groupBox_6);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_8->addWidget(label_14, 0, 2, 1, 1);

        label_12 = new QLabel(groupBox_6);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_8->addWidget(label_12, 1, 0, 1, 1);

        spinDownloadLimitAlt = new QSpinBox(groupBox_6);
        spinDownloadLimitAlt->setObjectName(QString::fromUtf8("spinDownloadLimitAlt"));
        spinDownloadLimitAlt->setMinimum(1);
        spinDownloadLimitAlt->setMaximum(1000000);
        spinDownloadLimitAlt->setValue(10);

        gridLayout_8->addWidget(spinDownloadLimitAlt, 1, 1, 1, 1);

        label_15 = new QLabel(groupBox_6);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_8->addWidget(label_15, 1, 2, 1, 1);


        gridLayout_9->addLayout(gridLayout_8, 0, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(222, 59, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_8, 0, 2, 1, 1);


        verticalLayout_11->addLayout(gridLayout_9);

        check_schedule = new QGroupBox(groupBox_6);
        check_schedule->setObjectName(QString::fromUtf8("check_schedule"));
        check_schedule->setCheckable(true);
        check_schedule->setChecked(false);
        verticalLayout_5 = new QVBoxLayout(check_schedule);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(5, -1, -1, -1);
        label_6 = new QLabel(check_schedule);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_13->addWidget(label_6);

        schedule_from = new QTimeEdit(check_schedule);
        schedule_from->setObjectName(QString::fromUtf8("schedule_from"));
        schedule_from->setDisplayFormat(QString::fromUtf8("hh:mm"));
        schedule_from->setCalendarPopup(false);
        schedule_from->setTime(QTime(8, 0, 0));

        horizontalLayout_13->addWidget(schedule_from);

        label_17 = new QLabel(check_schedule);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignCenter);

        horizontalLayout_13->addWidget(label_17);

        schedule_to = new QTimeEdit(check_schedule);
        schedule_to->setObjectName(QString::fromUtf8("schedule_to"));
        schedule_to->setDisplayFormat(QString::fromUtf8("hh:mm"));
        schedule_to->setTime(QTime(20, 0, 0));

        horizontalLayout_13->addWidget(schedule_to);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_9);


        verticalLayout_5->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(5, -1, -1, -1);
        label_18 = new QLabel(check_schedule);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_14->addWidget(label_18);

        schedule_days = new QComboBox(check_schedule);
        schedule_days->setObjectName(QString::fromUtf8("schedule_days"));

        horizontalLayout_14->addWidget(schedule_days);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_10);


        verticalLayout_5->addLayout(horizontalLayout_14);


        verticalLayout_11->addWidget(check_schedule);


        verticalLayout_33->addWidget(groupBox_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_33->addItem(verticalSpacer_2);

        scrollArea_9->setWidget(scrollAreaWidgetContents_9);

        verticalLayout_34->addWidget(scrollArea_9);

        tabOption->addWidget(page_2);
        tabEmuleOption = new QWidget();
        tabEmuleOption->setObjectName(QString::fromUtf8("tabEmuleOption"));
        verticalLayout_30 = new QVBoxLayout(tabEmuleOption);
        verticalLayout_30->setObjectName(QString::fromUtf8("verticalLayout_30"));
        scrollArea_5 = new QScrollArea(tabEmuleOption);
        scrollArea_5->setObjectName(QString::fromUtf8("scrollArea_5"));
        scrollArea_5->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 420, 241));
        verticalLayout_39 = new QVBoxLayout(scrollAreaWidgetContents_5);
        verticalLayout_39->setObjectName(QString::fromUtf8("verticalLayout_39"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        Username = new QGroupBox(scrollAreaWidgetContents_5);
        Username->setObjectName(QString::fromUtf8("Username"));
        verticalLayout_31 = new QVBoxLayout(Username);
        verticalLayout_31->setObjectName(QString::fromUtf8("verticalLayout_31"));
        editUserName = new QLineEdit(Username);
        editUserName->setObjectName(QString::fromUtf8("editUserName"));

        verticalLayout_31->addWidget(editUserName);


        horizontalLayout_9->addWidget(Username);


        verticalLayout_39->addLayout(horizontalLayout_9);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        groupBox_9 = new QGroupBox(scrollAreaWidgetContents_5);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout_15 = new QGridLayout(groupBox_9);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_25 = new QLabel(groupBox_9);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        horizontalLayout_7->addWidget(label_25);

        emuleSpinPort = new QSpinBox(groupBox_9);
        emuleSpinPort->setObjectName(QString::fromUtf8("emuleSpinPort"));
        emuleSpinPort->setMinimum(1);
        emuleSpinPort->setMaximum(65535);
        emuleSpinPort->setValue(4661);

        horizontalLayout_7->addWidget(emuleSpinPort);

        emuleRandomButton = new QPushButton(groupBox_9);
        emuleRandomButton->setObjectName(QString::fromUtf8("emuleRandomButton"));

        horizontalLayout_7->addWidget(emuleRandomButton);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_15);


        gridLayout_15->addLayout(horizontalLayout_7, 0, 0, 2, 1);

        checkEmuleSD = new QCheckBox(groupBox_9);
        checkEmuleSD->setObjectName(QString::fromUtf8("checkEmuleSD"));

        gridLayout_15->addWidget(checkEmuleSD, 4, 0, 1, 1);

        checkRandomServers = new QCheckBox(groupBox_9);
        checkRandomServers->setObjectName(QString::fromUtf8("checkRandomServers"));
        checkRandomServers->setChecked(true);

        gridLayout_15->addWidget(checkRandomServers, 5, 0, 1, 1);


        horizontalLayout_12->addWidget(groupBox_9);


        verticalLayout_39->addLayout(horizontalLayout_12);

        checkSaveSearchKeywords = new QCheckBox(scrollAreaWidgetContents_5);
        checkSaveSearchKeywords->setObjectName(QString::fromUtf8("checkSaveSearchKeywords"));

        verticalLayout_39->addWidget(checkSaveSearchKeywords);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_39->addItem(verticalSpacer_3);

        scrollArea_5->setWidget(scrollAreaWidgetContents_5);

        verticalLayout_30->addWidget(scrollArea_5);

        tabOption->addWidget(tabEmuleOption);
        tabOptionPage4 = new QWidget();
        tabOptionPage4->setObjectName(QString::fromUtf8("tabOptionPage4"));
        verticalLayout_15 = new QVBoxLayout(tabOptionPage4);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        scrollArea_4 = new QScrollArea(tabOptionPage4);
        scrollArea_4->setObjectName(QString::fromUtf8("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 512, 500));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents_4);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        AddBTFeaturesBox = new QGroupBox(scrollAreaWidgetContents_4);
        AddBTFeaturesBox->setObjectName(QString::fromUtf8("AddBTFeaturesBox"));
        verticalLayout_14 = new QVBoxLayout(AddBTFeaturesBox);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        checkDHT = new QGroupBox(AddBTFeaturesBox);
        checkDHT->setObjectName(QString::fromUtf8("checkDHT"));
        checkDHT->setCheckable(true);
        verticalLayout_18 = new QVBoxLayout(checkDHT);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        checkDifferentDHTPort = new QGroupBox(checkDHT);
        checkDifferentDHTPort->setObjectName(QString::fromUtf8("checkDifferentDHTPort"));
        checkDifferentDHTPort->setCheckable(true);
        checkDifferentDHTPort->setChecked(false);
        verticalLayout_17 = new QVBoxLayout(checkDifferentDHTPort);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(-1, -1, -1, 9);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(5, -1, -1, -1);
        dh_port_lbl = new QLabel(checkDifferentDHTPort);
        dh_port_lbl->setObjectName(QString::fromUtf8("dh_port_lbl"));

        horizontalLayout_8->addWidget(dh_port_lbl);

        spinDHTPort = new QSpinBox(checkDifferentDHTPort);
        spinDHTPort->setObjectName(QString::fromUtf8("spinDHTPort"));
        spinDHTPort->setMinimum(1);
        spinDHTPort->setMaximum(65525);
        spinDHTPort->setValue(6881);

        horizontalLayout_8->addWidget(spinDHTPort);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout_17->addLayout(horizontalLayout_8);


        verticalLayout_18->addWidget(checkDifferentDHTPort);


        verticalLayout_14->addWidget(checkDHT);

        checkPeX = new QCheckBox(AddBTFeaturesBox);
        checkPeX->setObjectName(QString::fromUtf8("checkPeX"));
        checkPeX->setChecked(true);

        verticalLayout_14->addWidget(checkPeX);

        checkLSD = new QCheckBox(AddBTFeaturesBox);
        checkLSD->setObjectName(QString::fromUtf8("checkLSD"));
        checkLSD->setChecked(true);

        verticalLayout_14->addWidget(checkLSD);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        lbl_encryption = new QLabel(AddBTFeaturesBox);
        lbl_encryption->setObjectName(QString::fromUtf8("lbl_encryption"));

        hboxLayout3->addWidget(lbl_encryption);

        comboEncryption = new QComboBox(AddBTFeaturesBox);
        comboEncryption->setObjectName(QString::fromUtf8("comboEncryption"));

        hboxLayout3->addWidget(comboEncryption);

        spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem3);


        verticalLayout_14->addLayout(hboxLayout3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        checkAnonymousMode = new QCheckBox(AddBTFeaturesBox);
        checkAnonymousMode->setObjectName(QString::fromUtf8("checkAnonymousMode"));

        horizontalLayout_6->addWidget(checkAnonymousMode);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_14);


        verticalLayout_14->addLayout(horizontalLayout_6);


        verticalLayout_7->addWidget(AddBTFeaturesBox);

        checkEnableQueueing = new QGroupBox(scrollAreaWidgetContents_4);
        checkEnableQueueing->setObjectName(QString::fromUtf8("checkEnableQueueing"));
        checkEnableQueueing->setCheckable(true);
        checkEnableQueueing->setChecked(false);
        gridLayout_2 = new QGridLayout(checkEnableQueueing);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(-1, -1, -1, 9);
        label_max_active_dl = new QLabel(checkEnableQueueing);
        label_max_active_dl->setObjectName(QString::fromUtf8("label_max_active_dl"));

        gridLayout_2->addWidget(label_max_active_dl, 0, 0, 1, 1);

        spinMaxActiveDownloads = new QSpinBox(checkEnableQueueing);
        spinMaxActiveDownloads->setObjectName(QString::fromUtf8("spinMaxActiveDownloads"));
        spinMaxActiveDownloads->setMinimum(-1);
        spinMaxActiveDownloads->setMaximum(999);
        spinMaxActiveDownloads->setValue(3);

        gridLayout_2->addWidget(spinMaxActiveDownloads, 0, 1, 1, 1);

        label_max_active_up = new QLabel(checkEnableQueueing);
        label_max_active_up->setObjectName(QString::fromUtf8("label_max_active_up"));

        gridLayout_2->addWidget(label_max_active_up, 1, 0, 1, 1);

        spinMaxActiveUploads = new QSpinBox(checkEnableQueueing);
        spinMaxActiveUploads->setObjectName(QString::fromUtf8("spinMaxActiveUploads"));
        spinMaxActiveUploads->setMinimum(-1);
        spinMaxActiveUploads->setMaximum(999);
        spinMaxActiveUploads->setValue(3);

        gridLayout_2->addWidget(spinMaxActiveUploads, 1, 1, 1, 1);

        maxActiveTorrents_lbl = new QLabel(checkEnableQueueing);
        maxActiveTorrents_lbl->setObjectName(QString::fromUtf8("maxActiveTorrents_lbl"));

        gridLayout_2->addWidget(maxActiveTorrents_lbl, 2, 0, 1, 1);

        spinMaxActiveTorrents = new QSpinBox(checkEnableQueueing);
        spinMaxActiveTorrents->setObjectName(QString::fromUtf8("spinMaxActiveTorrents"));
        spinMaxActiveTorrents->setMinimum(-1);
        spinMaxActiveTorrents->setMaximum(999);
        spinMaxActiveTorrents->setValue(5);

        gridLayout_2->addWidget(spinMaxActiveTorrents, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 0, 2, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_11, 2, 2, 1, 1);

        checkIgnoreSlowTorrentsForQueueing = new QCheckBox(checkEnableQueueing);
        checkIgnoreSlowTorrentsForQueueing->setObjectName(QString::fromUtf8("checkIgnoreSlowTorrentsForQueueing"));

        gridLayout_2->addWidget(checkIgnoreSlowTorrentsForQueueing, 3, 0, 1, 3);


        verticalLayout_7->addWidget(checkEnableQueueing);

        ratioBox = new QGroupBox(scrollAreaWidgetContents_4);
        ratioBox->setObjectName(QString::fromUtf8("ratioBox"));
        vboxLayout2 = new QVBoxLayout(ratioBox);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        vboxLayout2->setContentsMargins(-1, -1, -1, 9);
        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        checkMaxRatio = new QCheckBox(ratioBox);
        checkMaxRatio->setObjectName(QString::fromUtf8("checkMaxRatio"));

        hboxLayout4->addWidget(checkMaxRatio);

        spinMaxRatio = new QDoubleSpinBox(ratioBox);
        spinMaxRatio->setObjectName(QString::fromUtf8("spinMaxRatio"));
        spinMaxRatio->setEnabled(false);
        spinMaxRatio->setAlignment(Qt::AlignHCenter);
        spinMaxRatio->setDecimals(1);
        spinMaxRatio->setMinimum(0);
        spinMaxRatio->setMaximum(20);
        spinMaxRatio->setSingleStep(0.05);
        spinMaxRatio->setValue(1);

        hboxLayout4->addWidget(spinMaxRatio);

        label = new QLabel(ratioBox);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout4->addWidget(label);

        comboRatioLimitAct = new QComboBox(ratioBox);
        comboRatioLimitAct->setObjectName(QString::fromUtf8("comboRatioLimitAct"));
        comboRatioLimitAct->setEnabled(false);

        hboxLayout4->addWidget(comboRatioLimitAct);


        vboxLayout2->addLayout(hboxLayout4);


        verticalLayout_7->addWidget(ratioBox);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_8);

        scrollArea_4->setWidget(scrollAreaWidgetContents_4);

        verticalLayout_15->addWidget(scrollArea_4);

        tabOption->addWidget(tabOptionPage4);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_32 = new QVBoxLayout(page);
        verticalLayout_32->setObjectName(QString::fromUtf8("verticalLayout_32"));
        scrollArea_7 = new QScrollArea(page);
        scrollArea_7->setObjectName(QString::fromUtf8("scrollArea_7"));
        scrollArea_7->setWidgetResizable(true);
        scrollAreaWidgetContents_7 = new QWidget();
        scrollAreaWidgetContents_7->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_7"));
        scrollAreaWidgetContents_7->setGeometry(QRect(0, 0, 408, 535));
        verticalLayout_23 = new QVBoxLayout(scrollAreaWidgetContents_7);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        checkWebUi = new QGroupBox(scrollAreaWidgetContents_7);
        checkWebUi->setObjectName(QString::fromUtf8("checkWebUi"));
        checkWebUi->setCheckable(true);
        checkWebUi->setChecked(false);
        verticalLayout_2 = new QVBoxLayout(checkWebUi);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lblWebUiPort = new QLabel(checkWebUi);
        lblWebUiPort->setObjectName(QString::fromUtf8("lblWebUiPort"));

        horizontalLayout_3->addWidget(lblWebUiPort);

        spinWebUiPort = new QSpinBox(checkWebUi);
        spinWebUiPort->setObjectName(QString::fromUtf8("spinWebUiPort"));
        spinWebUiPort->setMinimum(1);
        spinWebUiPort->setMaximum(65535);
        spinWebUiPort->setValue(8080);

        horizontalLayout_3->addWidget(spinWebUiPort);

        spacerItem4 = new QSpacerItem(21, 29, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(spacerItem4);


        verticalLayout_2->addLayout(horizontalLayout_3);

        checkWebUIUPnP = new QCheckBox(checkWebUi);
        checkWebUIUPnP->setObjectName(QString::fromUtf8("checkWebUIUPnP"));
        checkWebUIUPnP->setChecked(true);

        verticalLayout_2->addWidget(checkWebUIUPnP);

        checkWebUiHttps = new QGroupBox(checkWebUi);
        checkWebUiHttps->setObjectName(QString::fromUtf8("checkWebUiHttps"));
        checkWebUiHttps->setCheckable(true);
        checkWebUiHttps->setChecked(false);
        gridLayout_11 = new QGridLayout(checkWebUiHttps);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        lblSslCertStatus = new QLabel(checkWebUiHttps);
        lblSslCertStatus->setObjectName(QString::fromUtf8("lblSslCertStatus"));
        lblSslCertStatus->setMinimumSize(QSize(22, 22));
        lblSslCertStatus->setMaximumSize(QSize(22, 22));

        gridLayout_11->addWidget(lblSslCertStatus, 0, 0, 1, 1);

        lblWebUiCrt = new QLabel(checkWebUiHttps);
        lblWebUiCrt->setObjectName(QString::fromUtf8("lblWebUiCrt"));
        lblWebUiCrt->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_11->addWidget(lblWebUiCrt, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnWebUiCrt = new QPushButton(checkWebUiHttps);
        btnWebUiCrt->setObjectName(QString::fromUtf8("btnWebUiCrt"));

        horizontalLayout_4->addWidget(btnWebUiCrt);

        horizontalSpacer_12 = new QSpacerItem(138, 28, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_12);


        gridLayout_11->addLayout(horizontalLayout_4, 0, 2, 1, 1);

        lblSslKeyStatus = new QLabel(checkWebUiHttps);
        lblSslKeyStatus->setObjectName(QString::fromUtf8("lblSslKeyStatus"));
        lblSslKeyStatus->setMinimumSize(QSize(22, 22));
        lblSslKeyStatus->setMaximumSize(QSize(22, 22));

        gridLayout_11->addWidget(lblSslKeyStatus, 1, 0, 1, 1);

        lblWebUiKey = new QLabel(checkWebUiHttps);
        lblWebUiKey->setObjectName(QString::fromUtf8("lblWebUiKey"));
        lblWebUiKey->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_11->addWidget(lblWebUiKey, 1, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        btnWebUiKey = new QPushButton(checkWebUiHttps);
        btnWebUiKey->setObjectName(QString::fromUtf8("btnWebUiKey"));

        horizontalLayout_5->addWidget(btnWebUiKey);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_13);


        gridLayout_11->addLayout(horizontalLayout_5, 1, 2, 1, 1);

        lblWebUIInfo = new QLabel(checkWebUiHttps);
        lblWebUIInfo->setObjectName(QString::fromUtf8("lblWebUIInfo"));
        lblWebUIInfo->setOpenExternalLinks(true);

        gridLayout_11->addWidget(lblWebUIInfo, 2, 0, 1, 3);


        verticalLayout_2->addWidget(checkWebUiHttps);

        groupWebUiAuth = new QGroupBox(checkWebUi);
        groupWebUiAuth->setObjectName(QString::fromUtf8("groupWebUiAuth"));
        gridLayout_10 = new QGridLayout(groupWebUiAuth);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        textWebUiUsername = new QLineEdit(groupWebUiAuth);
        textWebUiUsername->setObjectName(QString::fromUtf8("textWebUiUsername"));
        textWebUiUsername->setMaxLength(1000);
        textWebUiUsername->setEchoMode(QLineEdit::Normal);

        gridLayout_10->addWidget(textWebUiUsername, 0, 1, 3, 1);

        textWebUiPassword = new QLineEdit(groupWebUiAuth);
        textWebUiPassword->setObjectName(QString::fromUtf8("textWebUiPassword"));
        textWebUiPassword->setMaxLength(1000);
        textWebUiPassword->setEchoMode(QLineEdit::Password);

        gridLayout_10->addWidget(textWebUiPassword, 3, 1, 1, 1);

        checkBypassLocalAuth = new QCheckBox(groupWebUiAuth);
        checkBypassLocalAuth->setObjectName(QString::fromUtf8("checkBypassLocalAuth"));

        gridLayout_10->addWidget(checkBypassLocalAuth, 4, 0, 1, 2);

        lblWebUiUsername = new QLabel(groupWebUiAuth);
        lblWebUiUsername->setObjectName(QString::fromUtf8("lblWebUiUsername"));

        gridLayout_10->addWidget(lblWebUiUsername, 0, 0, 3, 1);

        lblWebUiPassword = new QLabel(groupWebUiAuth);
        lblWebUiPassword->setObjectName(QString::fromUtf8("lblWebUiPassword"));

        gridLayout_10->addWidget(lblWebUiPassword, 3, 0, 1, 1);


        verticalLayout_2->addWidget(groupWebUiAuth);

        checkDynDNS = new QGroupBox(checkWebUi);
        checkDynDNS->setObjectName(QString::fromUtf8("checkDynDNS"));
        checkDynDNS->setCheckable(true);
        checkDynDNS->setChecked(false);
        formLayout_5 = new QFormLayout(checkDynDNS);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        label_19 = new QLabel(checkDynDNS);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_19);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        comboDNSService = new QComboBox(checkDynDNS);
        comboDNSService->insertItems(0, QStringList()
         << QString::fromUtf8("DynDNS")
         << QString::fromUtf8("No-IP")
        );
        comboDNSService->setObjectName(QString::fromUtf8("comboDNSService"));

        horizontalLayout_11->addWidget(comboDNSService);

        registerDNSBtn = new QPushButton(checkDynDNS);
        registerDNSBtn->setObjectName(QString::fromUtf8("registerDNSBtn"));

        horizontalLayout_11->addWidget(registerDNSBtn);


        formLayout_5->setLayout(0, QFormLayout::FieldRole, horizontalLayout_11);

        label_20 = new QLabel(checkDynDNS);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, label_20);

        domainNameTxt = new QLineEdit(checkDynDNS);
        domainNameTxt->setObjectName(QString::fromUtf8("domainNameTxt"));
        domainNameTxt->setText(QString::fromUtf8("changeme.dyndns.org"));

        formLayout_5->setWidget(1, QFormLayout::FieldRole, domainNameTxt);

        label_21 = new QLabel(checkDynDNS);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        formLayout_5->setWidget(2, QFormLayout::LabelRole, label_21);

        DNSUsernameTxt = new QLineEdit(checkDynDNS);
        DNSUsernameTxt->setObjectName(QString::fromUtf8("DNSUsernameTxt"));
        DNSUsernameTxt->setMaxLength(50);

        formLayout_5->setWidget(2, QFormLayout::FieldRole, DNSUsernameTxt);

        label_22 = new QLabel(checkDynDNS);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        formLayout_5->setWidget(3, QFormLayout::LabelRole, label_22);

        DNSPasswordTxt = new QLineEdit(checkDynDNS);
        DNSPasswordTxt->setObjectName(QString::fromUtf8("DNSPasswordTxt"));
        DNSPasswordTxt->setMaxLength(50);
        DNSPasswordTxt->setEchoMode(QLineEdit::Password);

        formLayout_5->setWidget(3, QFormLayout::FieldRole, DNSPasswordTxt);


        verticalLayout_2->addWidget(checkDynDNS);


        verticalLayout_23->addWidget(checkWebUi);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_23->addItem(verticalSpacer);

        scrollArea_7->setWidget(scrollAreaWidgetContents_7);

        verticalLayout_32->addWidget(scrollArea_7);

        tabOption->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_35 = new QVBoxLayout(page_3);
        verticalLayout_35->setObjectName(QString::fromUtf8("verticalLayout_35"));
        scrollArea_advanced = new QScrollArea(page_3);
        scrollArea_advanced->setObjectName(QString::fromUtf8("scrollArea_advanced"));
        scrollArea_advanced->setWidgetResizable(true);
        scrollAreaWidgetContents_10 = new QWidget();
        scrollAreaWidgetContents_10->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_10"));
        scrollAreaWidgetContents_10->setGeometry(QRect(0, 0, 98, 28));
        verticalLayout_36 = new QVBoxLayout(scrollAreaWidgetContents_10);
        verticalLayout_36->setObjectName(QString::fromUtf8("verticalLayout_36"));
        scrollArea_advanced->setWidget(scrollAreaWidgetContents_10);

        verticalLayout_35->addWidget(scrollArea_advanced);

        tabOption->addWidget(page_3);
        hsplitter->addWidget(tabOption);

        verticalLayout_3->addWidget(hsplitter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonBox = new QDialogButtonBox(Preferences);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout_3->addLayout(horizontalLayout);

        QWidget::setTabOrder(buttonBox, tabSelection);
        QWidget::setTabOrder(tabSelection, scrollArea);
        QWidget::setTabOrder(scrollArea, comboI18n);
        QWidget::setTabOrder(comboI18n, checkAltRowColors);
        QWidget::setTabOrder(checkAltRowColors, actionTorrentDlOnDblClBox);
        QWidget::setTabOrder(actionTorrentDlOnDblClBox, actionTorrentFnOnDblClBox);
        QWidget::setTabOrder(actionTorrentFnOnDblClBox, checkShowSplash);
        QWidget::setTabOrder(checkShowSplash, checkStartMinimized);
        QWidget::setTabOrder(checkStartMinimized, checkProgramExitConfirm);
        QWidget::setTabOrder(checkProgramExitConfirm, checkShowSystray);
        QWidget::setTabOrder(checkShowSystray, checkMinimizeToSysTray);
        QWidget::setTabOrder(checkMinimizeToSysTray, checkCloseToSystray);
        QWidget::setTabOrder(checkCloseToSystray, comboTrayIcon);
        QWidget::setTabOrder(comboTrayIcon, checkAssociateTorrents);
        QWidget::setTabOrder(checkAssociateTorrents, checkAssociateMagnetLinks);
        QWidget::setTabOrder(checkAssociateMagnetLinks, checkAssociateecolls);
        QWidget::setTabOrder(checkAssociateecolls, checkAssociateED2KLinks);
        QWidget::setTabOrder(checkAssociateED2KLinks, checkPreventFromSuspend);
        QWidget::setTabOrder(checkPreventFromSuspend, checkUpdates);
        QWidget::setTabOrder(checkUpdates, scrollArea_2);
        QWidget::setTabOrder(scrollArea_2, checkAdditionDialog);
        QWidget::setTabOrder(checkAdditionDialog, checkStartPaused);
        QWidget::setTabOrder(checkStartPaused, textSavePath);
        QWidget::setTabOrder(textSavePath, browseSaveDirButton);
        QWidget::setTabOrder(browseSaveDirButton, checkAppendLabel);
        QWidget::setTabOrder(checkAppendLabel, checkPreallocateAll);
        QWidget::setTabOrder(checkPreallocateAll, checkTempFolder);
        QWidget::setTabOrder(checkTempFolder, textTempPath);
        QWidget::setTabOrder(textTempPath, browseTempDirButton);
        QWidget::setTabOrder(browseTempDirButton, checkAppendqB);
        QWidget::setTabOrder(checkAppendqB, scanFoldersView);
        QWidget::setTabOrder(scanFoldersView, addScanFolderButton);
        QWidget::setTabOrder(addScanFolderButton, removeScanFolderButton);
        QWidget::setTabOrder(removeScanFolderButton, checkExportDir);
        QWidget::setTabOrder(checkExportDir, textExportDir);
        QWidget::setTabOrder(textExportDir, browseExportDirButton);
        QWidget::setTabOrder(browseExportDirButton, autoRunBox);
        QWidget::setTabOrder(autoRunBox, autoRun_txt);
        QWidget::setTabOrder(autoRun_txt, scrollArea_3);
        QWidget::setTabOrder(scrollArea_3, spinPort);
        QWidget::setTabOrder(spinPort, randomButton);
        QWidget::setTabOrder(randomButton, checkUPnP);
        QWidget::setTabOrder(checkUPnP, checkMaxConnecs);
        QWidget::setTabOrder(checkMaxConnecs, spinMaxConnec);
        QWidget::setTabOrder(spinMaxConnec, checkMaxConnecsPerTorrent);
        QWidget::setTabOrder(checkMaxConnecsPerTorrent, spinMaxConnecPerTorrent);
        QWidget::setTabOrder(spinMaxConnecPerTorrent, checkMaxUploadsPerTorrent);
        QWidget::setTabOrder(checkMaxUploadsPerTorrent, spinMaxUploadsPerTorrent);
        QWidget::setTabOrder(spinMaxUploadsPerTorrent, comboProxyType);
        QWidget::setTabOrder(comboProxyType, textProxyIP);
        QWidget::setTabOrder(textProxyIP, spinProxyPort);
        QWidget::setTabOrder(spinProxyPort, checkProxyPeerConnecs);
        QWidget::setTabOrder(checkProxyPeerConnecs, checkProxyAuth);
        QWidget::setTabOrder(checkProxyAuth, textProxyUsername);
        QWidget::setTabOrder(textProxyUsername, textProxyPassword);
        QWidget::setTabOrder(textProxyPassword, checkIPFilter);
        QWidget::setTabOrder(checkIPFilter, textFilterPath);
        QWidget::setTabOrder(textFilterPath, browseFilterButton);
        QWidget::setTabOrder(browseFilterButton, IpFilterRefreshBtn);
        QWidget::setTabOrder(IpFilterRefreshBtn, scrollArea_9);
        QWidget::setTabOrder(scrollArea_9, ed2k_checkUploadLimit);
        QWidget::setTabOrder(ed2k_checkUploadLimit, ed2k_spinUploadLimit);
        QWidget::setTabOrder(ed2k_spinUploadLimit, ed2k_checkDownloadLimit);
        QWidget::setTabOrder(ed2k_checkDownloadLimit, ed2k_spinDownloadLimit);
        QWidget::setTabOrder(ed2k_spinDownloadLimit, checkUploadLimit);
        QWidget::setTabOrder(checkUploadLimit, spinUploadLimit);
        QWidget::setTabOrder(spinUploadLimit, checkDownloadLimit);
        QWidget::setTabOrder(checkDownloadLimit, spinDownloadLimit);
        QWidget::setTabOrder(spinDownloadLimit, checkuTP);
        QWidget::setTabOrder(checkuTP, checkLimituTPConnections);
        QWidget::setTabOrder(checkLimituTPConnections, checkLimitTransportOverhead);
        QWidget::setTabOrder(checkLimitTransportOverhead, spinUploadLimitAlt);
        QWidget::setTabOrder(spinUploadLimitAlt, spinDownloadLimitAlt);
        QWidget::setTabOrder(spinDownloadLimitAlt, check_schedule);
        QWidget::setTabOrder(check_schedule, schedule_from);
        QWidget::setTabOrder(schedule_from, schedule_to);
        QWidget::setTabOrder(schedule_to, schedule_days);
        QWidget::setTabOrder(schedule_days, scrollArea_5);
        QWidget::setTabOrder(scrollArea_5, editUserName);
        QWidget::setTabOrder(editUserName, emuleSpinPort);
        QWidget::setTabOrder(emuleSpinPort, emuleRandomButton);
        QWidget::setTabOrder(emuleRandomButton, checkEmuleSD);
        QWidget::setTabOrder(checkEmuleSD, scrollArea_4);
        QWidget::setTabOrder(scrollArea_4, checkDHT);
        QWidget::setTabOrder(checkDHT, checkDifferentDHTPort);
        QWidget::setTabOrder(checkDifferentDHTPort, spinDHTPort);
        QWidget::setTabOrder(spinDHTPort, checkPeX);
        QWidget::setTabOrder(checkPeX, checkLSD);
        QWidget::setTabOrder(checkLSD, comboEncryption);
        QWidget::setTabOrder(comboEncryption, checkAnonymousMode);
        QWidget::setTabOrder(checkAnonymousMode, checkEnableQueueing);
        QWidget::setTabOrder(checkEnableQueueing, spinMaxActiveDownloads);
        QWidget::setTabOrder(spinMaxActiveDownloads, spinMaxActiveUploads);
        QWidget::setTabOrder(spinMaxActiveUploads, spinMaxActiveTorrents);
        QWidget::setTabOrder(spinMaxActiveTorrents, checkIgnoreSlowTorrentsForQueueing);
        QWidget::setTabOrder(checkIgnoreSlowTorrentsForQueueing, checkMaxRatio);
        QWidget::setTabOrder(checkMaxRatio, spinMaxRatio);
        QWidget::setTabOrder(spinMaxRatio, comboRatioLimitAct);
        QWidget::setTabOrder(comboRatioLimitAct, scrollArea_advanced);

        retranslateUi(Preferences);
        QObject::connect(checkUploadLimit, SIGNAL(toggled(bool)), spinUploadLimit, SLOT(setEnabled(bool)));
        QObject::connect(checkDownloadLimit, SIGNAL(toggled(bool)), spinDownloadLimit, SLOT(setEnabled(bool)));
        QObject::connect(checkMaxConnecs, SIGNAL(toggled(bool)), spinMaxConnec, SLOT(setEnabled(bool)));
        QObject::connect(checkMaxConnecsPerTorrent, SIGNAL(toggled(bool)), spinMaxConnecPerTorrent, SLOT(setEnabled(bool)));
        QObject::connect(checkMaxUploadsPerTorrent, SIGNAL(toggled(bool)), spinMaxUploadsPerTorrent, SLOT(setEnabled(bool)));
        QObject::connect(checkMaxRatio, SIGNAL(toggled(bool)), spinMaxRatio, SLOT(setEnabled(bool)));
        QObject::connect(checkMaxRatio, SIGNAL(toggled(bool)), comboRatioLimitAct, SLOT(setEnabled(bool)));
        QObject::connect(checkuTP, SIGNAL(toggled(bool)), checkLimituTPConnections, SLOT(setEnabled(bool)));
        QObject::connect(ed2k_checkDownloadLimit, SIGNAL(toggled(bool)), ed2k_spinDownloadLimit, SLOT(setEnabled(bool)));
        QObject::connect(ed2k_checkUploadLimit, SIGNAL(toggled(bool)), ed2k_spinUploadLimit, SLOT(setEnabled(bool)));

        tabSelection->setCurrentRow(-1);
        tabOption->setCurrentIndex(0);
        actionTorrentDlOnDblClBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Preferences);
    } // setupUi

    void retranslateUi(QDialog *Preferences)
    {
        Preferences->setWindowTitle(QApplication::translate("Preferences", "Options", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = tabSelection->isSortingEnabled();
        tabSelection->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = tabSelection->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("Preferences", "Behavior", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ___qlistwidgetitem->setToolTip(QApplication::translate("Preferences", "Behavior", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        QListWidgetItem *___qlistwidgetitem1 = tabSelection->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("Preferences", "Downloads", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = tabSelection->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("Preferences", "Connection", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = tabSelection->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("Preferences", "Speed", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = tabSelection->item(5);
        ___qlistwidgetitem4->setText(QApplication::translate("Preferences", "BitTorrent", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem5 = tabSelection->item(6);
        ___qlistwidgetitem5->setText(QApplication::translate("Preferences", "eMule0.60 Remote", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem6 = tabSelection->item(7);
        ___qlistwidgetitem6->setText(QApplication::translate("Preferences", "Advanced", 0, QApplication::UnicodeUTF8));
        tabSelection->setSortingEnabled(__sortingEnabled);

        UISettingsBox->setTitle(QApplication::translate("Preferences", "Language", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Preferences", "User Interface Language:", 0, QApplication::UnicodeUTF8));
        lbl_i18n_info_2->setText(QApplication::translate("Preferences", "(Requires restart)", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("Preferences", "Transfer List", 0, QApplication::UnicodeUTF8));
        checkAltRowColors->setText(QApplication::translate("Preferences", "Use alternating row colors", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("Preferences", "Action on double-click", 0, QApplication::UnicodeUTF8));
        lblDlList_2->setText(QApplication::translate("Preferences", "Downloading torrents:", 0, QApplication::UnicodeUTF8));
        actionTorrentDlOnDblClBox->clear();
        actionTorrentDlOnDblClBox->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Start / Stop Torrent", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Open destination folder", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "No action", 0, QApplication::UnicodeUTF8)
        );
        lblUploadList->setText(QApplication::translate("Preferences", "Completed torrents:", 0, QApplication::UnicodeUTF8));
        actionTorrentFnOnDblClBox->clear();
        actionTorrentFnOnDblClBox->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Start / Stop Torrent", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Open destination folder", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "No action", 0, QApplication::UnicodeUTF8)
        );
        systrayBox->setTitle(QApplication::translate("Preferences", "Desktop", 0, QApplication::UnicodeUTF8));
        checkShowSplash->setText(QApplication::translate("Preferences", "Show splash screen on start up", 0, QApplication::UnicodeUTF8));
        checkStartMinimized->setText(QApplication::translate("Preferences", "Start eMule0.60 minimized", 0, QApplication::UnicodeUTF8));
        checkProgramExitConfirm->setText(QApplication::translate("Preferences", "Ask for program exit confirmation", 0, QApplication::UnicodeUTF8));
        checkShowSystray->setTitle(QApplication::translate("Preferences", "Show eMule0.60 in notification area", 0, QApplication::UnicodeUTF8));
        checkMinimizeToSysTray->setText(QApplication::translate("Preferences", "Minimize eMule0.60 to notification area", 0, QApplication::UnicodeUTF8));
        checkCloseToSystray->setText(QApplication::translate("Preferences", "Close eMule0.60 to notification area", "i.e: The systray tray icon will still be visible when closing the main window.", QApplication::UnicodeUTF8));
        label_trayIconStyle->setText(QApplication::translate("Preferences", "Tray icon style:", 0, QApplication::UnicodeUTF8));
        comboTrayIcon->clear();
        comboTrayIcon->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Normal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Monochrome (Dark theme)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Monochrome (Light theme)", 0, QApplication::UnicodeUTF8)
        );
        groupFileAssociation->setTitle(QApplication::translate("Preferences", "File association", 0, QApplication::UnicodeUTF8));
        checkAssociateTorrents->setText(QApplication::translate("Preferences", "Use eMule0.60 for .torrent files", 0, QApplication::UnicodeUTF8));
        checkAssociateMagnetLinks->setText(QApplication::translate("Preferences", "Use eMule0.60 for magnet links", 0, QApplication::UnicodeUTF8));
        checkAssociateecolls->setText(QApplication::translate("Preferences", "Use eMule0.60 for cmule collections", 0, QApplication::UnicodeUTF8));
        checkAssociateED2KLinks->setText(QApplication::translate("Preferences", "Use eMule0.60 for ed2k links", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Preferences", "Power Management", 0, QApplication::UnicodeUTF8));
        checkPreventFromSuspend->setText(QApplication::translate("Preferences", "Inhibit system sleep when torrents are active", 0, QApplication::UnicodeUTF8));
        checkUpdates->setText(QApplication::translate("Preferences", "Check for updates on start", 0, QApplication::UnicodeUTF8));
        torrentAdditionBox->setTitle(QApplication::translate("Preferences", "When adding a torrent", 0, QApplication::UnicodeUTF8));
        checkAdditionDialog->setText(QApplication::translate("Preferences", "Display torrent content and some options", 0, QApplication::UnicodeUTF8));
        checkStartPaused->setText(QApplication::translate("Preferences", "Do not start the download automatically", "The torrent will be added to download list in pause state", QApplication::UnicodeUTF8));
        torrentAdditionBox_2->setTitle(QApplication::translate("Preferences", "Hard Disk mule", 0, QApplication::UnicodeUTF8));
        groupBox_13->setTitle(QApplication::translate("Preferences", "Save files to location:", 0, QApplication::UnicodeUTF8));
        textSavePathMule->setText(QString());
        groupBox_14->setTitle(QApplication::translate("Preferences", "Keep incomplete files in", 0, QApplication::UnicodeUTF8));
        textTempPathMule->setText(QString());
        fileSystemBox->setTitle(QApplication::translate("Preferences", "Hard Disk torrents", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Preferences", "Save files to location:", 0, QApplication::UnicodeUTF8));
        textSavePath->setText(QString());
        checkAppendLabel->setText(QApplication::translate("Preferences", "Append the label of the torrent to the save path", 0, QApplication::UnicodeUTF8));
        checkPreallocateAll->setText(QApplication::translate("Preferences", "Pre-allocate disk space for all files", 0, QApplication::UnicodeUTF8));
        checkTempFolder->setTitle(QApplication::translate("Preferences", "Keep incomplete torrents in:", 0, QApplication::UnicodeUTF8));
        checkAppendqB->setText(QApplication::translate("Preferences", "Append .!qB extension to incomplete files", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Preferences", "Automatically add torrents from:", 0, QApplication::UnicodeUTF8));
        addScanFolderButton->setText(QApplication::translate("Preferences", "Add folder...", 0, QApplication::UnicodeUTF8));
        removeScanFolderButton->setText(QApplication::translate("Preferences", "Remove folder", 0, QApplication::UnicodeUTF8));
        checkExportDir->setTitle(QApplication::translate("Preferences", "Copy .torrent files to:", 0, QApplication::UnicodeUTF8));
        autoRunBox->setTitle(QApplication::translate("Preferences", "Run an external program on torrent completion", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Preferences", "The following parameters are supported:\n"
"<ul>\n"
"<li>%f: Torrent path</li>\n"
"<li>%n: Torrent name</li>\n"
"</ul>", 0, QApplication::UnicodeUTF8));
        ListeningPortBox->setTitle(QApplication::translate("Preferences", "Listening Port", 0, QApplication::UnicodeUTF8));
        lbl_ports->setText(QApplication::translate("Preferences", "Port used for incoming connections:", 0, QApplication::UnicodeUTF8));
        randomButton->setText(QApplication::translate("Preferences", "Random", 0, QApplication::UnicodeUTF8));
        checkUPnP->setText(QApplication::translate("Preferences", "Use UPnP / NAT-PMP port forwarding from my router", 0, QApplication::UnicodeUTF8));
        nbConnecBox->setTitle(QApplication::translate("Preferences", "Connections Limits", 0, QApplication::UnicodeUTF8));
        checkMaxConnecs->setText(QApplication::translate("Preferences", "Global maximum number of connections:", 0, QApplication::UnicodeUTF8));
        checkMaxConnecsPerTorrent->setText(QApplication::translate("Preferences", "Maximum number of connections per torrent:", 0, QApplication::UnicodeUTF8));
        checkMaxUploadsPerTorrent->setText(QApplication::translate("Preferences", "Maximum number of upload slots per torrent:", 0, QApplication::UnicodeUTF8));
        groupProxy->setTitle(QApplication::translate("Preferences", "Proxy Server", 0, QApplication::UnicodeUTF8));
        ProxyType_lbl->setText(QApplication::translate("Preferences", "Type:", 0, QApplication::UnicodeUTF8));
        comboProxyType->clear();
        comboProxyType->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "(None)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "SOCKS4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "SOCKS5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "HTTP", 0, QApplication::UnicodeUTF8)
        );
        lblProxyIP->setText(QApplication::translate("Preferences", "Host:", 0, QApplication::UnicodeUTF8));
        textProxyIP->setText(QString());
        lblProxyPort->setText(QApplication::translate("Preferences", "Port:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkProxyPeerConnecs->setToolTip(QApplication::translate("Preferences", "Otherwise, the proxy server is only used for tracker connections", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkProxyPeerConnecs->setText(QApplication::translate("Preferences", "Use proxy for peer connections", 0, QApplication::UnicodeUTF8));
        checkProxyAuth->setTitle(QApplication::translate("Preferences", "Authentication", 0, QApplication::UnicodeUTF8));
        lblProxyUsername->setText(QApplication::translate("Preferences", "Username:", 0, QApplication::UnicodeUTF8));
        textProxyUsername->setText(QString());
        lblProxyPassword->setText(QApplication::translate("Preferences", "Password:", 0, QApplication::UnicodeUTF8));
        textProxyPassword->setText(QString());
        checkIPFilter->setTitle(QApplication::translate("Preferences", "IP Filtering", 0, QApplication::UnicodeUTF8));
        lblFilterPath->setText(QApplication::translate("Preferences", "Filter path (.dat, .p2p, .p2b):", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        IpFilterRefreshBtn->setToolTip(QApplication::translate("Preferences", "Reload the filter", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        IpFilterRefreshBtn->setText(QString());
        groupBox_8->setTitle(QApplication::translate("Preferences", "Mule Rate Limits", 0, QApplication::UnicodeUTF8));
        ed2k_checkUploadLimit->setText(QApplication::translate("Preferences", "Upload:", 0, QApplication::UnicodeUTF8));
        ed2k_spinUploadLimit->setSuffix(QString());
        label_23->setText(QApplication::translate("Preferences", "KiB/s", 0, QApplication::UnicodeUTF8));
        ed2k_checkDownloadLimit->setText(QApplication::translate("Preferences", "Download:", 0, QApplication::UnicodeUTF8));
        ed2k_spinDownloadLimit->setSuffix(QString());
        label_24->setText(QApplication::translate("Preferences", "KiB/s", 0, QApplication::UnicodeUTF8));
        bandwidthBox->setTitle(QApplication::translate("Preferences", "Torrent Rate Limits", 0, QApplication::UnicodeUTF8));
        checkUploadLimit->setText(QApplication::translate("Preferences", "Upload:", 0, QApplication::UnicodeUTF8));
        spinUploadLimit->setSuffix(QString());
        label_10->setText(QApplication::translate("Preferences", "KiB/s", 0, QApplication::UnicodeUTF8));
        checkDownloadLimit->setText(QApplication::translate("Preferences", "Download:", 0, QApplication::UnicodeUTF8));
        spinDownloadLimit->setSuffix(QString());
        label_13->setText(QApplication::translate("Preferences", "KiB/s", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("Preferences", "Options", 0, QApplication::UnicodeUTF8));
        checkuTP->setText(QApplication::translate("Preferences", "Enable bandwidth management (uTP)", 0, QApplication::UnicodeUTF8));
        checkLimituTPConnections->setText(QApplication::translate("Preferences", "Apply rate limit to uTP connections", 0, QApplication::UnicodeUTF8));
        checkLimitTransportOverhead->setText(QApplication::translate("Preferences", "Apply rate limit to transport overhead", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("Preferences", "Alternative Torrent Rate Limits", 0, QApplication::UnicodeUTF8));
        label_16->setText(QString());
        label_11->setText(QApplication::translate("Preferences", "Upload:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Preferences", "KiB/s", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Preferences", "Download:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Preferences", "KiB/s", 0, QApplication::UnicodeUTF8));
        check_schedule->setTitle(QApplication::translate("Preferences", "Schedule the use of alternative rate limits", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Preferences", "from", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Preferences", "to", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Preferences", "When:", 0, QApplication::UnicodeUTF8));
        schedule_days->clear();
        schedule_days->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Every day", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Week days", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Week ends", 0, QApplication::UnicodeUTF8)
        );
        Username->setTitle(QApplication::translate("Preferences", "User name", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("Preferences", "Connection settings", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("Preferences", "Port used for incoming connections:", 0, QApplication::UnicodeUTF8));
        emuleRandomButton->setText(QApplication::translate("Preferences", "Random", 0, QApplication::UnicodeUTF8));
        checkEmuleSD->setText(QApplication::translate("Preferences", "Other users can see my shared catalogs and files", 0, QApplication::UnicodeUTF8));
        checkRandomServers->setText(QApplication::translate("Preferences", "Connect to random servers on start", 0, QApplication::UnicodeUTF8));
        checkSaveSearchKeywords->setText(QApplication::translate("Preferences", "Save search words history on disk when program exit", 0, QApplication::UnicodeUTF8));
        AddBTFeaturesBox->setTitle(QApplication::translate("Preferences", "Privacy", 0, QApplication::UnicodeUTF8));
        checkDHT->setTitle(QApplication::translate("Preferences", "Enable DHT (decentralized network) to find more peers", 0, QApplication::UnicodeUTF8));
        checkDifferentDHTPort->setTitle(QApplication::translate("Preferences", "Use a different port for DHT and BitTorrent", 0, QApplication::UnicodeUTF8));
        dh_port_lbl->setText(QApplication::translate("Preferences", "DHT port:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkPeX->setToolTip(QApplication::translate("Preferences", "Exchange peers with compatible Bittorrent clients (\302\265Torrent, Vuze, ...)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkPeX->setText(QApplication::translate("Preferences", "Enable Peer Exchange (PeX) to find more peers", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkLSD->setToolTip(QApplication::translate("Preferences", "Look for peers on your local network", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkLSD->setText(QApplication::translate("Preferences", "Enable Local Peer Discovery to find more peers", 0, QApplication::UnicodeUTF8));
        lbl_encryption->setText(QApplication::translate("Preferences", "Encryption mode:", 0, QApplication::UnicodeUTF8));
        comboEncryption->clear();
        comboEncryption->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Prefer encryption", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Require encryption", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Disable encryption", 0, QApplication::UnicodeUTF8)
        );
        checkAnonymousMode->setText(QApplication::translate("Preferences", "Enable anonymous mode", 0, QApplication::UnicodeUTF8));
        checkEnableQueueing->setTitle(QApplication::translate("Preferences", "Torrent Queueing", 0, QApplication::UnicodeUTF8));
        label_max_active_dl->setText(QApplication::translate("Preferences", "Maximum active downloads:", 0, QApplication::UnicodeUTF8));
        label_max_active_up->setText(QApplication::translate("Preferences", "Maximum active uploads:", 0, QApplication::UnicodeUTF8));
        maxActiveTorrents_lbl->setText(QApplication::translate("Preferences", "Maximum active torrents:", 0, QApplication::UnicodeUTF8));
        checkIgnoreSlowTorrentsForQueueing->setText(QApplication::translate("Preferences", "Do not count slow torrents in these limits", 0, QApplication::UnicodeUTF8));
        ratioBox->setTitle(QApplication::translate("Preferences", "Share Ratio Limiting", 0, QApplication::UnicodeUTF8));
        checkMaxRatio->setText(QApplication::translate("Preferences", "Seed torrents until their ratio reaches", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Preferences", "then", 0, QApplication::UnicodeUTF8));
        comboRatioLimitAct->clear();
        comboRatioLimitAct->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Pause them", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Remove them", 0, QApplication::UnicodeUTF8)
        );
        checkWebUi->setTitle(QApplication::translate("Preferences", "Enable Web User Interface (Remote control)", 0, QApplication::UnicodeUTF8));
        lblWebUiPort->setText(QApplication::translate("Preferences", "Port:", 0, QApplication::UnicodeUTF8));
        checkWebUIUPnP->setText(QApplication::translate("Preferences", "Use UPnP / NAT-PMP to forward the port from my router", 0, QApplication::UnicodeUTF8));
        checkWebUiHttps->setTitle(QApplication::translate("Preferences", "Use HTTPS instead of HTTP", 0, QApplication::UnicodeUTF8));
        lblSslCertStatus->setText(QString());
        lblWebUiCrt->setText(QApplication::translate("Preferences", "Certificate:", 0, QApplication::UnicodeUTF8));
        btnWebUiCrt->setText(QApplication::translate("Preferences", "Import SSL Certificate", 0, QApplication::UnicodeUTF8));
        lblSslKeyStatus->setText(QString());
        lblWebUiKey->setText(QApplication::translate("Preferences", "Key:", 0, QApplication::UnicodeUTF8));
        btnWebUiKey->setText(QApplication::translate("Preferences", "Import SSL Key", 0, QApplication::UnicodeUTF8));
        lblWebUIInfo->setText(QApplication::translate("Preferences", "<a href=http://httpd.apache.org/docs/2.2/ssl/ssl_faq.html#aboutcerts>Information about certificates</a>", 0, QApplication::UnicodeUTF8));
        groupWebUiAuth->setTitle(QApplication::translate("Preferences", "Authentication", 0, QApplication::UnicodeUTF8));
        textWebUiUsername->setText(QString());
        textWebUiPassword->setText(QString());
        checkBypassLocalAuth->setText(QApplication::translate("Preferences", "Bypass authentication for localhost", 0, QApplication::UnicodeUTF8));
        lblWebUiUsername->setText(QApplication::translate("Preferences", "Username:", 0, QApplication::UnicodeUTF8));
        lblWebUiPassword->setText(QApplication::translate("Preferences", "Password:", 0, QApplication::UnicodeUTF8));
        checkDynDNS->setTitle(QApplication::translate("Preferences", "Update my dynamic domain name", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Preferences", "Service:", 0, QApplication::UnicodeUTF8));
        registerDNSBtn->setText(QApplication::translate("Preferences", "Register", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("Preferences", "Domain name:", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Preferences", "Username:", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("Preferences", "Password:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Preferences: public Ui_Preferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONS_H
