#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QString>

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
    void putData();
public slots:
    void timerEvent(QTimerEvent *e);
    void connectIp();
    void disconnect();
    void mudaMax(int val);
    void mudaMin(int val);
    void mudaTime(int val);
    void StartTimer();
    void StopTimer();
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    int max=0,min=0,time=1;
    int timerId;
    bool timerOn=false,connected=false;
signals:
    void dado(QString);
};

#endif // MAINWINDOW_H
