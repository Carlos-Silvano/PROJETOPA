#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);


    connect(ui->connectip, SIGNAL(clicked(bool)), this, SLOT(connectIp()));
    connect(ui->disconnect, SIGNAL(clicked(bool)), this, SLOT(disconnectFromServer()));
    connect(ui->Update, SIGNAL(clicked(bool)), this, SLOT(updateList()));
    connect(ui->Start, SIGNAL(clicked(bool)), this, SLOT(startPlotting()));
    connect(ui->Stop, SIGNAL(clicked(bool)), this, SLOT(stopPlotting()));
    connect(ui->timer, SIGNAL(valueChanged(int)), this, SLOT(mudaTime(int)));
    connect(ui->lista, SIGNAL(currentTextChanged(QString)), this, SLOT(onIpSelected(QString)));


    connect(this, SIGNAL(plotDataReady(double,double)), this, SLOT(updatePlot(double,double)));


    ui->plotWidget->addGraph();
    ui->plotWidget->graph(0)->setPen(QPen(Qt::blue));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcpConnect(QString ip){
    if(!connected){
        socket->connectToHost(ip,1234);
        if(socket->waitForConnected(3000)){
            qDebug() << "Connected";
            connected=true;
        }
    }
}


void MainWindow::updatePlot(double key, double value)
{
    plotKeys.append(key);
    plotValues.append(value);


    if (plotKeys.size() > 100) {
        plotKeys.removeFirst();
        plotValues.removeFirst();
    }

    ui->plotWidget->graph(0)->setData(plotKeys, plotValues);
    ui->plotWidget->rescaleAxes();
    ui->plotWidget->replot();
}

void MainWindow::getData(){
    QString str, str_get;
    QStringList list;


    if(socket->state() == QAbstractSocket::ConnectedState && socket->isOpen()){
        str_get = "get " + currentIp + " 1\n";
        socket->write(str_get.toUtf8());
        socket->waitForBytesWritten(500);
        socket->waitForReadyRead(500);


        QCoreApplication::processEvents();

        while(socket->bytesAvailable()){
            str = socket->readLine().replace("\n","").replace("\r","");
            list = str.split(" ");
            if(list.size() == 2){
                bool keyOk, valOk;
                qint64 key = list.at(0).toLongLong(&keyOk);
                double value = list.at(1).toDouble(&valOk);
                if (keyOk && valOk) {

                    emit plotDataReady(key, value);
                    qDebug() << "Dado recebido:" << key << value;
                }
            }
        }
    }
}

void MainWindow::connectIp(){

    tcpConnect(ui->lineEdit->text());
}

void MainWindow::disconnectFromServer(){
    if(connected){
        stopPlotting();
        socket->disconnectFromHost();

        if(socket->waitForDisconnected(3000)){
            qDebug() << "Disconnected";
            connected=false;
        }
    }
}

void MainWindow::updateList(){
    if(socket->state() == QAbstractSocket::ConnectedState && socket->isOpen()){
        socket->write("list\n");
        socket->waitForBytesWritten(500);
        socket->waitForReadyRead(500);
        QCoreApplication::processEvents();

        ui->lista->clear();
        while(socket->bytesAvailable()){
            QString ip = socket->readLine().replace("\n","").replace("\r","");
            if (!ip.isEmpty()) {
                ui->lista->addItem(ip);
                qDebug() << "IP recebido:" << ip;
            }
        }
    }
}

void MainWindow::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);
    getData();
}

void MainWindow::startPlotting(){
    if(ipSelected && connected && !timerOn){
        timerId = startTimer(time * 1000);
        timerOn = true;
    }
}

void MainWindow::stopPlotting(){
    if(timerOn){
        killTimer(timerId);
        timerOn = false;
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

void MainWindow::onIpSelected(const QString &ip){
    currentIp = ip;
    ipSelected = true;
    qDebug() << "IP selecionado:" << currentIp;
}
