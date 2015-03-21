#include "preferences_widget.h"
#include "ui_preferences_widget.h"
#include "preferences.h"
#include "qtlibed2k/qed2ksession.h"
#include <QMessageBox>
#include <QFileDialog>

preferences_widget::preferences_widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    restoreFromPref();

    connect(checkConfirm, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
    connect(checkDisplaySpeed, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
    connect(editNick, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButtons()));
    connect(listenPort, SIGNAL(valueChanged(int)), this, SLOT(enableApplyButtons()));
    connect(checkUPnP, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
    connect(editInputDir, SIGNAL(textChanged(QString)), this, SLOT(enableApplyButtons()));
}

preferences_widget::~preferences_widget()
{
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
    serverHost->setText(pref.serverHost());
    serverPort->setValue(pref.serverPort());

    editNick->setText(pref.nick());
    listenPort->setValue(pref.listenPort());
    checkUPnP->setChecked(pref.forwardPort());

    editInputDir->setText(pref.inputDir());


}

void preferences_widget::on_btnOk_clicked() {

    if (!misc::prepareInputDirectory(editInputDir->text())) {
        QMessageBox::warning(this, tr("Input directory configuration"), tr("Unable to write metadata into directiry, check it exists and writeable"));
    } else {
        Preferences pref;
        pref.setConfirmOnExit(checkConfirm->isChecked());
        pref.setDisplaySpeedInTitle(checkDisplaySpeed->isChecked());
        pref.setNick(editNick->text());
        pref.setListenPort(listenPort->value());
        pref.setForwardPort(checkUPnP->isChecked());
        pref.setInputDir(editInputDir->text());
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
