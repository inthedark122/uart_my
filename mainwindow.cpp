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

void MainWindow::sWritePort() {
  char text_send = ui->portText->text().toUtf8().data()[0];

  //char c1 = 'S';
  char c2;
  for (int i =0; i < 10000; i++) {
    serial.write(&text_send,1);
  }
  serial.waitForReadyRead(3000);
  serial.read(&c2,1);
  ui->listWidget->addItem(QString(c2));
  qDebug() << c2;
}
