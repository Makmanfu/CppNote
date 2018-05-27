
#include "stdafx.h"
#include "frmnettool.h"
#include "ui_frmnettool.h"
#include "tcpserver.h"

#define TIMEMS qPrintable (QTime::currentTime().toString("HH:mm:ss zzz"))
#define TIME qPrintable (QTime::currentTime().toString("HH:mm:ss"))
#define QDATE qPrintable (QDate::currentDate().toString("yyyy-MM-dd"))
#define QTIME qPrintable (QTime::currentTime().toString("HH-mm-ss"))
#define DATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))
#define STRDATETIMEMS qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss-zzz"))

#define AppName "NetTool"
#define AppPath qApp->applicationDirPath()
#define AppDeskWidth qApp->desktop()->availableGeometry().width()
#define AppDeskHeight qApp->desktop()->availableGeometry().height()

int App::LeftPanelWidth = 50;
int App::RightPanelWidth = 50;
QString App::PlatForm = "WIN";
QString App::SendFileName = "send.txt";
QString App::DeviceFileName = "device.txt";

bool App::HexSendTcpServer = false;
bool App::HexReceiveTcpServer = false;
bool App::DebugTcpServer = false;
bool App::AutoSendTcpServer = false;
int App::IntervalTcpServer = 1000;
int App::TcpListenPort = 6000;

bool App::HexSendTcpClient = false;
bool App::HexReceiveTcpClient = false;
bool App::DebugTcpClient = false;
bool App::AutoSendTcpClient = false;
int App::IntervalTcpClient = 1000;
QString App::TcpServerIP = "127.0.0.1";
int App::TcpServerPort = 6000;

bool App::HexSendUdpServer = false;
bool App::HexReceiveUdpServer = false;
bool App::DebugUdpServer = false;
bool App::AutoSendUdpServer = false;
int App::IntervalUdpServer = 1000;
int App::UdpListenPort = 6000;
QString App::UdpClientIP = "127.0.0.1";
int App::UdpClientPort = 6000;

void App::ReadConfig()
{
    if (!CheckConfig())
        return;
    QString fileName = QString("%1/%2_Config.ini").arg(AppPath).arg(AppName);
    QSettings set(fileName, QSettings::IniFormat);
    set.beginGroup("AppConfig");
    App::LeftPanelWidth = set.value("LeftPanelWidth").toInt();
    App::RightPanelWidth = set.value("RightPanelWidth").toInt();
    App::PlatForm = set.value("PlatForm").toString();
    App::SendFileName = set.value("SendFileName").toString();
    App::DeviceFileName = set.value("DeviceFileName").toString();
    set.endGroup();
    set.beginGroup("TcpServerConfig");
    App::HexSendTcpServer = set.value("HexSendTcpServer").toBool();
    App::HexReceiveTcpServer = set.value("HexReceiveTcpServer").toBool();
    App::DebugTcpServer = set.value("DebugTcpServer").toBool();
    App::AutoSendTcpServer = set.value("AutoSendTcpServer").toBool();
    App::IntervalTcpServer = set.value("IntervalTcpServer").toInt();
    App::TcpListenPort = set.value("TcpListenPort").toInt();
    set.endGroup();
    set.beginGroup("TcpClientConfig");
    App::HexSendTcpClient = set.value("HexSendTcpClient").toBool();
    App::HexReceiveTcpClient = set.value("HexReceiveTcpClient").toBool();
    App::DebugTcpClient = set.value("DebugTcpClient").toBool();
    App::AutoSendTcpClient = set.value("AutoSendTcpClient").toBool();
    App::IntervalTcpClient = set.value("IntervalTcpClient").toInt();
    App::TcpServerIP = set.value("TcpServerIP").toString();
    App::TcpServerPort = set.value("TcpServerPort").toInt();
    set.endGroup();
    set.beginGroup("UdpServerConfig");
    App::HexSendUdpServer = set.value("HexSendUdpServer").toBool();
    App::HexReceiveUdpServer = set.value("HexReceiveUdpServer").toBool();
    App::DebugUdpServer = set.value("DebugUdpServer").toBool();
    App::AutoSendUdpServer = set.value("AutoSendUdpServer").toBool();
    App::IntervalUdpServer = set.value("IntervalUdpServer").toInt();
    App::UdpListenPort = set.value("UdpListenPort").toInt();
    App::UdpClientIP = set.value("UdpClientIP").toString();
    App::UdpClientPort = set.value("UdpClientPort").toInt();
    set.endGroup();
}

void App::WriteConfig()
{
    QString fileName = "Tomocfg.ini";// QString("%1/%2_Config.ini").arg(AppPath).arg(AppName);
    QSettings set(fileName, QSettings::IniFormat);
    set.beginGroup("AppConfig");
    set.setValue("LeftPanelWidth", App::LeftPanelWidth);
    set.setValue("RightPanelWidth", App::RightPanelWidth);
    set.setValue("PlatForm", App::PlatForm);
    set.setValue("SendFileName", App::SendFileName);
    set.setValue("DeviceFileName", App::DeviceFileName);
    set.endGroup();
    set.beginGroup("TcpServerConfig");
    set.setValue("HexSendTcpServer", App::HexSendTcpServer);
    set.setValue("HexReceiveTcpServer", App::HexReceiveTcpServer);
    set.setValue("DebugTcpServer", App::DebugTcpServer);
    set.setValue("AutoSendTcpServer", App::AutoSendTcpServer);
    set.setValue("IntervalTcpServer", App::IntervalTcpServer);
    set.setValue("TcpListenPort", App::TcpListenPort);
    set.endGroup();
    set.beginGroup("TcpClientConfig");
    set.setValue("HexSendTcpClient", App::HexSendTcpClient);
    set.setValue("HexReceiveTcpClient", App::HexReceiveTcpClient);
    set.setValue("DebugTcpClient", App::DebugTcpClient);
    set.setValue("AutoSendTcpClient", App::AutoSendTcpClient);
    set.setValue("IntervalTcpClient", App::IntervalTcpClient);
    set.setValue("TcpServerIP", App::TcpServerIP);
    set.setValue("TcpServerPort", App::TcpServerPort);
    set.endGroup();
    set.beginGroup("UdpServerConfig");
    set.setValue("HexSendUdpServer", App::HexSendUdpServer);
    set.setValue("HexReceiveUdpServer", App::HexReceiveUdpServer);
    set.setValue("DebugUdpServer", App::DebugUdpServer);
    set.setValue("AutoSendUdpServer", App::AutoSendUdpServer);
    set.setValue("IntervalUdpServer", App::IntervalUdpServer);
    set.setValue("UdpListenPort", App::UdpListenPort);
    set.setValue("UdpClientIP", App::UdpClientIP);
    set.setValue("UdpClientPort", App::UdpClientPort);
    set.endGroup();
}

void App::NewConfig()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#endif
    WriteConfig();
}

bool App::CheckConfig()
{
    QString fileName = QString("%1/%2_Config.ini").arg(AppPath).arg(AppName);
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(fileName);
    if (file.size() == 0)
    {
        NewConfig();
        return false;
    }
    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly))
    {
        bool ok = true;
        while (!file.atEnd())
        {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");
            if (list.count() == 2)
            {
                if (list.at(1) == "")
                {
                    ok = false;
                    break;
                }
            }
        }
        if (!ok)
        {
            NewConfig();
            return false;
        }
    }
    else
    {
        NewConfig();
        return false;
    }
    return true;
}

void App::WriteError(QString str)
{
    QString fileName = QString("%1/%2_Error_%3.txt").arg(AppPath).arg(AppName).arg(QDATE);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream stream(&file);
    stream << DATETIME << "  " << str << "\n";
}

void App::NewDir(QString dirName)
{
    //如果路径中包含斜杠字符则说明是绝对路径
    //linux系统路径字符带有 /  windows系统 路径字符带有 :/
    if (!dirName.startsWith("/") && !dirName.contains(":/"))
        dirName = QString("%1/%2").arg(AppPath).arg(dirName);
    QDir dir(dirName);
    if (!dir.exists())
        dir.mkpath(dirName);
}


frmNetTool::frmNetTool(QWidget* parent) : QWidget(parent)
    , ui(new Ui::frmNetTool)
{
    ui->setupUi(this);
    this->initConfig();
    this->initForm();
    this->readSendData();
    this->readDeviceData();
    CUiTool::Instance()->formInCenter(this);
}

frmNetTool::~frmNetTool()
{
    delete ui;
}

void frmNetTool::initForm()
{
    msgMaxCount = 50;
    countTcpServer = 0;
    countTcpClient = 0;
    countUdpServer = 0;
    QList<QHostAddress> addrs = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, addrs)
    {
        QString ip = addr.toString();
        if (ip.startsWith("192.168"))
        {
            this->setWindowTitle(TCSR("网络调试助手 本机IP[%1]").arg(ip));
            break;
        }
    }
    ui->btnSendTcpClient->setEnabled(false);
    ui->txtDataTcpClient->setReadOnly(true);
    ui->btnSendTcpServer->setEnabled(false);
    ui->txtDataTcpServer->setReadOnly(true);
    ui->btnSendUdpServer->setEnabled(false);
    ui->txtDataUdpServer->setReadOnly(true);
    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(tcpClientReadData()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpClientReadError()));
    tcpServer = new TcpServer(this);
    connect(tcpServer, SIGNAL(clientConnect(int, QString, int)),
            this, SLOT(clientConnect(int, QString, int)));
    connect(tcpServer, SIGNAL(clientDisConnect(int, QString, int)),
            this, SLOT(clientDisConnect(int, QString, int)));
    connect(tcpServer, SIGNAL(clientReadData(int, QString, int, QByteArray)),
            this, SLOT(clientReadData(int, QString, int, QByteArray)));
    udpServer = new QUdpSocket(this);
    udpServer->abort();
    connect(udpServer, SIGNAL(readyRead()), this, SLOT(udpServerReadData()));
    connect(udpServer, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(udpServerReadError()));
    timerTcpClient = new QTimer(this);
    timerTcpClient->setInterval(App::IntervalTcpClient);
    connect(timerTcpClient, SIGNAL(timeout()), this, SLOT(sendDataTcpClient()));
    timerTcpServer = new QTimer(this);
    timerTcpServer->setInterval(App::IntervalTcpServer);
    connect(timerTcpServer, SIGNAL(timeout()), this, SLOT(sendDataTcpServer()));
    timerUdpServer = new QTimer(this);
    timerUdpServer->setInterval(App::IntervalUdpServer);
    connect(timerUdpServer, SIGNAL(timeout()), this, SLOT(sendDataUdpServer()));
    if (App::AutoSendTcpClient)
        timerTcpClient->start();
    if (App::AutoSendTcpServer)
        timerTcpServer->start();
    if (App::AutoSendUdpServer)
        timerUdpServer->start();
    connect(ui->btnSendTcpServer, SIGNAL(clicked()), this, SLOT(sendDataTcpServer()));
    connect(ui->btnSendTcpClient, SIGNAL(clicked()), this, SLOT(sendDataTcpClient()));
    connect(ui->btnSendUdpServer, SIGNAL(clicked()), this, SLOT(sendDataUdpServer()));
}

void frmNetTool::initConfig()
{
    App::ReadConfig();
    QStringList interval;
    interval << "50" << "100" << "200" << "300";
    for (int i = 500; i < 10000; i = i + 500)
        interval << QString("%1").arg(i);
    ui->cboxIntervalTcpClient->addItems(interval);
    ui->cboxIntervalTcpServer->addItems(interval);
    ui->cboxIntervalUdpServer->addItems(interval);
    //ui->frmTcpServer->setMinimumWidth(App::RightPanelWidth);
    //ui->frmTcpServer->setMaximumWidth(App::RightPanelWidth);
    //ui->frmTcpClient->setMinimumWidth(App::RightPanelWidth);
    //ui->frmTcpClient->setMaximumWidth(App::RightPanelWidth);
    //ui->frmUdpServer->setMinimumWidth(App::RightPanelWidth);
    //ui->frmUdpServer->setMaximumWidth(App::RightPanelWidth);
    //Tcp server部分
    ui->ckHexSendTcpServer->setChecked(App::HexSendTcpServer);
    connect(ui->ckHexSendTcpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckHexReceiveTcpServer->setChecked(App::HexReceiveTcpServer);
    connect(ui->ckHexReceiveTcpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckDebugTcpServer->setChecked(App::DebugTcpServer);
    connect(ui->ckDebugTcpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckAutoSendTcpServer->setChecked(App::AutoSendTcpServer);
    connect(ui->ckAutoSendTcpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->cboxIntervalTcpServer->setEnabled(App::AutoSendTcpServer);
    ui->cboxIntervalTcpServer->setCurrentIndex(ui->cboxIntervalTcpServer->findText(QString::number(App::IntervalTcpServer)));
    connect(ui->cboxIntervalTcpServer, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));
    ui->txtTcpListenPort->setText(QString::number(App::TcpListenPort));
    connect(ui->txtTcpListenPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    //Tcp client部分
    ui->ckHexSendTcpClient->setChecked(App::HexSendTcpClient);
    connect(ui->ckHexSendTcpClient, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckHexReceiveTcpClient->setChecked(App::HexReceiveTcpClient);
    connect(ui->ckHexReceiveTcpClient, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckDebugTcpClient->setChecked(App::DebugTcpClient);
    connect(ui->ckDebugTcpClient, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckAutoSendTcpClient->setChecked(App::AutoSendTcpClient);
    connect(ui->ckAutoSendTcpClient, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->cboxIntervalTcpClient->setEnabled(App::AutoSendTcpClient);
    ui->cboxIntervalTcpClient->setCurrentIndex(ui->cboxIntervalTcpClient->findText(QString::number(App::IntervalTcpClient)));
    connect(ui->cboxIntervalTcpClient, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));
    ui->txtTcpServerIP->setText(App::TcpServerIP);
    connect(ui->txtTcpServerIP, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    ui->txtTcpServerPort->setText(QString::number(App::TcpServerPort));
    connect(ui->txtTcpServerPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    //Udp server部分
    ui->ckHexSendUdpServer->setChecked(App::HexSendUdpServer);
    connect(ui->ckHexSendUdpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckHexReceiveUdpServer->setChecked(App::HexReceiveUdpServer);
    connect(ui->ckHexReceiveUdpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckDebugUdpServer->setChecked(App::DebugUdpServer);
    connect(ui->ckDebugUdpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->ckAutoSendUdpServer->setChecked(App::AutoSendUdpServer);
    connect(ui->ckAutoSendUdpServer, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
    ui->cboxIntervalUdpServer->setEnabled(App::AutoSendUdpServer);
    ui->cboxIntervalUdpServer->setCurrentIndex(ui->cboxIntervalUdpServer->findText(QString::number(App::IntervalUdpServer)));
    connect(ui->cboxIntervalUdpServer, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));
    ui->txtUdpListenPort->setText(QString::number(App::UdpListenPort));
    connect(ui->txtUdpListenPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    ui->txtUdpClientIP->setText(App::UdpClientIP);
    connect(ui->txtUdpClientIP, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    ui->txtUdpClientPort->setText(QString::number(App::UdpClientPort));
    connect(ui->txtUdpClientPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmNetTool::saveConfig()
{
    App::HexSendTcpServer = ui->ckHexSendTcpServer->isChecked();
    App::HexReceiveTcpServer = ui->ckHexReceiveTcpServer->isChecked();
    App::DebugTcpServer = ui->ckDebugTcpServer->isChecked();
    bool autoSendTcpServer = ui->ckAutoSendTcpServer->isChecked();
    if (autoSendTcpServer != App::AutoSendTcpServer)
    {
        App::AutoSendTcpServer = autoSendTcpServer;
        ui->cboxIntervalTcpServer->setEnabled(App::AutoSendTcpServer);
        if (App::AutoSendTcpServer)
            timerTcpServer->start();
        else
            timerTcpServer->stop();
    }
    App::IntervalTcpServer = ui->cboxIntervalTcpServer->currentText().toInt();
    timerTcpServer->setInterval(App::IntervalTcpServer);
    App::TcpListenPort = ui->txtTcpListenPort->text().toInt();
    App::HexSendTcpClient = ui->ckHexSendTcpClient->isChecked();
    App::HexReceiveTcpClient = ui->ckHexReceiveTcpClient->isChecked();
    App::DebugTcpClient = ui->ckDebugTcpClient->isChecked();
    bool autoSendTcpClient = ui->ckAutoSendTcpClient->isChecked();
    if (autoSendTcpClient != App::AutoSendTcpClient)
    {
        App::AutoSendTcpClient = autoSendTcpClient;
        ui->cboxIntervalTcpClient->setEnabled(App::AutoSendTcpClient);
        if (App::AutoSendTcpClient)
            timerTcpClient->start();
        else
            timerTcpClient->stop();
    }
    App::IntervalTcpClient = ui->cboxIntervalTcpClient->currentText().toInt();
    timerTcpClient->setInterval(App::IntervalTcpClient);
    App::TcpServerIP = ui->txtTcpServerIP->text();
    App::TcpServerPort = ui->txtTcpListenPort->text().toInt();
    App::HexSendUdpServer = ui->ckHexSendUdpServer->isChecked();
    App::HexReceiveUdpServer = ui->ckHexReceiveUdpServer->isChecked();
    App::DebugUdpServer = ui->ckDebugUdpServer->isChecked();
    bool autoSendUdpServer = ui->ckAutoSendUdpServer->isChecked();
    if (autoSendUdpServer != App::AutoSendUdpServer)
    {
        App::AutoSendUdpServer = autoSendUdpServer;
        ui->cboxIntervalUdpServer->setEnabled(App::AutoSendUdpServer);
        if (App::AutoSendUdpServer)
            timerUdpServer->start();
        else
            timerUdpServer->stop();
    }
    App::IntervalUdpServer = ui->cboxIntervalUdpServer->currentText().toInt();
    timerUdpServer->setInterval(App::IntervalUdpServer);
    App::UdpListenPort = ui->txtUdpListenPort->text().toInt();
    App::UdpClientIP = ui->txtUdpClientIP->text();
    App::UdpClientPort = ui->txtUdpClientPort->text().toInt();
    App::WriteConfig();
}

void frmNetTool::appendTcpClient(quint8 type, QString msg)
{
    if (countTcpClient > msgMaxCount)
    {
        ui->txtDataTcpClient->clear();
        countTcpClient = 0;
    }
    QString str;
    if (type == 0)
    {
        str = TCSR(">> 发送 :");
        ui->txtDataTcpClient->setTextColor(QColor("dodgerblue"));
    }
    else if (type == 1)
    {
        str = TCSR("<< 接收 :");
        ui->txtDataTcpClient->setTextColor(QColor("red"));
    }
    ui->txtDataTcpClient->append(TCSR("时间[%1] %2 %3").arg(TIMEMS).arg(str).arg(msg));
    countTcpClient++;
}

void frmNetTool::appendTcpServer(quint8 type, QString msg)
{
    if (countTcpServer > msgMaxCount)
    {
        ui->txtDataTcpServer->clear();
        countTcpServer = 0;
    }
    QString str;
    if (type == 0)
    {
        str = TCSR(">> 发送 :");
        ui->txtDataTcpServer->setTextColor(QColor("dodgerblue"));
    }
    else if (type == 1)
    {
        str = TCSR("<< 接收 :");
        ui->txtDataTcpServer->setTextColor(QColor("red"));
    }
    ui->txtDataTcpServer->append(TCSR("时间[%1] %2 %3").arg(TIMEMS).arg(str).arg(msg));
    countTcpServer++;
}

void frmNetTool::sendDataUdpServer()
{
    QString data = ui->cboxSendUdpServer->currentText();
    sendDataUdpServer(data);
}

void frmNetTool::sendDataUdpServer(QString data)
{
    QString ip = ui->txtUdpClientIP->text();
    int port = ui->txtUdpClientPort->text().toInt();
    sendDataUdpServer(ip, port, data);
}

void frmNetTool::sendDataUdpServer(QString ip, int port, QString data)
{
    if (!data.isEmpty())
    {
        QByteArray buffer;
        if (App::HexSendUdpServer)
            buffer = CCastType::Instance()->hexStrToByteArray(data);
        else
            buffer = CCastType::Instance()->asciiStrToByteArray(data);
        udpServer->writeDatagram(buffer, QHostAddress(ip), port);
        QString str = TCSR("目标:%1[%2] ").arg(ip).arg(port);
        appendUdpServer(0, str + data);
    }
}

void frmNetTool::appendUdpServer(quint8 type, QString msg)
{
    if (countUdpServer > msgMaxCount)
    {
        ui->txtDataUdpServer->clear();
        countUdpServer = 0;
    }
    QString str;
    if (type == 0)
    {
        str = TCSR(">> 发送 :");
        ui->txtDataUdpServer->setTextColor(QColor("dodgerblue"));
    }
    else if (type == 1)
    {
        str = TCSR("<< 接收 :");
        ui->txtDataUdpServer->setTextColor(QColor("red"));
    }
    ui->txtDataUdpServer->append(TCSR("时间[%1] %2 %3").arg(TIMEMS).arg(str).arg(msg));
    countUdpServer++;
}

void frmNetTool::tcpClientReadData()
{
    if (tcpClient->bytesAvailable() <= 0)
        return;
    QByteArray data = tcpClient->readAll();
    QString buffer;
    if (App::HexReceiveTcpClient)
        buffer = CCastType::Instance()->byteArrayToHexStr(data);
    else
        buffer = CCastType::Instance()->byteArrayToAsciiStr(data);
    appendTcpClient(1, buffer);
    if (App::DebugTcpClient)
    {
        int count = keys.count();
        for (int i = 0; i < count; i++)
        {
            if (keys.at(i) == buffer)
            {
                sendDataTcpClient(values.at(i));
                break;
            }
        }
    }
}

void frmNetTool::tcpClientReadError()
{
    tcpClient->disconnectFromHost();
    ui->btnTcpConnect->setText(TCSR("连接"));
    ui->btnSendTcpClient->setEnabled(false);
    ui->cboxSendTcpClient->lineEdit()->clear();
    appendTcpClient(1, TCSR("连接服务器失败,原因 : %1").arg(tcpClient->errorString()));
}

void frmNetTool::clientReadData(int , QString ip, int port, QByteArray data)
{
    QString buffer;
    if (App::HexReceiveTcpServer)
        buffer = CCastType::Instance()->byteArrayToHexStr(data);
    else
        buffer = CCastType::Instance()->byteArrayToAsciiStr(data);
    appendTcpServer(1, QString("%1 [%2:%3]").arg(buffer).arg(ip).arg(port));
    if (App::DebugTcpServer)
    {
        int count = keys.count();
        for (int i = 0; i < count; i++)
        {
            if (keys.at(i) == buffer)
            {
                sendDataTcpServer(ip, port, values.at(i));
                break;
            }
        }
    }
}

void frmNetTool::clientConnect(int , QString ip, int port)
{
    appendTcpServer(1, TCSR("客户端[%1:%2] 上线").arg(ip).arg(port));
    ui->listTcpClient->clear();
    ui->listTcpClient->addItems(tcpServer->getClientInfo());
    ui->labTcpClientCount->setText(TCSR("已连接客户端共 %1 个").arg(tcpServer->getClientCount()));
    int count = ui->listTcpClient->count();
    if (count > 0)
        ui->listTcpClient->setCurrentRow(count - 1);
}

void frmNetTool::clientDisConnect(int , QString ip, int port)
{
    appendTcpServer(1, TCSR("客户端[%1:%2] 下线").arg(ip).arg(port));
    ui->listTcpClient->clear();
    ui->listTcpClient->addItems(tcpServer->getClientInfo());
    ui->labTcpClientCount->setText(TCSR("已连接客户端共 %1 个").arg(tcpServer->getClientCount()));
    int count = ui->listTcpClient->count();
    if (count > 0)
        ui->listTcpClient->setCurrentRow(count - 1);
}

void frmNetTool::udpServerReadData()
{
    QHostAddress senderServerIP;
    quint16 senderServerPort;
    QByteArray data;
    QString buffer;
    do
    {
        data.resize(udpServer->pendingDatagramSize());
        udpServer->readDatagram(data.data(), data.size(), &senderServerIP, &senderServerPort);
        if (App::HexReceiveUdpServer)
            buffer = CCastType::Instance()->byteArrayToHexStr(data);
        else
            buffer = CCastType::Instance()->byteArrayToAsciiStr(data);
        QString str = TCSR("来自:%1[%2] ").arg(senderServerIP.toString()).arg(senderServerPort);
        appendUdpServer(1, str + buffer);
        if (App::DebugUdpServer)
        {
            int count = keys.count();
            for (int i = 0; i < count; i++)
            {
                if (keys.at(i) == buffer)
                {
                    sendDataUdpServer(values.at(i));
                    break;
                }
            }
        }
    }
    while (udpServer->hasPendingDatagrams());
}

void frmNetTool::udpServerReadError()
{
    appendUdpServer(1, TCSR("发生错误,原因 : %1").arg(udpServer->errorString()));
}

void frmNetTool::readSendData()
{
    QString fileName = QString("%1/%2").arg(AppPath).arg(App::SendFileName);
    QFile file(fileName);
    if (!file.exists())
        return;
    ui->cboxSendTcpClient->clear();
    ui->cboxSendTcpServer->clear();
    ui->cboxSendUdpServer->clear();
    file.open(QFile::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line;
    do
    {
        line = in.readLine();
        if (line != "")
        {
            ui->cboxSendTcpClient->addItem(line);
            ui->cboxSendTcpServer->addItem(line);
            ui->cboxSendUdpServer->addItem(line);
        }
    }
    while (!line.isNull());
    file.close();
}

void frmNetTool::readDeviceData()
{
    QString fileName = QString("%1/%2").arg(AppPath).arg(App::DeviceFileName);
    QFile file(fileName);
    if (!file.exists())
        return;
    file.open(QFile::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line;
    do
    {
        line = in.readLine();
        if (line != "")
        {
            QStringList list = line.split(";");
            QString key = list.at(0);
            QString value;
            for (int i = 1; i < list.count(); i++)
                value += QString("%1;").arg(list.at(i));
            //去掉末尾分号
            value = value.mid(0, value.length() - 1);
            keys.append(key);
            values.append(value);
        }
    }
    while (!line.isNull());
    file.close();
}

void frmNetTool::sendDataTcpServer()
{
    QString data = ui->cboxSendTcpServer->currentText();
    sendDataTcpServer(data);
}

void frmNetTool::sendDataTcpServer(QString data)
{
    if (!tcpServer->isListening())
        return;
    if (data.isEmpty())
        return;
    bool all = ui->ckAllTcpServer->isChecked();
    QString str = ui->listTcpClient->currentIndex().data().toString();
    //没有一个连接则不用处理
    if (str.isEmpty())
        return;
    QStringList list = str.split(":");
    QString ip = list.at(0);
    int port = list.at(1).toInt();
    QByteArray buffer;
    if (App::HexSendTcpServer)
        buffer = CCastType::Instance()->hexStrToByteArray(data);
    else
        buffer = CCastType::Instance()->asciiStrToByteArray(data);
    if (!all)
        tcpServer->sendData(ip, port, buffer);
    else
        tcpServer->sendData(buffer);
    appendTcpServer(0, data);
}

void frmNetTool::sendDataTcpServer(QString ip, int port, QString data)
{
    if (!tcpServer->isListening())
        return;
    QByteArray buffer;
    if (App::HexSendTcpServer)
        buffer = CCastType::Instance()->hexStrToByteArray(data);
    else
        buffer = CCastType::Instance()->asciiStrToByteArray(data);
    tcpServer->sendData(ip, port, buffer);
    appendTcpServer(0, data);
}

void frmNetTool::sendDataTcpClient()
{
    QString data = ui->cboxSendTcpClient->currentText();
    sendDataTcpClient(data);
}

void frmNetTool::sendDataTcpClient(QString data)
{
    if (!tcpClient->isWritable())
        return;
    if (data.isEmpty())
        return;
    if (!ui->btnSendTcpClient->isEnabled())
        return;
    QByteArray buffer;
    if (App::HexSendTcpClient)
        buffer = CCastType::Instance()->hexStrToByteArray(data);
    else
        buffer = CCastType::Instance()->asciiStrToByteArray(data);
    tcpClient->write(buffer);
    appendTcpClient(0, data);
}

void frmNetTool::on_btnTcpListen_clicked()
{
    if (ui->btnTcpListen->text() == TCSR("监听"))
    {
        bool ok = tcpServer->listen(QHostAddress::Any, App::TcpServerPort);
        if (ok)
        {
            ui->btnTcpListen->setText(TCSR("停止"));
            ui->btnSendTcpServer->setEnabled(true);
            appendTcpServer(0, TCSR("监听成功"));
        }
        else
            appendTcpServer(1, TCSR("监听失败,请检查端口是否被占用"));
    }
    else
    {
        ui->listTcpClient->clear();
        tcpServer->closeAll();
        ui->btnTcpListen->setText(TCSR("监听"));
        ui->btnSendTcpServer->setEnabled(false);
        appendTcpServer(0, TCSR("停止监听成功"));
    }
}

void frmNetTool::on_btnClearTcpServer_clicked()
{
    ui->txtDataTcpServer->clear();
    countTcpServer = 0;
}

void frmNetTool::on_btnTcpConnect_clicked()
{
    if (ui->btnTcpConnect->text() == TCSR("连接"))
    {
        tcpClient->connectToHost(App::TcpServerIP, App::TcpServerPort);
        if (tcpClient->waitForConnected(1000))
        {
            ui->btnTcpConnect->setText(TCSR("断开"));
            ui->btnSendTcpClient->setEnabled(true);
            appendTcpClient(0, TCSR("连接服务器成功"));
        }
        else
            appendTcpClient(1, TCSR("连接服务器失败"));
    }
    else
    {
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000))
        {
            ui->btnTcpConnect->setText(TCSR("连接"));
            ui->btnSendTcpClient->setEnabled(false);
            appendTcpClient(0, TCSR("断开连接成功"));
        }
    }
}

void frmNetTool::on_btnClearTcpClient_clicked()
{
    ui->txtDataTcpClient->clear();
    countTcpClient = 0;
}

void frmNetTool::on_btnUdpListen_clicked()
{
    if (ui->btnUdpListen->text() == TCSR("监听"))
    {
        bool ok = udpServer->bind(App::UdpListenPort);
        if (ok)
        {
            ui->btnUdpListen->setText(TCSR("停止"));
            ui->btnSendUdpServer->setEnabled(true);
            appendUdpServer(0, TCSR("监听成功"));
        }
        else
            appendUdpServer(1, TCSR("监听失败,请检查端口是否被占用"));
    }
    else
    {
        udpServer->abort();
        ui->btnUdpListen->setText(TCSR("监听"));
        ui->btnSendUdpServer->setEnabled(false);
        appendUdpServer(0, TCSR("停止监听成功"));
    }
}

void frmNetTool::on_btnClearUdpServer_clicked()
{
    ui->txtDataUdpServer->clear();
    countUdpServer = 0;
}
