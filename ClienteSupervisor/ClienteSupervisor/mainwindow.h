#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void tcpConnect(QString ip);

private slots:
    void timerEvent(QTimerEvent *e);
    void getData();
    void connectIp();
    void disconnectFromServer();
    void updateList();
    void startPlotting();
    void stopPlotting();
    void mudaTime(int val);
    void onIpSelected(const QString &ip);
    void updatePlot(double key, double value);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    int timerId;
    bool timerOn=false, connected=false, ipSelected=false;
    int time=1;
    QString currentIp;


    QVector<double> plotKeys;
    QVector<double> plotValues;

signals:

    void plotDataReady(double key, double value);
};

#endif // MAINWINDOW_H
