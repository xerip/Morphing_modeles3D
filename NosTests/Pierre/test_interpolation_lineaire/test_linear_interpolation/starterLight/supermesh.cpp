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

QVector3D SuperMesh::coords_barycentre(QVector3D A, QVector3D B, QVector3D C, QVector3D P) //coordonnes barycentre du point P dans le triangle ABC
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


VertexHandle SuperMesh::get_plus_proche_vh_of_triangle(MyMesh *_mesh, VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, MyMesh::Point p, vector<bool> vertex_used)
{
    bool b1=vertex_used[vh1.idx()];
    bool b2=vertex_used[vh2.idx()];
    bool b3=vertex_used[vh3.idx()];
    QVector3D P,p1,p2,p3;
    float d1,d2,d3;
    MyMesh::Point A(_mesh->point(vh1));
    MyMesh::Point B(_mesh->point(vh2));
    MyMesh::Point C(_mesh->point(vh3));
    P = UtilsMesh::to_qvector3D(p);
    p1 = UtilsMesh::to_qvector3D(A);
    p2 = UtilsMesh::to_qvector3D(B);
    p3 = UtilsMesh::to_qvector3D(C);
    d1 = P.distanceToPoint(p1);
    d2 = P.distanceToPoint(p2);
    d3 = P.distanceToPoint(p3);
    if (((b1==false)&&(b2==false)&&(b3==false)) || ((b1==true)&&(b2==true)&&(b3==true))) //000 | 111
    {
        if ((d1<=d2) && (d1<=d3)) return vh1;
        else if ((d2<=d1) && (d2<=d3)) return vh2;
        else return vh3;
    }
    else if (((b1==false)&&(b2==false)&&(b3==true))) //001
    {
        if (d1<=d2) return vh1;
        else return vh2;
    }
    else if (((b1==false)&&(b2==true)&&(b3==false))) //010
    {
        if (d1<=d3) return vh1;
        else return vh3;
    }
    else if (((b1==false)&&(b2==true)&&(b3==true))) //011
    {
        return vh1;
    }
    else if (((b1==true)&&(b2==false)&&(b3==false))) //100
    {
        if (d2<=d3) return vh2;
        else return vh3;
    }
    else if (((b1==true)&&(b2==false)&&(b3==true))) //101
    {
        return vh2;
    }
    else //if (((b1==true)&&(b2==false)&&(b3==true))) //110
    {
        return vh3;
    }
}

QVector3D SuperMesh::linear_interpolation(QVector3D p0, QVector3D p1, float t) //retourne le point à l'instant t=[0,1] entre p0 et p1
{
    float xt,yt,zt; // coordonnees du nouveau point
    xt = p0[0] + t*(p1[0]-p0[0]); // = ((1-t)*p0[0] + + t*p1[0])
    yt = p0[1] + t*(p1[1]-p0[1]);
    zt = p0[2] + t*(p1[2]-p0[2]);
    return QVector3D(xt,yt,zt);
}

void SuperMesh::build()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    vector<bool> vertex_used(superM.n_vertices()-1, false);
    for (MyMesh::VertexIter v_it = meshSrc.vertices_begin(); v_it != meshSrc.vertices_end(); v_it++)
    {
        VertexHandle vh = *v_it;
        qDebug() << "\tsommet" << vh.idx() << "/" << meshSrc.n_vertices();
        MyMesh::Point p = meshSrc.point(vh);
        for (MyMesh::FaceIter f_it = superM.faces_begin(); f_it != superM.faces_end(); f_it++)
        {
            FaceHandle fh = *f_it;
            if (my_intersect(&superM, fh.idx(), origin, p)) //p est le point du raycasting intersectant le supermesh
            {
                VertexHandle vh1,vh2,vh3;
                UtilsMesh::get_vh_of_triangle_face(&superM, fh.idx(), vh1, vh2, vh3);

                //vh le plus proche du point intersecte avec le supermesh qui n'a pas deja ete pris (sauf si tous sont pris)
                VertexHandle pp_vertex = get_plus_proche_vh_of_triangle(&superM,vh1,vh2,vh3,p,vertex_used);
                vertex_used[pp_vertex.idx()]=true;


            }
        }
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}
