#include "utilsMesh.h"

#include <QDebug>

UtilsMesh::UtilsMesh()
{

}

void UtilsMesh::add_face(MyMesh *_mesh, vector<VertexHandle> vhs)
{
    vector<MyMesh::VertexHandle> uneNouvelleFace;
    for (auto vh : vhs)
    {
        uneNouvelleFace.push_back(vh);
    }
    _mesh->add_face(uneNouvelleFace);
    FaceHandle fh = _mesh->face_handle(_mesh->n_faces()-1);
    _mesh->set_color(fh, MyMesh::Color(150, 150, 150));
}

void UtilsMesh::get_vh_of_edge(MyMesh *_mesh, int edgeID,
                               VertexHandle &vh0, VertexHandle &vh1)
{
    EdgeHandle eh = _mesh->edge_handle(edgeID);
    MyMesh::HalfedgeHandle heh1 =  _mesh->halfedge_handle(eh, 0);
    MyMesh::HalfedgeHandle heh2 =  _mesh->halfedge_handle(eh, 1);
    vh0 = _mesh->to_vertex_handle(heh1);
    vh1 = _mesh->to_vertex_handle(heh2);
}

void UtilsMesh::get_eh_of_triangle_face(MyMesh *_mesh, int faceID,
                                        EdgeHandle &eh0, EdgeHandle &eh1, EdgeHandle &eh2)
{
    FaceHandle fh = _mesh->face_handle(faceID);
    int cpt = 1;
    for (MyMesh::FaceEdgeCWIter fe_it = _mesh->fe_cwiter(fh); fe_it.is_valid(); fe_it++)
    {
        if (cpt==1)         eh0 = *fe_it;
        else if (cpt==2)    eh1 = *fe_it;
        else if (cpt==3)    eh2 = *fe_it;
        else {
            qWarning() << "In " << __FUNCTION__ << ": Error cpt > 3 -> not a triangle!";
            exit (1);
        }
        cpt++;
    }
}

void UtilsMesh::get_vh_of_triangle_face(MyMesh *_mesh, int faceID,
                                        VertexHandle &vh0, VertexHandle &vh1, VertexHandle &vh2)
{
    FaceHandle fh = _mesh->face_handle(faceID);
    int cpt = 1;
    for (MyMesh::FaceVertexCWIter fv_it = _mesh->fv_cwiter(fh); fv_it.is_valid(); fv_it++)
    {
        if (cpt==1)         vh0 = *fv_it;
        else if (cpt==2)    vh1 = *fv_it;
        else if (cpt==3)    vh2 = *fv_it;
        else {
            qWarning() << "In " << __FUNCTION__ << ": Error cpt > 3 -> not a triangle!";
            exit (1);
        }
        cpt++;
    }
}

void UtilsMesh::get_points_of_triangle_face(MyMesh *_mesh, int faceID,
                                   MyMesh::Point &p1, MyMesh::Point &p2, MyMesh::Point &p3)
{
    FaceHandle fh = _mesh->face_handle(faceID);
    int cpt = 1;
    for (MyMesh::FaceVertexCWIter fv_it = _mesh->fv_cwiter(fh); fv_it.is_valid(); fv_it++)
    {
        VertexHandle vh = *fv_it;
        if (cpt==1)         p1 = _mesh->point(vh);
        else if (cpt==2)    p2 = _mesh->point(vh);
        else if (cpt==3)    p3 = _mesh->point(vh);
        else {
            qWarning() << "In " << __FUNCTION__ << ": Error cpt > 3, not a triangle!";
            exit (1);
        }
        cpt++;
    }
}

MyMesh::Point UtilsMesh::barycentre_triangle(MyMesh *_mesh, int faceID)
{
    MyMesh::Point _p1, _p2, _p3;
    get_points_of_triangle_face(_mesh, faceID, _p1, _p2, _p3);

    MyMesh::Point P;
    P  = _p1;
    P += _p2;
    P += _p3;
    P /= 3;

    return P;
}

QVector3D UtilsMesh::to_qvector3D(MyMesh::Point p)
{
    return QVector3D(p[0], p[1], p[2]);
}

float UtilsMesh::distance_point_to_face(MyMesh *_mesh, MyMesh::Point p, int faceID)
{
    MyMesh::Point _P = UtilsMesh::barycentre_triangle(_mesh, faceID);
    QVector3D P = UtilsMesh::to_qvector3D(_P);
    return P.distanceToPoint(UtilsMesh::to_qvector3D(p));
}

float  UtilsMesh::middle_edge_coord(float p1, float p2)
{
    float dist = fabs(p1 - p2);
    float res;
    if (p1 <= p2) {
        res = p1 + (dist/2.f);
    }
    else {
        res = p1 -(dist/2.f);
    }
    return res;
}


/*--------------------------------------------------------------------------
 *  Renvoie la positon du milieu d'une arête d'indice @edgeID
 * -------------------------------------------------------------------------*/
MyMesh::Point UtilsMesh::middle_edge(MyMesh *_mesh, int edgeID)
{
    EdgeHandle eh = _mesh->edge_handle(edgeID);

    MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 0);
    MyMesh::HalfedgeHandle heh2 = _mesh->halfedge_handle(eh, 1);
    VertexHandle vh1 = _mesh->to_vertex_handle(heh1);
    VertexHandle vh2 = _mesh->to_vertex_handle(heh2);
    MyMesh::Point p1 = _mesh->point(vh1);
    MyMesh::Point p2 = _mesh->point(vh2);

    float X = UtilsMesh::middle_edge_coord(p1[0], p2[0]);
    float Y = UtilsMesh::middle_edge_coord(p1[1], p2[1]);
    float Z = UtilsMesh::middle_edge_coord(p1[2], p2[2]);
    MyMesh::Point myP(X, Y, Z);
    return myP;
}
