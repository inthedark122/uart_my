//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>

#include <QtCore/QDebug>
// qDebug() << portName;

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // connect(ui->calc, SIGNAL(clicked()), this, SLOT(calc_action()));
  connect(ui->conectPort, SIGNAL(clicked()), this, SLOT(sOpenPort()));
  connect(ui->closePort, SIGNAL(clicked()), this, SLOT(sClosePort()));
  connect(ui->writePort, SIGNAL(clicked()), this, SLOT(sWritePort()));
  connect(ui->robot_go, SIGNAL(clicked()), this, SLOT(sRobotGo()));
  connect(ui->robot_stop, SIGNAL(clicked()), this, SLOT(sRobotStop()));
  connect(ui->robot_back, SIGNAL(clicked()), this, SLOT(sRobotBack()));
  setPortName();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setPortName() {
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    //ui->portName->
    ui->portName->addItem(info.portName());
    //ui->listWidget->addItem(info.portName());
    //qDebug() << "Name        : " << info.portName();
    //qDebug() << "Description : " << info.description();
    //qDebug() << "Manufacturer: " << info.manufacturer();


  }
}

void MainWindow::openPort(QString portName) {
  ui->listWidget->addItem(QString("Открытие порта"));
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    if (info.portName() == portName) {
      // Example use QSerialPort
      serial.setPort(info);
      if (serial.open(QIODevice::ReadWrite)) {
        ui->listWidget->addItem(QString("Порт открыт"));
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        serial.setDataBits(QSerialPort::Data8);
        return;
      } else {
        ui->listWidget->addItem(QString("Не могу открыть порт"));
      }
      //    serial.close();
    }
  }
}

// Отлавливаем события
void MainWindow::sOpenPort() {
  QString cur_text = ui->portName->currentText();
  ui->listWidget->addItem(cur_text);
  openPort(cur_text);
}

void MainWindow::sClosePort() {
  serial.close();
  ui->listWidget->addItem(QString("Порт закрыт"));
}

void MainWindow::sRobotGo() {
  cmd = 1;
  sWritePort();
}

void MainWindow::sRobotStop() {
  cmd = 0;
  sWritePort();
}

void MainWindow::sRobotBack() {
  cmd = 2;
  sWritePort();
}


void MainWindow::sWritePort() {
  if (cmd == 1) {
    qDebug() << "14";
    for (int i =0; i < 13; i++) {
      serial.write("14", 2); // 35
      serial.waitForBytesWritten(3000);
    }
  } else if (cmd == 2) {
    qDebug() << "16";
    for (int i =0; i < 13; i++) {
      serial.write("16", 2); // 56
      serial.waitForBytesWritten(3000);
    }
  } else {
    qDebug() << "15";
    for (int i =0; i < 13; i++) {
      serial.write("15", 2); // 63
      serial.waitForBytesWritten(3000);
    }
  }

   //read(&c2,6);
  //for (int kkkkkk = 0; kkkkkk < 10; kkkkkk++) {
  //  serial.waitForReadyRead(3000);
  //  QByteArray tmp = serial.readAll();
  //  qDebug() << tmp;
  //}

  //ui->listWidget->addItem(QString((int)c2));

  /*QByteArray read_ir;
  if (serial.waitForReadyRead(3000)) {
    read_ir = serial.readAll();
    int tmp[read_ir.length()];
    for (int i = 0; i < read_ir.length(); i++) {
      tmp[i] = (int)read_ir.at(i);
      //qDebug() << (int)read_ir.at(i);
    }
    qDebug() << tmp;
    qDebug() << "next";
  }*/
  qDebug() << "Отправил";
}
