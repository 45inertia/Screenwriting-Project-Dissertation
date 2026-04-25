#include "welcomescreen.h"



WelcomeScreen::WelcomeScreen(QWidget *parent)
    : QWidget(parent),
    titleLabel_(nullptr),
    subtitleLabel_(nullptr){
    setAutoFillBackground(true);
    setupUi();
}

void WelcomeScreen::setupUi() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(16);

    titleLabel_ = new QLabel("SSA", this);
    titleLabel_->setAlignment(Qt::AlignCenter);

    subtitleLabel_ = new QLabel("Screenwriting Software Application", this);
    subtitleLabel_->setAlignment(Qt::AlignCenter);

    QPushButton* newButton = new QPushButton("New Script", this);
    newButton->setFixedWidth(200);
    newButton->setStyleSheet(
        "QPushButton {"
        "  font-family: 'Courier New';"
        "  font-size: 12px;"
        "  padding: 10px;"
        "  background-color: #0078d4;"
        "  color: #ffffff;"
        "  border: none;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #106ebe;"
        "}"
        );

    QPushButton* openButton = new QPushButton("Open Script...", this);
    openButton->setFixedWidth(200);
    openButton->setStyleSheet(
        "QPushButton {"
        "  font-family: 'Courier New';"
        "  font-size: 12px;"
        "  padding: 10px;"
        "  background-color: transparent;"
        "  color: #0078d4;"
        "  border: 1px solid #0078d4;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #e6f2fb;"
        "}"
        );

    layout->addWidget(titleLabel_);
    layout->addWidget(subtitleLabel_);
    layout->addSpacing(16);
    layout->addWidget(newButton, 0, Qt::AlignCenter);
    layout->addWidget(openButton, 0, Qt::AlignCenter);

    connect(newButton, &QPushButton::clicked,
            this, &WelcomeScreen::newScriptRequested);
    connect(openButton, &QPushButton::clicked,
            this, &WelcomeScreen::openScriptRequested);

    // apply light theme as default so buttons can get styled before system theme is looked at
    //setLightTheme();
}


void WelcomeScreen::setDarkTheme() {
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(30, 30, 30));
    setPalette(pal);

    if (!titleLabel_) return;
    titleLabel_->setStyleSheet(
        "font-family: 'Courier New'; font-size: 48px;"
        "font-weight: bold; color: #d4d4d4;");
    subtitleLabel_->setStyleSheet(
        "font-family: 'Courier New'; font-size: 14px; color: #888888;");
}

void WelcomeScreen::setLightTheme() {
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 245, 245));
    setPalette(pal);

    if(!titleLabel_) return;
    titleLabel_->setStyleSheet(
        "font-family: 'Courier New'; font-size: 48px;"
        "font-weight: bold; color: #555555;");
    subtitleLabel_->setStyleSheet(
        "font-family: 'Courier New'; font-size: 14px; color: #888888;");
}
