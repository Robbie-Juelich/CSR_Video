#include "configdialog.h"
#include "ui_configdialog.h"
#include "udpheartbeat.h"
#include "udpregister.h"

#include <QMessageBox>

ConfigDialog::ConfigDialog(int conn, int heart, int yellow,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    connect_timeout(conn), heartbeat_timeout(heart), yellow_timeout(yellow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()),
            this, SLOT(onAccepted()));
    ui->spinBox_conn->setValue(connect_timeout);
    ui->spinBox_heart->setValue(heartbeat_timeout);
    //ui->spinBox_yellow->setValue(yellow_timeout);
    ui->spinBox_yellow->setValue(yellow_timeout);
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
    //yellow_timeout=ui->spinBox_yellow->value();
    yellow_timeout=ui->spinBox_yellow->value();
}

void ConfigDialog::on_pushButton_default_clicked()
{
    connect_timeout = default_conn_timeout;
    heartbeat_timeout = default_heart_timeout;
 //   yellow_timeout=default_yellow_timeout;
    yellow_timeout=default_yellow_timeout;
    ui->spinBox_conn->setValue(connect_timeout);
    ui->spinBox_heart->setValue(heartbeat_timeout);
  //  ui->spinBox_yellow->setValue(yellow_timeout);
    ui->spinBox_yellow->setValue(yellow_timeout);
}

void ConfigDialog::on_spinBox_yellow_valueChanged(int arg1)
{

    if(ui->spinBox_yellow->value()>=ui->spinBox_heart->value())
    {
        QMessageBox::information(this, QString::fromLocal8Bit( "请重新设置!"),
                                 QString::fromLocal8Bit("网络预警时间必须小于心跳超时时间!"));
            ui->spinBox_yellow->setValue(yellow_timeout);
    }

    ui->spinBox_yellow->update();
}
