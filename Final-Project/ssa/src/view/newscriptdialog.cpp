#include "newscriptdialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QPushButton>
#include <QRegularExpression>

NewScriptDialog::NewScriptDialog(QWidget *parent)
    : QDialog(parent),
    titleEdit_(nullptr),
    authorEdit_(nullptr)
{
    setWindowTitle("New Script");
    setMinimumWidth(400);
    setupUi();
}

void NewScriptDialog::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout();

    titleEdit_ = new QLineEdit(this);
    titleEdit_->setPlaceholderText("Enter script title...");

    authorEdit_ = new QLineEdit(this);
    authorEdit_->setPlaceholderText("Enter author name...");

    formLayout->addRow("Title:", titleEdit_);
    formLayout->addRow("Author:", authorEdit_);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        this
    );

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

}

// ----getters and setters-------------------------------------------------------------------------


QString NewScriptDialog::getTitle() const {
    return titleEdit_->text();
}

QString NewScriptDialog::getAuthor() const {
    return authorEdit_->text();
}

