#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "utilsMesh.h"

#include <QVector3D>

#include <vector>
using namespace std;


class RayCasting
{
public:
    RayCasting();

    static QVector3D get_point_of_line(QVector3D O, QVector3D P, float t);

    static bool intersect(QVector3D p1, QVector3D p2, QVector3D p3,
                          QVector3D P1, QVector3D P2, QVector3D &P);
};

#endif // RAYCASTING_H
