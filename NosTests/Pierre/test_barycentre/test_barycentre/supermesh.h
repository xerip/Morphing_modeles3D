#ifndef SUPERMESH_H
#define SUPERMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyConnectivity.hh>

#include <QVector3D>

#include "utils.h"
#include "utilsMesh.h"
#include "raycasting.h"

using namespace std;

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

class SuperMesh
{
public:
    SuperMesh();
    SuperMesh(MyMesh &mesh, MyMesh::Point &_origin);
//    SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt, MyMesh::Point &_origin);

    MyMesh get_superM();

    void build();
    void test (VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, VertexHandle vh4);
    void old_split();
    void my_split(int numFace, vector<int> &sommetsRajoutes);

private:
    MyMesh meshSrc ;
    MyMesh meshTgt;
    MyMesh &superM;
    MyMesh::Point origin;
    float rayon;

    void split_1_to_4(int numFace, vector<VertexHandle> vhs, vector<VertexHandle> vhPs);
    void split_edges(VertexHandle &vhP1, VertexHandle &vhP2, VertexHandle &vhP3);
    bool my_intersect(MyMesh *_mesh, int face, MyMesh::Point O, MyMesh::Point P);
    void retriangle_neighbours(int faceID, vector<FaceHandle> fhPs, vector<VertexHandle> vhPs);
    void rectangle_to_triangle (int faceID, int vhSplitID);

    vector<FaceHandle> get_3_neighbour_faces(int numFace,
                                                EdgeHandle eh1, EdgeHandle eh2, EdgeHandle eh3);
};

#endif // SUPERMESH_H
