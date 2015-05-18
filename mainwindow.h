#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

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

  int cmd; // 63 bit
  int speed; // Скорость двигателей

  // Port
  void setPortName();
  void openPort(QString);

  void setDisableButton(bool disable);

  private slots:
    void sOpenPort();
    void sClosePort();
    void sWritePort();

    void sRobotGo();
    void sRobotStop();
    void sRobotBack();
    void sRobotLeft();
    void sRobotRight();

    void sSetPortName();
    void sClearLog();
    void sSetSpeed();
    void sReadyRead();
    void sTimerDelay();

protected:
    //void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
  Ui::MainWindow *ui;
  QSerialPort *serial;
  QTimer *timer_delay;
};

#endif // MAINWINDOW_H
