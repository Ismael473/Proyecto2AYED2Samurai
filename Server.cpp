#include "server.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

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
    QTimer *gameStartTimer = new QTimer(this);
    connect(gameStartTimer, &QTimer::timeout, this, &Server::startGame);
    gameStartTimer->start(10000);  // 10 segundos
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

    if (json.contains("action") && json["action"].toString() == "placeObstacle") {
        if (!gameStarted) {
            handlePlaceObstacle(json);
        }
    } else if (gameStarted && json.contains("action") && json["action"].toString() == "moveSamuraiA") {
        handleMoveSamuraiA();
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
        gameMatrix[y][x] = tipo + 1;

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

    // Aquí calculamos la siguiente posición para SamuraiA usando el algoritmo A*
    std::pair<int, int> nextPos = samuraiA.move(gameMatrix);

    // Actualizar la posición de SamuraiA
    samuraiA.setPosition(nextPos.first, nextPos.second);

    gameMatrix[oldY][oldX] = 0; // Restablece la posición anterior a 0 (vacío)
    gameMatrix[samuraiA.y()][samuraiA.x()] = 4; // Cambiado a 4 para que 4 represente al samurai
    QJsonObject response;
    response["status"] = "success";
    response["x"] = samuraiA.x();
    response["y"] = samuraiA.y();
    response["oldX"] = oldX;  // Agregamos las coordenadas anteriores al JSON
    response["oldY"] = oldY;
    response["tipo"] = 3;  // Tipo 3 para representar al samurai A
    qDebug() << "Sending SamuraiA position to client: X =" << samuraiA.x() << ", Y =" << samuraiA.y();
    sendToClient(clientSocket, response);
}

