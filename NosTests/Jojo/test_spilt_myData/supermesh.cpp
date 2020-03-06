#include "supermesh.h"

#include <QDebug>

//SuperMesh::SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt, MyMesh::Point &_origin) :
//    meshSrc(_meshSrc), meshTgt(_meshTgt), origin(_origin)
//{
//    qDebug() << "<" << __FUNCTION__ << ">";

//    // test();
//    superM = MyMesh(meshTgt);

//    qDebug() << "</" << __FUNCTION__ << ">";
//}

SuperMesh::SuperMesh(MyMesh &mesh, MyMesh::Point &_origin) :
    superM(mesh), origin(_origin)
{
    qsrand(time(NULL));
    build();
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

void SuperMesh::build_my_data()
{
    qDebug() << "\t<" << __FUNCTION__ << ">";

    vector<int> numVertices;

    myData = MyData(superM.n_vertices());
    for (MyMesh::VertexIter v_it = superM.vertices_begin(); v_it != superM.vertices_end(); v_it++)
    {
        VertexHandle vh = *v_it;
        MyMesh::Point p = superM.point(vh);
        MyVertex myVh = MyVertex(vh, p, p, VERTEX_ORIGIN);
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

void SuperMesh::build()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    float distMin;
    float epsilon = 0.06f;

    build_my_data();

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

//void SuperMesh::rectangle_to_triangle(int faceID, int vhSplitID)
//{
//    qDebug() << "\t\t<" << __FUNCTION__ << ">";
////    if (! UtilsMesh::vertex_is_in_face(&superM, faceID, vhSplitID)) {
////        qWarning() << "Warning in " << __FUNCTION__ << ": @vertexSplit has to"
////                                               " be part of the face @faceID";
////        qDebug() << "\t\t</" << __FUNCTION__ << ">";
////        return;
////    }


//    // V2
//    FaceHandle fh = superM.face_handle(faceID);
//    VertexHandle vhSplit = superM.vertex_handle(vhSplitID);
//    vector<VertexHandle> vhs;
//    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
//    {
//        VertexHandle vhTmp = *fv_it;
//        vhs.push_back(vhTmp);
//    }
//    superM.set_color(fh, MyMesh::Color(150, 150, 150));
//    superM.split_copy(fh, vhSplit);

//    // V1
////    FaceHandle fh = superM.face_handle(faceID);
////    VertexHandle vhSplit = superM.vertex_handle(vhSplitID);

////    int cpt=0;
////    int r, v, b;
////    vector<VertexHandle> vhs;
////    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
////    {
////        VertexHandle vhTmp = *fv_it;
////        if (vhTmp.idx() != vhSplitID) {
//////            if (cpt ==1) {  r=255; v=0; b=0;    }
//////            if (cpt ==2) {  r=0; v=255; b=0;    }
//////            if (cpt ==3) {  r=0; v=0; b=255;    }
//////            superM.set_color(vhTmp, MyMesh::Color(r, v, b));
//////            superM.data(vhTmp).thickness = 8;
////            vhs.push_back(vhTmp);
////        }
////        cpt++;
////    }

////    // Erase
////    superM.delete_face(fh, false);
////    superM.garbage_collection();

////    //    UtilsMesh::add_triangle_face(&superM, vhs[0], vhSplit, vhs[1]);
////    //    UtilsMesh::add_triangle_face(&superM, vhSplit, vhs[1], vhs[2]);

////    superM.set_color(vhSplit, MyMesh::Color(250, 0, 250));
////    vector<VertexHandle> tmps = {VertexHandle(1), VertexHandle(2), VertexHandle(6)};
////    UtilsMesh::add_triangle_face(&superM, vhSplit, tmps[1], tmps[2]);
//////    UtilsMesh::add_triangle_face(&superM, tmps[0], vhSplit, tmps[2]);

////    // TMP COLOR
////    superM.set_color(superM.face_handle(superM.n_faces()-1), MyMesh::Color(150, 150, 150));
////    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
////    {
////        superM.data(*curEdge).thickness = 1;
////        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
////    }


//    qDebug() << "\t\t</" << __FUNCTION__ << ">";
//}

///*--------------------------------------------------------------------------
// * Retriangule les faces voisines de la face @faceID.
// * ===================================================================
// * @fhPs contient les 3 faces voisines à @faceID.
// * @vhPs contient les nouveaux sommets de la face @faceID.
// * Attention:
// *  - La face @faceID doit encore exister dans le mesh.
// * -------------------------------------------------------------------------*/
//void SuperMesh::retriangle_neighbours(int faceID,
//                                      vector<FaceHandle> fhPs,
//                                      vector<VertexHandle> vhPs)
//{
//    qDebug() << "\t<" << __FUNCTION__ << ">";

//    if (fhPs.size() > 3) {
//        qWarning() << "Warning in "
//                   << __FUNCTION__
//                   << ": @faceID can't have more of 3 neighbours faces (in @fhPs)";
//        qDebug() << "\t\t</" << __FUNCTION__ << ">";
//        return;
//    }

//// TEST
////    superM.set_color(fhPs[1], MyMesh::Color(100, 100, 100));
////    superM.data(vhPs[1]).thickness = 8;
////    rectangle_to_triangle(fhPs[1].idx(), vhPs[1].idx());

//    int cpt=0;
//    for (unsigned i=0; i<fhPs.size(); i++)
//    {
////        superM.set_color(vhPs[i], MyMesh::Color(cpt*100, cpt*100, cpt*100));
//        superM.set_color(fhPs[i], MyMesh::Color(cpt*100, cpt*100, cpt*100));
//        superM.data(vhPs[i]).thickness = 8;

//        rectangle_to_triangle(fhPs[i].idx(), vhPs[i].idx());

////        //Erase
////        superM.delete_face(fhPs[i], false);
////        superM.garbage_collection();

//        cpt++;
//    }


////    for (auto fh : fhPs)
////    {
////        for (unsigned i=0; i<ehPs.size(); i++)
////        {
////            if (UtilsMesh::edge_is_in_face(&superM, ehPs[i].idx(), fh.idx())) {
////                rectangle_to_triangle(fh.idx(), vhPs[i].idx());
////                // Erase
////                superM.delete_face(fh, false);
////                superM.garbage_collection();
////            }
////        }
////    }

//    // OLD
////    FaceHandle fhBase = superM.face_handle(faceID);
////    for (MyMesh::FaceFaceCWIter ff_it = superM.ff_cwiter(fhBase); ff_it.is_valid(); ff_it++)
////    {
////        FaceHandle fhNeigh = *ff_it;
////        qDebug() << "\t\tface" << fhNeigh.idx() << "found";

////        for (unsigned i=0; i<ehPs.size(); i++) {
////            if (UtilsMesh::edge_is_in_face(&superM, ehPs[i].idx(), fhNeigh.idx())) {
////                    rectangle_to_triangle(fhNeigh.idx(), vhPs[i].idx());
////            }
////        }

//////        // ATTENTION -> Utile !?
//////        if (UtilsMesh::faces_have_common_edge(&superM, faceID, fhNeigh.idx()))
//////        {
//////            qDebug() << "\t\t\tface" << fhNeigh.idx() << "OK";
//////        }
////    }

//    qDebug() << "\t</" << __FUNCTION__ << ">";
//}

///*------------------------------------------------------------------------------
// * Renvoi les 3 faces adjacentes aux 3 arêtes du triangle @numFace.
// * ----------------------------------------------------------------------------*/
//vector<FaceHandle> SuperMesh::get_3_neighbour_faces(int numFace,
//                                      EdgeHandle eh1, EdgeHandle eh2, EdgeHandle eh3)
//{
//    vector<FaceHandle> vhFaces;
//    FaceHandle fh;
//    int faceNeighID;

//    // EH1
//    faceNeighID = UtilsMesh::get_opposite_edgeFace(&superM, numFace, eh1.idx());
//    if (faceNeighID >= 0) {
//        fh = superM.face_handle(faceNeighID);
////        superM.set_color(fh, MyMesh::Color(0, 255, 0));
//        vhFaces.push_back(fh);
//    }
//    // EH2
//    faceNeighID = UtilsMesh::get_opposite_edgeFace(&superM, numFace, eh2.idx());
//    if (faceNeighID >= 0) {
//        fh = superM.face_handle(faceNeighID);
////        superM.set_color(fh, MyMesh::Color(0, 255, 0));
//        vhFaces.push_back(fh);
//    }
//    // EH3
//    faceNeighID = UtilsMesh::get_opposite_edgeFace(&superM, numFace, eh3.idx());
//    if (faceNeighID >= 0) {
//        fh = superM.face_handle(faceNeighID);
////        superM.set_color(fh, MyMesh::Color(0, 255, 0));
//        vhFaces.push_back(fh);
//    }

//    return vhFaces;
//}

//void SuperMesh::split_1_to_4(int numFace,
//                             vector<VertexHandle> vhs,
//                             vector<VertexHandle> vhPs)
//{
//    FaceHandle fh = superM.face_handle(numFace);

//    // Erase
//    fh = superM.face_handle(numFace);
//    superM.delete_face(fh, false);
//    superM.garbage_collection();

//    // Centrale
//    UtilsMesh::add_triangle_face(&superM, vhPs[0], vhPs[1], vhPs[2]);
//    // Autres
//    UtilsMesh::add_triangle_face(&superM, vhs[0], vhPs[0], vhPs[2]);
////    UtilsMesh::add_triangle_face(&superM, vheh2A, vhP2, vhP1);
////    UtilsMesh::add_triangle_face(&superM, vheh3A, vhP3, vhP2);
//}

//void SuperMesh::split_edges(vector<VertexHandle> vhPs, vector<int> numEdges)
//{
//    vector<EdgeHandle> ehPs(3);
//    // On fait cela avant car l'ajout d'aretes pourrait changer les id des edges.
//    for (unsigned i=0; i<numEdges.size(); i++) {
//        if (numEdges[i]>=0) ehPs[i] = superM.edge_handle(numEdges[i]);
//    }

//// TMP
//int i = 1;
//if (numEdges[i]>=0) {
//    UtilsMesh::add_vertex_to_edge(&superM, ehPs[i], vhPs[i]);
//    superM.garbage_collection();
//}

////    for (unsigned i=0; i<numEdges.size(); i++)
////    {
////        // Si l'arete existe et que le vhP correspondant n'est pas encore ajouté aux faces
////        if (numEdges[i]>=0) {
////            UtilsMesh::add_vertex_to_edge(&superM, ehPs[i], vhPs[i]);
////            superM.garbage_collection();
////        }
////    }


////    EdgeHandle eh1, eh2, eh3;
////    UtilsMesh::get_eh_of_triangle_face(&superM, 0, eh1, eh2, eh3);

////    MyMesh::Point myP1 = UtilsMesh::middle_edge(&superM, eh1.idx());
////    vhP1 = superM.add_vertex(myP1);
////    MyMesh::Point myP2 = UtilsMesh::middle_edge(&superM, eh2.idx());
////    vhP2 = superM.add_vertex(myP2);
////    MyMesh::Point myP3 = UtilsMesh::middle_edge(&superM, eh3.idx());
////    vhP3 = superM.add_vertex(myP3);

////    superM.split_edge_copy(eh1, vhP1);
////    superM.split_edge_copy(eh2, vhP2);
////    superM.split_edge_copy(eh3, vhP3);

////    superM.garbage_collection();
//}

///*--------------------------------------------------------------------
// * Identifie à quel sommet de @vhs, le sommet @vhNewPoint est rattaché.
// * Renvoi l'id dans vhs.
// * ------------------------------------------------------------------*/
//int SuperMesh::identify_middle_point(FaceHandle fh,
//                                   VertexHandle vhNewPoint,
//                                   vector<VertexHandle> vhs)
//{
//    qDebug() << "\t\t<" << __FUNCTION__ << ">";
//    bool first = true;
//    bool rencontre = false;
//    VertexHandle tmp1, tmp2;
//    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
//    {
//        VertexHandle vhParcours = *fv_it;

//        // On rencontre le sommet cherché.
//        if (vhParcours == vhNewPoint) rencontre = true;

//        if (first && Utils::is_in_vector(vhs, vhParcours)) {
//            tmp2 = vhParcours;
//            first = false;
//        }

//        if (rencontre) {
//            if (Utils::is_in_vector(vhs, vhParcours)) {
//                tmp2 = vhParcours;
//                break;
//            }
//        }
//        else {
//            if (Utils::is_in_vector(vhs, vhParcours)) {
//                tmp1 = vhParcours;
//            }
//        }
//    }

//    // chercher correspondances
//    unsigned i=0;
//    for (; i<vhs.size(); i++) {
//        if (tmp1 == vhs[i]) {    break;   }
//    }
//    unsigned j=0;
//    for (; j<vhs.size(); j++) {
//        if (tmp2 == vhs[j]) {    break;   }
//    }

//    qDebug() << "\t\t\tvhP[" << i << "] identified";
//    qDebug() << "\t\t</" << __FUNCTION__ << ">";

//    if (i<j)    return i;
//    else        return j;
//}

///*----------------------------------------------------------------------
// * Cré un point au milieu de l'arete correspondante à @vh et qui
// * se trouve dans la face contenant les sommes dans @vhs.
// * Ne cré le point que s'il n'existe pas.
// * ATTENTION: Ne splite pas les aretes, le nouveau point n'est pas
// *  ajouté dans la face.
// * ====================================
// * Renvoi le numéro de l'arete si un vertex est créé à partir d'elle.
// * sinon -1.
// * --------------------------------------------------------------------*/
//int SuperMesh::create_new_point_middle_edge(VertexHandle vh,
//                                             VertexHandle &newVh,
//                                             int &idVhs,
//                                             vector<VertexHandle> vhs,
//                                             vector<int> sommetsRajoutes)
//{
//    qDebug() << "\t\t<" << __FUNCTION__ << ">";
//    int numEdge = -1;
//    MyMesh::Point P;
//    for (MyMesh::VertexEdgeCWIter ve_it = superM.ve_cwiter(vh); ve_it.is_valid(); ve_it++)
//    {
//        EdgeHandle eh = *ve_it;
//        VertexHandle vhA, vhB;
//        UtilsMesh::get_vh_of_edge(&superM, eh.idx(), vhA, vhB);
//        if (vhA != vh) {    Utils::permut(vhA, vhB);    }
//        // identifier vhA
//        unsigned i=0;
//        for (; i<vhs.size(); i++) {
//            if (vhA == vhs[i]) {    break;   }
//        }
//        // Si vhB est un vh original à la suite de vhA
//        int j;
//        if (i==vhs.size()-1) {j=0;}
//        else {j=i+1;}
//        if (vhB == vhs[j])
//        {
//            P = UtilsMesh::middle_edge(&superM, eh.idx());
//            newVh = superM.add_vertex(P);
//            idVhs = i;
//            //vhPTmp[i] = newVh;
//            qDebug() << "\t\t\tvhP[" << i << "] add!";
//            numEdge = eh.idx();
//        // TMP
//            if (vh.idx() == 1)
//            {
//                superM.data(vhA).thickness = 8;
//                superM.set_color(vhA, MyMesh::Color(255, 0, 0));
//                superM.data(vhB).thickness = 8;
//                superM.set_color(vhB, MyMesh::Color(255, 0, 0));
//                superM.data(eh).thickness = 10;
//                superM.set_color(eh, MyMesh::Color(0, 0, 255));
//            }
//            goto end;
//            break;
//        }
//    }
//    qDebug() << "\t\t\tnot add...";
//    numEdge = -1;
//    end:
//    qDebug() << "\t\t</" << __FUNCTION__ << ">";
//    return numEdge;
//}

///*-----------------------------------------------------------------------------
// * Identifie les nouveaux points (point milieu) déjà ajoutés à la face @numFace,
// * et rajoute dans le mesh ceux qui n'ont pas encore été créés.
// * Renvoi @vhPs, le tableau contenant les vhP (new sommets) dans l'ordre
// * correspondant à @vhs (les sommets de la face originale).
// * ATTENTION: Ne splite pas les aretes, les nouveaux points ne sont pas
// *  ajoutés dans la face, mais seulement dans le mesh.
// * ---------------------------------------------------------------------------*/
//void SuperMesh::add_split_points(int numFace,
//                                 vector<VertexHandle> vhs,
//                                 vector<VertexHandle> &vhPs,
//                                 vector<int> &numEdges,
//                                 vector<int> &sommetsRajoutes)
//{
//    qDebug() << "\t<" << __FUNCTION__ << ">";
//    FaceHandle fh = superM.face_handle(numFace);
//    vector<VertexHandle> vhPTmp(3);
//    vector<int> numEdgesTmp(3, -1);
//    MyMesh::Point P;
//    VertexHandle newVh;

//    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
//    {
//        VertexHandle vh = *fv_it;
//        qDebug() << "\t\tvh.idx() = " << vh.idx();

//        // Si vh est un nouveau sommet déjà ajouté (vhP).
//        if (Utils::is_in_vector(sommetsRajoutes, vh.idx()))
//        {
//            int id = identify_middle_point(fh, vh, vhs);
//            vhPTmp[id] = vh;
//        }
//        else
//        {
//            int id;
//            VertexHandle newVh;
//            int numEdge;
//            numEdge = create_new_point_middle_edge(vh, newVh, id, vhs, sommetsRajoutes);
//            if (numEdge>=0)
//                numEdgesTmp[id] = numEdge;

//            vhPTmp[id] = newVh;
//            sommetsRajoutes.push_back(newVh.idx());
//        }
//    }
//    vhPs = vhPTmp;
//    numEdges = numEdgesTmp;
//    qDebug() << "\t</" << __FUNCTION__ << ">";
//}

unsigned SuperMesh::create_new_point_middle_edge2(unsigned posF, unsigned posV, unsigned posV2)
{
    qDebug() << "\t<" << __FUNCTION__ << ">";

    MyVertex myV = myData.vhs[posV];
    MyVertex myV2 = myData.vhs[posV2];
    MyFace myF = myData.fhs[posF];

    MyMesh::Point p = UtilsMesh::middle_edge(&superM, myV.vh.idx(), myV2.vh.idx());
    VertexHandle vh = superM.add_vertex(p);
    MyVertex myVhNew(vh, p, p, VERTEX_SPLIT);
    myData.add_vertex(myVhNew);
    superM.delete_vertex(vh, false);
    superM.garbage_collection();
    unsigned idNewV = myData.vhs.size()-1;
    // qDebug() << "vh.idx() = " << myData.vhs[myData.vhs.size()-1].vh.idx();
    myF.add_vertex(idNewV, posV2);  // Insère avant posV2 (et non après)
    myData.update_face(posF, myF);

    qDebug() << "\t</" << __FUNCTION__ << ">";
    return idNewV;
}

void SuperMesh::my_split2(int numFace)
{
    qDebug() << "<" << __FUNCTION__ << ">";

    VertexHandle vh1, vh2, vh3;
    UtilsMesh::get_vh_of_triangle_face(&superM, numFace, vh1, vh2, vh3);
    vector<unsigned> posFaces;

    posFaces = myData.get_faces_by_original_id(numFace);
    // Si la face est déjà splitée (plusieurs faces venant de la meme face d'origine)
    if (posFaces.size() > 1) {
        qDebug() << "</" << __FUNCTION__ << ">";
        return;
    }

    // position de la face dans le tableau de faces de myData
    unsigned posF = posFaces[0];

    // Si face déjà complète (6 sommets)
    if (myData.fhs[posF].size()>=6)   {
        qDebug() << "</" << __FUNCTION__ << ">";
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

            unsigned idNewV = create_new_point_middle_edge2(posF, posV, posV2);
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

    qDebug() << "</" << __FUNCTION__ << ">";
}

void SuperMesh::generate_by_myData()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    for (int v=0; v<myData.vhs.size(); v++)
    {
        VertexHandle vh;
        vh = superM.add_vertex(myData.vhs[v].posTgt);
        myData.vhs[v].vh = vh;
    }
    for (auto f : myData.fhs)
    {
        vector<VertexHandle> vhs;
        vhs.clear();
        for (auto v: f.vertices) {
            vhs.push_back(myData.vhs[v].vh);
        }
        UtilsMesh::add_face(&superM, vhs);
    }


//    for (auto f : myData.fhs)
//    {
//        vector<VertexHandle> vhs;
//        vhs.clear();
//        VertexHandle vh;

//        for (auto v: f.vertices)
//        {
//            vhs.clear();
//            if (myData.vhs[v].vh.idx()<0) {
//                vh = superM.add_vertex(myData.vhs[v].posTgt);
//                myData.vhs[v].vh = vh;
//            }
//            vhs.push_back(myData.vhs[v].vh);
//        }
//        UtilsMesh::add_face(&superM, vhs);
//    }

    superM.update_normals();

    qDebug() << "</" << __FUNCTION__ << ">";
}

void SuperMesh::clear_mesh()
{
    qDebug() << "<" << __FUNCTION__ << ">";
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

    for (MyMesh::VertexIter curVert = superM.vertices_begin(); curVert != superM.vertices_end(); curVert++)
    {
        VertexHandle vh = *curVert;
        qDebug() << "vh.idx()=" << vh.idx();
    }
    qDebug() << "</" << __FUNCTION__ << ">";
}

//void SuperMesh::my_split(int numFace, vector<int> &sommetsRajoutes)
//{
//    qDebug() << "<" << __FUNCTION__ << ">";
//    VertexHandle vh1, vh2, vh3;
//    VertexHandle vhP1, vhP2, vhP3;
//    EdgeHandle eh1, eh2, eh3;
//    FaceHandle fh;
//    vector<FaceHandle> oldFaces;
//    fh = superM.face_handle(numFace);

//// TMP
//    qDebug() << "\tsommetRajoutes:";
//    for (auto n : sommetsRajoutes) {
//        qDebug() << "\tvh.idx() = " << n;
//    }
//    qDebug() << "\tsommetFace:";
//    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
//    {
//        VertexHandle vh = *fv_it;
//        qDebug() << "\tvh.idx() = " << vh.idx();
//    }

//    for (MyMesh::FaceFaceCWIter ff_it = superM.ff_cwiter(fh); ff_it.is_valid(); ff_it++)
//    {
//        FaceHandle fhTmp = *ff_it;
//        oldFaces.push_back(fhTmp);
//    }

//    // Trouver les vh de base du triangle d'origine
//    int cptV = 1;
//    int cptP = 1;
//    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
//    {
//        VertexHandle vh = *fv_it;
//        if (! Utils::is_in_vector(sommetsRajoutes, vh.idx()))
//        {
//            if (cptV==1)         vh1 = *fv_it;
//            else if (cptV==2)    vh2 = *fv_it;
//            else if (cptV==3)    vh3 = *fv_it;
//            cptV++;
//        }
//    }

//    // Ajouter les sommets qui coupent une arête (s'ils ne sont pas déjà présents)
//    qDebug() << "\tAlgo:";
//    vector<VertexHandle> vhPs;
//    vector<int> numEdges;
//    add_split_points(numFace, vector<VertexHandle>{vh1, vh2, vh3}, vhPs, numEdges, sommetsRajoutes);
//    vhP1 = vhPs[0];
//    vhP2 = vhPs[1];
//    vhP3 = vhPs[2];

//    // Ajouter ces sommets au sein des arêtes
//    split_edges(vhPs, numEdges);

//    // COLOR
//    int thick = 8;
////    for (MyMesh::VertexIter curVert = superM.vertices_begin(); curVert != superM.vertices_end(); curVert++)
////    {
////        superM.data(*curVert).thickness = 8;
////        superM.set_color(*curVert, MyMesh::Color(0, 0, 0));
////    }
////    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
////    {
//////        EdgeHandle eh = *curEdge;
//////        if (eh.idx() == 1)  continue;
////        superM.data(*curEdge).thickness = 1;
////        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
////    }
//    for (MyMesh::FaceIter curFace = superM.faces_begin(); curFace != superM.faces_end(); curFace++)
//    {
//        superM.set_color(*curFace, MyMesh::Color(150, 150, 150));
//    }
////    superM.set_color(vhP1, MyMesh::Color(255, 0, 0));
////    superM.set_color(vhP2, MyMesh::Color(0, 255, 0));
////    superM.set_color(vhP3, MyMesh::Color(0, 0, 255));
////    superM.data(vhP1).thickness = thick;
////    superM.data(vhP2).thickness = thick;
////    superM.data(vhP3).thickness = thick;

////    int cpt = 1;
////    VertexHandle vhTest1, vhTest2;
    
//    //UtilsMesh::get_vh_of_edge(&superM, eh.idx(), vhTest1, vhTest2);


////    // Obtenus avec ClockWise (sens aiguilles d'une montre)
////    UtilsMesh::get_vh_of_triangle_face(&superM, 0, vh1, vh2, vh3);
////    UtilsMesh::get_eh_of_triangle_face(&superM, 0, eh1, eh2, eh3);

//    // obtenues donc dans le même ordre que les nouveaux points/vertex
//    //vhFaces = get_3_neighbour_faces(numFace, eh1, eh2, eh3);


//// TMP
////    return;

////    split_edges(vhP1, vhP2, vhP3);

////    split_1_to_4(numFace,
////                 vector<VertexHandle>{vh1, vh2, vh3},
////                 vector<VertexHandle>{vhP1, vhP2, vhP3});

////    // COLOR
////    int thick = 8;
////    for (MyMesh::VertexIter curVert = superM.vertices_begin(); curVert != superM.vertices_end(); curVert++)
////    {
////        superM.data(*curVert).thickness = 1;
////        superM.set_color(*curVert, MyMesh::Color(0, 0, 0));
////    }
////    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
////    {
////        superM.data(*curEdge).thickness = 1;
////        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
////    }
////    for (MyMesh::FaceIter curFace = superM.faces_begin(); curFace != superM.faces_end(); curFace++)
////    {
////        superM.set_color(*curFace, MyMesh::Color(150, 150, 150));
////    }
////    superM.set_color(vhP1, MyMesh::Color(255, 0, 0));
////    superM.set_color(vhP2, MyMesh::Color(0, 255, 0));
////    superM.set_color(vhP3, MyMesh::Color(0, 0, 255));
////    superM.data(vhP1).thickness = thick;
////    superM.data(vhP2).thickness = thick;
////    superM.data(vhP3).thickness = thick;


//    // OLD

////    superM.split(fh);
////    vector<FaceHandle> fhSplit;
////    fhSplit.push_back(superM.face_handle(superM.n_faces()-1));
////    fhSplit.push_back(superM.face_handle(superM.n_faces()-2));
////    fhSplit.push_back(superM.face_handle(superM.n_faces()-3));
////    fhSplit.push_back(superM.face_handle(superM.n_faces()-4));

////    // COLOR
////    int thick = 8;
////    for (MyMesh::VertexIter curVert = superM.vertices_begin(); curVert != superM.vertices_end(); curVert++)
////    {
////        superM.data(*curVert).thickness = 1;
////        superM.set_color(*curVert, MyMesh::Color(0, 0, 0));
////    }
////    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
////    {
////        superM.data(*curEdge).thickness = 1;
////        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
////    }
////    for (MyMesh::FaceIter curFace = superM.faces_begin(); curFace != superM.faces_end(); curFace++)
////    {
////        superM.set_color(*curFace, MyMesh::Color(150, 150, 150));
////    }


////    for (auto f : fhSplit)
////    {
////        superM.set_color(f, MyMesh::Color(0, 255, 0));
//////        for (MyMesh::FaceFaceCWIter ff_it = superM.ff_cwiter(f); ff_it.is_valid(); ff_it++)
//////        {
//////            FaceHandle fhTmp = *ff_it;
//////            // Si face n'est pas une partie de l'ancienne face splitée
//////            if (! Utils::is_in_vector(fhSplit, fhTmp))
//////            {
//////                // Erase
//////                superM.delete_face(fhTmp, false);
//////            }
//////        }
////    }
////    superM.garbage_collection();

//////    for (auto f : oldFaces)
//////    {
//////        ;
//////    }

//    qDebug() << "<\t" << __FUNCTION__ << ">";
//}

//void SuperMesh::old_split()
//{
//    qDebug() << "<" << __FUNCTION__ << ">";

//    int numFace = 0;
//    FaceHandle fh1 = superM.face_handle(numFace);
//    superM.split(fh1);

//// TMP
//    return;



//    VertexHandle vh1, vh2, vh3;
//    VertexHandle vhP1, vhP2, vhP3;
//    EdgeHandle eh1, eh2, eh3;
//    FaceHandle fh;
//    vector<FaceHandle> vhFaces;

//    //vhFaces = UtilsMesh::get_neighbour_faces(&superM, numFace);

//    // Obtenus avec ClockWise (sens aiguilles d'une montre)
//    UtilsMesh::get_vh_of_triangle_face(&superM, 0, vh1, vh2, vh3);
//    UtilsMesh::get_eh_of_triangle_face(&superM, 0, eh1, eh2, eh3);
//    MyMesh::Point myP1 = UtilsMesh::middle_edge(&superM, eh1.idx());
//    vhP1 = superM.add_vertex(myP1);
//    MyMesh::Point myP2 = UtilsMesh::middle_edge(&superM, eh2.idx());
//    vhP2 = superM.add_vertex(myP2);
//    MyMesh::Point myP3 = UtilsMesh::middle_edge(&superM, eh3.idx());
//    vhP3 = superM.add_vertex(myP3);

//    // obtenues donc dans le même ordre que les nouveaux points/vertex
//    vhFaces = get_3_neighbour_faces(numFace, eh1, eh2, eh3);
//    // qDebug() << "\tvhFaces.size() =" << vhFaces.size();


//    // Réglages
//    VertexHandle vheh1A, vheh1B;
//    // Attention au sens de ces 2 vertex!
//    UtilsMesh::get_vh_of_edge(&superM, eh1.idx(), vheh1A, vheh1B);
//    if (vheh1A == vh2) {
//        Utils::permut(vheh1A, vheh1B);
//        qDebug() << "\tPermut 1";
//    }
//    VertexHandle vheh2A, vheh2B;
//    // Attention au sens de ces 2 vertex!
//    UtilsMesh::get_vh_of_edge(&superM, eh2.idx(), vheh2A, vheh2B);
//    if (vheh2A == vh3) {
//        Utils::permut(vheh2A, vheh2B);
//        qDebug() << "\tPermut 2";
//    }
//    VertexHandle vheh3A, vheh3B;
//    // Attention au sens de ces 2 vertex!
//    UtilsMesh::get_vh_of_edge(&superM, eh3.idx(), vheh3A, vheh3B);
//    if (vheh3A == vh1) {
//        Utils::permut(vheh3A, vheh3B);
//        qDebug() << "\tPermut 3";
//    }
//    // COLOR
//    //    superM.set_color(vheh1A, MyMesh::Color(255, 0, 0));
//    //    superM.data(vheh1A).thickness = 8;
//    //    superM.set_color(vheh2A, MyMesh::Color(0, 255, 0));
//    //    superM.data(vheh2A).thickness = 8;
//    //    superM.set_color(vheh3A, MyMesh::Color(0, 0, 255));
////    superM.set_color(vh1, MyMesh::Color(255, 0, 0));
////    superM.data(vh1).thickness = 8;
////    superM.set_color(vh2, MyMesh::Color(0, 255, 0));
////    superM.data(vh2).thickness = 8;
////    superM.set_color(vh3, MyMesh::Color(0, 0, 255));
////    superM.data(vh3).thickness = 8;

//    superM.data(eh1).thickness = 1;
//    superM.set_color(eh1, MyMesh::Color(255, 0, 0));
//    superM.data(eh2).thickness = 1;
//    superM.set_color(eh2, MyMesh::Color(0, 255, 0));
//    superM.data(eh3).thickness = 1;

//    superM.set_color(eh3, MyMesh::Color(0, 0, 255));
//    superM.set_color(vhP1, MyMesh::Color(255, 0, 0));
//    superM.set_color(vhP2, MyMesh::Color(0, 255, 0));
//    superM.set_color(vhP3, MyMesh::Color(0, 0, 255));
//    superM.data(vhP1).thickness = 8;
//    superM.data(vhP2).thickness = 8;
//    superM.data(vhP3).thickness = 8;

//    // Erase
//    fh = superM.face_handle(numFace);
//    superM.delete_face(fh, false);
//    superM.garbage_collection();

//    // Centrale
//    UtilsMesh::add_triangle_face(&superM, vhP1, vhP2, vhP3);
//    // Autres
//    UtilsMesh::add_triangle_face(&superM, vheh1A, vhP1, vhP3);
//    UtilsMesh::add_triangle_face(&superM, vheh2A, vhP2, vhP1);
//    UtilsMesh::add_triangle_face(&superM, vheh3A, vhP3, vhP2);

//    // RETRIANGLE
//    retriangle_neighbours(numFace,
//                          vhFaces,
//                          vector<VertexHandle>{vhP1, vhP2, vhP3});

//    // COLOR
//    int thick = 8;
//    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
//    {
//        superM.data(*curEdge).thickness = 1;
//        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
//    }
////    superM.set_color(vh1, MyMesh::Color(255, 0, 0));
////    superM.set_color(vh2, MyMesh::Color(255, 0, 0));
////    superM.set_color(vh3, MyMesh::Color(255, 0, 0));
////    superM.data(vh1).thickness = thick;
////    superM.data(vh2).thickness = thick;
////    superM.data(vh3).thickness = thick;
////    superM.set_color(vhP1, MyMesh::Color(255, 0, 0));
////    superM.set_color(vhP2, MyMesh::Color(0, 255, 0));
////    superM.set_color(vhP3, MyMesh::Color(0, 0, 255));
////    superM.data(vhP1).thickness = thick;
////    superM.data(vhP2).thickness = thick;
////    superM.data(vhP3).thickness = thick;


//    qDebug() << "</" << __FUNCTION__ << ">";
//}


//void SuperMesh::test (VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, VertexHandle vh4)
//{
//    superM.set_color(vh1, MyMesh::Color(0, 0, 255));
//    superM.set_color(vh2, MyMesh::Color(0, 150, 255));
//    superM.set_color(vh3, MyMesh::Color(0, 255, 255));
//    superM.set_color(vh4, MyMesh::Color(255, 0, 0));
//    superM.data(vh1).thickness = 5;
//    superM.data(vh2).thickness = 5;
//    superM.data(vh3).thickness = 5;
//    superM.data(vh4).thickness = 8;

//    int r = Utils::randInt(0, 255);
//    int g = Utils::randInt(0, 255);
//    int b = Utils::randInt(0, 255);
//    for (MyMesh::FaceIter f_it = superM.faces_begin(); f_it != superM.faces_end(); f_it++)
//    {
//        FaceHandle fh = *f_it;
//        superM.set_color(fh, MyMesh::Color(r, g, b));
//    }

//    for (MyMesh::EdgeIter e_it = superM.edges_begin(); e_it != superM.edges_end(); e_it++)
//    {
//        EdgeHandle eh = *e_it;
//        superM.set_color(eh, MyMesh::Color(0, 0, 0));
//    }
//}


