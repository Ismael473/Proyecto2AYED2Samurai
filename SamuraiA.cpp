#include "SamuraiA.h"
#include <queue>
#include <set>
#include <cmath>
#include <QDebug>
#include "arduinocontroller.h"
SamuraiA::SamuraiA() :
    posX(0),
    posY(0),
    resistencia(10) {}

void SamuraiA::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void SamuraiA::setEdad(float edad) {
    this->edad = edad;
}

void SamuraiA::setEmocional(float emocional) {
    this->emocional = emocional;
}

void SamuraiA::setFisica(float fisica) {
    this->fisica = fisica;
}

void SamuraiA::setfSuperior(float fSuperior) {
    this->fSuperior = fSuperior;
}

void SamuraiA::setfInferior(float fInferior) {
    this->fInferior = fInferior;
}

void SamuraiA::setResistencia(float emocional, float fisica, float fSuperior, float fInferior, float edad) {
    this->resistencia = 5 + emocional + fisica + fSuperior + fInferior + ((-1/20) * edad + 2);
}

float SamuraiA::getEdad() {
    return this->edad;
}

float SamuraiA::getEmocional() {
    return this->emocional;
}

float SamuraiA::getFisica() {
    return this->fisica;
}

float SamuraiA::getfSuperior() {
    return this->fSuperior;
}

float SamuraiA::getfInferior() {
    return this->fInferior;
}

float SamuraiA::getResistencia() {
    return this->resistencia;
}

std::pair<int, int> SamuraiA::move(const std::vector<std::vector<int>>& gameMatrix) {
    return aStarPathfinding(gameMatrix);
}

int SamuraiA::x() const {
    return posX;
}

int SamuraiA::y() const {
    return posY;
}

void SamuraiA::reduceResistencia(int obstacleValue) {
    switch (obstacleValue) {
    case 5:
        this->resistencia -= 1;
        break;
    case 6:
        this->resistencia -= 2;
        break;
    case 7:
        this->resistencia -= 4;
        break;
    }
    qDebug() << "Resistencia actual: " << resistencia;
    // Llamar a sendSoundCommand de ArduinoController aquí:
    ArduinoController::instance().sendSoundCommand();
}

struct Node {
    int x, y;
    float cost, heuristic;
    Node* parent;

    Node(int x, int y, float cost, float heuristic, Node* parent = nullptr)
        : x(x), y(y), cost(cost), heuristic(heuristic), parent(parent) {}

    float totalCost() const {
        return cost + heuristic;
    }
};

struct CompareNode {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->totalCost() > rhs->totalCost();
    }
};

float heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool SamuraiA::isWithinObstacleRange(int x, int y, const std::vector<std::vector<int>>& matrix) {
    int obstacleRange[] = { 0, 1, 2, 3 };  // 0 para lugares vacíos, 1 para Yari, 2 para Arco y flechas, y 3 para Tanegashima

    for (int i = std::max(0, x - 3); i <= std::min(static_cast<int>(matrix.size()) - 1, x + 3); ++i) {
        for (int j = std::max(0, y - 3); j <= std::min(static_cast<int>(matrix[0].size()) - 1, y + 3); ++j) {
            int obstacle = matrix[i][j] - 1;  // Restamos 1 porque matrix tiene valores de 1 a 3 para obstáculos
            if (obstacle > 0 && heuristic(x, y, i, j) <= obstacleRange[obstacle]) {
                return true;  // Está dentro del rango de un obstáculo
            }
        }
    }

    return false;
}

std::pair<int, int> SamuraiA::aStarPathfinding(const std::vector<std::vector<int>>& matrix) {
    const int targetX = matrix.size() - 1;
    const int targetY = matrix[0].size() - 1;

    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    std::set<std::pair<int, int>> closedSet;

    Node* startNode = new Node(posX, posY, 0.0f, heuristic(posX, posY, targetX, targetY));
    openSet.push(startNode);

    Node* currentNode = nullptr;
    while (!openSet.empty()) {
        currentNode = openSet.top();
        openSet.pop();

        if (currentNode->x == targetX && currentNode->y == targetY) {
            break;
        }

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                if (std::abs(dx) == 1 && std::abs(dy) == 1) continue; // Excluir movimientos diagonales

                int newX = currentNode->x + dx;
                int newY = currentNode->y + dy;

                // Excluir celdas fuera de los límites y con obstáculos
                if (newX < 0 || newX >= matrix.size() || newY < 0 || newY >= matrix[0].size()) {
                    continue;
                }

                if (!isTransitable(matrix[newX][newY]) || closedSet.find({newX, newY}) != closedSet.end()) {
                    continue;
                }

                Node* successor = new Node(newX, newY, currentNode->cost + 1, heuristic(newX, newY, targetX, targetY), currentNode);
                openSet.push(successor);
            }
        }

        closedSet.insert({currentNode->x, currentNode->y});
        qDebug() << "Processed Node: " << currentNode->x << ", " << currentNode->y;
    }

    if (currentNode->x == targetX && currentNode->y == targetY) {
        //Sólo nos interesa el siguiente movimiento
        while (currentNode->parent && currentNode->parent->parent) {
            currentNode = currentNode->parent;
        }

        int nextX = currentNode->x;
        int nextY = currentNode->y;

        qDebug() << "Path found. Next Move: " << nextX << ", " << nextY;

        // Cleanup
        while (startNode) {
            Node* tmp = startNode;
            startNode = startNode->parent;
            delete tmp;
        }

        if (matrix[nextX][nextY] == 0) {
            return {nextX, nextY};
        } else if (matrix[nextX][nextY] == 5 || matrix[nextX][nextY] == 6 || matrix[nextX][nextY] == 7) {
            reduceResistencia(matrix[nextX][nextY]);
            return {nextX, nextY};
        }
    }

    qDebug() << "No path found. Staying in place.";
    // Cleanup
    while (startNode) {
        Node* tmp = startNode;
        startNode = startNode->parent;
        delete tmp;
    }

    return {posX, posY}; // Si no se encontró un camino, regresa la posición actual
}

bool SamuraiA::isTransitable(int cellValue) const {
    return cellValue == 0 || cellValue == 5|| cellValue == 6| cellValue == 7;
}

