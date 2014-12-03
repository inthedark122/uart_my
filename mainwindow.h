#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

/*
 * Управление
 * 1 - вперед
 * 2 - назад
 * 3 - влево
 * 4 - вправо
 * default:0 = stop
*/

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
  int cmd;

  // Port
  void setPortName();
  void openPort(QString);

  private slots:
    void sOpenPort();
    void sClosePort();
    void sWritePort();

    void sRobotGo();
    void sRobotStop();
    void sRobotBack();
private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
