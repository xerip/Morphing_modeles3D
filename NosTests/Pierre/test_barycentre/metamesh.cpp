#include "metamesh.h"

#include <QDebug>

//////////////////////////////////  Constructeurs   ///////////////////////////////////

MetaMesh::MetaMesh(MyMesh &_mesh, MyMesh::Point &_origin) :
    mesh(_mesh), origin(_origin)
{
    ;
}

MetaMesh::MetaMesh(MyMesh &_mesh, MyMesh::Point &_origin, float _rayon) :
    mesh(_mesh), origin(_origin), rayon(_rayon)
{
    ;
}

vector<float> MetaMesh::_distPointsBase() { return distsPointsBase; }


//////////////////////////////////  Méthodes    ///////////////////////////////////

/*--------------------------------------------------------------------------
 * méthode permettant d'obtenir le point de paramètre @t avec l'équation
 * de la droite (@O, @P)
 * -------------------------------------------------------------------------*/
MyMesh::Point MetaMesh::get_point_of_line(MyMesh::Point O, MyMesh::Point P, float t)
{
    //     P = P1 + t * (P2 - P1)
    MyMesh::Point myP;
    myP[0] = O[0] + t*(P[0] - O[0]);    // x
    myP[1] = O[1] + t*(P[1] - O[1]);    // y
    myP[2] = O[2] + t*(P[2] - O[2]);    // z
    return myP;
}

/*--------------------------------------------------------------------------
 * Déplace le sommet d'indice @vertId de l'origine jusqu'au @rayon
 * (sorte de rayCasting).
 * Déplace sommet de l'origine vers une distance.
 * @precision est une valeur bornée entre 0 et 10 inclus
 * qui permet d'améliorer la projection du point d'id @vertID
 * (peut sensiblement augmenter le temps de calcul)
 * (réglé par défaut à 2)
 * -------------------------------------------------------------------------*/
void MetaMesh::map_to_sphere(int vertId, float rayon, int precision)
{
    if (precision<0)    precision = 0;
    if (precision>10)    precision = 10;
    MyMesh *_mesh = &mesh;
    //precision = true;
    float t = rayon * 0.1f;
    float pas = t;
    VertexHandle vh = _mesh->vertex_handle(vertId);
    MyMesh::Point P = _mesh->point(vh);
    // vecteur OP
    MyMesh::Point vecOP = P - origin;
    //float distOP = vecOP.norm();
    //double distPR = rayon - distOP;

    /////////////// Etape 1 ////////////////////////////
    MyMesh::Point myVec = vecOP;
    // Déplacer myVec vers P jusqu'au rayon R
    while (myVec.norm() < rayon) {
        myVec = get_point_of_line(origin, P, pas);
        pas+=t;
    }

    /////////////// Etape 2 (précision) ////////////////////////////
    double coef =  pow(0.1f, precision);
    if (coef <= 0.0)   {
        qWarning() << "error in " << __FUNCTION__ << ": coef <= 0.f";
        exit (1);
    }
    float t2 = t*coef;
    if (/*precision && */myVec.norm()!=rayon)
    {
        // Déplacer myVec vers origin jusqu'au rayon R
        while (myVec.norm() > rayon) {
            pas-=t2;
            myVec = get_point_of_line(origin, P, pas);
        }
    }

    _mesh->set_point(vh, myVec);
}

/*--------------------------------------------------------------------------
 *  Déplace sommet vers origine jusqu'à ce qu'il ait la @distance
 * souhaitée.
 * -------------------------------------------------------------------------*/
void MetaMesh::demap_from_sphere(int vertId, float distance, int precision)
{
    if (precision<0)    precision = 0;
    if (precision>10)    precision = 10;

    MyMesh *_mesh = &mesh;
    //precision = true;
    float t = -distance * 0.1f;
    float pas = t;
    VertexHandle vh = _mesh->vertex_handle(vertId);
    MyMesh::Point P = _mesh->point(vh);
    MyMesh::Point vecOP = P - origin;

    /////////////// Etape 1 ////////////////////////////
    MyMesh::Point myVec = vecOP;
    // Déplacer myVec vers P jusqu à la distance
    while (myVec.norm() > distance) {
//        myVec = get_point_of_line(origin, P, pas);
        myVec = get_point_of_line(P, origin, pas);
        pas-=t;
    }

    /////////////// Etape 2 (précision) ////////////////////////////
    double coef =  pow(0.1f, precision);
    if (coef <= 0.0)   {
        qWarning() << "error in " << __FUNCTION__ << ": coef <= 0.f";
        exit (1);
    }
    float t2 = t*coef;

    if (/*precision && */myVec.norm()!=distance)
    {
        // Déplacer myVec vers P jusqu'au rayon R
        while (myVec.norm() < distance) {
            pas+=t2;
//            myVec = get_point_of_line(origin, P, pas);
            myVec = get_point_of_line(P, origin, pas);
        }
    }

    _mesh->set_point(vh, myVec);
}

/*--------------------------------------------------------------------------
 * Projette chaque point du maillage sur le point à la distance
 * @distsPointsBase[idPoint] entre lui et l'origin
 * -------------------------------------------------------------------------*/
void MetaMesh::despherization(int precision)
{
    despherization(distsPointsBase, precision);
}

/*--------------------------------------------------------------------------
 * Projette chaque point du maillage sur le point à la distance
 * @distsPB[idPoint] entre lui et l'origin
 * -------------------------------------------------------------------------*/
void MetaMesh::despherization(vector<float> distsPB, int precision)
{
    qDebug() << "<" << __FUNCTION__ << ">";
    qDebug() << "\tprecision +" << precision * 10 << "%";
    MyMesh *_mesh = &mesh;
    if (distsPB.size() != _mesh->n_vertices()) {
        qWarning () << "IN " << __FUNCTION__ << ": @distsPB doesn't match size of number of vertices" ;
        return;
    }
    for (MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++)
    {
        VertexHandle vh = *v_it;
        //map_from_origine_to_distance(vh.idx(), distsPB[vh.idx()], precision);
        demap_from_sphere(vh.idx(), distsPB[vh.idx()], precision);
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}

void MetaMesh::spherization(float _rayon, int precision)
{
    qDebug() << "<" << __FUNCTION__ << ">";
    qDebug() << "\tprecision +" << precision * 10 << "%";
    MyMesh *_mesh = &mesh;
    rayon = _rayon;

    distsPointsBase.clear();
    for (MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++)
    {
        VertexHandle vh = *v_it;

        // Sauvegarde
        MyMesh::Point oldPoint = _mesh->point(vh);
        oldPoint = oldPoint - origin;
        float myDist = fabs(oldPoint.norm());
        //qDebug() << "\tvh " << vh.idx() << " myDist = " << myDist;
        distsPointsBase.push_back(myDist);

        // Projection
        map_to_sphere(vh.idx(), _rayon, precision);
        //pointsBase.push_back(tmpP);
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}

/*--------------------------------------------------------------------------
 * Mini fonction pour éviter des problèmes de taille de rayon
 * -------------------------------------------------------------------------*/
bool MetaMesh::verif_rayon()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    qDebug() << "\trayon =" << rayon ;
    MyMesh *_mesh = &mesh;
    for (MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++)
    {
        VertexHandle vh = *v_it;
        MyMesh::Point p = _mesh->point(vh);
        MyMesh::Point vecOP = p - origin;
        double distOP = vecOP.norm();
        if (distOP > rayon) {
            qDebug() << "\tdistOP =" << distOP ;
            qDebug() << "</" << __FUNCTION__ << ">";
            return false;
        }
    }
    qDebug() << "</" << __FUNCTION__ << ">";
    return true;
}
