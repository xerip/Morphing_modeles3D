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
                float dist = UtilsMesh::distance_point_to_face(&superM, p, fh.idx()); //distance du point p (qui a intersecté) au barycentre de fh
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

void SuperMesh::rectangle_to_triangle(int faceID, int vhSplitID)
{
    qDebug() << "\t\t<" << __FUNCTION__ << ">";
//    if (! UtilsMesh::vertex_is_in_face(&superM, faceID, vhSplitID)) {
//        qWarning() << "Warning in " << __FUNCTION__ << ": @vertexSplit has to"
//                                               " be part of the face @faceID";
//        qDebug() << "\t\t</" << __FUNCTION__ << ">";
//        return;
//    }


    // V2
    FaceHandle fh = superM.face_handle(faceID);
    VertexHandle vhSplit = superM.vertex_handle(vhSplitID);
    vector<VertexHandle> vhs;
    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
    {
        VertexHandle vhTmp = *fv_it;
        vhs.push_back(vhTmp);
    }
    superM.set_color(fh, MyMesh::Color(150, 150, 150));
    superM.split_copy(fh, vhSplit);

    // V1
//    FaceHandle fh = superM.face_handle(faceID);
//    VertexHandle vhSplit = superM.vertex_handle(vhSplitID);

//    int cpt=0;
//    int r, v, b;
//    vector<VertexHandle> vhs;
//    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
//    {
//        VertexHandle vhTmp = *fv_it;
//        if (vhTmp.idx() != vhSplitID) {
////            if (cpt ==1) {  r=255; v=0; b=0;    }
////            if (cpt ==2) {  r=0; v=255; b=0;    }
////            if (cpt ==3) {  r=0; v=0; b=255;    }
////            superM.set_color(vhTmp, MyMesh::Color(r, v, b));
////            superM.data(vhTmp).thickness = 8;
//            vhs.push_back(vhTmp);
//        }
//        cpt++;
//    }

//    // Erase
//    superM.delete_face(fh, false);
//    superM.garbage_collection();

//    //    UtilsMesh::add_triangle_face(&superM, vhs[0], vhSplit, vhs[1]);
//    //    UtilsMesh::add_triangle_face(&superM, vhSplit, vhs[1], vhs[2]);

//    superM.set_color(vhSplit, MyMesh::Color(250, 0, 250));
//    vector<VertexHandle> tmps = {VertexHandle(1), VertexHandle(2), VertexHandle(6)};
//    UtilsMesh::add_triangle_face(&superM, vhSplit, tmps[1], tmps[2]);
////    UtilsMesh::add_triangle_face(&superM, tmps[0], vhSplit, tmps[2]);

//    // TMP COLOR
//    superM.set_color(superM.face_handle(superM.n_faces()-1), MyMesh::Color(150, 150, 150));
//    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
//    {
//        superM.data(*curEdge).thickness = 1;
//        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
//    }


    qDebug() << "\t\t</" << __FUNCTION__ << ">";
}

/*--------------------------------------------------------------------------
 * Retriangule les faces voisines de la face @faceID.
 * ===================================================================
 * @fhPs contient les 3 faces voisines à @faceID.
 * @vhPs contient les nouveaux sommets de la face @faceID.
 * Attention:
 *  - La face @faceID doit encore exister dans le mesh.
 * -------------------------------------------------------------------------*/
void SuperMesh::retriangle_neighbours(int faceID,
                                      vector<FaceHandle> fhPs,
                                      vector<VertexHandle> vhPs)
{
    qDebug() << "\t<" << __FUNCTION__ << ">";

    if (fhPs.size() > 3) {
        qWarning() << "Warning in "
                   << __FUNCTION__
                   << ": @faceID can't have more of 3 neighbours faces (in @fhPs)";
        qDebug() << "\t\t</" << __FUNCTION__ << ">";
        return;
    }

// TEST
//    superM.set_color(fhPs[1], MyMesh::Color(100, 100, 100));
//    superM.data(vhPs[1]).thickness = 8;
//    rectangle_to_triangle(fhPs[1].idx(), vhPs[1].idx());

    int cpt=0;
    for (unsigned i=0; i<fhPs.size(); i++)
    {
//        superM.set_color(vhPs[i], MyMesh::Color(cpt*100, cpt*100, cpt*100));
        superM.set_color(fhPs[i], MyMesh::Color(cpt*100, cpt*100, cpt*100));
        superM.data(vhPs[i]).thickness = 8;

        rectangle_to_triangle(fhPs[i].idx(), vhPs[i].idx());

//        //Erase
//        superM.delete_face(fhPs[i], false);
//        superM.garbage_collection();

        cpt++;
    }


//    for (auto fh : fhPs)
//    {
//        for (unsigned i=0; i<ehPs.size(); i++)
//        {
//            if (UtilsMesh::edge_is_in_face(&superM, ehPs[i].idx(), fh.idx())) {
//                rectangle_to_triangle(fh.idx(), vhPs[i].idx());
//                // Erase
//                superM.delete_face(fh, false);
//                superM.garbage_collection();
//            }
//        }
//    }

    // OLD
//    FaceHandle fhBase = superM.face_handle(faceID);
//    for (MyMesh::FaceFaceCWIter ff_it = superM.ff_cwiter(fhBase); ff_it.is_valid(); ff_it++)
//    {
//        FaceHandle fhNeigh = *ff_it;
//        qDebug() << "\t\tface" << fhNeigh.idx() << "found";

//        for (unsigned i=0; i<ehPs.size(); i++) {
//            if (UtilsMesh::edge_is_in_face(&superM, ehPs[i].idx(), fhNeigh.idx())) {
//                    rectangle_to_triangle(fhNeigh.idx(), vhPs[i].idx());
//            }
//        }

////        // ATTENTION -> Utile !?
////        if (UtilsMesh::faces_have_common_edge(&superM, faceID, fhNeigh.idx()))
////        {
////            qDebug() << "\t\t\tface" << fhNeigh.idx() << "OK";
////        }
//    }

    qDebug() << "\t</" << __FUNCTION__ << ">";
}

/*------------------------------------------------------------------------------
 * Renvoi les 3 faces adjacentes aux 3 arêtes du triangle @numFace.
 * ----------------------------------------------------------------------------*/
vector<FaceHandle> SuperMesh::get_3_neighbour_faces(int numFace,
                                      EdgeHandle eh1, EdgeHandle eh2, EdgeHandle eh3)
{
    vector<FaceHandle> vhFaces;
    FaceHandle fh;
    int faceNeighID;

    // EH1
    faceNeighID = UtilsMesh::get_opposite_edgeFace(&superM, numFace, eh1.idx());
    if (faceNeighID >= 0) {
        fh = superM.face_handle(faceNeighID);
//        superM.set_color(fh, MyMesh::Color(0, 255, 0));
        vhFaces.push_back(fh);
    }
    // EH2
    faceNeighID = UtilsMesh::get_opposite_edgeFace(&superM, numFace, eh2.idx());
    if (faceNeighID >= 0) {
        fh = superM.face_handle(faceNeighID);
//        superM.set_color(fh, MyMesh::Color(0, 255, 0));
        vhFaces.push_back(fh);
    }
    // EH3
    faceNeighID = UtilsMesh::get_opposite_edgeFace(&superM, numFace, eh3.idx());
    if (faceNeighID >= 0) {
        fh = superM.face_handle(faceNeighID);
//        superM.set_color(fh, MyMesh::Color(0, 255, 0));
        vhFaces.push_back(fh);
    }

    return vhFaces;
}

void SuperMesh::split_1_to_4(int numFace,
                             vector<VertexHandle> vhs,
                             vector<VertexHandle> vhPs)
{
    FaceHandle fh = superM.face_handle(numFace);

    // Erase
    fh = superM.face_handle(numFace);
    superM.delete_face(fh, false);
    superM.garbage_collection();

    // Centrale
    UtilsMesh::add_triangle_face(&superM, vhPs[0], vhPs[1], vhPs[2]);
    // Autres
    UtilsMesh::add_triangle_face(&superM, vhs[0], vhPs[0], vhPs[2]);
//    UtilsMesh::add_triangle_face(&superM, vheh2A, vhP2, vhP1);
//    UtilsMesh::add_triangle_face(&superM, vheh3A, vhP3, vhP2);
}

void SuperMesh::split_edges(VertexHandle &vhP1, VertexHandle &vhP2, VertexHandle &vhP3)
{
    EdgeHandle eh1, eh2, eh3;
    UtilsMesh::get_eh_of_triangle_face(&superM, 0, eh1, eh2, eh3);

    MyMesh::Point myP1 = UtilsMesh::middle_edge(&superM, eh1.idx());
    vhP1 = superM.add_vertex(myP1);
    MyMesh::Point myP2 = UtilsMesh::middle_edge(&superM, eh2.idx());
    vhP2 = superM.add_vertex(myP2);
    MyMesh::Point myP3 = UtilsMesh::middle_edge(&superM, eh3.idx());
    vhP3 = superM.add_vertex(myP3);

    superM.split_edge_copy(eh1, vhP1);
    superM.split_edge_copy(eh2, vhP2);
    superM.split_edge_copy(eh3, vhP3);

//    superM.garbage_collection();
}

void SuperMesh::my_split(int numFace, vector<int> &sommetsRajoutes)
{
    VertexHandle vh1, vh2, vh3;
    VertexHandle vhP1, vhP2, vhP3;
    EdgeHandle eh1, eh2, eh3;
    FaceHandle fh;
    vector<FaceHandle> vhFaces;
    fh = superM.face_handle(numFace);


    int cptV = 1;
    int cptP = 1;
    for (MyMesh::FaceVertexCWIter fv_it = superM.fv_cwiter(fh); fv_it.is_valid(); fv_it++)
    {
        VertexHandle vh = *fv_it;
        if (! Utils::is_in_vector(sommetsRajoutes, vh.idx()))
        {
            if (cptV==1)         vh1 = *fv_it;
            else if (cptV==2)    vh2 = *fv_it;
            else if (cptV==3)    vh3 = *fv_it;
            cptV++;
        }
    }

    int cpt = 1;
    VertexHandle vhTest1, vhTest2;

    //UtilsMesh::get_vh_of_edge(&superM, eh.idx(), vhTest1, vhTest2);


//    // Obtenus avec ClockWise (sens aiguilles d'une montre)
//    UtilsMesh::get_vh_of_triangle_face(&superM, 0, vh1, vh2, vh3);
//    UtilsMesh::get_eh_of_triangle_face(&superM, 0, eh1, eh2, eh3);

    // obtenues donc dans le même ordre que les nouveaux points/vertex
    //vhFaces = get_3_neighbour_faces(numFace, eh1, eh2, eh3);


// TMP
    return;

    split_edges(vhP1, vhP2, vhP3);

    split_1_to_4(numFace,
                 vector<VertexHandle>{vh1, vh2, vh3},
                 vector<VertexHandle>{vhP1, vhP2, vhP3});

    // COLOR
    int thick = 8;
    for (MyMesh::VertexIter curVert = superM.vertices_begin(); curVert != superM.vertices_end(); curVert++)
    {
        superM.data(*curVert).thickness = 1;
        superM.set_color(*curVert, MyMesh::Color(0, 0, 0));
    }
    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
    {
        superM.data(*curEdge).thickness = 1;
        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
    for (MyMesh::FaceIter curFace = superM.faces_begin(); curFace != superM.faces_end(); curFace++)
    {
        superM.set_color(*curFace, MyMesh::Color(150, 150, 150));
    }
    superM.set_color(vhP1, MyMesh::Color(255, 0, 0));
    superM.set_color(vhP2, MyMesh::Color(0, 255, 0));
    superM.set_color(vhP3, MyMesh::Color(0, 0, 255));
    superM.data(vhP1).thickness = thick;
    superM.data(vhP2).thickness = thick;
    superM.data(vhP3).thickness = thick;

}

void SuperMesh::old_split()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    int numFace = 0;
    FaceHandle fh1 = superM.face_handle(numFace);
    superM.split(fh1);

// TMP
    return;



    VertexHandle vh1, vh2, vh3;
    VertexHandle vhP1, vhP2, vhP3;
    EdgeHandle eh1, eh2, eh3;
    FaceHandle fh;
    vector<FaceHandle> vhFaces;

    //vhFaces = UtilsMesh::get_neighbour_faces(&superM, numFace);

    // Obtenus avec ClockWise (sens aiguilles d'une montre)
    UtilsMesh::get_vh_of_triangle_face(&superM, 0, vh1, vh2, vh3);
    UtilsMesh::get_eh_of_triangle_face(&superM, 0, eh1, eh2, eh3);
    MyMesh::Point myP1 = UtilsMesh::middle_edge(&superM, eh1.idx());
    vhP1 = superM.add_vertex(myP1);
    MyMesh::Point myP2 = UtilsMesh::middle_edge(&superM, eh2.idx());
    vhP2 = superM.add_vertex(myP2);
    MyMesh::Point myP3 = UtilsMesh::middle_edge(&superM, eh3.idx());
    vhP3 = superM.add_vertex(myP3);

    // obtenues donc dans le même ordre que les nouveaux points/vertex
    vhFaces = get_3_neighbour_faces(numFace, eh1, eh2, eh3);
    // qDebug() << "\tvhFaces.size() =" << vhFaces.size();


    // Réglages
    VertexHandle vheh1A, vheh1B;
    // Attention au sens de ces 2 vertex!
    UtilsMesh::get_vh_of_edge(&superM, eh1.idx(), vheh1A, vheh1B);
    if (vheh1A == vh2) {
        Utils::permut(vheh1A, vheh1B);
        qDebug() << "\tPermut 1";
    }
    VertexHandle vheh2A, vheh2B;
    // Attention au sens de ces 2 vertex!
    UtilsMesh::get_vh_of_edge(&superM, eh2.idx(), vheh2A, vheh2B);
    if (vheh2A == vh3) {
        Utils::permut(vheh2A, vheh2B);
        qDebug() << "\tPermut 2";
    }
    VertexHandle vheh3A, vheh3B;
    // Attention au sens de ces 2 vertex!
    UtilsMesh::get_vh_of_edge(&superM, eh3.idx(), vheh3A, vheh3B);
    if (vheh3A == vh1) {
        Utils::permut(vheh3A, vheh3B);
        qDebug() << "\tPermut 3";
    }
    // COLOR
    //    superM.set_color(vheh1A, MyMesh::Color(255, 0, 0));
    //    superM.data(vheh1A).thickness = 8;
    //    superM.set_color(vheh2A, MyMesh::Color(0, 255, 0));
    //    superM.data(vheh2A).thickness = 8;
    //    superM.set_color(vheh3A, MyMesh::Color(0, 0, 255));
//    superM.set_color(vh1, MyMesh::Color(255, 0, 0));
//    superM.data(vh1).thickness = 8;
//    superM.set_color(vh2, MyMesh::Color(0, 255, 0));
//    superM.data(vh2).thickness = 8;
//    superM.set_color(vh3, MyMesh::Color(0, 0, 255));
//    superM.data(vh3).thickness = 8;

    superM.data(eh1).thickness = 1;
    superM.set_color(eh1, MyMesh::Color(255, 0, 0));
    superM.data(eh2).thickness = 1;
    superM.set_color(eh2, MyMesh::Color(0, 255, 0));
    superM.data(eh3).thickness = 1;

    superM.set_color(eh3, MyMesh::Color(0, 0, 255));
    superM.set_color(vhP1, MyMesh::Color(255, 0, 0));
    superM.set_color(vhP2, MyMesh::Color(0, 255, 0));
    superM.set_color(vhP3, MyMesh::Color(0, 0, 255));
    superM.data(vhP1).thickness = 8;
    superM.data(vhP2).thickness = 8;
    superM.data(vhP3).thickness = 8;

    // Erase
    fh = superM.face_handle(numFace);
    superM.delete_face(fh, false);
    superM.garbage_collection();

    // Centrale
    UtilsMesh::add_triangle_face(&superM, vhP1, vhP2, vhP3);
    // Autres
    UtilsMesh::add_triangle_face(&superM, vheh1A, vhP1, vhP3);
    UtilsMesh::add_triangle_face(&superM, vheh2A, vhP2, vhP1);
    UtilsMesh::add_triangle_face(&superM, vheh3A, vhP3, vhP2);

    // RETRIANGLE
    retriangle_neighbours(numFace,
                          vhFaces,
                          vector<VertexHandle>{vhP1, vhP2, vhP3});

    // COLOR
    int thick = 8;
    for (MyMesh::EdgeIter curEdge = superM.edges_begin(); curEdge != superM.edges_end(); curEdge++)
    {
        superM.data(*curEdge).thickness = 1;
        superM.set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
//    superM.set_color(vh1, MyMesh::Color(255, 0, 0));
//    superM.set_color(vh2, MyMesh::Color(255, 0, 0));
//    superM.set_color(vh3, MyMesh::Color(255, 0, 0));
//    superM.data(vh1).thickness = thick;
//    superM.data(vh2).thickness = thick;
//    superM.data(vh3).thickness = thick;
//    superM.set_color(vhP1, MyMesh::Color(255, 0, 0));
//    superM.set_color(vhP2, MyMesh::Color(0, 255, 0));
//    superM.set_color(vhP3, MyMesh::Color(0, 0, 255));
//    superM.data(vhP1).thickness = thick;
//    superM.data(vhP2).thickness = thick;
//    superM.data(vhP3).thickness = thick;


    qDebug() << "</" << __FUNCTION__ << ">";
}


void SuperMesh::test (VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, VertexHandle vh4)
{
    superM.set_color(vh1, MyMesh::Color(0, 0, 255));
    superM.set_color(vh2, MyMesh::Color(0, 150, 255));
    superM.set_color(vh3, MyMesh::Color(0, 255, 255));
    superM.set_color(vh4, MyMesh::Color(255, 0, 0));
    superM.data(vh1).thickness = 5;
    superM.data(vh2).thickness = 5;
    superM.data(vh3).thickness = 5;
    superM.data(vh4).thickness = 8;

    int r = Utils::randInt(0, 255);
    int g = Utils::randInt(0, 255);
    int b = Utils::randInt(0, 255);
    for (MyMesh::FaceIter f_it = superM.faces_begin(); f_it != superM.faces_end(); f_it++)
    {
        FaceHandle fh = *f_it;
        superM.set_color(fh, MyMesh::Color(r, g, b));
    }

    for (MyMesh::EdgeIter e_it = superM.edges_begin(); e_it != superM.edges_end(); e_it++)
    {
        EdgeHandle eh = *e_it;
        superM.set_color(eh, MyMesh::Color(0, 0, 0));
    }
}


