// samurai.h
#pragma once

#include <QObject>

class Samurai : public QObject {
    Q_OBJECT
public:
    enum Type {
        A, // Rojo
        B  // Azul
    };

    Samurai(Type type, int id, int x, int y);

    Type type() const;
    int x() const;
    int y() const;
    int resistencia() const;
    int id() const;

private:
    Type m_type;
    int m_x;
    int m_y;
    int m_resistencia;
    int m_id;  // Identificador único del samurái
};
