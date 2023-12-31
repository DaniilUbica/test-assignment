#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);

    ui->error_label->setStyleSheet("color: red");

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 7878);
    send_port = 1234;
}

Server::~Server() {
    delete ui;
    delete socket;
}

void Server::sendData() {
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    stream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    quint8 x = ui->x_edit->text().toInt();
    qint8 y = ui->y_edit->text().toInt();
    quint8 v = ui->v_edit->text().toInt();
    quint8 m = ui->m_edit->text().toInt();
    quint8 s = ui->s_edit->text().toInt();
    qfloat16 a = ui->a_edit->text().toFloat();
    quint8 p = ui->p_edit->text().toInt();
    qint16 r(0);

    int8_t coded_a = round(a*10)/10 * 10;

    if (y < 0) {
        if (y != -32) {
            quint8 mask = 1 << 5;
            y = y ^ mask;
            y = abs(y);
        }
    }

    x = x & 0x3F;
    y = y & 0x3F;
    m = m & 0x03;
    s = s & 0x03;

    quint16 first_word = 0;
    first_word |= y << 8;
    first_word |= x;

    quint16 second_word = 0;
    second_word |= s << 12;
    second_word |= m << 8;
    second_word |= v;

    quint16 third_word = 0;
    third_word |= coded_a << 8;
    third_word |= p;

    qDebug() << third_word << "\n";
    for (int i = 0; i < R_BITS_AMOUNT; i++) {
        (r <<= 1) |= r_text[i].digitValue();
    }

    qDebug() << "Packet size: " << sizeof(first_word) + sizeof(second_word) + sizeof(third_word) + sizeof(r) << "bytes\n";

    stream << first_word << second_word << third_word << r;
    socket->writeDatagram(datagram, QHostAddress::LocalHost, send_port);
}
bool Server::checkData(int x, int y, int v, int m, int s, float a, int p) {
    if (x <= 63 && x >= 0 && y >=-32 && y <= 31 && v >= 0 && v <= 255 &&
            m >= 0 && m <= 3 && s >= 0 && s <= 3 && a >= -12.7
            && a < 12.9 && p >= 0 && p <= 130) {
        return true;
    }
    return false;
}

QString Server::paintSymbol(int idx) {
    QString text;

    for (int i = 0; i < R_BITS_AMOUNT; i++) {
        if (i == idx) {
            text += QString("<span style=\"color:red;\">%1</span>").arg(r_text.at(i));
        }
        else {
            text += QString("<span>%1</span>").arg(r_text.at(i));
        }
    }
    return text;
}

void Server::on_send_button_clicked() {
    int x = ui->x_edit->text().toInt();
    int y = ui->y_edit->text().toInt();
    int v = ui->v_edit->text().toInt();
    int m = ui->m_edit->text().toInt();
    int s = ui->s_edit->text().toInt();
    float a = ui->a_edit->text().toFloat();
    int p = ui->p_edit->text().toInt();

    if (checkData(x, y, v, m, s, a, p) && !ui->x_edit->text().isEmpty() && !ui->y_edit->text().isEmpty()
            && !ui->v_edit->text().isEmpty() && !ui->m_edit->text().isEmpty()
            && !ui->s_edit->text().isEmpty() && !ui->a_edit->text().isEmpty() && !ui->p_edit->text().isEmpty()) {
        sendData();


        ui->r_bits->setText("0000000000000000");
        r_text = "0000000000000000";
        curr_bit = 0;
        ui->error_label->clear();
    }
    else {
        ui->error_label->setText("Error in input!");
    }
}


void Server::on_confirm_button_clicked() {
    if (ui->serv_port_edit->text().toInt() > 0) {
        qDebug() << ui->serv_port_edit->text().toInt() << "\n";

        socket->close();
        socket->bind(QHostAddress::LocalHost, ui->serv_port_edit->text().toInt());
        ui->error_label->clear();
        ui->serv_port_edit->clear();
    }
    else {
        ui->error_label->setText("Error in server's port input!");
    }
    if (ui->send_port_edit->text().toInt() > 0) {
        qDebug() << send_port << "\n";

        send_port = ui->send_port_edit->text().toInt();
        ui->error_label->clear();
        ui->send_port_edit->clear();
    }
    else {
        ui->error_label->setText("Error in send port input!");
    }
}


void Server::on_next_bit_button_clicked() {
    curr_bit++;
    if (curr_bit == 16) {
        curr_bit = 0;
    }

    QString text = paintSymbol(curr_bit);

    ui->r_bits->setText(text);
}


void Server::on_change_bit_button_clicked() {
    if (r_text[curr_bit] == '0') {
        r_text[curr_bit] = '1';
    }
    else {
        r_text[curr_bit] = '0';
    }

    QString text = paintSymbol(curr_bit);

    ui->r_bits->setText(text);
}

