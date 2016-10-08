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
    explicit ConfigDialog(int conn, int heart, QWidget *parent = 0);
    ~ConfigDialog();

    int get_conn_timeout() {
        return connect_timeout;
    }

    int get_heart_timeout() {
        return heartbeat_timeout;
    }

private slots:
    void on_pushButton_default_clicked();
    void onAccepted();

private:
    Ui::ConfigDialog *ui;
    int connect_timeout;
    int heartbeat_timeout;
};

#endif // CONFIGDIALOG_H
