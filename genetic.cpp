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

/*
solutions Genetic::getSolutions() {
    return this->solutions;
}

void Genetic::elitism() {
    if (this->samuraiA.getResistencia() > getSolutions().solucionA.getResistencia()) {
        this->solutions.solucionA = getSamuraiA();
    }

    if (this->samuraiB.getResistencia() > getSolutions().solucionB.getResistencia()) {
        this->solutions.solucionB = getSamuraiB();
    }
}
*/

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

        int crossoverAtributeA = rand() % 1 + 5;

        if (crossoverAtributeA == 1) {
            this->samuraiA.setEdad(this->samuraiB.getEdad());
        }

        else if (crossoverAtributeA == 2) {
            this->samuraiA.setEmocional(this->samuraiB.getEmocional());
        }

        else if (crossoverAtributeA == 3) {
            this->samuraiA.setFisica(this->samuraiB.getFisica());
        }

        else if (crossoverAtributeA == 4) {
            this->samuraiA.setfSuperior(this->samuraiB.getfSuperior());
        }

        else {
            this->samuraiA.setfInferior(this->samuraiB.getfInferior());
        }
    }

    if (crossoverSamuraiB == 1) {

        int crossoverAtributeB = rand() % 1 + 5;

        if (crossoverAtributeB == 1) {
            this->samuraiB.setEdad(this->samuraiA.getEdad());
        }

        else if (crossoverAtributeB == 2) {
            this->samuraiB.setEmocional(this->samuraiA.getEmocional());
        }

        else if (crossoverAtributeB == 3) {
            this->samuraiB.setFisica(this->samuraiA.getFisica());
        }

        else if (crossoverAtributeB == 4) {
            this->samuraiB.setfSuperior(this->samuraiA.getfSuperior());
        }

        else {
            this->samuraiB.setfInferior(this->samuraiA.getfInferior());
        }
    }
}

void Genetic::run() {
    int i = this->Generaciones;

    while (i > 0) {

        //elitism();

        crossover();

        mutacion();

        i--;
    }
}
