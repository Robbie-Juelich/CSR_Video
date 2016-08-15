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

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QCommandLineOption>
static int width = 0;
static int height = 0;
static void parseOption(const QApplication &app)
{
    QCommandLineOption op1("w");
    QCommandLineOption op2("h");
    QCommandLineParser parser;

    op1.setValueName("width");
    op2.setValueName("height");
    parser.addOption(op1);
    parser.addOption(op2);
    parser.process(app);
    bool ok = true;
    if (parser.isSet(op1)) {
        width = parser.value(op1).toInt(&ok);
    }
    if (!ok) {
        qDebug() << "Invalid w option: " << parser.value(op1);
        qDebug() << "Ignore the video size option";
    }
    if (parser.isSet(op2)) {
        height = parser.value(op2).toInt(&ok);
    }
    if (!ok) {
        qDebug() << "Invalid h option: " << parser.value(op2);
        qDebug() << "Ignore the video size option";
    }
    if (ok && width > 0 && height > 0) {
        qDebug() << "video minimum width:" << width << ", height:" << height;
    } else {
        qDebug() << "Error options or no options set";
    }
}
#endif

using namespace QsLogging;

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    if (a.isRunning()) {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("程序运行错误"),
            QString::fromLocal8Bit("程序已经在运行"));
        return 0;
    }
    parseOption(a);

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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (width > 0 && height > 0) {
        w.setVideoPlayerSize((unsigned int)width, (unsigned int)height);
    }
#endif

    splash->finish(&w);
    delete splash;

    QDesktopWidget *pDesk = QApplication::desktop();
    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 2);
    w.show();
	return a.exec();
}
