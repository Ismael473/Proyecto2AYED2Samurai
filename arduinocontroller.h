#ifndef ARDUINOCONTROLLER_H
#define ARDUINOCONTROLLER_H

#include <QObject>
#include <QSerialPort>

/**
 * @brief Clase para controlar la comunicación entre la aplicación y Arduino vía puerto serial.
 * Utiliza el patrón Singleton para garantizar una única instancia de esta clase.
 */
class ArduinoController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Obtiene la única instancia de ArduinoController.
     * @return Referencia a la única instancia de ArduinoController.
     */
    static ArduinoController& instance();

    // Elimina la capacidad de copiar y asignar.
    ArduinoController(ArduinoController const&) = delete;
    void operator=(ArduinoController const&) = delete;

    /**
     * @brief Intenta conectarse a Arduino vía puerto serial.
     * @return Verdadero si la conexión fue exitosa, falso en caso contrario.
     */
    bool connectToArduino();

    /**
     * @brief Envía el comando para incrementar un número al Arduino.
     */
    void sendIncrementCommand();

    /**
     * @brief Envía el comando para hacer un sonido al Arduino.
     */
    void sendSoundCommand();

    /**
     * @brief Desconecta del puerto serial.
     */
    void disconnectFromArduino();

signals:
    /**
     * @brief Señal emitida cuando se reciben datos del Arduino.
     * @param data Datos recibidos del Arduino.
     */
    void dataReceived(const QByteArray &data);

private slots:
    void onDataReceived();

private:
    // Constructor y destructor privados para implementar el patrón Singleton.
    ArduinoController(QObject *parent = nullptr);
    ~ArduinoController();

    QSerialPort serial;  ///< Puerto serial para comunicarse con Arduino.
};

#endif // ARDUINOCONTROLLER_H
