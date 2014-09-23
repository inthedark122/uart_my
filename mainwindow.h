#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
  // Variable
  QSerialPort serial;

  // Port
  void setPortName();
  void openPort(QString);

  private slots:
    void sOpenPort();
    void sClosePort();
    void sWritePort();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
