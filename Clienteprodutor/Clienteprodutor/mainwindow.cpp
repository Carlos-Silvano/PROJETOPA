#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDateTime>
#include <QTcpSocket>
#include <QTimerEvent>
#include <QRandomGenerator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);


    connect(ui->connectip, &QPushButton::clicked, this, &MainWindow::connectIp);
    connect(ui->disconnect, &QPushButton::clicked, this, &MainWindow::disconnect);
    connect(ui->maximo, &QSlider::valueChanged, this, &MainWindow::mudaMax);
    connect(ui->minimo, &QSlider::valueChanged, this, &MainWindow::mudaMin);
    connect(ui->timing, &QSlider::valueChanged, this, &MainWindow::mudaTime);
    connect(ui->start, &QPushButton::clicked, this, &MainWindow::StartTimer);
    connect(ui->stop, &QPushButton::clicked, this, &MainWindow::StopTimer);
    connect(this, &MainWindow::dado, ui->quadro, &QTextEdit::append);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::tcpConnect(QString ip){
    if(!connected){
        socket->connectToHost(ip, 1234);
        if(socket->waitForConnected(3000)){
            qDebug() << "Connected";
            connected = true;
        } else {
            qDebug() << "Connection failed:" << socket->errorString();
        }
    }
}

void MainWindow::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);
    putData();
}

void MainWindow::connectIp(){

    tcpConnect(ui->iptcp->text());
}

void MainWindow::disconnect(){
    if(connected){
        StopTimer();
        socket->disconnectFromHost();

        if(socket->waitForDisconnected(3000)){
            qDebug() << "Disconnected";
            connected = false;
        }
    }
}

void MainWindow::mudaMax(int val){

    max = val;
    if (max < min) {
        min = max;
        ui->minimo->setValue(min);
    }
}

void MainWindow::mudaMin(int val){
    min = val;
    if (min > max) {
        max = min;
        ui->maximo->setValue(max);
    }
}

void MainWindow::mudaTime(int val){
    if (val > 0) {
        time = val;
        if(timerOn){
            killTimer(timerId);
            timerId = startTimer(time * 1000);
        }
    }
}

void MainWindow::StartTimer(){
    if(connected && !timerOn){
        timerId = startTimer(time * 1000);
        timerOn = true;
    }
}

void MainWindow::StopTimer(){
    if(timerOn){
        killTimer(timerId);
        timerOn = false;
    }
}

void MainWindow::putData(){
    if(socket->state() == QAbstractSocket::ConnectedState){
        qint64 msecdate = QDateTime::currentMSecsSinceEpoch();


        int randomValue = 0;
        if (min <= max) {
            randomValue = QRandomGenerator::global()->bounded(min, max + 1);
        }

        QString str = "set " + QString::number(msecdate) + " " + QString::number(randomValue);
        emit dado(str);


        socket->write(str.toUtf8() + "\n");
        qDebug() << "Sent:" << str;
    }
}
