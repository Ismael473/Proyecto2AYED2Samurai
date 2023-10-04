#ifndef SAMURAIA_H
#define SAMURAIA_H

#include <vector>
#include <utility> // para std::pair

class SamuraiA {
public:
    SamuraiA();
    void setPosition(int x, int y);
    std::pair<int, int> move(const std::vector<std::vector<int>>& gameMatrix);
    int x() const;
    int y() const;

private:
    int posX, posY;
    int resistencia;
    bool isWithinObstacleRange(int x, int y, const std::vector<std::vector<int>>& matrix);

    std::pair<int, int> aStarPathfinding(const std::vector<std::vector<int>>& matrix);
};

#endif // SAMURAIA_H
