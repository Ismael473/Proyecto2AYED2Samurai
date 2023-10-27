#include "server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include "arduinocontroller.h"
// Definimos una estructura para el alcance de cada obstáculo
struct ObstacleInfo {
    int range;
};

const std::map<int, ObstacleInfo> obstacleData = {
    {1, {1}},  // Yari
    {2, {2}},  // Arco y flechas
    {3, {3}}   // Tanegashima
};

Server::Server(QObject *parent)
    : QTcpServer(parent), clientSocket(nullptr), koban(30), gameStarted(false) {
    gameMatrix = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));

    // Configurar el timer para iniciar el juego después de 10 segundos
    //QTimer *gameStartTimer = new QTimer(this);
    //connect(gameStartTimer, &QTimer::timeout, this, &Server::startGame);
    //gameStartTimer->start(10000);  // 10 segundos
}

bool Server::isCellWithinObstacleRange(int x, int y, int obsX, int obsY, int range) {
    return (std::abs(x - obsX) <= range && std::abs(y - obsY) <= range);
}

void Server::sendToClient(QTcpSocket* socket, const QJsonObject &json) {
    QByteArray dataArray = QJsonDocument(json).toJson();
    socket->write(dataArray);
}

void Server::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "Cliente intentando conectar...";

    clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Cliente conectado.";

        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readFromClient);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

        // Informar al cliente sobre la posición inicial de Samurai A
        QJsonObject response;
        response["status"] = "success";
        response["x"] = 0;  // posición inicial x
        response["y"] = 0;  // posición inicial y
        response["tipo"] = 3;  // tipo para Samurai A
        sendToClient(clientSocket, response);
    } else {
        qDebug() << "Error al conectar con el cliente:" << clientSocket->errorString();
    }
}

void Server::clientDisconnected() {
    qDebug() << "Cliente desconectado.";
    clientSocket->deleteLater();
    clientSocket = nullptr;
}

void Server::startGame() {
    gameStarted = true;
    gameMatrix[0][0] = 4;  // Representamos a Samurai A con el valor 4

    QJsonObject response;
    response["status"] = "gameStarted";
    response["x"] = 0;  // posición inicial x
    response["y"] = 0;  // posición inicial y
    response["tipo"] = 3;  // tipo para Samurai A
    sendToClient(clientSocket, response);
}

void Server::readFromClient() {
    QByteArray data = clientSocket->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject json = jsonDoc.object();

    // Siempre permitir la colocación de obstáculos
    if (json.contains("action") && json["action"].toString() == "placeObstacle") {
        handlePlaceObstacle(json);
    } else if (json.contains("action") && json["action"].toString() == "moveSamuraiA") {
        handleMoveSamuraiA();
    } else if (json.contains("action") && json["action"].toString() == "moveSamuraiB") {
    handleMoveSamuraiB();
    }
}


void Server::handlePlaceObstacle(const QJsonObject &json) {
    int x = json["x"].toInt();
    int y = json["y"].toInt();
    int tipo = json["tipo"].toInt();

    int cost;
    if (tipo == 0) cost = 10;  // Yari
    else if (tipo == 1) cost = 15;  // Arco y flechas
    else if (tipo == 2) cost = 20;  // Tanegashima
    else return;

    if (koban >= cost) {
        koban -= cost;

        // Establecer el alcance alrededor del obstáculo principal si es tipo Yari
        if (tipo == 0) {
            for (int i = x - 1; i <= x + 1; ++i) {
                for (int j = y - 1; j <= y + 1; ++j) {
                    if (i >= 0 && i < 10 && j >= 0 && j < 10 && (i != x || j != y)) {
                        gameMatrix[i][j] = 5;
                    }
                }
            }
        }
        // Establecer el alcance de dos celdas alrededor del obstáculo principal si es tipo Arco y flechas
        else if (tipo == 1) {
            for (int i = x - 2; i <= x + 2; ++i) {
                for (int j = y - 2; j <= y + 2; ++j) {
                    if (i >= 0 && i < 10 && j >= 0 && j < 10 && (i != x || j != y)) {
                        gameMatrix[i][j] = 6;
                    }
                }
            }
        }
        // Establecer el alcance de dos celdas alrededor del obstáculo principal si es tipo Tanegashima
        else if (tipo == 2) {
            for (int i = x - 2; i <= x + 2; ++i) {
                for (int j = y - 2; j <= y + 2; ++j) {
                    if (i >= 0 && i < 10 && j >= 0 && j < 10 && (i != x || j != y)) {
                        gameMatrix[i][j] = 7;
                    }
                }
            }
        }

        gameMatrix[x][y] = tipo + 1;  // Aquí colocamos el obstáculo principal

        qDebug() << "Matrix after placing obstacle:";
        printGameMatrix();

        QJsonObject response;
        response["status"] = "success";
        response["x"] = x;
        response["y"] = y;
        response["tipo"] = tipo;
        response["currentKoban"] = koban;
        sendToClient(clientSocket, response);
    } else {
        QJsonObject response;
        response["status"] = "error";
        response["error"] = "Not enough koban";
        sendToClient(clientSocket, response);
    }
}

void Server::handleMoveSamuraiA() {
    int oldX = samuraiA.x();
    int oldY = samuraiA.y();

    std::pair<int, int> nextPos = samuraiA.move(gameMatrix);
    int attempts = 0;

    while (!isValidPosition(nextPos.first, nextPos.second) && attempts < 3) {
        qDebug() << "Obstacle detected at" << nextPos.first << ", " << nextPos.second << ". Recalculating path...";
        nextPos = samuraiA.move(gameMatrix);
        attempts++;
    }
    if ((samuraiA.x() == 9 && samuraiA.y() == 9) || attempts >= 3) {
        gameMatrix[samuraiA.x()][samuraiA.y()] = 0; // Set Samurai A's current position to 0, making it disappear
        spawnSamuraiB(); // Spawn Samurai B
        printGameMatrix();
        return; // End function early since Samurai A has reached the destination or can't move
    }
    // Verificación adicional justo antes del movimiento
    if (gameMatrix[nextPos.first][nextPos.second] == 1 || gameMatrix[nextPos.first][nextPos.second] == 2 || gameMatrix[nextPos.first][nextPos.second] == 3) {
        qDebug() << "Immediate obstacle check failed. Samurai stays in place.";
        nextPos = { oldX, oldY };
    }else {
        // Actualizar la posición de SamuraiA
        samuraiA.setPosition(nextPos.first, nextPos.second);
        gameMatrix[oldX][oldY] = 0;  // Cambiamos el orden aquí
        gameMatrix[samuraiA.x()][samuraiA.y()] = 4;  // Y aquí también
    }

    if (contadorMovimientos == 3) {
        contadorAuxiliar++;
        contadorMovimientos = 0;

        // Imprimir el valor de contadorAuxiliar cuando aumenta
        qDebug() << "Numero de iteraciones actual:" << contadorAuxiliar;
        ArduinoController::instance().sendIncrementCommand();

    }

    QJsonObject response;
    response["status"] = "success";
    response["x"] = samuraiA.x();
    response["y"] = samuraiA.y();
    response["oldX"] = oldX;
    response["oldY"] = oldY;
    response["tipo"] = 3;
    // Imprime la matriz cada vez que se coloca un obstáculo
    qDebug() << "Matriz actual";
    printGameMatrix();
    qDebug() << "Sending SamuraiA position to client: Y =" << samuraiA.x() << ", X =" << samuraiA.y();
    // Incrementar los contadores
    contadorMovimientos++;
    sendToClient(clientSocket, response);
}


bool Server::isValidPosition(int x, int y) {
    // Verifica si las coordenadas están dentro de los límites de la matriz
    if (x < 0 || x >= 10 || y < 0 || y >= 10) {
        return false;
    }

    // Verifica si la celda en la matriz no contiene un obstáculo
    // (asumiendo que los valores 1, 2 y 3 son obstáculos)
    if (gameMatrix[y][x] >= 1 && gameMatrix[y][x] <= 3) {
        return false;
    }

    return true;
}

void Server::printGameMatrix() {
    for (int i = 0; i < gameMatrix.size(); i++) {
        QString row;
        for (int j = 0; j < gameMatrix[i].size(); j++) {
            row += QString::number(gameMatrix[i][j]) + " ";
        }
        qDebug() << row;
    }
    qDebug() << "\n";  // Añade una línea en blanco para separar las matrices impresas
}

void Server::spawnSamuraiB() {

    /**
    samuraiB.setEdad();
    samuraiB.setEmocional();
    samuraiB.setFisica();
    samuraiB.setfSuperior();
    samuraiB.setfInferior();

    samuraiB.setResistencia();
    **/

    samuraiB.setPosition(0, 0); // Establecer posición inicial para SamuraiB
    gameMatrix[0][0] = 8;  // Representamos a Samurai B con el valor 8

    // Notificamos al cliente del spawn de SamuraiB
    QJsonObject response;
    response["status"] = "success";
    response["x"] = 0; // Posición inicial x
    response["y"] = 0; // Posición inicial y
    response["tipo"] = 8; // tipo para Samurai B
    sendToClient(clientSocket, response);
}

void Server::handleMoveSamuraiB() {
    int oldX = samuraiB.x();
    int oldY = samuraiB.y();

    std::pair<int, int> nextPos = samuraiB.move(gameMatrix);
    int attempts = 0;

    while (!isValidPosition(nextPos.first, nextPos.second) && attempts < 3) {
        qDebug() << "Obstacle detected at" << nextPos.first << ", " << nextPos.second << ". Recalculating path...";
        nextPos = samuraiB.move(gameMatrix);
        attempts++;
    }

    if ((samuraiB.x() == 9 && samuraiB.y() == 9) || attempts >= 3) {
        gameMatrix[samuraiB.x()][samuraiB.y()] = 0; // Set Samurai B's current position to 0, making it disappear
        printGameMatrix();
        return; // End function early since Samurai B has reached the destination or can't move
    }

    // Verificación adicional justo antes del movimiento
    if (gameMatrix[nextPos.first][nextPos.second] == 1 || gameMatrix[nextPos.first][nextPos.second] == 2 || gameMatrix[nextPos.first][nextPos.second] == 3) {
        qDebug() << "Immediate obstacle check failed. SamuraiB stays in place.";
        nextPos = { oldX, oldY };
    } else {
        // Actualizar la posición de SamuraiB
        samuraiB.setPosition(nextPos.first, nextPos.second);
        gameMatrix[oldX][oldY] = 0;
        gameMatrix[samuraiB.x()][samuraiB.y()] = 8;
    }

    if (contadorMovimientos == 3) {
        contadorAuxiliar++;
        contadorMovimientos = 0;

        // Imprimir el valor de contadorAuxiliar cuando aumenta
        qDebug() << "Numero de iteraciones actual:" << contadorAuxiliar;
         ArduinoController::instance().sendIncrementCommand();
    }
    QJsonObject response;
    response["status"] = "success";
    response["x"] = samuraiB.x();
    response["y"] = samuraiB.y();
    response["oldX"] = oldX;
    response["oldY"] = oldY;
    response["tipo"] = 8;
    printGameMatrix();
    qDebug() << "Sending SamuraiB position to client: Y =" << samuraiB.x() << ", X =" << samuraiB.y();
    // Incrementar los contadores
    contadorMovimientos++;
    sendToClient(clientSocket, response);
}
