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

    void build();

private:
    MyMesh meshSrc ;
    MyMesh meshTgt;
    MyMesh superM;
    MyMesh::Point origin;

    void alignement(std::vector<int> list1, std::vector<int> list2);
    float rayon;

    bool my_intersect(MyMesh *_mesh, int face, MyMesh::Point O, MyMesh::Point P);
};

#endif // SUPERMESH_H
