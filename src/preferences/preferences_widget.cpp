#include "preferences_widget.h"
#include "ui_preferences_widget.h"
#include "preferences.h"
#include "qtlibed2k/qed2ksession.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QLocale>
#include <QApplication>
#include <QTranslator>

QString languageToLocalizedString(QLocale::Language language, const QString& country)
{
    switch(language)
    {
        case QLocale::English: return "English";
        case QLocale::French: return QString::fromUtf8("Français");
        case QLocale::German: return QString::fromUtf8("Deutsch");
        case QLocale::Hungarian: return QString::fromUtf8("Magyar");
        case QLocale::Italian: return QString::fromUtf8("Italiano");
        case QLocale::Dutch: return QString::fromUtf8("Nederlands");
        case QLocale::Spanish: return QString::fromUtf8("Español");
        case QLocale::Catalan: return QString::fromUtf8("Catal ");
        case QLocale::Galician: return QString::fromUtf8("Galego");
        case QLocale::Portuguese: {
            if (country == "br")  return QString::fromUtf8("Portugu�ês brasileiro");
            return QString::fromUtf8("Português");
        }
        case QLocale::Polish: return QString::fromUtf8("Polski");
        case QLocale::Lithuanian: return QString::fromUtf8("Lietuvių");
        case QLocale::Czech: return QString::fromUtf8("Čeština");
        case QLocale::Slovak: return QString::fromUtf8("Slovenčina");
        case QLocale::Serbian: return QString::fromUtf8("Српски");
        case QLocale::Croatian: return QString::fromUtf8("Hrvatski");
        case QLocale::Armenian: return QString::fromUtf8("Հայերեն");
        case QLocale::Romanian: return QString::fromUtf8("Română");
        case QLocale::Turkish: return QString::fromUtf8("Türkçe");
        case QLocale::Greek: return QString::fromUtf8("Ελληνικά");
        case QLocale::Swedish: return QString::fromUtf8("Svenska");
        case QLocale::Finnish: return QString::fromUtf8("Suomi");
        case QLocale::Norwegian: return QString::fromUtf8("Norsk");
        case QLocale::Danish: return QString::fromUtf8("Dansk");
        case QLocale::Bulgarian: return QString::fromUtf8("Български");
        case QLocale::Ukrainian: return QString::fromUtf8("Українська");
        case QLocale::Russian: return QString::fromUtf8("Русский");
        case QLocale::Japanese: return QString::fromUtf8("日本語");
        case QLocale::Arabic: return QString::fromUtf8("عربي");
        case QLocale::Georgian: return QString::fromUtf8("ქა �თული");
        case QLocale::Byelorussian: return QString::fromUtf8("Беларуская");
        case QLocale::Basque: return QString::fromUtf8("Euskara");
        case QLocale::Chinese: {
        if (country == "cn")
            return QString::fromUtf8("中文 (简体)");
            return QString::fromUtf8("中文 (繁體)");
        }
        case QLocale::Korean: return QString::fromUtf8("한글");
        default: {
            // Fallback to English
            const QString eng_lang = QLocale::languageToString(language);
            qWarning() << "Unrecognized language name: " << eng_lang;
            return eng_lang;
        }
    }
}

preferences_widget::preferences_widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
#ifndef Q_OS_WIN
    file_associations->setVisible(false);
#endif
    // List language files
    const QDir lang_dir(":/lang");
    const QStringList lang_files = lang_dir.entryList(QStringList() << QString(misc::productName() + QString::fromUtf8("_*.qm")), QDir::Files);
    foreach (QString lang_file, lang_files)
    {
        QString localeStr = lang_file.mid(misc::productName().length() + 1); // remove productName
        localeStr.chop(3); // Remove ".qm"
        QLocale locale(localeStr);
        const QString country = locale.name().split("_").last().toLower();
        QString language_name = languageToLocalizedString(locale.language(), country);
        comboLang->addItem(/*QIcon(":/Icons/flags/"+country+".png"), */language_name, locale.name());
        qDebug() << "Supported locale:" << locale.name();
    }

    restoreFromPref();
    connect(checkConfirm, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
    connect(checkDisplaySpeed, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
    connect(checkPausedCT, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
    connect(editNick, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButtons()));
    connect(listenPort, SIGNAL(valueChanged(int)), this, SLOT(enableApplyButtons()));
    connect(checkUPnP, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
    connect(editInputDir, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButtons()));
    connect(comboLang, SIGNAL(currentIndexChanged(int)), this, SLOT(enableApplyButtons()));
    connect(checkAssocCollections, SIGNAL(clicked()), this, SLOT(enableApplyButtons()));
    connect(checkAssocLinks, SIGNAL(clicked()), this, SLOT(enableApplyButtons()));
}

preferences_widget::~preferences_widget()
{
}


void preferences_widget::setLocale(const QString& localeStr)
{
    QLocale locale(localeStr);
    // Attempt to find exact match
    int index = comboLang->findData(locale.name(), Qt::UserRole);
    if (index < 0)
    {
        // Unreconized, use US English
        index = comboLang->findData(QLocale("en").name(), Qt::UserRole);
        Q_ASSERT(index >= 0);
    }

    comboLang->setCurrentIndex(index);
}

void preferences_widget::enableApplyButtons() {
    btnOk->setEnabled(true);
    btnCancel->setEnabled(true);
}

void preferences_widget::disableApplyButtons() {
    btnOk->setEnabled(false);
    btnCancel->setEnabled(false);
}

void preferences_widget::restoreFromPref() {
    Preferences pref;

    checkConfirm->setChecked(pref.confirmOnExit());
    checkDisplaySpeed->setChecked(pref.displaySpeedInTitle());
    checkPausedCT->setChecked(pref.pausedTransfersFromCollection());
    serverHost->setText(pref.serverHost());
    serverPort->setValue(pref.serverPort());

    editNick->setText(pref.nick());
    listenPort->setValue(pref.listenPort());
    checkUPnP->setChecked(pref.forwardPort());
    editInputDir->setText(pref.inputDir());
    setLocale(pref.getLocale());
#ifdef Q_OS_WIN
    checkAssocCollections->setChecked(Preferences::isLinkAssocSet("ed2k"));
    checkAssocLinks->setChecked(Preferences::isFileAssocSet(".emulecollection"));
#endif
}

void preferences_widget::on_btnOk_clicked() {

    if (!misc::prepareInputDirectory(editInputDir->text())) {
        QMessageBox::warning(this, tr("Input directory configuration"), tr("Unable to write metadata into directiry, check it exists and writeable"));
    } else {
        Preferences pref;
        QString strLocale = comboLang->itemData(comboLang->currentIndex(), Qt::UserRole).toString();

        // Load the translation
        if (pref.getLocale() != strLocale)
        {
            QTranslator *translator = new QTranslator;
            if (translator->load(QString::fromUtf8(":/lang/%1_%2").arg(misc::productName()).arg(strLocale)))
            {
                qDebug("%s locale recognized, using translation.", qPrintable(strLocale));
                pref.setLocale(strLocale);
            }
            else
            {
                qDebug("%s locale unrecognized, using default (en_GB).", qPrintable(strLocale));
            }

            qApp->installTranslator(translator);
        }

        pref.setConfirmOnExit(checkConfirm->isChecked());
        pref.setDisplaySpeedInTitle(checkDisplaySpeed->isChecked());
        pref.setPausedTransfersFromCollection(checkPausedCT->isChecked());
        pref.setNick(editNick->text());
        pref.setListenPort(listenPort->value());
        pref.setForwardPort(checkUPnP->isChecked());
        pref.setInputDir(editInputDir->text());
#ifdef Q_OS_WIN
        // Windows: file association settings
        Preferences::setFileAssoc(".emulecollection", checkAssocCollections->isChecked());
        Preferences::setLinkAssoc("ed2k", checkAssocLinks->isChecked());
        Preferences::setCommonAssocSection(checkAssocCollections->isChecked() || checkAssocLinks);
#endif
        disableApplyButtons();
        Session::instance()->loadDirectory(pref.inputDir());
    }
}

void preferences_widget::on_btnCancel_clicked() {
    restoreFromPref();
    disableApplyButtons();
}

void preferences_widget::on_browseSaveDirButton_clicked()
{
    const QString path = misc::expandPath(editInputDir->text());
    QDir inputDir(path);
    QString dir;
    if (!path.isEmpty() && inputDir.exists()) {
        dir = QFileDialog::getExistingDirectory(this, tr("Choose input directory"), inputDir.absolutePath());
    } else {
        dir = QFileDialog::getExistingDirectory(this, tr("Choose input directory"), QDir::homePath());
    }

    if (!dir.isNull()) {
        misc::normalizePath(dir);
        editInputDir->setText(dir);
    }
}
