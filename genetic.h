#ifndef GENETIC_H
#define GENETIC_H

#include <SamuraiA.h>
#include <SamuraiB.h>

class genetic
{
public:
    genetic();  // Constructor

    void setGeneraciones(int generaciones);

    void setAptitudA(int aptitudA);

    void setAptitudB(int aptitudB);

    void setSamuraiA(SamuraiA samuraiA);

    void setSamuraiB(SamuraiB samuraiB);

    void run();

private:
    int Generaciones;   //Cantidad de generaciones antes de detenerse
    SamuraiA samuraiA; //Objeto Samurai A
    SamuraiB samuraiB; //Objeto Samurai B
    int AptitudA;   //Aptitud del samuraiA (resistencia)
    int AptitudB;   //Aptitud del samuraiB (resistencia)

    void elitism(); //Elitismo para decidir mejores generaciones
    void mutacion(); //Mutacion random del samurai
    void crossover(); //Crossover random del samurai
};

#endif // GENETIC_H
