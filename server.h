#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();

private slots:
    void on_send_button_clicked();

    void on_confirm_button_clicked();

    void on_next_bit_button_clicked();

    void on_change_bit_button_clicked();

private:
    void sendData();
    bool checkData(int x, int y, int v, int m, int s, float a, int p, int r);
    QString paintSymbol(int idx);

    const int R_BITS_AMOUNT = 16;
    Ui::Server *ui;
    QUdpSocket* socket;
    quint16 send_port;

    int curr_bit = 0;
    QString r_text = "0000000000000000";
};
#endif // SERVER_H
