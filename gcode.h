#ifndef GCODE_H
#define GCODE_H



//Dependancies
#include <QtSerialPort/QSerialPort> //To handle Serial Communication
#include <QTimer> //For periodically occuring events


class GCodeLib
{
    public:
        bool InitializeCommunication(QString PortName, qint32 BaudRate, QSerialPort::DataBits DataBits = QSerialPort::Data8, QSerialPort::Parity Parity = QSerialPort::NoParity, QSerialPort::StopBits StopBits = QSerialPort::OneStop, QSerialPort::FlowControl FlowControl = QSerialPort::NoFlowControl);
        void Send(QString Command);
};
#endif // GCODE_H
