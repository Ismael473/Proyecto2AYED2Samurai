#ifndef SAMURAIB_H
#define SAMURAIB_H

#include <vector>
#include <utility>

class SamuraiB {
public:
    // Constructor
    SamuraiB();

    // Setea la posición del SamuraiB en el tablero
    void setPosition(int x, int y);

    // Setea la edad del samurai
    void setEdad();

    // Setea la inteligencia emocional del samurai
    void setEmocional();

    // Setea la condicion fisica del samurai
    void setFisica();

    // Setea la fuerza superior del samurai
    void setfSuperior();

    // Setea la fuerza inferior del samurai
    void setfInferior();

    // Calcula y asigna la resistencia al samurai
    void setResistencia();

    // Retorna el siguiente movimiento basado en la matriz del juego proporcionada
    std::pair<int, int> move(const std::vector<std::vector<int>>& gameMatrix);

    // Retorna la coordenada x del SamuraiB
    int x() const;

    // Retorna la coordenada y del SamuraiB
    int y() const;

private:
    int posX, posY; // Coordenadas X y Y actual del SamuraiB

    int probSupervivencia;
    int GenSupervivencia;

    int resistencia; // Resistencia del SamuraiB

    int edad; // Edad del samurai utilizada para calcular la resistencia (20 - 40)
    int emocional; // Inteligencia emocional del samurai utilizada para calcular la resistencia (0 - 1)
    int fisica; // Condición física del samurai utilizada para calcular la resistencia (0 - 1)
    int fSuperior; // Fuerza superior del samurai utilizada para calcular la resistencia (0 - 1)
    int fInferior; // Fuerza inferior utilizada para calcular la resistencia (0 - 1)

    // Función privada para determinar si una celda es transitable
    bool isTransitable(int cellValue) const;

    // Función privada para la lógica de movimiento usando backtracking
    std::pair<int, int> backtrackingPathfinding(const std::vector<std::vector<int>>& matrix);

    //Funcion privada que reduce la resistencia del samurai al colisionar con un obstaculo
    void reduceResistencia(int tileValue);
};

#endif // SAMURAIB_H
