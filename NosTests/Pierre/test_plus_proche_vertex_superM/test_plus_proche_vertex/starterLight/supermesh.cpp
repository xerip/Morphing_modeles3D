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

QVector3D SuperMesh::coords_barycentre(QVector3D A, QVector3D B, QVector3D C, QVector3D P)
{
    float d = (QVector3D::dotProduct(C-A,C-A)*QVector3D::dotProduct(B-A,B-A))-(QVector3D::dotProduct(C-A,B-A)*QVector3D::dotProduct(B-A,C-A));
    float beta = ((QVector3D::dotProduct(B-A,B-A)*QVector3D::dotProduct(P-A,C-A))-(QVector3D::dotProduct(B-A,C-A)*QVector3D::dotProduct(P-A,B-A)))/d;
    float gamma = ((QVector3D::dotProduct(C-A,C-A)*QVector3D::dotProduct(P-A,B-A))-(QVector3D::dotProduct(C-A,B-A)*QVector3D::dotProduct(P-A,C-A)))/d;
    float alpha = 1-(beta+gamma);
    /*std::cout << "alpha : " << alpha << std::endl;
    std::cout << "beta : " << beta << std::endl;
    std::cout << "gamma : " << gamma << std::endl;*/
    return QVector3D(alpha,beta,gamma);
}

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
                /*float dist = UtilsMesh::distance_point_to_face(&superM, p, fh.idx()); //distance du point p (qui a intersecté) au barycentre de fh
                if (dist <= epsilon) {
                    // Retenir la face ici
                    break;
                }
                else if (dist < distMin) {
                    distMin = dist;
                    // Retenir la face ici
                }*/
                MyMesh::Point A,B,C;
                UtilsMesh::get_points_of_triangle_face(&superM, fh.idx(), A, B, C);
                //QVector3D barycentre = UtilsMesh::to_qvector3D(UtilsMesh::barycentre_triangle(&superM, fh.idx()));

                //alpha = c_bar[0], beta=c_bar[1], gamma=c_bar[2]
                QVector3D c_bar = coords_barycentre(UtilsMesh::to_qvector3D(A), UtilsMesh::to_qvector3D(B), UtilsMesh::to_qvector3D(C), UtilsMesh::to_qvector3D(p));
                qDebug() << c_bar;

                /* test dist p du meshSrc au point plus proche du triangle SuperM
                Prendre plus_proche_vertex_of_triangle(MyMesh mesh,fh.idx(), p)
                Si pp_vertex n'a pas deja ete pris alors on le choisit puis on le met dans un tableau vertex_used
                Sinon si vertex_used == taille 3 alors prendre le pp_vertex
                Sinon prendre le pp_vertex apres celui deja dans le tableau vertex_used
                  */
            }
        }
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}
