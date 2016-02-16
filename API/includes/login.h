#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QString>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "ILogin.hh"

class Client;

namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(Client *, QWidget *parent = 0);
    ~login();

    void read_return(int const, int const);

private slots:
    void on_pushButton_clicked();

    void on_reg_clicked();

private:
    Client      *client;
    Ui::login   *ui;
    std::string name;
    std::map<int, std::string> _retVal;
};

#endif // LOGIN_H
