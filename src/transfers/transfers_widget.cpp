#include "transfers_widget.h"
#include "ui_transfers_widget.h"
#include "transfer_model.h"
#include "transferlist_delegate.h"

transfers_widget::transfers_widget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    model = new TransferModel(this);
    model->populate();
    trView->setItemDelegate(new TransferListDelegate(this));
    trView->setModel(model);
}

transfers_widget::~transfers_widget()
{
}
