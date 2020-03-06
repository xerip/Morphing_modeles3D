#include "utilsMesh.h"

#include <QDebug>

UtilsMesh::UtilsMesh()
{

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
