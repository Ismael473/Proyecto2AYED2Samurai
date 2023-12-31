#include "SamuraiB.h"
#include <stack>
#include <vector>
#include <QDebug>
#include <random>
#include "arduinocontroller.h"

SamuraiB::SamuraiB() :
    posX(0),
    posY(0),
    resistencia(0) {}

void SamuraiB::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void SamuraiB::setEdad(float edad) {
    this->edad = edad;
}

void SamuraiB::setEmocional(float emocional) {
    this->emocional = emocional;
}

void SamuraiB::setFisica(float fisica) {
    this->fisica = fisica;
}

void SamuraiB::setfSuperior(float fSuperior) {
    this->fSuperior = fSuperior;
}

void SamuraiB::setfInferior(float fInferior) {
    this->fInferior = fInferior;
}

void SamuraiB::setResistencia(float emocional, float fisica, float fSuperior, float fInferior, float edad) {
    this->resistencia = 5 + emocional + fisica + fSuperior + fInferior + ((-1/20) * edad + 2);
}

float SamuraiB::getEdad() {
    return this->edad;
}

float SamuraiB::getEmocional() {
    return this->emocional;
}

float SamuraiB::getFisica() {
    return this->fisica;
}

float SamuraiB::getfSuperior() {
    return this->fSuperior;
}

float SamuraiB::getfInferior() {
    return this->fInferior;
}

float SamuraiB::getResistencia() {
    return this->resistencia;
}

std::pair<int, int> SamuraiB::move(const std::vector<std::vector<int>>& gameMatrix) {
    return backtrackingPathfinding(gameMatrix);
}

int SamuraiB::x() const {
    return posX;
}

int SamuraiB::y() const {
    return posY;
}

void SamuraiB::reduceResistencia(int obstacleValue) {
    switch (obstacleValue) {
    case 5:
        resistencia -= 1;
        break;
    case 6:
        resistencia -= 2;
        break;
    case 7:
        resistencia -= 4;
        break;
    }
    qDebug() << "Resistencia actual: " << resistencia;
    // Llamar a sendSoundCommand de ArduinoController aquí:
    ArduinoController::instance().sendSoundCommand();
}

bool SamuraiB::isTransitable(int cellValue) const {
    return cellValue == 0 || cellValue == 5 || cellValue == 6 || cellValue == 7;
}

std::pair<int, int> SamuraiB::backtrackingPathfinding(const std::vector<std::vector<int>>& matrix) {
    const int targetX = matrix.size() - 1;
    const int targetY = matrix[0].size() - 1;

    if (posX == targetX && posY == targetY) {
        return { posX, posY }; // Ya está en el objetivo.
    }

    const int dx[] = {0, 1, 0};
    const int dy[] = {1, 0, -1};


    for (int d = 0; d < 4; d++) {
        int newX = posX + dx[d];
        int newY = posY + dy[d];

        // Verifica límites y si es transitable y no es la posición actual.
        if (newX >= 0 && newX < matrix.size() && newY >= 0 && newY < matrix[0].size() && isTransitable(matrix[newX][newY]) && (newX != posX || newY != posY)) {
            return { newX, newY }; // Retorna la siguiente posición válida.
        }
    }

    qDebug() << "No viable moves for SamuraiB. Staying in place.";
    return { posX, posY }; // Si no hay movimientos viables, permanece en su posición.
}
