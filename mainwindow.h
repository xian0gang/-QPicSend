#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <iconhelper.h>
#include <QTimer>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mythread.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void socket_Read_Data();
    void socket_close();
    void socket_Read_Data2();
    void socket_close2();
    void mousePressEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *e);

    int file_size(char *filename);

    void time_out1();
    void time_out2();
    void time_out3();
    void time_out4();
    void time_out5();
    void time_out6();

private slots:
    void on_sendBtn_clicked();
    void on_connectBtn_clicked();
    void on_filepathBtn_clicked();

    void on_leftbtn_clicked();
    void on_rightbtn_clicked();
    void on_beforebtn_clicked();
    void on_backbtn_clicked();

    void on_cam_up_btn_clicked();

    void on_cam_down_btn_clicked();

    void on_quit_clicked();

    void on_menu_closse_btn_clicked();

    void on_menu_min_btn_clicked();

    void ReadData();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_horizontalSlider_actionTriggered(int action);
    void on_horizontalSlider_2_actionTriggered(int action);
    void on_horizontalSlider_3_actionTriggered(int action);
    void on_horizontalSlider_4_actionTriggered(int action);
    void on_led1_comboBox_activated(const QString &arg1);
    void on_led2_comboBox_activated(const QString &arg1);

    void on_pushButton_11_clicked();



    void on_sendBtn_2_clicked();



    void on_filepathBtn_2_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void showImg(QImage img);
    void getquit();

    void on_pushButton_15_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTcpSocket *socket;
    QTcpSocket *socket2;
    int WriteSetTing();
    int ReadSetTing();
    QFont iconFont;             //图形字体

    bool mousePressed;
    bool max;
    QPoint mousePoint;

    QTimer *timeout1;
    QTimer *timeout2;
    QTimer *timeout3;
    QTimer *timeout4;
    QTimer *timeout5;
    QTimer *timeout6;

    FILE *fq;

    mythread T;
    QPixmap pix;
    int change;
    bool changesize;
    int wwww;
    int hhhh;

};

#endif // MAINWINDOW_H
