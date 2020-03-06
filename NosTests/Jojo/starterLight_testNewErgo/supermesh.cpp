#include "supermesh.h"

#include <QDebug>

SuperMesh::SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt,
                     MyMesh &_meshSrcSphere, MyMesh &_meshTgt_Sphere,
                     MyMesh &_superM, MyMesh::Point &_origin) :
    meshSrc(_meshSrc), meshTgt(_meshTgt),
    meshSrcSphere(_meshSrcSphere), meshTgtSphere(_meshTgt_Sphere),
    superM(_superM), origin(_origin)
{
    qDebug() << "<" << __FUNCTION__ << ">";

    superM = MyMesh(meshTgtSphere);

    qDebug() << "</" << __FUNCTION__ << ">";
}

//SuperMesh::SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt,
//                     MyMesh &_superM, MyMesh::Point &_origin) :
//    meshSrc(_meshSrc), meshTgt(_meshTgt),
//    superM(_superM), origin(_origin)
//{
//    qDebug() << "<" << __FUNCTION__ << ">";

//    superM = MyMesh(meshTgtSphere);

//    qDebug() << "</" << __FUNCTION__ << ">";
//}

MyMesh SuperMesh::get_superM()  {   return superM;  }

/*--------------------------------------------------------------------------
 * Fonction de coordonnees barycentriques.
 * retourne les coordonnes barycentriques du point P dans le triangle ABC.
 * -------------------------------------------------------------------------*/
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

MyMesh::Point SuperMesh::my_coords_barycentrique(MyMesh *_mesh, unsigned numFace, MyMesh::Point P)
{
    VertexHandle vhA, vhB, vhC;
    UtilsMesh::get_vh_of_triangle_face(_mesh, numFace, vhA, vhB, vhC);

    MyMesh::Point A = _mesh->point(vhA);
    MyMesh::Point B = _mesh->point(vhB);
    MyMesh::Point C = _mesh->point(vhC);

    QVector3D vP(P[0], P[1], P[2]);
    QVector3D vA(A[0], A[1], A[2]);
    QVector3D vB(B[0], B[1], B[2]);
    QVector3D vC(C[0], C[1], C[2]);

    vP = coords_barycentre(vA, vB, vC, vP);

    MyMesh::Point newPoint(vP.x(), vP.y(), vP.z());

    return newPoint;
}

/*--------------------------------------------------------------------------
 * retourne le vh le plus proche du point intersecte avec le _mesh
 * qui n'a pas deja ete pris (sauf si tous sont pris)
 * -------------------------------------------------------------------------*/
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

VertexHandle SuperMesh::get_plus_proche_vh_of_triangle_bis(MyMesh *_mesh, int numFace, vector<bool> vertex_used)
{
    VertexHandle vh1, vh2, vh3;
    UtilsMesh::get_vh_of_triangle_face(_mesh, numFace, vh1, vh2, vh3);
    if (!vertex_used[vh1.idx()]) {
        vertex_used[vh1.idx()] = true;
        return vh1;
    }
    else if (!vertex_used[vh2.idx()]) {
        vertex_used[vh2.idx()] = true;
        return vh2;
    }
    else if (!vertex_used[vh3.idx()]) {
        vertex_used[vh3.idx()] = true;
        return vh3;
    }
    else
    {
        int truc = Utils::randInt(1, 3);
        if (truc == 1) {
            return vh1;
        }
        else if (truc == 2) {
            return vh2;
        }
        else if (truc == 3) {
            return vh3;
        }
        else exit(3);
    }
}

/*--------------------------------------------------------------------------
 * Retourne le point à l'instant @t=[0,1] entre @p0 et @p1
 * -------------------------------------------------------------------------*/
MyMesh::Point SuperMesh::my_linear_interpolation(MyMesh::Point p0, MyMesh::Point p1, float t)
{
    if (t<0.f || t>1.f) {
        qWarning() << "@t have to be between 0 & 1: t=" << t;
        exit (2);
    }
    QVector3D vp0(p0[0], p0[1], p0[2]);
    QVector3D vp1(p1[0], p1[1], p1[2]);
    QVector3D vp = UtilsMesh::linear_interpolation(vp0, vp1, t);

    MyMesh::Point p(vp.x(), vp.y(), vp.z());
    return p;
}

/*--------------------------------------------------------------------------
 * Fonction de Raycasting.
 * Vérifie si la droite (@O, @P) intersecte la face @face.
 * -------------------------------------------------------------------------*/
bool SuperMesh::my_intersect(MyMesh *_mesh, int face, MyMesh::Point O, MyMesh::Point P,
                             MyMesh::Point &resP)
{
    MyMesh::Point _p1, _p2, _p3;
    UtilsMesh::get_points_of_triangle_face(_mesh, face, _p1, _p2, _p3);

    QVector3D p1 (_p1[0], _p1[1], _p1[2]);
    QVector3D p2 (_p2[0], _p2[1], _p2[2]);
    QVector3D p3 (_p3[0], _p3[1], _p3[2]);
    QVector3D P1 (O[0], O[1], O[2]);
    QVector3D P2 (P[0], P[1], P[2]);
    QVector3D vResP;

    bool ok = RayCasting::intersect(p1, p2, p3, P1, P2, vResP);
    resP[0]=vResP.x();
    resP[1]=vResP.y();
    resP[2]=vResP.z();
    return ok;
}

/*--------------------------------------------------------------------------
 * Initialise avec les données de @superM, les données dans
 * la structure @myData
 * -------------------------------------------------------------------------*/
void SuperMesh::build_my_data()
{
    qDebug() << "\t<" << __FUNCTION__ << ">";

    vector<int> numVertices;

    myData = MyData(superM.n_vertices());
    for (MyMesh::VertexIter v_it = superM.vertices_begin(); v_it != superM.vertices_end(); v_it++)
    {
        VertexHandle vh = *v_it;
        MyMesh::Point p = superM.point(vh);
        MyMesh::Point pTgt = meshTgt.point(vh);
        MyVertex myVh = MyVertex(vh, p, pTgt, pTgt, VERTEX_ORIGIN);
        myData.vhs.push_back(myVh);
    }

    for (MyMesh::FaceIter f_it = superM.faces_begin(); f_it != superM.faces_end(); f_it++)
    {
        FaceHandle fh = *f_it;
        numVertices.clear();
        vector<VertexHandle> vhs = UtilsMesh::get_vh_of_face(&superM, fh.idx());
        myData.add_face(fh.idx(), vhs, FACE_ORIGIN);
    }

    //myData.display();

    qDebug() << "\t</" << __FUNCTION__ << ">";
}


void SuperMesh::add_pos_vh_split_Tgt_to_Src()
{
    qDebug() << "\t<" << __FUNCTION__ << ">";
    /*** superM -> intersect -> meshSrc -> recup vertex split pos***/

    float epsilon = 0.06f;
    float distMin;
    //    unsigned cpt=0;
    for (MyMesh::VertexIter v_it = superM.vertices_begin(); v_it != superM.vertices_end(); v_it++)
    {
        //        qDebug() << "boucle" << cpt++;
        VertexHandle vh = *v_it;
        //        qDebug() << "vh.idx() = " << vh.idx();
        // qDebug() << "\tsommet" << vh.idx() << "/" << superM.n_vertices();

        int posV = myData.get_pos_of_vh(vh);
        if (posV<0) {
            qWarning() << "pb in" << __FUNCTION__ << ": @vh not in myData -> posV="
                       << posV;
            exit (1);
        }
        if (myData.vhs[posV].type == VERTEX_SPLIT)
        {
            MyMesh::Point p = superM.point(vh);
            distMin = INT_MAX;
            for (MyMesh::FaceIter f_it = meshSrcSphere.faces_begin(); f_it != meshSrcSphere.faces_end(); f_it++)
            {
                FaceHandle fhSrcSphere = *f_it;
                MyMesh::Point newP;
                if (my_intersect(&meshSrcSphere, fhSrcSphere.idx(), origin, p, newP))
                {
                    FaceHandle fh = meshSrcSphere.face_handle(fhSrcSphere.idx());
                    FaceHandle fhSrc = meshSrc.face_handle(fhSrcSphere.idx());
                    float dist = UtilsMesh::distance_point_to_face(&superM, p, fh.idx()); //distance du point p (qui a intersecté) au barycentre de fh
                    if (dist <= epsilon) {
                        // newP = my_coords_barycentrique(&meshSrc, fhSrc.idx(), p);
                        newP = UtilsMesh::barycentre_triangle(&meshSrc, fhSrc.idx());
                        myData.vhs[posV].add_pos_src(newP);
                        break;
                    }
                    else if (dist < distMin) {
                        // newP = my_coords_barycentrique(&meshSrc, fhSrc.idx(), p);
                        newP = UtilsMesh::barycentre_triangle(&meshSrc, fhSrc.idx());
                        myData.vhs[posV].add_pos_src(newP);
                        distMin = dist;
                    }
                }
            }
        }
    }
    qDebug() << "\t</" << __FUNCTION__ << ">";
}

void SuperMesh::handle_intersection(unsigned numFaceSM, MyMesh::Point pSrc,
                                    MyMesh::Point pIntersect, vector<bool> &vertex_used)
{
//    qDebug() << "\t<" << __FUNCTION__ << ">";

//    qDebug() << "\t\tnumFace:" << numFaceSM;
    // Retenir la face ici
    my_split(numFaceSM);

    VertexHandle vh1,vh2,vh3;
    UtilsMesh::get_vh_of_triangle_face(&superM, numFaceSM, vh1, vh2, vh3);

    //on recupere le vh du plus proche du point intersecte avec le supermesh qui n'a pas deja ete pris (sauf si tous sont pris)
    //    VertexHandle vh = get_plus_proche_vh_of_triangle(&superM, vh1, vh2, vh3, pIntersect, vertex_used);
    VertexHandle vh = get_plus_proche_vh_of_triangle_bis(&superM, numFaceSM, vertex_used);

//    qDebug() << "\t\tvh.idx():" << vh.idx();
    vertex_used[vh.idx()]=true;

    // A DEBUGER !!!    Pbs quand par exemple bunnySuperLowPoly en src, et Kitten en tgt
    //    MyMesh::Point pp_point (superM.point(vh));
    int pos_v = myData.get_pos_of_vh(vh);
    if (pos_v<0) {
        qWarning() << "pb in" << __FUNCTION__ << ": @vh not in myData -> pos_v="
                   << pos_v;
        exit (1);
    }
    myData.vhs[static_cast<unsigned>(pos_v)].add_pos_src(pSrc);

//    qDebug() << "\t</" << __FUNCTION__ << ">";
}

void SuperMesh::build()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    float distMin;
    float epsilon = 0.06f;  // Un paramètre pour choisir efficacement une face d'intersection.
    vector<bool> vertex_used(superM.n_vertices()-1, false); // tableau des sommets deja pris pour les plus proches voisins

    build_my_data();

    for (MyMesh::VertexIter v_it = meshSrcSphere.vertices_begin(); v_it != meshSrcSphere.vertices_end(); v_it++)
    {
        VertexHandle vhSrcSphere = *v_it;
        qDebug() << "\tsommet" << vhSrcSphere.idx() << "/" << meshSrcSphere.n_vertices();
        MyMesh::Point pSrcSphere = meshSrcSphere.point(vhSrcSphere);
        distMin = INT_MAX;
        for (MyMesh::FaceIter f_it = superM.faces_begin(); f_it != superM.faces_end(); f_it++)
        {
            FaceHandle fh = *f_it;
            MyMesh::Point newP;
            if (my_intersect(&superM, fh.idx(), origin, pSrcSphere, newP))
            {
                VertexHandle vhSrc = meshSrc.vertex_handle(vhSrcSphere.idx());
                MyMesh::Point pSrc = meshSrc.point(vhSrc);
                float dist = UtilsMesh::distance_point_to_face(&superM, pSrcSphere, fh.idx()); //distance du point p (qui a intersecté) au barycentre de fh
                if (dist <= epsilon) {
                    handle_intersection(fh.idx(), pSrc, newP, vertex_used);
                    break;
                }
                else if (dist < distMin) {
                    distMin = dist;
                    handle_intersection(fh.idx(), pSrc, newP, vertex_used);
                }
            }
        }
    }

    clear_mesh();
    generate_by_myData();

    // myData.display_vertices();

    //add_pos_src du vertex_split qui intersecte le meshSrc depuis le superMesh
    add_pos_vh_split_Tgt_to_Src();

    qDebug() << "</" << __FUNCTION__ << ">";
}

/*--------------------------------------------------------------------------
 * Crée un nouveau point au milieu de l'arête (@posV, @posV2)
 * Ne modifie des données que dans la structure @myData.
 * Le mesh @superM reste inchangé.
 * -------------------------------------------------------------------------*/
unsigned SuperMesh::create_new_point_middle_edge(unsigned posF, unsigned posV, unsigned posV2)
{
    // qDebug() << "\t<" << __FUNCTION__ << ">";

    MyVertex myV = myData.vhs[posV];
    MyVertex myV2 = myData.vhs[posV2];
    MyFace myF = myData.fhs[posF];

    MyMesh::Point p = UtilsMesh::middle_edge(&superM, myV.vh.idx(), myV2.vh.idx());
    MyMesh::Point pTgt = UtilsMesh::middle_edge(&meshTgt, myV.vh.idx(), myV2.vh.idx());

    VertexHandle vh = superM.add_vertex(p);
//    MyVertex myVhNew(vh, pTgt, pTgt, VERTEX_SPLIT);
    MyVertex myVhNew(vh, p, pTgt, pTgt, VERTEX_SPLIT);
    myData.add_vertex(myVhNew);
    superM.delete_vertex(vh, false);
    superM.garbage_collection();
    unsigned idNewV = myData.vhs.size()-1;
    // qDebug() << "vh.idx() = " << myData.vhs[myData.vhs.size()-1].vh.idx();
    myF.add_vertex(idNewV, posV2);  // Insère avant posV2 (et non après)
    myData.update_face(posF, myF);

    // qDebug() << "\t</" << __FUNCTION__ << ">";
    return idNewV;
}

/*--------------------------------------------------------------------------
 * Un split 1-to-4 de la face @face.
 * Ne modifie des données que dans la structure @myData.
 * Le mesh @superM reste inchangé.
 * -------------------------------------------------------------------------*/
void SuperMesh::my_split(int numFace)
{
    //    qDebug() << "<" << __FUNCTION__ << ">";

    VertexHandle vh1, vh2, vh3;
    UtilsMesh::get_vh_of_triangle_face(&superM, numFace, vh1, vh2, vh3);
    vector<unsigned> posFaces;

    posFaces = myData.get_faces_by_original_id(numFace);
    // Si la face est déjà splitée (plusieurs faces venant de la meme face d'origine)
    if (posFaces.size() > 1) {
        //        qDebug() << "</" << __FUNCTION__ << ">";
        return;
    }

    // position de la face dans le tableau de faces de myData
    unsigned posF = posFaces[0];

    // Si face déjà complète (6 sommets)
    if (myData.fhs[posF].size()>=6)   {
        //        qDebug() << "</" << __FUNCTION__ << ">";
        return;
    }

    MyFace myF;
    bool flag_face_complete = false;
    while (!flag_face_complete)
    {
        myF = myData.fhs[posF];
        if (myF.size()>=6)   {
            flag_face_complete = true;
            break;
        }

        // Pour chaque sommet de la face myF
        for (unsigned i=0; i<myF.size(); i++)
        {
            flag_face_complete = true;
            unsigned posV = myF.vertices[i];
            MyVertex myV = myData.vhs[posV];

            // Si sommet courant est nouveau sommet déjà ajouté
            if (myV.type == VERTEX_SPLIT)   {
                // qDebug() << "\tV1 = VERTEX_N";
                continue;
            }
            // Fonction pour gérer liste circulaire
            unsigned posV2 = Utils::get_next_of(myF.vertices, myF.vertices[i]);
            MyVertex myV2 = myData.vhs[posV2];
            // Si sommet suivant est nouveau sommet déjà ajouté
            if (myV2.type == VERTEX_SPLIT) {
                // qDebug() << "\tV2 = VERTEX_N";
                continue;
            }

            // unsigned idNewV = create_new_point_middle_edge(posF, posV, posV2);
            create_new_point_middle_edge(posF, posV, posV2);
            myData.update_faces_neigh(&superM, numFace);

            // Si on arrive ici c'est qu'on a ajouté un nouveau sommet.
            // On doit alors repartir du début pour éviter les problèmes en
            // explorant la même liste augmentée.
            flag_face_complete = false;
            break;
        }
    }

    if (myData.fhs[posF].size()<6)   {
        qWarning() << "Error in" << __FUNCTION__
                   << ": - of 6 vertices";
        exit(1);
    }
    //Il faut split ici
    // myData.trianguler();
    //     myData.trianguler_face(posF);
    //     myData.erase_face(posF);

    //    qDebug() << "</" << __FUNCTION__ << ">";
}

/*--------------------------------------------------------------------------
 * Génère le mesh @superM à l'aide des données dans la struct @myData
 * Attention ne gère pas en avance la présence de données antèrieures
 * dans le mesh ou dans la structure @myData. Assurez vous de bien
 * gérer vous-même les données avant d'appeler cette fonction.
 * -------------------------------------------------------------------------*/
void SuperMesh::generate_by_myData()
{
    qDebug() << "\t<" << __FUNCTION__ << ">";

    for (unsigned v=0; v<myData.vhs.size(); v++)
    {
        VertexHandle vh;
//        //TMP
//        if (myData.vhs[v].type==VERTEX_SPLIT) continue;

        vh = superM.add_vertex(myData.vhs[v].pos);
        myData.vhs[v].vh = vh;
    }
    for (auto f : myData.fhs)
    {
        vector<VertexHandle> vhs;
        vhs.clear();
        for (auto v: f.vertices)
        {
//            // TMP
//            if (myData.vhs[v].type==VERTEX_SPLIT) continue;

            vhs.push_back(myData.vhs[v].vh);
        }
        UtilsMesh::add_face(&superM, vhs);
    }

    superM.update_normals();

    qDebug() << "\t</" << __FUNCTION__ << ">";
}

/*--------------------------------------------------------------------------
 * Vide entièrement @superM
 * -------------------------------------------------------------------------*/
void SuperMesh::clear_mesh()
{
    qDebug() << "\t<" << __FUNCTION__ << ">";
    superM.request_face_status();
    superM.request_edge_status();
    superM.request_vertex_status();

    for (MyMesh::FaceIter curFace = superM.faces_begin(); curFace != superM.faces_end(); curFace++)
    {
        FaceHandle fh = *curFace;
        superM.delete_face(fh, true);
    }
    superM.garbage_collection();

    // Déréférencer les vh.
    for (auto v : myData.vhs) {
        VertexHandle vh;
        v.vh = vh;
    }

    //    for (MyMesh::VertexIter curVert = superM.vertices_begin(); curVert != superM.vertices_end(); curVert++)
    //    {
    //        VertexHandle vh = *curVert;
    //        qDebug() << "vh.idx()=" << vh.idx();
    //    }
    qDebug() << "\t</" << __FUNCTION__ << ">";
}

void SuperMesh::fusion(float t)
{
    qDebug() << "<" << __FUNCTION__ << ">";
    // int cpt=0;
    for (MyMesh::VertexIter v_it = superM.vertices_begin(); v_it != superM.vertices_end(); v_it++)
    {
        // qDebug() << "boucle" << cpt++;
        VertexHandle vh = *v_it;

        int posV = myData.get_pos_of_vh(vh);
        if (posV<0) {
            qWarning() << "pb in" << __FUNCTION__ << ": @vh not in myData -> posV="
                       << posV;
            exit (1);
        }
//        //TMP
//        superM.set_point(vh, origin);
//        continue;
//        if (myData.vhs[posV].type == VERTEX_SPLIT)  {
//            superM.set_point(vh, origin);
//            continue;
//        }

        MyMesh::Point P1 = myData.vhs[posV].posSrc;
        MyMesh::Point P2 = myData.vhs[posV].posTgt;

        MyMesh::Point P = my_linear_interpolation(P1, P2, t);
        superM.set_point(vh, P);
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}






