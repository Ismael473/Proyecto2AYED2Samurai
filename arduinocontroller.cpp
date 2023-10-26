#include "arduinocontroller.h"

ArduinoController& ArduinoController::instance() {
    static ArduinoController instance;  // Instancia única.
    return instance;
}

ArduinoController::ArduinoController(QObject *parent)
    : QObject(parent)
{
    connectToArduino();
}

ArduinoController::~ArduinoController()
{
    if (serial.isOpen()) {
        serial.close();
    }
}

bool ArduinoController::connectToArduino()
{
    serial.setPortName("COM3");  // Establece directamente el nombre del puerto aquí.
    serial.setBaudRate(QSerialPort::Baud9600);

    if (!serial.open(QIODevice::ReadWrite)) {
        return false;  // Falló la conexión.
    }

    connect(&serial, &QSerialPort::readyRead, this, &ArduinoController::onDataReceived);
    return true;
}

void ArduinoController::sendIncrementCommand()
{
    QString command = "incrementNumber\n";
    serial.write(command.toLocal8Bit());
}

void ArduinoController::sendSoundCommand()
{
    QString command = "soundDmg\n";
    serial.write(command.toLocal8Bit());
}

void ArduinoController::onDataReceived()
{
    // Aquí puedes manejar la data que es recibida del Arduino si es necesario.
    QByteArray data = serial.readAll();
    // Emite la señal con los datos recibidos, en caso de que otras partes de tu programa quieran escucharla.
    emit dataReceived(data);
    // Procesa data si es necesario.
}

void ArduinoController::disconnectFromArduino()
{
    if (serial.isOpen()) {
        serial.close();
    }
}


