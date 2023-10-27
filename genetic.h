#ifndef GENETIC_H
#define GENETIC_H

#include <random>
#include <SamuraiA.h>
#include <SamuraiB.h>

class Genetic
{
public:
    Genetic();  // Constructor

    void setGeneraciones(int generaciones);

    void setAptitudA(int aptitudA);

    void setAptitudB(int aptitudB);

    void setSamuraiA(SamuraiA samuraiA);

    void setSamuraiB(SamuraiB samuraiB);

    SamuraiA getSamuraiA();

    SamuraiB getSamuraiB();

    //solutions getSolutions();

    void run();

private:
    int Generaciones; //Cantidad de generaciones antes de detenerse
    SamuraiA samuraiA; //Objeto Samurai A
    SamuraiB samuraiB; //Objeto Samurai B
    int AptitudA; //Aptitud del samuraiA (resistencia)
    int AptitudB; //Aptitud del samuraiB (resistencia)

    // Struct para guardar los mejores samurai
    struct solutions {
        SamuraiA solucionA;
        SamuraiB solucionB;
    };


    void elitism(); //Elitismo para decidir mejores generaciones
    void mutacion(); //Mutacion random del samurai
    void crossover(); //Crossover random del samurai
};

#endif // GENETIC_H
