#include "supermesh.h"

#include <QDebug>

SuperMesh::SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt, MyMesh::Point &_origin) :
    meshSrc(_meshSrc), meshTgt(_meshTgt), origin(_origin)
{
    qDebug() << "<" << __FUNCTION__ << ">";

    superM = MyMesh(meshTgt);

    qDebug() << "</" << __FUNCTION__ << ">";
}

MyMesh SuperMesh::get_superM()  {   return superM;  }

bool SuperMesh::my_intersect(MyMesh *_mesh, int face, MyMesh::Point O, MyMesh::Point P)
{
    MyMesh::Point _p1, _p2, _p3;
    UtilsMesh::get_points_of_triangle_face(_mesh, face, _p1, _p2, _p3);

    QVector3D p1 (_p1[0], _p1[1], _p1[2]);
    QVector3D p2 (_p2[0], _p2[1], _p2[2]);
    QVector3D p3 (_p3[0], _p3[1], _p3[2]);
    QVector3D P1 (O[0], O[1], O[2]);
    QVector3D P2 (P[0], P[1], P[2]);

    return RayCasting::intersect(p1, p2, p3, P1, P2);
}

void SuperMesh::build()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    float distMin;
    float epsilon = 0.06f;

    for (MyMesh::VertexIter v_it = meshSrc.vertices_begin(); v_it != meshSrc.vertices_end(); v_it++)
    {
        VertexHandle vh = *v_it;
        qDebug() << "\tsommet" << vh.idx() << "/" << meshSrc.n_vertices();
        MyMesh::Point p = meshSrc.point(vh);
        distMin = INT_MAX;
        for (MyMesh::FaceIter f_it = superM.faces_begin(); f_it != superM.faces_end(); f_it++)
        {
            FaceHandle fh = *f_it;
            if (my_intersect(&superM, fh.idx(), origin, p))
            {
                float dist = UtilsMesh::distance_point_to_face(&superM, p, fh.idx());
                if (dist <= epsilon) {
                    // Retenir la face ici
                    break;
                }
                else if (dist < distMin) {
                    distMin = dist;
                    // Retenir la face ici
                }
            }
        }
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}

void SuperMesh::alignement(std::vector<int> list1, std::vector<int> list2)
{
    std::vector<MyMesh::Point> vect_deplacement;
    MyMesh::Point deplacement;
    MyMesh* mesh1=&meshSrc;
    MyMesh* mesh2=&meshTgt;
    for(int i=0;i<min(list1.size(),list2.size());i++)
    {
        MyMesh::VertexHandle vh1 = mesh1->vertex_handle(list1[i]);
        MyMesh::VertexHandle vh2 = mesh2->vertex_handle(list2[i]);
        qDebug()<< "x = " << mesh1->point(vh1)[0];

        deplacement=MyMesh::Point(mesh1->point(vh1)[0]-mesh2->point(vh2)[0],
                mesh1->point(vh1)[1]-mesh2->point(vh2)[1],
                mesh1->point(vh1)[2]-mesh2->point(vh2)[2]);
        vect_deplacement.push_back(deplacement);
    }
}
