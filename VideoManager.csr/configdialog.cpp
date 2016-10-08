#include "configdialog.h"
#include "ui_configdialog.h"
#include "udpheartbeat.h"
#include "udpregister.h"

ConfigDialog::ConfigDialog(int conn, int heart, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    connect_timeout(conn), heartbeat_timeout(heart)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()),
            this, SLOT(onAccepted()));
    ui->spinBox_conn->setValue(connect_timeout);
    ui->spinBox_heart->setValue(heartbeat_timeout);
//    ui->spinBox_conn->setMinimum(1);
//    ui->spinBox_heart->setMinimum(1);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::onAccepted()
{
    connect_timeout = ui->spinBox_conn->value();
    heartbeat_timeout = ui->spinBox_heart->value();
}

void ConfigDialog::on_pushButton_default_clicked()
{
    connect_timeout = default_conn_timeout;
    heartbeat_timeout = default_heart_timeout;
    ui->spinBox_conn->setValue(connect_timeout);
    ui->spinBox_heart->setValue(heartbeat_timeout);
}
