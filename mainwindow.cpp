#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QBuffer>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

int www;
int hhh;
int mode;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort;
    mode = 1;   //默认不全屏

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    wwww = screenRect.width();
    hhhh = screenRect.height();
    this->resize(wwww, hhhh);
/*
    if((wwww == 1920)&& (hhhh == 1080))
    {
        ui->label_3->resize(720, 405);
        www = 720;
        hhh = 405;
    }
    else
    {
        www = 480;
        hhh = 270;
    }


    this->resize(wwww, hhhh);

*/

    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setMaximum(16);
    ui->horizontalSlider->setValue(4);

    ui->horizontalSlider_2->setMinimum(1);
    ui->horizontalSlider_2->setMaximum(16);
    ui->horizontalSlider_2->setValue(10);

    ui->horizontalSlider_3->setMinimum(1);
    ui->horizontalSlider_3->setMaximum(100);
    ui->horizontalSlider_3->setValue(40);
    ui->horizontalSlider_4->setMinimum(1);
    ui->horizontalSlider_4->setMaximum(100);
    ui->horizontalSlider_4->setValue(50);

    ui->comboBox_2->addItem("com1");
    ui->comboBox_2->addItem("com2");
    ui->comboBox_2->addItem("com3");
    ui->comboBox_2->addItem("com4");
    ui->comboBox_2->addItem("com5");
    ui->comboBox_2->addItem("com6");
    ui->comboBox_2->addItem("com7");
    ui->comboBox_2->addItem("com8");
    ui->comboBox_2->addItem("com9");
    ui->comboBox_2->addItem("com10");
    ui->comboBox_2->addItem("com11");

    ui->comboBox_3->addItem("9600");
    ui->comboBox_3->addItem("19200");
    ui->comboBox_3->addItem("38400");
    ui->comboBox_3->addItem("57600");
    ui->comboBox_3->addItem("115200");
    ui->comboBox_3->setCurrentIndex(4);
    ui->comboBox_4->addItem("8");
    ui->comboBox_5->addItem("null");
    ui->comboBox_6->addItem("1");
    ui->comboBox_7->addItem("null");

    ui->led1_comboBox->addItem("NONE");
    ui->led1_comboBox->addItem("Blue");
    ui->led1_comboBox->addItem("Green");
    ui->led1_comboBox->addItem("Cyan");
    ui->led1_comboBox->addItem("Red");
    ui->led1_comboBox->addItem("Magenta");
    ui->led1_comboBox->addItem("Yellow");
    ui->led1_comboBox->addItem("White");
    ui->led1_comboBox->addItem("ALL");
    ui->led1_comboBox->setCurrentIndex(1);

    ui->led2_comboBox->addItem("NONE");
    ui->led2_comboBox->addItem("Blue");
    ui->led2_comboBox->addItem("Green");
    ui->led2_comboBox->addItem("Cyan");
    ui->led2_comboBox->addItem("Red");
    ui->led2_comboBox->addItem("Magenta");
    ui->led2_comboBox->addItem("Yellow");
    ui->led2_comboBox->addItem("White");
    ui->led2_comboBox->addItem("ALL");
    ui->led2_comboBox->setCurrentIndex(2);


    timeout1 = new QTimer(this);
    connect(timeout1,SIGNAL(timeout()), this, SLOT(time_out1()));
    timeout2 = new QTimer(this);
    connect(timeout2,SIGNAL(timeout()), this, SLOT(time_out2()));
    timeout3 = new QTimer(this);
    connect(timeout3,SIGNAL(timeout()), this, SLOT(time_out3()));
    timeout4 = new QTimer(this);
    connect(timeout4,SIGNAL(timeout()), this, SLOT(time_out4()));
    timeout5 = new QTimer(this);
    connect(timeout5,SIGNAL(timeout()), this, SLOT(time_out5()));
    timeout6 = new QTimer(this);
    connect(timeout6,SIGNAL(timeout()), this, SLOT(time_out6()));

    ui->title_pic->setStyleSheet("border-image: url(:/qss/logo.png);");
    this->setWindowIcon(QIcon(":/qss/logo.png"));  //设置窗口图标
    mousePressed=false;
    max=false;

//    IconHelper::Instance()->setIcon(ui->title_pic, QChar(0xf099), 30);
    IconHelper::Instance()->setIcon(ui->menu_min_btn, QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->pushButton_15, QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->menu_closse_btn, QChar(0xf00d));
    ui->title->installEventFilter(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    ReadSetTing();

    socket = new QTcpSocket();
    connect(socket,SIGNAL(readyRead()),this,SLOT(socket_Read_Data()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(socket_close()));

    socket2 = new QTcpSocket();
    connect(socket2,SIGNAL(readyRead()),this,SLOT(socket_Read_Data2()));
    connect(socket2,SIGNAL(disconnected()),this,SLOT(socket_close2()));



//   ui->stateLabel->setStyleSheet("border-image: url(:/qss/psblack/radiobutton_checked_disable.png);");
//   ui->stateLabel->setStyleSheet("border-image: url(:/qss/22.png);");
//   ui->pushButton_1->setStyleSheet("QPushButton{border-image:url(:/images/image/Icon19.png);}");//图片在资源文件中
//   ui->stateLabel2->setText("离线");

    changesize = false;

    //创建文件夹，存放视频
    QDir dir;
    if(!dir.exists("./video"))
    {
        bool res = dir.mkpath("./video");
        qDebug() << "新建目录是否成功" << res;
    }
//显示视频
    connect(&T, SIGNAL(getImg(QImage)), this, SLOT(showImg(QImage)));
//    确认退出线程，切换分辨率
    connect(&T, SIGNAL(sendquit()), this, SLOT(getquit()));

}

void MainWindow::socket_close()
{
    ui->state1->setText("离线");
}

void MainWindow::socket_close2()
{
    ui->state2->setText("离线");
}

void MainWindow::ReadData()
 {
//     QByteArray buf;
//     buf = serial->readAll();
//     if(!buf.isEmpty())
//     {
//         QString str/* = ui->textEdit->toPlainText()*/;
//         str+=tr(buf);
////         ui->textEdit->clear();
////         ui->textEdit->append(str);
//         qDebug()<<str;

//     }
//     buf.clear();
 }

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    buffer = socket->readAll();
    qDebug()<<"read:"<<buffer;
    if(buffer == "all bytes recvd")
    {
//        ui->radioButton->setChecked(true);
        qDebug()<<"all bytes recvd";
        QMessageBox msgBox;
        msgBox.setText("发送成功");
        msgBox.exec();
    }
}

void MainWindow::socket_Read_Data2()
{
    QByteArray buffer;
    buffer = socket2->readAll();
    qDebug()<<"len:"<<buffer.length();
    qDebug()<<"read:"<<buffer;

}

MainWindow::~MainWindow()
{
    WriteSetTing();
    delete ui;
}

int MainWindow::file_size(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(!fp)
        return -1;
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fclose(fp);

    return size;
}

void MainWindow::on_sendBtn_clicked()
{
    QString pix_path = ui->filepathlineEdit->text();
    if(pix_path.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("请先选择图片！");
        msgBox.exec();
        return;
    }

    char index[1];
    index[0] = 0x68;
    socket->write(index, 1);

    FILE *fq;
    QByteArray ba = pix_path.toLatin1();

    int size = file_size(ba.data());
    QByteArray bb;
    bb.resize(sizeof(size));
    memcpy(bb.data(), &size, sizeof(size));
    socket->write(bb);

    char index2[1];
    index2[0] = 0x02;
    socket->write(index2, 1);

    if((fq = fopen(ba.data(), "rb")) == NULL)
    {
        qDebug()<<"failed";
        return;
    }
    char buffer[5120];
    while(!feof(fq))
    {
        int len = fread(buffer, 1, 5120, fq);
        socket->write(buffer, len);
    }
    fclose(fq);
}

void MainWindow::on_sendBtn_2_clicked()
{
    QString pix_path = ui->filepathlineEdit_2->text();
    if(pix_path.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("请先选择图片！");
        msgBox.exec();
        return;
    }

    char index[1];
    index[0] = 0x68;
    socket->write(index, 1);

    FILE *fq;
    QByteArray ba = pix_path.toLatin1();

    int size = file_size(ba.data());
    QByteArray bb;
    bb.resize(sizeof(size));
    memcpy(bb.data(), &size, sizeof(size));
    socket->write(bb);

    char index2[1];
    index2[0] = 0x01;
    socket->write(index2, 1);

    if((fq = fopen(ba.data(), "rb")) == NULL)
    {
        qDebug()<<"failed";
        return;
    }
    char buffer[5120];
    while(!feof(fq))
    {
        int len = fread(buffer, 1, 5120, fq);
        socket->write(buffer, len);
    }
    fclose(fq);
}


void MainWindow::on_connectBtn_clicked()
{
    ui->state1->setText("正在连接···");
    QString IP = ui->IPlineEdit->text();
    QString PORT = ui->PORTlineEdit->text();
    if(IP.isEmpty() || PORT.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("请先设置IP和端口！");
        msgBox.exec();
        return;
    }

    qDebug()<<"取消连接";
    socket->abort();

    qDebug()<<"开始连接";
    socket->connectToHost(IP.toLatin1(),PORT.toInt());
//    socket2->connectToHost(IP.toLatin1(),9001);

    if(!socket->waitForConnected(30000))
    {
//        qDebug("连接服务器失败！");
        ui->state1->setText("连接失败");
        QMessageBox msgBox;
        msgBox.setText("连接服务端失败！");
        msgBox.exec();
    }
    else
    {
//        ui->stateLabel->setStyleSheet("border-image: url(:/qss/11.png);");
//        ui->stateLabel2->setText("在线");
//        qDebug("连接服务器成功！");
        ui->state1->setText("在线");
        QMessageBox msgBox;
        msgBox.setText("连接服务端成功！");
        msgBox.exec();
    }

    ui->state2->setText("正在连接···");
    socket2->abort();
    socket2->connectToHost("192.168.1.150",6000);
    if(!socket2->waitForConnected(3000))
    {
        qDebug("连接服务器失败！");
        ui->state2->setText("连接失败");
    }
    else
    {
        ui->state2->setText("在线");
        qDebug("连接服务器成功！");
    }
}

void MainWindow::on_filepathBtn_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.png *.bmp *.jpg)"));
    qDebug()<<filename;
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        ui->filepathlineEdit->setText(filename);
    }

}

//写程序状态信息
int MainWindow::WriteSetTing()
{
    QSettings setting("setting.ini",QSettings::IniFormat);
    QString ip;
    QString port;

    setting.beginGroup(tr("net"));//节点开始

    ip = ui->IPlineEdit->text();
    port = ui->PORTlineEdit->text();
    setting.setValue("ip",ip);//设置key和value，也就是参数和值
    setting.setValue("port",port);

    setting.endGroup();//节点结束

    return 0;
}

//读取程序启动状态信息
int MainWindow::ReadSetTing()
{
    //设置配置文件的目录和位置，如果有，则不动，没有，会自动创建
    QSettings setting("setting.ini",QSettings::IniFormat);
    QString ip;
    QString port;
    if(setting.contains(tr("net/ip"))&&setting.contains(tr("net/port")))//如果已经存在这个文件，那就进行读取
    {
        ip = setting.value("net/ip").toString();//将读取出的数据进行使用
        port = setting.value("net/port").toString();
        ui->IPlineEdit->setText(ip);
        ui->PORTlineEdit->setText(port);
    }
    return 0;
}


void MainWindow::on_leftbtn_clicked()
{
    QString str = "write-relay:relay2=1;";
    QByteArray data(str.toLatin1());
    int ll = socket2->write(data);
    qDebug()<<"ll:"<<ll;
    timeout1->start(500);
}

void MainWindow::time_out1()
{
    QString str2 = "write-relay:relay2=0;";
    QByteArray data2(str2.toLatin1());
    int ll2 = socket2->write(data2);
    qDebug()<<"ll2:"<<ll2;
    timeout1->stop();
}

void MainWindow::on_rightbtn_clicked()
{
    QString str = "write-relay:relay4=1;";
    QByteArray data(str.toLatin1());
    int ll = socket2->write(data);
    qDebug()<<"ll:"<<ll;
    timeout2->start(500);
}

void MainWindow::time_out2()
{
    QString str2 = "write-relay:relay4=0;";
    QByteArray data2(str2.toLatin1());
    int ll2 = socket2->write(data2);
    qDebug()<<"ll2:"<<ll2;
    timeout2->stop();
}

void MainWindow::on_beforebtn_clicked()
{
    QString str = "write-relay:relay3=1;";
    QByteArray data(str.toLatin1());
    int ll = socket2->write(data);
    qDebug()<<"ll:"<<ll;
    timeout3->start(500);
}

void MainWindow::time_out3()
{
    QString str2 = "write-relay:relay3=0;";
    QByteArray data2(str2.toLatin1());
    int ll2 = socket2->write(data2);
    qDebug()<<"ll2:"<<ll2;
    timeout3->stop();
}

void MainWindow::on_backbtn_clicked()
{
    QString str = "write-relay:relay1=1;";
    QByteArray data(str.toLatin1());
    int ll = socket2->write(data);
    qDebug()<<"ll:"<<ll;
    timeout4->start(500);
}

void MainWindow::time_out4()
{
    QString str2 = "write-relay:relay1=0;";
    QByteArray data2(str2.toLatin1());
    int ll2 = socket2->write(data2);
    qDebug()<<"ll2:"<<ll2;
    timeout4->stop();
}

void MainWindow::on_cam_up_btn_clicked()
{
    QString str = "write-relay:relay6=1;";
    QByteArray data(str.toLatin1());
    int ll = socket2->write(data);
    qDebug()<<"ll:"<<ll;
    timeout5->start(500);
}

void MainWindow::time_out5()
{
    QString str2 = "write-relay:relay6=0;";
    QByteArray data2(str2.toLatin1());
    int ll2 = socket2->write(data2);
    qDebug()<<"ll2:"<<ll2;
    timeout5->stop();
}

void MainWindow::on_cam_down_btn_clicked()
{
    QString str = "write-relay:relay5=1;";
    QByteArray data(str.toLatin1());
    int ll = socket2->write(data);
    qDebug()<<"ll:"<<ll;
    timeout6->start(500);
}

void MainWindow::time_out6()
{
    QString str2 = "write-relay:relay5=0;";
    QByteArray data2(str2.toLatin1());
    int ll2 = socket2->write(data2);
    qDebug()<<"ll2:"<<ll2;
    timeout6->stop();
}

void MainWindow::on_quit_clicked()
{
    this->close();
}

void MainWindow::on_menu_closse_btn_clicked()
{
    close();
}

void MainWindow::on_menu_min_btn_clicked()
{
    showMinimized();
}
/***************************************************
函数名：mouseMoveEvent
参数：QMouseEvent*
功能：重写鼠标移动窗口事件
返回值：无
***************************************************/
void MainWindow::mouseMoveEvent(QMouseEvent*e)
{
    if((mousePressed&&(e->buttons()&&Qt::LeftButton)&&!max))
    {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}

/***************************************************
函数名：mousePressEvent
参数：QMouseEvent*
功能：重写鼠标按下事件
返回值：无
***************************************************/
void MainWindow::mousePressEvent(QMouseEvent*e)
{
    if(e->button()==Qt::LeftButton)
    {
        mousePressed=true;
        mousePoint=e->globalPos()-this->pos();
        e->accept();
    }
}

/***************************************************
函数名：mouseReleaseEvent
参数：QMouseEvent*
功能：重写鼠标松开事件
返回值：无
***************************************************/
void MainWindow::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed=false;
}

void MainWindow::on_pushButton_clicked()
{
    QString portname = ui->comboBox_2->currentText();
    QString baudrate = ui->comboBox_3->currentText();
    serial->setPortName(portname);
    bool open_flag;
    open_flag = serial->open(QIODevice::ReadWrite);
    if(!open_flag)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("警告");
        msgBox.setText("串口打开失败，请检查串口是否被占用");
        msgBox.exec();
        qDebug()<<"open fail";
        return;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("提示");
        msgBox.setText("串口打开成功");
        msgBox.exec();
    }


    if(baudrate == "115200")
    {
        serial->setBaudRate(QSerialPort::Baud115200);//设置波特率为115200
    }
    serial->setDataBits(QSerialPort::Data8);//设置数据位8
    serial->setParity(QSerialPort::NoParity); //校验位设置为0
    serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
    serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
}

void MainWindow::on_pushButton_2_clicked()
{
   // serial->clear();
    serial->close();
   // serial->deleteLater();
}

void MainWindow::on_pushButton_3_clicked()
{
    int colcr = ui->led1_comboBox->currentIndex();
    int qiangdu = ui->horizontalSlider->value();
        QByteArray aa;
        aa[0] = 0xFA;
        aa[1] = 0x13;
        aa[2] = 0x06;
        aa[3] = 0xfa;
        aa[4] = 0x01;
        aa[5] = colcr;
        aa[6] = qiangdu;
        aa[7] = 0x00;
        aa[8] = 0x00;
        aa[9] = 0x00;
        aa[10] = 1 + colcr + qiangdu;
        aa[11] = 0x16;
        serial->write(aa);
}

void MainWindow::on_pushButton_4_clicked()
{
    int color = ui->led2_comboBox->currentIndex();
    int qiangdu = ui->horizontalSlider_2->value();
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x13;
    aa[2] = 0x06;
    aa[3] = 0xfa;
    aa[4] = 0x00;
    aa[5] = 0x00;
    aa[6] = 0x00;
    aa[7] = 0x02;
    aa[8] = color;
    aa[9] = qiangdu;
    aa[10] = 2 + color + qiangdu;
    aa[11] = 0x16;
    serial->write(aa);
}

void MainWindow::on_pushButton_5_clicked()
{
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x13;
    aa[2] = 0x06;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = 0x00;
    aa[6] = 0x00;
    aa[7] = 0x02;
    aa[8] = 0x00;
    aa[9] = 0x00;
    aa[10] = 0x03;
    aa[11] = 0x16;
    serial->write(aa);
}

void MainWindow::on_pushButton_6_clicked()
{
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x14;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = 0x01;
    aa[6] = 0x02;
    aa[7] = 0x01;
    aa[8] = 0x05;
    aa[9] = 0x16;
    serial->write(aa);
}

void MainWindow::on_pushButton_7_clicked()
{
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x14;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = 0x02;
    aa[6] = 0x02;
    aa[7] = 0x02;
    aa[8] = 0x07;
    aa[9] = 0x16;

    serial->write(aa);
}

void MainWindow::on_pushButton_8_clicked()
{
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x14;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = 0x00;
    aa[6] = 0x02;
    aa[7] = 0x00;
    aa[8] = 0x03;
    aa[9] = 0x16;
    serial->write(aa);
}

void MainWindow::on_pushButton_9_clicked()
{
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x12;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = 0x64;
    aa[6] = 0x00;
    aa[7] = 0x00;
    aa[8] = 0x65;
    aa[9] = 0x16;
    serial->write(aa);
}

void MainWindow::on_pushButton_10_clicked()
{
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x12;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x00;
    aa[5] = 0x00;
    aa[6] = 0x02;
    aa[7] = 0x64;
    aa[8] = 0x66;
    aa[9] = 0x16;
    serial->write(aa);
}

void MainWindow::on_pushButton_11_clicked()
{
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x12;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = 0x00;
    aa[6] = 0x02;
    aa[7] = 0x00;
    aa[8] = 0x03;
    aa[9] = 0x16;
    serial->write(aa);
}



void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    int pos = ui->horizontalSlider->value();
    int color = ui->led1_comboBox->currentIndex();
    qDebug()<<pos;
    qDebug()<<action;
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x13;
    aa[2] = 0x06;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = color;
    aa[6] = pos;
    aa[7] = 0x00;
    aa[8] = 0x00;
    aa[9] = 0x00;
    aa[10] = 2 + pos + color;
    aa[11] = 0x16;
    serial->write(aa);

}

void MainWindow::on_horizontalSlider_2_actionTriggered(int action)
{

    int pos = ui->horizontalSlider_2->value();
    int color = ui->led2_comboBox->currentIndex();
    qDebug()<<pos;
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x13;
    aa[2] = 0x06;
    aa[3] = 0xfa;
    aa[4] = 0x00;
    aa[5] = 0x00;
    aa[6] = 0x00;
    aa[7] = 0x02;
    aa[8] = color;
    aa[9] = pos;
    aa[10] = pos + 4 + color;
    aa[11] = 0x16;
    serial->write(aa);
}

void MainWindow::on_horizontalSlider_3_actionTriggered(int action)
{
    int pos = ui->horizontalSlider_3->value();
    qDebug()<<pos;
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x12;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = pos;
    aa[6] = 0x00;
    aa[7] = 0x00;
    aa[8] = 1+pos;
    aa[9] = 0x16;
    serial->write(aa);
}

void MainWindow::on_horizontalSlider_4_actionTriggered(int action)
{
    int pos = ui->horizontalSlider_4->value();
    qDebug()<<pos;
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x12;
    aa[2] = 0x04;
    aa[3] = 0xfa;
    aa[4] = 0x00;
    aa[5] = 0x00;
    aa[6] = 0x02;
    aa[7] = pos;
    aa[8] = pos+2;
    aa[9] = 0x16;
    serial->write(aa);
}

void MainWindow::on_led1_comboBox_activated(const QString &arg1)
{
    int colcr = ui->led1_comboBox->currentIndex();
    int qiangdu = ui->horizontalSlider->value();
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x13;
    aa[2] = 0x06;
    aa[3] = 0xfa;
    aa[4] = 0x01;
    aa[5] = colcr;
    aa[6] = qiangdu;
    aa[7] = 0x00;
    aa[8] = 0x00;
    aa[9] = 0x00;
    aa[10] = 1 + colcr + qiangdu;
    aa[11] = 0x16;
    serial->write(aa);
}

void MainWindow::on_led2_comboBox_activated(const QString &arg1)
{
    int color = ui->led2_comboBox->currentIndex();
    int qiangdu = ui->horizontalSlider_2->value();
    QByteArray aa;
    aa[0] = 0xFA;
    aa[1] = 0x13;
    aa[2] = 0x06;
    aa[3] = 0xfa;
    aa[4] = 0x00;
    aa[5] = 0x00;
    aa[6] = 0x00;
    aa[7] = 0x02;
    aa[8] = color;
    aa[9] = qiangdu;
    aa[10] = 2 + color + qiangdu;
    aa[11] = 0x16;
    serial->write(aa);
}





void MainWindow::on_filepathBtn_2_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.png *.bmp *.jpg)"));
    qDebug()<<filename;
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        ui->filepathlineEdit_2->setText(filename);
    }
}

//    确认退出线程，切换分辨率
void MainWindow::getquit()
{
    if(changesize)
    {
        ui->label_3->clear();
        ui->label_3->setText("切换分辨率成功");
    }
    changesize = false;
}

//显示视频
void MainWindow::showImg(QImage img)
{
    pix.convertFromImage(img);
    //pix.save("pix.bmp", "bmp");

//    update();
    ui->label_3->setPixmap(pix);
}

void MainWindow::on_pushButton_12_clicked()
{
    on_pushButton_14_clicked();
    changesize = true;
}

void MainWindow::on_pushButton_13_clicked()
{
    int index = ui->comboBox->currentIndex();
    switch (index) {
    case 0:
        qDebug()<<"320x240";
        T.ff_set(320, 240, 30);
        break;
    case 1:
        T.ff_set(640, 480, 30);
        qDebug()<<"640x480";
        break;
    case 2:
        T.ff_set(800, 600, 60);
        qDebug()<<"800x600";
        break;
    case 3:
        T.ff_set(1024, 768, 30);
        qDebug()<<"1024x768";
        break;
    case 4:
        T.ff_set(1280, 720, 60);
        qDebug()<<"1280x720";
        break;
    case 5:
        T.ff_set(1280, 1024, 30);
        qDebug()<<"1280x1024";
        break;
    case 6:
        T.ff_set(1920, 1080, 30);
        qDebug()<<"1920x1080";
        break;
    default:
        break;
    }
    ui->label_3->clear();
    T.start();
}

void MainWindow::on_pushButton_14_clicked()
{
    T.th_quit(true);
    ui->label_3->clear();
}

void MainWindow::on_pushButton_15_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max)
    {


       /* ui->showlabel_1->clear();
        ui->showlabel_2->clear();
        ui->showlabel_3->clear();
        ui->showlabel_4->clear();

        ui->play_lab->clear();
        ui->play_lab->resize(812, 496);*/
        ui->label_3->clear();
        ui->label_3->resize(480,270);
        this->setGeometry(location);

        //qDebug("mode 1 label_w:%d", label_w);
       // qDebug("mode 1 label_d:%d", label_h);
       // ui->showlabel_1->changesize(label_w, label_h);
       // ui->showlabel_2->changesize(label_w, label_h);
       // ui->showlabel_3->changesize(label_w, label_h);
       // ui->showlabel_4->changesize(label_w, label_h);

        mode = 1;
       // qDebug("www:%d", www);
        //P.ChangeSize(www, hhh);
    }
    else
    {

        mode = 2;


        //ui->showlabel_1->clear();
       // ui->showlabel_2->clear();
       // ui->showlabel_3->clear();
       // ui->showlabel_4->clear();
       // ui->play_lab->clear();
        ui->label_3->clear();
        ui->label_3->resize(960,540);
        //location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());

        qDebug()<<ui->label_3->height();
        qDebug()<<ui->label_3->width();




        //qDebug()<<wwww;
        //qDebug()<<hhhh;

        //int w = ui->showlabel_1->width();
       // int h = ui->showlabel_1->height();
       // qDebug("mode 2 label_w:%d", w);
       // qDebug("mode 2 label_d:%d", h);
       // www = w;
       // hhh = h;
       // ui->showlabel_1->changesize(w, h);
       // ui->showlabel_2->changesize(w, h);
       // ui->showlabel_3->changesize(w, h);
       // ui->showlabel_4->changesize(w, h);
       // int xgw = ui->play_lab->width();
       // int xgh = ui->play_lab->height();

       // P.ChangeSize(xgw, xgh);

    }
    this->setProperty("canMove", max);
    max = !max;
}
