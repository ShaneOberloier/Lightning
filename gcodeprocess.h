#ifndef GCODEPROCESS_H
#define GCODEPROCESS_H

//Dependancies
#include <QtSerialPort/QSerialPort> //To handle Serial Communication
#include <QTimer> //For periodically occuring events
#include <QObject> //For Object Oriented Things

//Program Constants & Parameters
#define RxFreq 1 //How often to check for new received messages (in ms)

class GCodeProcess : public QObject
{
    Q_OBJECT
public:
    explicit GCodeProcess(QObject *parent = nullptr);
    bool InitializeCommunication(QString PortName, qint32 BaudRate, QSerialPort::DataBits DataBits = QSerialPort::Data8, QSerialPort::Parity Parity = QSerialPort::NoParity, QSerialPort::StopBits StopBits = QSerialPort::OneStop, QSerialPort::FlowControl FlowControl = QSerialPort::NoFlowControl);
    void Send(QString Command);
    void QueueGCode(QString Command);

signals:

private slots:
    void Receive();
    void ServiceQueue();
};

#endif // GCODEPROCESS_H
