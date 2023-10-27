#ifndef SAMURAIA_H
#define SAMURAIA_H

#include <vector>
#include <utility> // para std::pair

class SamuraiA {
public:
    // Constructor
    SamuraiA();

    // Setea la posición del SamuraiA en el tablero
    void setPosition(int x, int y);

    // Setea la edad del samurai
    void setEdad(float edad);

    // Setea la inteligencia emocional del samurai
    void setEmocional(float emocional);

    // Setea la condicion fisica del samurai
    void setFisica(float fisica);

    // Setea la fuerza superior del samurai
    void setfSuperior(float fSuperior);

    // Setea la fuerza inferior del samurai
    void setfInferior(float fInferior);

    // Calcula y setea la resistencia al samurai
    void setResistencia(float emocional, float fisica, float fSuperior, float fInferior, float edad);

    // Retorna la edad del samurai
    float getEdad();

    // Retorna la inteligencia emocional del samurai
    float getEmocional();

    // Retorna la condicion fisica del samurai
    float getFisica();

    // Retorna la fuerza superior del samurai
    float getfSuperior();

    // Retorna la fuerza inferior del samurai
    float getfInferior();

    // Retorna la resistencia del Samurai
    float getResistencia();

    // Retorna el siguiente movimiento basado en la matriz del juego proporcionada
    std::pair<int, int> move(const std::vector<std::vector<int>>& gameMatrix);

    // Retorna la coordenada x del SamuraiA
    int x() const;

    // Retorna la coordenada y del SamuraiA
    int y() const;

private:
    int posX, posY; // Coordenadas X y Y actual del SamuraiB

    int resistencia;// Resistencia del SamuraiB

    float edad; // Edad del samurai utilizada para calcular la resistencia (20 - 40)
    float emocional; // Inteligencia emocional del samurai utilizada para calcular la resistencia (0 - 1)
    float fisica; // Condición física del samurai utilizada para calcular la resistencia (0 - 1)
    float fSuperior; // Fuerza superior del samurai utilizada para calcular la resistencia (0 - 1)
    float fInferior; // Fuerza inferior utilizada para calcular la resistencia (0 - 1)

    bool isWithinObstacleRange(int x, int y, const std::vector<std::vector<int>>& matrix);

    // Función privada para determinar si una celda es transitable
    bool isTransitable(int cellValue) const;

    std::pair<int, int> aStarPathfinding(const std::vector<std::vector<int>>& matrix);

    //Funcion privada que reduce la resistencia del samurai al colisionar con un obstaculo
    void reduceResistencia(int tileValue);
};

#endif // SAMURAIA_H
