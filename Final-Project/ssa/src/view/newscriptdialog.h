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
    QString getFilePath() const;

private slots:
    void onTitleChanged(const QString& title);
    void onAccepted();

private:
    QLineEdit* titleEdit_;
    QLineEdit* authorEdit_;
    QLineEdit* filenameEdit_;
    QString filePath_;

    void setupUi();
};

#endif // NEWSCRIPTDIALOG_H
