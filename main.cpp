#include <QApplication>
#include "client.h"
#include "Server.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Crear e iniciar el servidor
    Server server;
    if (!server.listen(QHostAddress::Any, 1234)) {
        qDebug() << "Error al iniciar el servidor en el puerto 1234.";
        return -1;
    }
    qDebug() << "Servidor iniciado en el puerto 1234.";

    // Crear e iniciar el cliente
    Client client("localhost", 1234);
    client.show();

    return a.exec();
}
