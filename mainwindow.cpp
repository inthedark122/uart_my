#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>

#include <QtCore/QDebug>
#include <QKeyEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  serial = new QSerialPort(this);
  timer_delay = new QTimer(this);
  speed = 0;
  is_command = false;
  command = 0;

  connect(ui->conectPort, SIGNAL(clicked()), this, SLOT(sOpenPort()));
  connect(ui->closePort, SIGNAL(clicked()), this, SLOT(sClosePort()));
  connect(ui->writePort, SIGNAL(clicked()), this, SLOT(sWritePort()));
  connect(ui->robot_go, SIGNAL(clicked()), this, SLOT(sRobotGo()));
  connect(ui->robot_stop, SIGNAL(clicked()), this, SLOT(sRobotStop()));
  connect(ui->robot_back, SIGNAL(clicked()), this, SLOT(sRobotBack()));
  connect(ui->robot_left, SIGNAL(clicked()), this, SLOT(sRobotLeft()));
  connect(ui->robot_right, SIGNAL(clicked()), this, SLOT(sRobotRight()));
  connect(ui->reinit_sensor, SIGNAL(clicked()), this, SLOT(sReinitSensor()));

  // Поиск доступных портов для передачи данных
  setPortName();
  connect(ui->scan_port, SIGNAL(clicked()), this, SLOT(sSetPortName()));

  // Очистка лога
  connect(ui->clear_log, SIGNAL(clicked()), this, SLOT(sClearLog()));

  // Установка скорости двигателей
  connect(ui->set_speed, SIGNAL(clicked()), this, SLOT(sSetSpeed()));

  // Прием данных по UART
  connect(serial, SIGNAL(readyRead()), this, SLOT(sReadyRead()));

  // Обработка задержки
  connect(timer_delay, SIGNAL(timeout()), this, SLOT(sTimerDelay()));
}


void MainWindow::sReadyRead() {
    QByteArray tmp = serial->readAll();

    int tmp_data=0;
    memcpy(&tmp_data, tmp, sizeof(int));

    //qDebug() <<"data=" << num;

    //return;
    if (is_command) {
        QString text_status = QString("Получаю ");
        text_status += QString::number(command) + ":" + QString::number(tmp_data);;
        switch(command) {
        case 8:
            ui->dist->setText(QString::number(tmp_data));
            break;
        case 9:
            ui->touch->setText(QString::number(tmp_data));
            break;
        case 11:
            ui->micro->setText(QString::number(tmp_data));
            break;
        }

        qDebug() << tmp.toHex();
        command = -1;
        is_command = false;
        ui->listWidget->insertItem(0, text_status);
    } else {
        is_command = true;
        command = tmp_data;
        qDebug() << "commad=" << command;
    }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::sSetPortName() {
    while (ui->portName->count()) {
      ui->portName->removeItem(0);
    }
    this->setPortName();
}

void MainWindow::sClearLog() {
    ui->listWidget->clear();
}

void MainWindow::sSetSpeed() {
    speed = ui->speed->value();
}

void MainWindow::setPortName() {
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    ui->portName->addItem(info.portName());
    //qDebug() << "Name        : " << info.portName();
    //qDebug() << "Description : " << info.description();
    //qDebug() << "Manufacturer: " << info.manufacturer();
  }
}

void MainWindow::openPort(QString portName) {
  ui->listWidget->insertItem(0, QString("Открытие порта"));
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    if (info.portName() == portName) {
      serial->flush();
      serial->setPort(info);
      if (serial->open(QIODevice::ReadWrite)) {
        ui->listWidget->insertItem(0, QString("Порт открыт"));
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setDataBits(QSerialPort::Data8);
        return;
      } else {
        ui->listWidget->insertItem(0, QString("Не могу открыть порт"));
        qDebug() << "error code = " << serial->error();
        qDebug() << "error string = " << serial->errorString();
      }
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
  serial->close();
  ui->listWidget->insertItem(0, QString("Порт закрыт"));
}

void MainWindow::sRobotGo() {
  cmd = 1;
  sWritePort();
}

void MainWindow::sRobotLeft() {
  cmd = 2;
  sWritePort();
}

void MainWindow::sRobotStop() {
  cmd = 3;
  sWritePort();
}

void MainWindow::sRobotRight() {
  cmd = 4;
  sWritePort();
}

void MainWindow::sRobotBack() {
  cmd = 5;
  sWritePort();
}


void MainWindow::sWritePort(bool is_set_timer) {
    int data = 0; // max 31 bit
    if (cmd <= 5) {
        data = speed;
    }

  QString text = QString().sprintf("%c%c", cmd, data);
  ui->listWidget->insertItem(0, QString().sprintf("Отправляю %i:%i", cmd, data));
  serial->write(text.toStdString().c_str(), 2); // 63
  serial->waitForBytesWritten(3000);
  ui->listWidget->insertItem(0, QString("Отправил"));

  if (is_set_timer && ui->set_timer->isChecked() && !timer_delay->isActive()) {
      int delay_time = QString(ui->time_deley->text()).toInt();
      timer_delay->start(delay_time);
      setDisableButton(true);
  }

}


void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    cmd = 0;
    switch(event->key()) {
    case Qt::Key_Up:
        cmd = 1;
        break;
    case Qt::Key_Left:
        cmd = 2;
        break;
    case Qt::Key_Right:
        cmd = 4;
        break;
    case Qt::Key_Down:
        cmd = 5;
        break;
    case Qt::Key_Space:
        cmd = 3;
        break;
    }
    //qDebug() << cmd;
    if (cmd != 0) {
        sWritePort();
    }
}


void MainWindow::sTimerDelay() {
    cmd = 3;
    sWritePort();

    timer_delay->stop();
    setDisableButton(false);
}

void MainWindow::setDisableButton(bool disable) {
    ui->robot_go->setDisabled(disable);
    ui->robot_back->setDisabled(disable);
    ui->robot_left->setDisabled(disable);
    ui->robot_right->setDisabled(disable);
    ui->robot_stop->setDisabled(disable);
    ui->reinit_sensor->setDisabled(disable);
}

void MainWindow::sReinitSensor() {
    cmd = 49;
    sWritePort(false);
}
