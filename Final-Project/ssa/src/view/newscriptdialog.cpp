#include "newscriptdialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QPushButton>
#include <QRegularExpression>




NewScriptDialog::NewScriptDialog(QWidget *parent)
    : QDialog(parent),
    titleEdit_(nullptr),
    authorEdit_(nullptr),
    filenameEdit_(nullptr)
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

    filenameEdit_ = new QLineEdit(this);
    filenameEdit_->setPlaceholderText("filename.osf");

    formLayout->addRow("Title:", titleEdit_);
    formLayout->addRow("Author:", authorEdit_);
    formLayout->addRow("Filename:", filenameEdit_);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        this
    );

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttons);

    // auto populating filename from title as user types
    connect(titleEdit_, &QLineEdit::textChanged,
            this, &NewScriptDialog::onTitleChanged);

    connect(buttons, &QDialogButtonBox::accepted,
            this, &NewScriptDialog::onAccepted);

    connect(buttons, &QDialogButtonBox::accepted,
            this, &QDialog::reject);

}

void NewScriptDialog::onTitleChanged(const QString &title) {

    // converting the title to a safe filename
    QString filename = title.toLower();
    filename.replace(" ", "-");
    filename.remove(QRegularExpression("[^a-z0-9_]"));
    filenameEdit_->setText(filename + ".osf");
}

void NewScriptDialog::onAccepted() {
    // open the file dialog to choose save location
    QString defaultPath = QStandardPaths::writableLocation(
        QStandardPaths::DocumentsLocation);

    QString path = QFileDialog::getSaveFileName(
        this,
        "Save New Script",
        defaultPath,
        "Open Screenplay Format (*.osf)"
    );

    if(path.isEmpty()) {
        // user cancelled the file dialog
        return;
    }

    filePath_ = path;
    accept();
}

// ----getters and setters-------------------------------------------------------------------------


QString NewScriptDialog::getTitle() const {
    return titleEdit_->text();
}

QString NewScriptDialog::getAuthor() const {
    return authorEdit_->text();
}

QString NewScriptDialog::getFilePath() const {
    return filePath_;
}
