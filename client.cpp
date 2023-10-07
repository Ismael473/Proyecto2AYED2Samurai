#include "client.h"
#include <QJsonDocument>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QJsonObject>


Client::Client(const QString &hostName, int port, QWidget *parent)
    : QWidget(parent), socket(new QTcpSocket(this)), gridLayout(new QGridLayout),
    selectedObstacle(-1) {
    // Connect to server
    socket->connectToHost(hostName, port);

    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Client::onError);

    setupMatrix();

    // Añade los botones de obstáculos y conecta a una ranura que almacena el obstáculo seleccionado
    QStringList obstacles = {"Yari", "Arco y Flechas", "Tanegashima"};
    QHBoxLayout* obstacleLayout = new QHBoxLayout;
    for(int i = 0; i < obstacles.size(); ++i) {
        QPushButton *button = new QPushButton(obstacles[i], this);
        connect(button, &QPushButton::clicked, this, [=](){ selectedObstacle = i; });
        obstacleLayout->addWidget(button);
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(obstacleLayout);  // Añade los botones de obstáculo al layout principal
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    // Configuración del QTimer para mover el samurai
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Client::requestMoveSamuraiA);
    moveTimer->start(500); // Inicia el timer para que se ejecute cada 5 segundos
}

void Client::setupMatrix() {
    int rows = 10, cols = 10;
    matrixButtons.resize(rows);
    for(int i = 0; i < rows; ++i) {
        matrixButtons[i].resize(cols);
        for(int j = 0; j < cols; ++j) {
            matrixButtons[i][j] = new QPushButton(this);
            matrixButtons[i][j]->setFixedSize(40, 40);
            gridLayout->addWidget(matrixButtons[i][j], i, j);
            connect(matrixButtons[i][j], &QPushButton::clicked, this, &Client::onButtonClicked);
        }
    }
}

void Client::sendToServer(const QJsonObject &json) {
    QByteArray dataArray = QJsonDocument(json).toJson();
    socket->write(dataArray);
}

void Client::onConnected() {
    qDebug() << "Conectado al servidor.";
}

void Client::onError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    qDebug() << "Error:" << socket->errorString();
}

void Client::onButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button && selectedObstacle != -1) {  // Verifica si hay un obstáculo seleccionado
        int x = -1, y = -1;

        // Encuentra las coordenadas del botón que se hizo clic en la matriz.
        for(int i = 0; i < matrixButtons.size(); ++i) {
            if(x != -1 && y != -1) break;  // Break outer loop if coordinates found
            for(int j = 0; j < matrixButtons[i].size(); ++j) {
                if (matrixButtons[i][j] == button) {
                    x = i;
                    y = j;
                    break;
                }
            }
        }

        if(x != -1 && y != -1) {
            onPlaceObstacle(x, y, selectedObstacle);
            selectedObstacle = -1;  // Reset selected obstacle after placing it
        }
    }
}

void Client::onPlaceObstacle(int x, int y, int tipo) {
    QJsonObject json;
    json["action"] = "placeObstacle";
    json["x"] = x;
    json["y"] = y;
    json["tipo"] = tipo;
    sendToServer(json);
}

void Client::onReadyRead() {
    QByteArray data = socket->readAll();
    qDebug() << "Data received:" << data;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject json = jsonDoc.object();

    if (json.contains("status")) {
        if (json["status"].toString() == "success") {
            int x = json["x"].toInt(); // Coordenada x
            int y = json["y"].toInt(); // Coordenada y
            int tipo = json["tipo"].toInt(); // Tipo

            QColor color; // Color que se aplicará al botón

            // Determina el color basado en el tipo
            switch(tipo) {
            case 0: // Yari
                color = Qt::green;
                break;
            case 1: // Arco y Flechas
                color = Qt::yellow;
                break;
            case 2: // Tanegashima
                color = Qt::black;
                break;
            case 3: // Samurai A
                color = Qt::red;
                break;
            case 8: // Samurai B
                qDebug() << "Setting Samurai B color";
                color = Qt::blue;
                qDebug() << "Setting Samurai B color";
                break;
            default:
                qWarning() << "Tipo desconocido: " << tipo;
                return;
            }

            // Aplica el color al botón correspondiente
            matrixButtons[x][y]->setStyleSheet("background-color: " + color.name() + ";");

            if(tipo != 3 && tipo != 8) { // Si no es un movimiento de samurai
                setObstacleRange(x, y, tipo);
            }
            // Verifica si el movimiento es del samurai y si el JSON incluye las coordenadas antiguas
            if((tipo == 3 || tipo == 8) && json.contains("oldX") && json.contains("oldY")) {
                int oldX = json["oldX"].toInt(); // Coordenada x anterior del samurai
                int oldY = json["oldY"].toInt(); // Coordenada y anterior del samurai
                matrixButtons[oldX][oldY]->setStyleSheet("background-color: white;"); // Setea la antigua posición a blanco
            }// Si Samurai A llega a (9,9), espera 3 segundos antes de cambiar la casilla a blanco
            if (tipo == 3 && x == 9 && y == 9) {
                QTimer::singleShot(3000, [this]() {
                    matrixButtons[9][9]->setStyleSheet("background-color: white;");
                    moveSamuraiA = false;
                });
            }

        } else if (json["status"].toString() == "error") {
            QString errorString = json["error"].toString();
            qDebug() << "Error colocando el obstáculo:" << errorString;
        }
    }
    // Pueden haber otros campos en el JSON
}


// Nueva función para enviar la solicitud de movimiento al servidor
void Client::requestMoveSamuraiA() {
    QJsonObject json;
    if (moveSamuraiA) {
        json["action"] = "moveSamuraiA";
    } else {
        json["action"] = "moveSamuraiB";
    }
    sendToServer(json);
}
void Client::setObstacleRange(int x, int y, int tipo) {
    int range = 0;

    // Determina el rango basado en el tipo de obstáculo
    switch(tipo) {
    case 0: // Yari
        range = 1;
        break;
    case 1: // Arco y Flechas
    case 2: // Tanegashima
        range = 2;
        break;
    default:
        qWarning() << "Tipo de obstáculo desconocido: " << tipo;
            return;
    }

    // Establece el color gris para las celdas dentro del rango
    for (int i = x - range; i <= x + range; ++i) {
        for (int j = y - range; j <= y + range; ++j) {
            // Verifica si las coordenadas están dentro de los límites de la matriz y no son las coordenadas centrales del obstáculo
            if (i >= 0 && i < matrixButtons.size() && j >= 0 && j < matrixButtons[i].size() && (i != x || j != y)) {
                matrixButtons[i][j]->setStyleSheet("background-color: gray;");
            }
        }
    }
}
