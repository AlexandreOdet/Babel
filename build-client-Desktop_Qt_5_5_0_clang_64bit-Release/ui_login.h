/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *nickname;
    QLabel *label_2;
    QLineEdit *password;
    QSplitter *splitter;
    QPushButton *pushButton;
    QPushButton *reg;
    QLabel *check;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QStringLiteral("login"));
        login->resize(406, 181);
        QIcon icon;
        icon.addFile(QStringLiteral("./ressources/Chatot.png"), QSize(), QIcon::Normal, QIcon::Off);
        login->setWindowIcon(icon);
        centralwidget = new QWidget(login);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        nickname = new QLineEdit(centralwidget);
        nickname->setObjectName(QStringLiteral("nickname"));

        gridLayout->addWidget(nickname, 0, 1, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        password = new QLineEdit(centralwidget);
        password->setObjectName(QStringLiteral("password"));
        password->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoAutoUppercase|Qt::ImhNoPredictiveText|Qt::ImhSensitiveData);
        password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(password, 1, 1, 1, 1);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        pushButton = new QPushButton(splitter);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        splitter->addWidget(pushButton);
        reg = new QPushButton(splitter);
        reg->setObjectName(QStringLiteral("reg"));
        splitter->addWidget(reg);

        gridLayout->addWidget(splitter, 2, 0, 1, 2);

        check = new QLabel(centralwidget);
        check->setObjectName(QStringLiteral("check"));

        gridLayout->addWidget(check, 3, 0, 1, 2);

        login->setCentralWidget(centralwidget);
        menubar = new QMenuBar(login);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 406, 25));
        login->setMenuBar(menubar);
        statusbar = new QStatusBar(login);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        login->setStatusBar(statusbar);

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QMainWindow *login)
    {
        login->setWindowTitle(QApplication::translate("login", "Babel - Login", 0));
        label->setText(QApplication::translate("login", "Login:", 0));
        nickname->setText(QApplication::translate("login", "Login", 0));
        label_2->setText(QApplication::translate("login", "Password:", 0));
        password->setInputMask(QString());
        password->setText(QString());
        pushButton->setText(QApplication::translate("login", "Connect", 0));
        reg->setText(QApplication::translate("login", "register", 0));
        check->setText(QApplication::translate("login", "Enter login and pass word", 0));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
