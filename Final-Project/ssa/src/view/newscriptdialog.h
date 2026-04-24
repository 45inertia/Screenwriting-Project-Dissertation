#ifndef NEWSCRIPTDIALOG_H
#define NEWSCRIPTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>

class NewScriptDialog : public QDialog {

Q_OBJECT

public:
    explicit NewScriptDialog(QWidget* parent = nullptr);

    QString getTitle() const;
    QString getAuthor() const;

private:
    QLineEdit* titleEdit_;
    QLineEdit* authorEdit_;

    void setupUi();
};

#endif // NEWSCRIPTDIALOG_H
