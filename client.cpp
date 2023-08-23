#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client) {
    ui->setupUi(this);

    ui->error_label->setStyleSheet("color: red");

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1234);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}

Client::~Client() {
    delete ui;
    delete socket;
}

void Client::readData() {
    QHostAddress server;
    quint16 server_port;
    quint8 x, v, m, s, p;
    qint8 y;
    int8_t a;
    qint16 r;

    while (socket->hasPendingDatagrams()) {
        quint16 first_word;
        quint16 second_word;
        quint16 third_word;

        QByteArray message;
        message.resize(socket->pendingDatagramSize());
        socket->readDatagram(message.data(), message.size(), &server, &server_port);
        QDataStream stream(message);

        stream >> first_word >> second_word >> third_word >> r;

        y = (first_word >> 8) & 63;
        x = first_word & 63;

        s = (second_word >> 12) & 3;
        m = (second_word >> 8) & 3;
        v = second_word & 255;

        a = (third_word >> 8) & 255;
        p = third_word & 255;

        qfloat16 encoded_a = int(a) / 10.0;
        if (int(a) == -128) {
            encoded_a *= -1.0;
        }

        if (y == 32) {
            y = -32;
        }
        else if (y & 0x20) {
            y = y ^ (1 << 5);
            y = -y;
        }

        ui->x_data->setText(QString::number(x));
        ui->y_data->setText(QString::number(y));
        ui->v_data->setText(QString::number(v));
        ui->m_data->setText(QString::number(m));
        ui->s_data->setText(QString::number(s));
        ui->a_data->setText(QString::number(round(encoded_a*10)/10));
        ui->p_data->setText(QString::number(p));
        ui->r_data->setText(QString::number(r));
    }
}


void Client::on_confirm_button_clicked() {
    if (ui->port_edit->text().toInt() >= 0) {
        qDebug() << ui->port_edit->text().toInt() << "\n";

        socket->close();
        socket->bind(QHostAddress::LocalHost, ui->port_edit->text().toInt());
        ui->error_label->clear();
        ui->port_edit->clear();
    }
    else {
        ui->error_label->setText("Error in port input!");
    }
}

