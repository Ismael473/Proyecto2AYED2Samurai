#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>

class Client : public QWidget {
    Q_OBJECT
public:
    explicit Client(const QString &hostName, int port, QWidget *parent = nullptr);
    void setupMatrix();
    void sendToServer(const QJsonObject &json);

private slots:
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    void onButtonClicked();
    void onPlaceObstacle(int x, int y, int tipo);
    void requestMoveSamuraiA();
private:
    int samuraiAX;  // Coordenada X actual del Samurai A
    int samuraiAY;  // Coordenada Y actual del Samurai A
    QTcpSocket *socket;
    QTimer *moveTimer;
    QGridLayout *gridLayout;
    QVector<QVector<QPushButton*>> matrixButtons;
    QList<QPushButton*> obstacleButtons;  // Lista para almacenar botones de obstáculos
    int selectedObstacle;
};

#endif // CLIENT_H
