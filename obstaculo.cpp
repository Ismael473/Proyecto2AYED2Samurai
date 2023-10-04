// obstaculo.cpp
#include "obstaculo.h"

Obstaculo::Obstaculo(Tipo tipo) : tipo(tipo) {}

int Obstaculo::getCosto() const {
    switch(tipo) {
    case Tipo::Yari: return 10;
    case Tipo::ArcoYFlechas: return 15;
    case Tipo::Tanegashima: return 20;
    }
    return 0;
}

int Obstaculo::getDano() const {
    switch(tipo) {
    case Tipo::Yari: return 1;
    case Tipo::ArcoYFlechas: return 2;
    case Tipo::Tanegashima: return 4;
    }
    return 0;
}

int Obstaculo::getAlcance() const {
    switch(tipo) {
    case Tipo::Yari: return 1;
    case Tipo::ArcoYFlechas: case Tipo::Tanegashima: return 2;
    }
    return 0;
}

QString Obstaculo::getColor() const {
    switch(tipo) {
    case Tipo::Yari: return "verde";
    case Tipo::ArcoYFlechas: return "amarillo";
    case Tipo::Tanegashima: return "negro";
    }
    return "";
}
