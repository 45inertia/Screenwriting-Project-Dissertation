#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class WelcomeScreen : public QWidget {

    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget* parent = nullptr);

    void setDarkTheme();
    void setLightTheme();

signals:
    void newScriptRequested();
    void openScriptRequested();

private:
    QLabel* titleLabel_;
    QLabel* subtitleLabel_;

    void setupUi();
};

#endif // WELCOMESCREEN_H
