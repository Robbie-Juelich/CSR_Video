#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(int conn, int heart, int yellow,QWidget *parent = 0);
    ~ConfigDialog();

    int get_conn_timeout() {
        return connect_timeout;
    }

    int get_heart_timeout() {
        return heartbeat_timeout;
    }

  //  int get_yellow_timeout(){
  //      return yellow_timeout;
  //  }

    int get_yellow_timeout(){
        return yellow_timeout;
    }

private slots:
    void on_pushButton_default_clicked();
    void onAccepted();

    void on_spinBox_yellow_valueChanged(int arg1);

private:
    Ui::ConfigDialog *ui;
    int connect_timeout;
    int heartbeat_timeout;
    //int yellow_timeout;//new add
    int yellow_timeout;//new add
};

#endif // CONFIGDIALOG_H
