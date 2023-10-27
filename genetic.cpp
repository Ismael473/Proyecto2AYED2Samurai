#include "genetic.h"

Genetic::Genetic() {}

void Genetic::setGeneraciones(int generaciones) {
    this->Generaciones = generaciones;
}

void Genetic::setAptitudA(int aptitudA) {
    this->AptitudA = aptitudA;
}

void Genetic::setAptitudB(int aptitudB) {
    this->AptitudB = aptitudB;
}

void Genetic::setSamuraiA(SamuraiA samuraiA) {
    this->samuraiA = samuraiA;
}

SamuraiA Genetic::getSamuraiA() {
    return this->samuraiA;
}

void Genetic::setSamuraiB(SamuraiB samuraiB) {
    this->samuraiB = samuraiB;
}

SamuraiB Genetic::getSamuraiB() {
    return this->samuraiB;
}

void Genetic::elitism() {

}

void Genetic::mutacion() {
    int mutacionSamuraiA = rand() % 1;
    int mutacionSamuraiB = rand() % 1;

    if (mutacionSamuraiB == 1) {
        this->samuraiB.setEdad((rand() % 1 + 10) / 10);
        this->samuraiB.setEmocional((rand() % 1 + 10) / 10);
        this->samuraiB.setFisica((rand() % 1 + 10) / 10);
        this->samuraiB.setfSuperior((rand() % 1 + 10) / 10);
        this->samuraiB.setfInferior((rand() % 1 + 10) / 10);
    }

    if (mutacionSamuraiA == 1) {
        this->samuraiA.setEdad((rand() % 1 + 10) / 10);
        this->samuraiA.setEmocional((rand() % 1 + 10) / 10);
        this->samuraiA.setFisica((rand() % 1 + 10) / 10);
        this->samuraiA.setfSuperior((rand() % 1 + 10) / 10);
        this->samuraiA.setfInferior((rand() % 1 + 10) / 10);
    }
}

void Genetic::crossover() {
    int crossoverSamuraiA = rand() % 1 + 5;
    int crossoverSamuraiB = rand() % 1 + 5;

    if (crossoverSamuraiA == 5) {
        int crossoverAtribute = rand() % 1 + 5;
        if (crossoverAtribute == 1) {
            this->samuraiA.setEdad(this->samuraiB.getEdad());
        }

    }

}

void Genetic::run() {

}
