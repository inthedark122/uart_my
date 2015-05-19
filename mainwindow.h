#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

/*
 * Управление
 * 1 - вперед
 * 2 - влево
 * 3 - stop
 * 4 - вправо
 * 5 - назад
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
  bool is_command;
  int command;

  // Port
  void setPortName();
  void openPort(QString);

  void setDisableButton(bool disable);

  private slots:
    void sOpenPort();
    void sClosePort();
    void sWritePort(bool is_set_timer = true);

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

    void sReinitSensor();

protected:
    //void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
  Ui::MainWindow *ui;
  QSerialPort *serial;
  QTimer *timer_delay;
};

#endif // MAINWINDOW_H
