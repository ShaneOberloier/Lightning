//This .cpp file is responsible for both sending and receiving G-Code Commands via Serial.

//Headers
#include "gcodeprocess.h"
#include "gcodedefinitions.h" //Defines Language Specific G-Code terms

//Dependancies
#include <QString> //To utilize all the features for the QString Datatype
#include <QtSerialPort/QSerialPort> //To handle Serial Communication
#include <QDebug> //To view Debug Messages in QtCreator Terminal
#include <QTimer> //For periodically occuring events



//Public Entities
QSerialPort Serial;
bool MachineReady = true;
QTimer *ReceiveTimer;
QTimer *QueueTimer;
QStringList GCodeQueue;
int GCodeQueueCurrentLine=0;

GCodeProcess::GCodeProcess(QObject *parent) : QObject(parent)
{
    ///////////////////////////////////////////////////////////////
    ////This is the constructor function
    ////It runs whenever a new GCodeProcess is instantiated
    ///////////////////////////////////////////////////////////////

    //Make connections for send and receive timer
    ReceiveTimer = new QTimer(this);
    QObject::connect(ReceiveTimer, SIGNAL(timeout()), this, SLOT(Receive()));

    QueueTimer = new QTimer(this);
    QObject::connect(QueueTimer, SIGNAL(timeout()), this, SLOT(ServiceQueue()));
}

bool GCodeProcess::InitializeCommunication(QString PortName, qint32 BaudRate, QSerialPort::DataBits DataBits, QSerialPort::Parity Parity, QSerialPort::StopBits StopBits, QSerialPort::FlowControl FlowControl)
{
    ///////////////////////////////////////////////////////////////
    ////This function begins the serial communication with the device
    ////And also begins the Receive Handler
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

            //Immediatly start receiving data from machine
            ReceiveTimer->start(RxFreq);


            return 1; //Indicate that the serial port was connected to
        }
        else
        {
            qDebug() << "Serial Connection Failed";
            return 0; //Indicate that the serial port was not connected to
        }
    }
}

void GCodeProcess::Send(QString Command)
{
    ///////////////////////////////////////////////////////////////
    ////This function is responsible for sending GCode Commands
    ////It assumes that it is always okay to send a command
    ////This command should not be used directly - instead use QueueGCode()
    ///////////////////////////////////////////////////////////////

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

void GCodeProcess::QueueGCode(QString Command)
{
    GCodeQueue.append(Command);//Add Command to end of the Queue
    QueueTimer->start(TxFreq);//Start Servicing the queue
}

void GCodeProcess::ServiceQueue()
{
    ///////////////////////////////////////////////////////////////
    ////This function is a slot which is periodically called by a timer
    ////If the machine is ready, it will send the next GCode Command in queue
    ///////////////////////////////////////////////////////////////

    //If the Queue is empty - no reason to keep running the timer.
    if (GCodeQueue.isEmpty())
    {
        QueueTimer->stop();
        return;
    }
    //If the machine is ready, send the command
    if (MachineReady)
    {
        //Continue Here -- Send top GCode command, and then clear it.
    }

}

void GCodeProcess::Receive()
{
    ///////////////////////////////////////////////////////////////
    ////This function is a slot which is periodically called by a timer
    ////It reads & documents received messages
    ///////////////////////////////////////////////////////////////

    QString data=""; // Create a string to receive the incoming message
    if(Serial.canReadLine()) //See if there is serial data available on the port
    {
        data = Serial.readLine(); //Read the data
        data.remove("\n"); //Remove new line character for formatting reasons
        if (data!="") //Make Sure the data isn't blank
        {
            qDebug() << data;//Report the Received Data

            //Check and see if this is an acknowledgement statement - if indicate the machine is ready
            if (data==GCodeAcknowledge){
                MachineReady=true;
                return;
            }
        }
    }
}
