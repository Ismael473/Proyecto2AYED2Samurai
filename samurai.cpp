// samurai.cpp
#include "samurai.h"
#include <QRandomGenerator>

Samurai::Samurai(Type type, int id, int x, int y)
    : m_type(type), m_id(id), m_x(x), m_y(y) {
    m_resistencia = QRandomGenerator::global()->bounded(10, 21); // [10, 20] para ambos tipos A y B
}

Samurai::Type Samurai::type() const {
    return m_type;
}

int Samurai::x() const {
    return m_x;
}

int Samurai::y() const {
    return m_y;
}

int Samurai::resistencia() const {
    return m_resistencia;
}

int Samurai::id() const {
    return m_id;
}
