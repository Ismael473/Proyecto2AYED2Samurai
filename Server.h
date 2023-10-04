#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QJsonObject>
#include "SamuraiA.h"

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
    void sendToClient(QTcpSocket* socket, const QJsonObject &json);
    void handlePlaceObstacle(const QJsonObject &json);
    void handleMoveSamuraiA();
    bool isCellWithinObstacleRange(int x, int y, int obsX, int obsY, int range);
    bool isValidPosition(int x, int y);
    SamuraiA samuraiA;
    bool gameStarted;
    std::vector<std::vector<int>> gameMatrix; // Assume a 2D matrix for the game board
    QTcpSocket* clientSocket;
    int koban;  // Moneda para colocar obstáculos
    void printGameMatrix();
};

#endif // SERVER_H
