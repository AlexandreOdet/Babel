/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Mainwidget
{
public:
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QPushButton *add;
    QPushButton *send;
    QTextEdit *chat;
    QPushButton *del;
    QListWidget *channel;
    QListWidget *contact;
    QPushButton *call;

    void setupUi(QWidget *Mainwidget)
    {
        if (Mainwidget->objectName().isEmpty())
            Mainwidget->setObjectName(QStringLiteral("Mainwidget"));
        Mainwidget->resize(804, 536);
        QIcon icon;
        icon.addFile(QStringLiteral("./ressources/Chatot.png"), QSize(), QIcon::Normal, QIcon::Off);
        Mainwidget->setWindowIcon(icon);
        gridLayout = new QGridLayout(Mainwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textBrowser = new QTextBrowser(Mainwidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(textBrowser, 0, 2, 3, 2);

        add = new QPushButton(Mainwidget);
        add->setObjectName(QStringLiteral("add"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("./ressources/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        add->setIcon(icon1);

        gridLayout->addWidget(add, 1, 0, 1, 1);

        send = new QPushButton(Mainwidget);
        send->setObjectName(QStringLiteral("send"));
        send->setMinimumSize(QSize(50, 49));
        QIcon icon2;
        icon2.addFile(QStringLiteral("./ressources/send.png"), QSize(), QIcon::Normal, QIcon::Off);
        send->setIcon(icon2);

        gridLayout->addWidget(send, 4, 3, 1, 1);

        chat = new QTextEdit(Mainwidget);
        chat->setObjectName(QStringLiteral("chat"));
        chat->setMaximumSize(QSize(16777215, 100));

        gridLayout->addWidget(chat, 3, 2, 2, 1);

        del = new QPushButton(Mainwidget);
        del->setObjectName(QStringLiteral("del"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("./ressources/del.png"), QSize(), QIcon::Normal, QIcon::Off);
        del->setIcon(icon3);

        gridLayout->addWidget(del, 1, 1, 1, 1);

        channel = new QListWidget(Mainwidget);
        channel->setObjectName(QStringLiteral("channel"));
        channel->setMinimumSize(QSize(170, 0));
        channel->setMaximumSize(QSize(150, 16777215));

        gridLayout->addWidget(channel, 2, 0, 3, 2);

        contact = new QListWidget(Mainwidget);
        contact->setObjectName(QStringLiteral("contact"));
        contact->setMinimumSize(QSize(170, 0));
        contact->setMaximumSize(QSize(150, 16777215));

        gridLayout->addWidget(contact, 0, 0, 1, 2);

        call = new QPushButton(Mainwidget);
        call->setObjectName(QStringLiteral("call"));
        call->setMaximumSize(QSize(100, 16777215));
        QIcon icon4;
        icon4.addFile(QStringLiteral("./ressources/call.png"), QSize(), QIcon::Normal, QIcon::Off);
        call->setIcon(icon4);

        gridLayout->addWidget(call, 3, 3, 1, 1);


        retranslateUi(Mainwidget);

        QMetaObject::connectSlotsByName(Mainwidget);
    } // setupUi

    void retranslateUi(QWidget *Mainwidget)
    {
        Mainwidget->setWindowTitle(QApplication::translate("Mainwidget", "Form", 0));
        textBrowser->setHtml(QApplication::translate("Mainwidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans'; font-size:10pt;\">      </span></p></body></html>", 0));
        add->setText(QString());
        send->setText(QString());
        del->setText(QString());
        call->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Mainwidget: public Ui_Mainwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
