#ifndef UTILSMESH_H
#define UTILSMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <cmath>

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

#include <QVector3D>

struct MyTraits : public OpenMesh::DefaultTraits
{
    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    // store the previous halfedge
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    // use face normals face colors
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    EdgeAttributes( OpenMesh::Attributes::Color | OpenMesh::Attributes::Status );
    // vertex thickness
    VertexTraits{float thickness; float value;};
    // edge thickness
    EdgeTraits{float thickness;};
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

class UtilsMesh
{
public:
    UtilsMesh();

    static void get_points_of_triangle_face(MyMesh *_mesh, int faceID,
                                            MyMesh::Point &p1, MyMesh::Point &p2, MyMesh::Point &p3);
    static MyMesh::Point barycentre_triangle(MyMesh *_mesh, int faceID);
    static QVector3D to_qvector3D(MyMesh::Point p);
    static float distance_point_to_face(MyMesh *_mesh, MyMesh::Point p, int faceID);
};

#endif // UTILS_H
