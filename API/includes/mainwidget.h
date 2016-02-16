#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include <QModelIndex>
#include <QListWidget>
#include <QListWidgetItem>
#include <QInputDialog>
#include <sstream>

#include "IAudio.hh"
#include "PAudio.hh"
#include "IMainWidget.hh"

class Client;

namespace Ui {
    class Mainwidget;
}

class Mainwidget : public QDialog
{
    Q_OBJECT
public:
    explicit Mainwidget(Client *, QWidget *parent = 0);
    ~Mainwidget();

    void writeChannel(std::string const &, std::string const &);
    void addContact(std::string const &, bool const);
    void addChannel(std::string const &);
    void deleteContact(std::string const &);
    void deleteChannel(std::string const &);
    void changeColor(std::string const &, bool const);
    void updateConv(std::string const &);

    void openPopUp(std::string const &);
    void failurePopUp(std::string const &);
    bool questionPopUp(std::string const &);

private slots:
    void on_send_clicked();
    void on_channel_itemClicked(QListWidgetItem *item);
    void on_channel_itemDoubleClicked(QListWidgetItem *item);
    void on_contact_itemClicked(QListWidgetItem *item);
    void on_contact_itemDoubleClicked(QListWidgetItem *item);
    void on_add_clicked();
    void on_del_clicked();
    void on_call_clicked();

private:
    Ui::Mainwidget  *ui;
    QListWidgetItem *_current;
    QListWidgetItem *_channel;
    Client          *client;
    IAudio          *audio;
};

#endif // MAINWIDGET_H
