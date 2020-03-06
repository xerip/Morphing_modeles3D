#include "raycasting.h"
#include <QVector3D>

RayCasting::RayCasting()
{
    ;
}

/*--------------------------------------------------------------------------
 * méthode permettant d'obtenir le point de paramètre @t avec l'équation
 * de la droite (@O, @P)
 * -------------------------------------------------------------------------*/
QVector3D RayCasting::get_point_of_line(QVector3D O, QVector3D P, float t)
{
    QVector3D myP;
    myP[0] = O[0] + t*(P[0] - O[0]);    // x
    myP[1] = O[1] + t*(P[1] - O[1]);    // y
    myP[2] = O[2] + t*(P[2] - O[2]);    // z
    return myP;
}


QVector3D cross(QVector3D v1, QVector3D v2)
{
    return QVector3D::crossProduct(v1, v2);
}

float dot(QVector3D v1, QVector3D v2)
{
    return QVector3D::dotProduct(v1, v2);
}

/*--------------------------------------------------------------------------
 * Vrai si la droite (@P1, @P2), intersecte la face triangulaire
 * (@p1, @p2, @p3).
 * -------------------------------------------------------------------------*/
bool RayCasting::intersect(QVector3D p1, QVector3D p2, QVector3D p3,
                           QVector3D P1, QVector3D P2,
                           QVector3D &resP)
{

    // TEST
    float epsilon = 0.f;

    // Compute the plane supporting the triangle (p1, p2, p3)
    //     normal: n
    //     offset: d
    //
    // A point P lies on the supporting plane iff n.dot(P) + d = 0
    QVector3D v21 = p2 - p1;
    QVector3D v31 = p3 - p1;

    QVector3D n = cross(v21, v31);
    float d = dot(-n, p1);

    // A point P belongs to the line from P1 to P2 iff
    //     P = P1 + t * (P2 - P1)
    //
    // Find the intersection point P(t) between the line and
    // the plane supporting the triangle:
    //     n.dot(P) + d = 0
    //                  = n.dot(P1 + t (P2 - P1)) + d
    //                  = n.dot(P1) + t n.dot(P2 - P1) + d
    //
    //     t = -(n.dot(P1) + d) / n.dot(P2 - P1)
    //

    QVector3D P21 = P2 - P1;
    float nDotP21 = dot(n, P21);

    // Ignore line parallel to (or lying in) the plane
    if (fabs(nDotP21) < epsilon)    return false;

    float t = -1.f * (dot(n, P1) + d) / nDotP21 ;
    resP = P1 + t * P21;

    // Plane bounding the inside half-space of edge (p1, p2):
    //     normal: n21 = n x (p2 - p1)
    //     offset: d21 = -n21.dot(p1)
    //
    // A point P is in the inside half-space iff n21.dot(P) + d21 > 0
    //

    // Edge (p1, p2)
    QVector3D n21 = cross(n, v21);
    float d21 = dot(-n21, p1);
    if ( dot(n21, resP) + d21 <= 0.f)
        return false;

    // Edge (p2, p3)
    QVector3D v32 = p3 - p2;
    QVector3D n32 = cross(n, v32);
    float d32 = dot(-n32, p2);
    if (dot(n32, resP) + d32 <= 0.f)
        return false;

    // Edge (p3, p1)
    QVector3D n13 = cross(n, -v31);
    float d13 = dot(-n13, p3);
    if (dot(n13, resP) + d13 <= 0.f)
        return false;

    return true;
}
