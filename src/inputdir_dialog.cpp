#include "inputdir_dialog.h"
#include "ui_inputdir_dialog.h"

#include "misc.h"
#include "preferences.h"
#include <QFileDialog>

inputdir_dialog::inputdir_dialog(QWidget *parent) :
    QDialog(parent) {
    setupUi(this);
    labelMsg->hide();
}

inputdir_dialog::~inputdir_dialog() {
}

void inputdir_dialog::on_toolButton_clicked() {
    const QString path = misc::expandPath(testInputDir->text());
    QDir inputDir(path);
    QString dir;
    if (!path.isEmpty() && inputDir.exists()) {
        dir = QFileDialog::getExistingDirectory(this, tr("Choose input directory"), inputDir.absolutePath());
    } else {
        dir = QFileDialog::getExistingDirectory(this, tr("Choose input directory"), QDir::homePath());
    }

    if (!dir.isNull()) {
        misc::normalizePath(dir);
        testInputDir->setText(dir);
    }
}

void inputdir_dialog::done(int r) {
    if (r == QDialog::Accepted) {
        if (!misc::prepareInputDirectory(testInputDir->text())) {
            labelMsg->setText(tr("Unable to prepare input directory, check it exists and writeable"));
            labelMsg->show();
        } else  {
            Preferences().setInputDir(testInputDir->text());
            QDialog::done(r);
        }
    } else {
        QDialog::done(r);
    }
}

void inputdir_dialog::on_testInputDir_textChanged(const QString &arg1) {
    labelMsg->hide();
}
