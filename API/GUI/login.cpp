#include "login.h"
#include "ui_login.h"
#include "client.hh"

login::login(Client *c, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    client = c;
    ui->setupUi(this);
    this->_retVal[200] = "Succesfully registered";
    this->_retVal[311] = "Already logged in";
    this->_retVal[312] = "Wrong password";
    this->_retVal[313] = "Not registered";
    this->_retVal[314] = "Someone else already connected";
    this->_retVal[321] = "This account already exists";
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
  QString  tmp(ui->nickname->text() + " " + ui->password->text());
  QString  Name(ui->nickname->text());
  QString  Pass(ui->password->text());

  if (Name.size() > 0 && Pass.size() > 0)
  {
    this->name = ui->nickname->text().toStdString();
    this->client->writeToNet(1, tmp.toStdString());
    ui->check->setText("checking... please wait.");
  }
  else
    ui->check->setText("Please write a name and a password");
}

void login::read_return(int const cmd, int const val)
{
  if (val == 200 && cmd == 1)
  {
    ui->check->setText("Welcome.");
    sleep(1);
    this->client->startMain(name);
    this->hide();
  }
  else
  {
    std::map<int,std::string>::const_iterator it = _retVal.find(val);
    if (it != _retVal.end())
      ui->check->setText(this->_retVal[val].c_str());
    else
      ui->check->setText("Unknow error");
  }
}

void login::on_reg_clicked()
{
  QString  tmp(ui->nickname->text() + " " + ui->password->text());
  QString  Name(ui->nickname->text());
  QString  Pass(ui->password->text());

  if (Name.size() > 0 && Pass.size() > 0)
  {
    this->client->writeToNet(2, tmp.toStdString());
    ui->check->setText("checking... please wait.");
  }
  else
    ui->check->setText("Please write a name and a password");
}
