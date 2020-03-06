#ifndef UTILSMESH_H
#define UTILSMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <cmath>

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

#include <QVector3D>
#include <vector>

using namespace std;

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
private:
    static float middle_edge_coord(float p1, float p2);
public:
    UtilsMesh();

    static void add_face(MyMesh *_mesh, vector<VertexHandle> vhs);

    static void get_vh_of_edge(MyMesh *_mesh, int edgeID,
                               VertexHandle &vh0, VertexHandle &vh1);
    static void get_eh_of_triangle_face(MyMesh *_mesh, int faceID,
                                        EdgeHandle &eh0, EdgeHandle &eh1, EdgeHandle &eh2);
    static void get_vh_of_triangle_face(MyMesh *_mesh, int faceID,
                                        VertexHandle &vh0, VertexHandle &vh1, VertexHandle &vh2);
    static void get_points_of_triangle_face(MyMesh *_mesh, int faceID,
                                            MyMesh::Point &p1, MyMesh::Point &p2, MyMesh::Point &p3);

    static MyMesh::Point middle_edge(MyMesh *_mesh, int edgeID);

    static MyMesh::Point barycentre_triangle(MyMesh *_mesh, int faceID);
    static QVector3D to_qvector3D(MyMesh::Point p);
    static float distance_point_to_face(MyMesh *_mesh, MyMesh::Point p, int faceID);
};









#endif // UTILS_H















