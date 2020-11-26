#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   connectData("653276237","387186","7777");
    subscribeTopic("ZRYQ","1");
    publishTopic("ZRYQ","123.54");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectData(QString strDevice, QString strUsrName, QString pw)
{
    //头 L  固 定 M  Q  T  T  固 定 时 间 长度+设备ID                      长度+产品ID             长度+鉴权信息

    QString deviceID = strDevice;
    QString userName = strUsrName;
    QString passWord = pw;

    QByteArray deviceArry = deviceID.toLatin1();
    QByteArray userNameArry = userName.toLatin1();
    QByteArray passWordArry = passWord.toLatin1();

    QByteArray connectArry;
    connectArry[0] = 0x10;  //头
    //connectArry[1] = 0x00;  //L(1-4byte可变)

    QByteArray headArry;
    headArry[0] = 0x00;  //固
    headArry[1] = 0x04;  //定
    headArry[2] = 0x4D;  //M
    headArry[3] = 0x51;  //Q
    headArry[4] = 0x54;  //T
    headArry[5] = 0x54;  //T
    headArry[6] = 0x04;  //固
    headArry[7] = 0xC2;  //定
    headArry[8] = 0x00;  //时
    headArry[9] = 0x78;  //间

    //长度+设备ID
    int length = deviceArry.length();

    QByteArray bodyArry;
    bodyArry.append((length>>8)&0xFF);
    bodyArry.append(length&0xFF);
    bodyArry.append(deviceArry);

    //长度+产品ID（用户名）
    length = userNameArry.length();

    bodyArry.append((length>>8)&0xFF);
    bodyArry.append(length&0xFF);
    bodyArry.append(userNameArry);

    //长度+鉴权信息（密码）
    length = passWordArry.length();

    bodyArry.append((length>>8)&0xFF);
    bodyArry.append(length&0xFF);
    bodyArry.append(passWordArry);

    length = bodyArry.length() + 10;

    if( length < 128 ) //计算数据长度（1-4byte可变）
    {
        connectArry[1] = length;  //L(1-4byte可变)
    }
    else if(length < 16384)
    {
        connectArry[1] = (length >> 8) & 0xFF;
        connectArry[2] = length & 0xFF;
    }
    connectArry.append(headArry);
    connectArry.append(bodyArry);
    qDebug()<<connectArry.toHex();
 //   mpSerialPort->write(connectArry);
}
void MainWindow::subscribeTopic(QString topics, QString QOS)
{
    QByteArray ba;
    ba[0] = 0x82;

    QByteArray headArry;
    headArry[0] = 0x00;
    headArry[1] = 0x0A;

    //长度+主体
    //QString topic = QString("ZRYQ");
    QString topic = topics;
    QByteArray topicArry = topic.toLatin1();
    int length = topicArry.length();
    headArry.append((length>>8)&0xFF);
    headArry.append(length&0xFF);
    headArry.append(topicArry);
    if(QOS == "1")
    {
        headArry[headArry.length()] = 0x01;
    }
    else
    {
        headArry[headArry.length()] = 0x00;
    }


    ba[1] = headArry.length();
    ba.append(headArry);
 qDebug()<<ba.toHex();
  // mpSerialPort->write(ba);
}

void MainWindow::publishTopic(QString topics, QString data)
{
    QByteArray ba;
    ba[0] = 0x30;

    QByteArray headArry;

    //长度+主体
    //QString topic = QString("ZRYQ");
    QString topic = topics;
    QByteArray topicArry = topic.toLatin1();
    int length = topicArry.length();
    headArry.append((length>>8)&0xFF);
    headArry.append(length&0xFF);
    headArry.append(topicArry);

    //QString content = QString("Seeking is good!");
    QString content = data;
    headArry.append(content.toLatin1());
    ba[1] = headArry.length();
    ba.append(headArry);
qDebug()<<ba.toHex();
   // mpSerialPort->write(ba);
}
