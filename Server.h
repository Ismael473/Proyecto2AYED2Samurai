#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QJsonObject>
#include "SamuraiA.h"
#include "SamuraiB.h"
#include "genetic.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void readFromClient();
    void clientDisconnected();
    void startGame();

private:
    int contadorMovimientos = 0;
    int contadorAuxiliar = 0;
    void sendToClient(QTcpSocket* socket, const QJsonObject &json);
    void handlePlaceObstacle(const QJsonObject &json);
    void handleMoveSamuraiA();
    void handleMoveSamuraiB();
    bool isCellWithinObstacleRange(int x, int y, int obsX, int obsY, int range);
    bool isValidPosition(int x, int y);
    SamuraiA samuraiA;
    SamuraiB samuraiB;
    Genetic genetic;
    bool gameStarted;
    std::vector<std::vector<int>> gameMatrix; // Assume a 2D matrix for the game board
    QTcpSocket* clientSocket;
    int koban;  // Moneda para colocar obstáculos
    void printGameMatrix();
    bool samuraiBActive = false;
    void spawnSamuraiB();
    void moveObstacles(const QJsonObject &json);

};

#endif // SERVER_H

