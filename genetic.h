#ifndef GENETIC_H
#define GENETIC_H

#include <SamuraiA.h>
#include <SamuraiB.h>

class genetic
{
public:
    genetic();  // Constructor

    //Calcula y guarda la aptitud del samurai
    void setFitness(char s);

    //Retorna la aptitud del samurai
    int getFitness(char s);

private:
    int Generaciones;   //Cantidad de generaciones antes de detenerse
    int Aptitud;    //Aptitud del samurai
    SamuraiA SamA;  //Objeto SamuraiA
    SamuraiB SamB;  //Objeto SamuraiB
};

#endif // GENETIC_H
