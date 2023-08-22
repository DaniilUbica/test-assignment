#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QFloat16>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();
public slots:
    void readData();

private slots:
    void on_confirm_button_clicked();

private:
    Ui::Client *ui;
    QUdpSocket* socket;
};
#endif // CLIENT_H
