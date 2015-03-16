#include "preferences_widget.h"
#include "ui_preferences_widget.h"
#include "preferences.h"

preferences_widget::preferences_widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    restoreFromPref();

    connect(checkConfirm, SIGNAL(clicked(bool)), this, SLOT(enableApplyButtons()));
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
    serverHost->setText(pref.serverHost());
    serverPort->setValue(pref.serverPort());

    editNick->setText(pref.nick());
    listenPort->setValue(pref.listenPort());
    checkUPnP->setChecked(pref.forwardPort());

    editInputDir->setText(pref.inputDir());


}

void preferences_widget::on_btnOk_clicked() {
    Preferences pref;
    pref.setConfirmOnExit(checkConfirm->isChecked());
    pref.setNick(editNick->text());
    pref.setListenPort(listenPort->value());
    pref.setForwardPort(checkUPnP->isChecked());
    pref.setInputDir(editInputDir->text());
    disableApplyButtons();
}

void preferences_widget::on_btnCancel_clicked() {
    restoreFromPref();
    disableApplyButtons();
}
