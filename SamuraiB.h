#ifndef SAMURAIB_H
#define SAMURAIB_H

#include <vector>
#include <utility>

class SamuraiB {
public:
    SamuraiB();
    SamuraiB(int edad,
             int emocional,
             int fisica,
             int fSuperior,
             int fInferior) {

        this->edad = edad;
        this->emocional = emocional;
        this->fisica = fisica;
        this->fSuperior = fSuperior;
        this->fInferior = fInferior;

    }; // Constructor

    // Setea la posición del SamuraiB en el tablero
    void setPosition(int x, int y);

    // Retorna el siguiente movimiento basado en la matriz del juego proporcionada
    std::pair<int, int> move(const std::vector<std::vector<int>>& gameMatrix);

    // Retorna la coordenada x del SamuraiB
    int x() const;

    // Retorna la coordenada y del SamuraiB
    int y() const;

    // Calcula y asigna la resistencia al samurai
    void setResistencia();

private:
    int posX; // Coordenada x actual del SamuraiB
    int posY; // Coordenada y actual del SamuraiB

    int probSupervivencia;
    int GenSupervivencia;

    int edad; // Edad del samurai utilizada para calcular la resistencia (20 - 40)
    int emocional; // Inteligencia emocional del samurai utilizada para calcular la resistencia (0 - 1)
    int fisica; // Condición física del samurai utilizada para calcular la resistencia (0 - 1)
    int fSuperior; // Fuerza superior del samurai utilizada para calcular la resistencia (0 - 1)
    int fInferior; // Fuerza inferior utilizada para calcular la resistencia (0 - 1)

    int resistencia; // Resistencia del SamuraiB (basado en el diseño de SamuraiA)

    // Función privada para determinar si una celda es transitable
    bool isTransitable(int cellValue) const;

    // Función privada para la lógica de movimiento usando backtracking
    std::pair<int, int> backtrackingPathfinding(const std::vector<std::vector<int>>& matrix);
};

#endif // SAMURAIB_H
