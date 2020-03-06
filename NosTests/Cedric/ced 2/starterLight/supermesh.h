#ifndef SUPERMESH_H
#define SUPERMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include "metamesh.h"
#include "utilsMesh.h"

using namespace std;

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

class SuperMesh
{
public:
    SuperMesh();
    SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt, MyMesh::Point &_origin);

private:
    MyMesh &meshSrc ;
    MyMesh &meshTgt;
    MyMesh::Point origin;
    void alignement(std::vector<int> list1, std::vector<int> list2);
    float rayon;
};

#endif // SUPERMESH_H
