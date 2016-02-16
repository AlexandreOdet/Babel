#include "mainwidget.h"
#include "ui_form.h"
#include "client.hh"

Mainwidget::Mainwidget(Client *c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mainwidget)
{
  this->client = c;
  this->ui->setupUi(this);
  this->_channel = NULL;
  this->_current = NULL;
}

Mainwidget::~Mainwidget()
{
}

/* POPUP */

void Mainwidget::openPopUp(std::string const &text)
{
  std::cout << text << std::endl;
  QMessageBox msgBox;
  msgBox.setText(QString::fromUtf8(text.c_str()));
  msgBox.exec();
}

void Mainwidget::failurePopUp(std::string const &text)
{
  QMessageBox::critical(this, "", QString::fromUtf8(text.c_str()));
}

bool Mainwidget::questionPopUp(std::string const &text)
{
  if (QMessageBox::question(this, "", QString::fromUtf8(text.c_str()),
    QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    return true;
  else
    return false;
}

/* BUTTON CLICK */

void Mainwidget::on_send_clicked()
{
  if (_channel != NULL)
  {
    QString name(QString::fromStdString(client->getName()));

    writeChannel(name.toStdString(), ui->chat->toPlainText().toStdString());

    std::string toSend = _channel->text().toStdString();
    toSend +=  " " + ui->chat->toPlainText().toStdString();

    conversationManager  tmp = client->getConvManager();
    tmp.addMsgToConv(ui->chat->toPlainText().toStdString(), client->getName() , _channel->text().toStdString());
    this->client->setConvManager(tmp);

    ui->chat->clear();
    client->writeToNet(13, toSend);
  }
  else
    failurePopUp("Please select a channel to send a message");
}

void Mainwidget::on_call_clicked()
{
  if (_channel)
    questionPopUp("Do you want to call on " + _channel->text().toStdString() + " ?");
}

void Mainwidget::on_add_clicked()
{

  bool ok = false;
  QString pseudo = QInputDialog::getText(this, "Add friend",
    "Enter friend's nickname", QLineEdit::Normal, QString(), &ok);
  if (ok && !pseudo.isEmpty() && pseudo.toStdString() != client->getName())
      this->client->writeToNet(4, pseudo.toStdString());
  else if (pseudo.toStdString() == client->getName())
    failurePopUp("You can\'t add yourself, sorry.");
  else
    failurePopUp("Please write a nickname");
}

void Mainwidget::on_del_clicked()
{
  if (this->_current != NULL)
  {
    QString message("Delete " + _current->text() + " ?");
    if (questionPopUp(message.toStdString()))
    {
      this->client->writeToNet(5, (_current->text()).toStdString());
      deleteContact(_current->text().toStdString());
    }
  }
  else
    openPopUp("Please select someone to delete");
}

void Mainwidget::on_channel_itemClicked(QListWidgetItem *item)
{
  this->_channel = item;
  item->setForeground(Qt::black);
  updateConv(_channel->text().toStdString());
}

void Mainwidget::on_channel_itemDoubleClicked(QListWidgetItem *item)
{
  this->_channel = item;
  if (questionPopUp("Do you really want to quit this conversation ?") == true)
  {
    deleteChannel(_channel->text().toStdString());
    this->client->writeToNet(8, _channel->text().toStdString());
  }
}

void Mainwidget::on_contact_itemClicked(QListWidgetItem *item)
{
  this->_current = item;
}

void Mainwidget::on_contact_itemDoubleClicked(QListWidgetItem *item)
{
  this->_current = item;
  this->client->writeToNet(7, item->text().toStdString());
}

/* OTHERS ACTIONS */

void Mainwidget::writeChannel(std::string const &name, std::string const &text)
{
  QString a = "[" + QString::fromStdString(name) + "] : ";
  QString b = QString::fromStdString(text);
  QString tmp(a + b);
  ui->textBrowser->append(tmp);
}

void Mainwidget::changeColor(std::string const &name, bool const state)
{
  QList<QListWidgetItem *> item;
  item = ui->contact->findItems(QString::fromStdString(name), Qt::MatchExactly);
  if (item.size() != 0)
  {
    if (state == true)
      item[0]->setForeground(Qt::green);
    else
      item[0]->setForeground(Qt::red);
  }
}

void Mainwidget::addContact(std::string const &name, bool const state)
{
  QList<QListWidgetItem *> 	item;
  item = ui->contact->findItems(QString::fromStdString(name), Qt::MatchExactly);
  if (item.size() == 0)
    ui->contact->addItem(QString::fromStdString(name));

  changeColor(name, state);

  contactManager  tmp = client->getContacts();
  tmp.createcontact(name);
  this->client->setContacts(tmp);
}

void Mainwidget::addChannel(std::string const &name)
{
  QList<QListWidgetItem *> 	item;
  item = ui->channel->findItems(QString::fromStdString(name), Qt::MatchExactly);
  if (item.size() == 0)
    ui->channel->addItem(QString::fromStdString(name));

  conversationManager  tmp = client->getConvManager();
  tmp.createConversation(name);
  this->client->setConvManager(tmp);
}

void Mainwidget::updateConv(std::string const &conv)
{
  /* Si on est sur la bonne fenetre, update les messages */
  if (_channel && conv.compare(_channel->text().toStdString()) == 0)
  {
    ui->textBrowser->clear();
    std::list<s_msg>  messages = client->getConvManager().getMessages(conv);
    for (std::list<s_msg>::iterator it = messages.begin(); it != messages.end(); it++)
      writeChannel((*it).user, (*it).msg);
  }
  else
  {
    QList<QListWidgetItem *> item;
    item = ui->channel->findItems(QString::fromStdString(conv), Qt::MatchExactly);

    if (item.size() > 0)
      item[0]->setForeground(Qt::green);
  }
}

void Mainwidget::deleteContact(std::string const &name)
{
  QList<QListWidgetItem *> 	item;
  item = ui->contact->findItems(QString::fromStdString(name), Qt::MatchExactly);
  if (item.size() > 0)
  {
    ui->contact->takeItem(ui->contact->row(item[0]));

    contactManager  tmp = client->getContacts();
    tmp.deletecontact(name);
    this->client->setContacts(tmp);
  }
}

void Mainwidget::deleteChannel(std::string const &name)
{
  QList<QListWidgetItem *> 	item;
  item = ui->channel->findItems(QString::fromStdString(name), Qt::MatchExactly);
  if (item.size() > 0)
    ui->channel->takeItem(ui->channel->row(item[0]));
}
