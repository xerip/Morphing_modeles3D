/*-------------------------------------------------------------
 * Module permettant de projeter un maillage sur une sphère
 * -----------------------------------------------------------*/
#ifndef SPHERE_H
#define SPHERE_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "utilsMesh.h"

#include <vector>
using namespace std;

using namespace OpenMesh;
using namespace OpenMesh::Attributes;


class Sphere
{
public:
    Sphere();
    Sphere(MyMesh &_mesh, MyMesh::Point &_origin);
    Sphere(MyMesh &_mesh, MyMesh::Point &_origin, float _rayon);

    vector<float> _distPointsBase();

    // Mes fonctions
    bool verif_rayon(MyMesh::Point origin, float _rayon);
    MyMesh::Point get_point_of_line(MyMesh::Point O, MyMesh::Point P, float t);
    void despherization(int precision=2);
    void despherization(vector<float> distsPB, int precision=2);
    void map_to_sphere(int vertId, float rayon, int precision=2);
    void demap_from_sphere(int vertId, float distance, int precision=2);
    void spherization(float _rayon, int precision=2);

private:
    MyMesh &mesh ;
    MyMesh::Point origin;
    vector<float> distsPointsBase;
    float rayon;

    // Obsolète pour l'instant
    vector<MyMesh::Point> pointsBase;
};

#endif // SPHERE_H
