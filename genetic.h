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
    int Fitness;    //Aptitud del samurai
    SamuraiA SamA;  //Objeto SamuraiA
    SamuraiB SamB;  //Objeto SamuraiB

};

#endif // GENETIC_H
