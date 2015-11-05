#include "mainwindow.h"
#include <QApplication>
#include <qtsingleapplication.h>
#include <QGst/Init>
#include <QsLog.h>
#include <QsLogDest.h>
#include <QMessageBox>
#include <QTextCodec>
#include <QSplashScreen>
#include<QDesktopWidget>
#include "videoplayer.h"
#include "audioplayer.h"

using namespace QsLogging;

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    if (a.isRunning()) {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("程序运行错误"),
            QString::fromLocal8Bit("程序已经在运行"));
        return 0;
    }

	QSplashScreen *splash = new QSplashScreen();
	splash->setPixmap(QPixmap(":/images/splash.png"));
	splash->show();

	Qt::Alignment topRight = Qt::AlignTop | Qt::AlignRight;
    splash->showMessage(QString::fromLocal8Bit("初始化日志系统..."), topRight);

	QDir(a.applicationDirPath()).mkdir("log");
	Logger &logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("log/server_log.txt"));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
                                       sLogPath, EnableLogRotation, MaxSizeBytes(5120000), MaxOldLogCount(2)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    // DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));
    // logger.addDestination(functorDestination);
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    QLOG_INFO() << "Program started";
    QLOG_DEBUG() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();

    splash->showMessage(QString::fromLocal8Bit("初始化流媒体..."), topRight);
	QGst::init(&argc, &argv);

    splash->showMessage(QString::fromLocal8Bit("初始化主窗口..."), topRight);
    MainWindow w;

    splash->finish(&w);
    delete splash;

    QDesktopWidget *pDesk = QApplication::desktop();
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 2);
    w.show();
	return a.exec();
}
