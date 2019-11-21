//This .cpp file is responsible for both sending and receiving G-Code Commands via Serial.

//Headers
#include "gcode.h"

//Dependancies
#include <QString> //To utilize all the features for the QString Datatype
#include <QtSerialPort/QSerialPort> //To handle Serial Communication
#include <QDebug> //To view Debug Messages in QtCreator Terminal
#include <QTimer> //For periodically occuring events



//Public Entities
QSerialPort Serial;
bool MachineReady = true;
QTimer ReceiveTimer;

bool GCodeLib::InitializeCommunication(QString PortName, qint32 BaudRate, QSerialPort::DataBits DataBits, QSerialPort::Parity Parity, QSerialPort::StopBits StopBits, QSerialPort::FlowControl FlowControl)
{
    ///////////////////////////////////////////////////////////////
    //This function begins the serial communication with the device
    //And also begins the Receive Handler
    ///////////////////////////////////////////////////////////////

    if(Serial.isOpen())
    {
        qDebug() << "A serial is already open";
        return 0; //Indicate that the serial port was not connected to
    }
    else
    {
        //Define Port Properties
        Serial.setPortName(PortName);
        Serial.setBaudRate(BaudRate);
        Serial.setDataBits(DataBits);
        Serial.setParity(Parity);
        Serial.setStopBits(StopBits);
        Serial.setFlowControl(FlowControl);

        //Now Attempt to Connect to the Port
        if(Serial.open(QIODevice::ReadWrite))
        {
            qDebug() << "Serial Connection Successful";

/*            //Begin the Received Data Handler
            ReceiveTimer = new QTimer(this);
            //setup signal and slot
            connect(ReceiveTimer, SIGNAL(timeout()),
                  this, SLOT(Receive())); */ // Continue Here.

            return 1; //Indicate that the serial port was connected to
        }
        else
        {
            qDebug() << "Serial Connection Failed";
            return 0; //Indicate that the serial port was not connected to
        }
    }
}

void GCodeLib::Send(QString Command)
{
    QByteArray CommandBytes = Command.toLocal8Bit(); //Convert the string to a usable format
    if(Serial.isOpen()){
        Serial.write(CommandBytes + "\n"); //Send command & append new line characer
        qDebug() << "Sent: " << Command;
        MachineReady = false; //Assert that since a command is being sent, the machine is not ready till it acknowledges
    }
    else
    {
        qDebug() << "Cannot send command since serial port is closed";
    }
}

void Receive()
{

}
