// obstaculo.h
#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QString>

class Obstaculo
{
public:
    enum class Tipo { Yari, ArcoYFlechas, Tanegashima };

    Obstaculo(Tipo tipo);

    int getCosto() const;
    int getDano() const;
    int getAlcance() const;
    QString getColor() const;

private:
    Tipo tipo;
};

#endif // OBSTACULO_H
