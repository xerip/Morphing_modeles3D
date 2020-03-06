#ifndef SUPERMESH_H
#define SUPERMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <QVector3D>

#include "utilsMesh.h"
#include "raycasting.h"

using namespace std;

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

class SuperMesh
{
public:
    SuperMesh();
    SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt, MyMesh::Point &_origin);

    MyMesh get_superM();

    QVector3D coords_barycentre(QVector3D A, QVector3D B, QVector3D C, QVector3D P);
    void build();

private:
    MyMesh meshSrc ;
    MyMesh meshTgt;
    MyMesh superM;
    MyMesh::Point origin;
    float rayon;

    bool my_intersect(MyMesh *_mesh, int face, MyMesh::Point O, MyMesh::Point P);
};

#endif // SUPERMESH_H
