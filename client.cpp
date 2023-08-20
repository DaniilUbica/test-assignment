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
    float a;
    qint16 r;

    while (socket->hasPendingDatagrams()) {
        QByteArray message;
        message.resize(socket->pendingDatagramSize());
        socket->readDatagram(message.data(), message.size(), &server, &server_port);
        QDataStream stream(message);
        stream >> x >> y >> v >> m >> s >> a >> p >> r;
        qDebug() << y << '\n';
        if (y == 32) {
            y = -32;
        }
        else if (y & 0x20) {
            y = y ^ (1 << 5);
            y = -y;
        }
        qDebug() << x << " " << y << " " << v << " " << m << " " << s << " " << a << " " << p << '\n';

        ui->x_data->setText(QString::number(x));
        ui->y_data->setText(QString::number(y));
        ui->v_data->setText(QString::number(v));
        ui->m_data->setText(QString::number(m));
        ui->s_data->setText(QString::number(s));
        ui->a_data->setText(QString::number(a));
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

