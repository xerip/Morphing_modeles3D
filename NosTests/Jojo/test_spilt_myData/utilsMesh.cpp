#include "utilsMesh.h"

#include <QDebug>

UtilsMesh::UtilsMesh()
{
    ;
}

/*------------------------------------------------------------------------------
 * Renvoie l'id de la face opposée de @faceID par rapport à l'arete @edgeID
 * ATTENTION
 *  @edgeID doit appartenir à la face @faceID
 * ----------------------------------------------------------------------------*/
int UtilsMesh::get_opposite_edgeFace(MyMesh *_mesh, int faceID, int edgeID)
{
    FaceHandle fh;
    if (! UtilsMesh::edge_is_in_face(_mesh, edgeID, faceID)) {
        qWarning() << "Warning in " << __FUNCTION__ << ": @edgeID has to"
                                                       " be part of the face @faceID";
        return -1;
    }
    EdgeHandle eh = _mesh->edge_handle(edgeID);
    if (_mesh->is_boundary(eh)) return -1;

    MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 0);
    fh = _mesh->face_handle(heh1);
    if (fh.idx() != faceID) return fh.idx();

    MyMesh::HalfedgeHandle heh2 = _mesh->halfedge_handle(eh, 1);
    fh = _mesh->face_handle(heh2);
    if (fh.idx() != faceID) return fh.idx();

    return -1;
}

vector<FaceHandle> UtilsMesh::get_neighbour_faces(MyMesh *_mesh, int numFace)
{
    vector<FaceHandle> faces;
    FaceHandle fhBase = _mesh->face_handle(numFace);
    for (MyMesh::FaceFaceCWIter ff_it = _mesh->ff_cwiter(fhBase); ff_it.is_valid(); ff_it++)
    {
        FaceHandle fhTmp = *ff_it;
        faces.push_back(fhTmp);
    }
    return faces;
}

bool UtilsMesh::edge_is_in_face(MyMesh *_mesh, int edgeID, int faceID)
{
    FaceHandle fh = _mesh->face_handle(faceID);

    for (MyMesh::FaceEdgeCWIter fe_it = _mesh->fe_cwiter(fh); fe_it.is_valid(); fe_it++)
    {
        EdgeHandle eh = *fe_it;
        if (eh.idx() == edgeID)
            return true;
    }

    return false;
}

bool UtilsMesh::faces_have_common_edge(MyMesh *_mesh, int face1ID, int face2ID)
{
    FaceHandle fh1 = _mesh->face_handle(face1ID);
    for (MyMesh::FaceEdgeCWIter fe_it = _mesh->fe_cwiter(fh1); fe_it.is_valid(); fe_it++)
    {
        EdgeHandle eh = *fe_it;
        if (edge_is_in_face(_mesh, eh.idx(), face2ID))
            return true;
    }
    return false;
}

bool UtilsMesh::vertex_is_in_face(MyMesh *_mesh, int faceID, int vertexID)
{
    FaceHandle fh = _mesh->face_handle(faceID);
    for (MyMesh::FaceVertexCWIter fv_it = _mesh->fv_cwiter(fh); fv_it.is_valid(); fv_it++)
    {
       VertexHandle visitor = *fv_it;
       if (vertexID == visitor.idx())   return true;
    }
    return false;
}

void UtilsMesh::add_triangle_face(MyMesh *_mesh,
                                  VertexHandle vh1,
                                  VertexHandle vh2,
                                  VertexHandle vh3)
{
    _mesh->add_face(vh1, vh2, vh3);
    FaceHandle fh = _mesh->face_handle(_mesh->n_faces()-1);
    _mesh->set_color(fh, MyMesh::Color(150, 150, 150));
}

void UtilsMesh::add_face(MyMesh *_mesh, vector<VertexHandle> vhs)
{
    vector<MyMesh::VertexHandle> uneNouvelleFace;
    for (auto vh : vhs)
    {
        uneNouvelleFace.push_back(vh);
    }
    _mesh->OpenMesh::TriConnectivity::add_face(uneNouvelleFace);
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

vector<VertexHandle> UtilsMesh::get_vh_of_face(MyMesh *_mesh, int faceID)
{
    vector<VertexHandle> vhs;

    FaceHandle fh = _mesh->face_handle(faceID);
    for (MyMesh::FaceVertexCWIter fv_it = _mesh->fv_cwiter(fh); fv_it.is_valid(); fv_it++)
    {
        VertexHandle vh = *fv_it;
        vhs.push_back(vh);
    }

    return vhs;
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
            qWarning() << "In " << __FUNCTION__ << ": Error cpt > 3 -> not a triangle!";
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

void UtilsMesh::add_vertex_to_edge(MyMesh *_mesh, EdgeHandle eh, VertexHandle vh)
{
    qDebug() << "<" << __FUNCTION__ << ">";

    FaceHandle fh;
    VertexHandle vhA, vhB;
    UtilsMesh::get_vh_of_edge(_mesh, eh.idx(), vhA, vhB);
    qDebug() << "\tvhA.idx() =" << vhA.idx();
    qDebug() << "\tvhB.idx() =" << vhB.idx();
    vector<VertexHandle> vhs;
    vector<VertexHandle> vhs0;
    vector<VertexHandle> vhs1;
//    for (int numHeh = 0; numHeh<2; numHeh++)
//    {
//        MyMesh::HalfedgeHandle heh = _mesh->halfedge_handle(eh, numHeh);
//        if (!_mesh->is_valid_handle(heh) || _mesh->is_boundary(heh)) {
//            qDebug() << "\tBoundary!";
//            continue;
//        }
//    }
    vhs.clear();

    ///////////////////////////// V1 ////////////////////////////////////////
//    _mesh->request_face_status();
//    _mesh->request_edge_status();
//    _mesh->request_vertex_status();

////    MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 0);
////    MyMesh::HalfedgeHandle heh2 = _mesh->halfedge_handle(eh, 1);
////    _mesh->set_vertex_handle(heh1, vh);
//    FaceHandle fh1, fh2;

//    VertexHandle vhTest;
//    MyMesh::HalfedgeHandle heh1 =  _mesh->halfedge_handle(eh, 0);
//    fh1 = _mesh->face_handle(heh1);
//    vhTest = _mesh->to_vertex_handle(heh1);
//    if (vhTest == vhA)    qDebug() << "\tok3";
//    _mesh->set_halfedge_handle(vh, heh1);
//    // _mesh->set_vertex_handle(heh1, vh);
//    MyMesh::HalfedgeHandle heh2 =  _mesh->halfedge_handle(eh, 1);
//    if ((!_mesh->is_valid_handle(heh2) || _mesh->is_boundary(heh2)))
//    {
//        fh2 = _mesh->face_handle(heh2);
//        vhTest = _mesh->to_vertex_handle(heh2);
//        if (vhTest == vhB)    qDebug() << "\tok4";
//        _mesh->set_halfedge_handle(vh, heh2);
//        // _mesh->set_vertex_handle(heh2, vh);
//    }


//    MyMesh::HalfedgeHandle hehVhTo1 = _mesh->halfedge_handle(vh);
//    _mesh->set_halfedge_handle(vhA, hehVhTo1);
//    _mesh->set_face_handle(hehVhTo1, fh1);
//    // _mesh->set_vertex_handle(hehVhTo1, vhA);
//    MyMesh::HalfedgeHandle hehVhTo2;
//    if ((!_mesh->is_valid_handle(heh2) || _mesh->is_boundary(heh2)))
//    {
//        hehVhTo2 = _mesh->halfedge_handle(vh);
//        _mesh->set_halfedge_handle(vhB, hehVhTo2);
//        _mesh->set_face_handle(hehVhTo2, fh2);
//        // _mesh->set_vertex_handle(hehVhTo2, vhB);
//    }

//    qDebug() << "\t?";

//    _mesh->garbage_collection();
//    _mesh->request_face_status();
//    _mesh->request_edge_status();
//    _mesh->request_vertex_status();

//    vhTest = _mesh->to_vertex_handle(hehVhTo1);
//    if (vhTest == vhB) {
//        qDebug() << "\tOkB";
//    }
//    if ((!_mesh->is_valid_handle(heh2) || _mesh->is_boundary(heh2)))
//    {
//        vhTest = _mesh->to_vertex_handle(hehVhTo2);
//        if (vhTest == vhA) {
//            qDebug() << "\tOkA";
//        }
//    }
//qDebug() << "</" << __FUNCTION__ << ">";
//return;

    ///////////////////////////// V2 ////////////////////////////////////////


    // Pour bien insérer dans les 2 faces autour de eh
    for (int numHeh = 0; numHeh<2; numHeh++)
    {
        qDebug() << "\n\t/////////////////";
        qDebug() << "\tBoucle" << numHeh;
        vhs.clear();
        _mesh->request_face_status();
        _mesh->request_edge_status();
        _mesh->request_vertex_status();

        MyMesh::HalfedgeHandle heh = _mesh->halfedge_handle(eh, numHeh);
        if (!_mesh->is_valid_handle(heh) || _mesh->is_boundary(heh)) {
            qDebug() << "\tBoundary!";
            continue;
        }

        fh = _mesh->face_handle(heh);
        if (numHeh==0)   _mesh->set_color(fh, MyMesh::Color(0, 255, 0));
        if (numHeh==1)   _mesh->set_color(fh, MyMesh::Color(0, 0, 255));
        qDebug() << "\tfh.idx() =" << fh.idx();
        vhs = UtilsMesh::get_vh_of_face(_mesh, fh.idx());
        int idA = Utils::get_pos_of_val_in_vector(vhs, vhA);
        int idB = Utils::get_pos_of_val_in_vector(vhs, vhB);
        qDebug() << "\tidA : " << idA;
        qDebug() << "\tidB : " << idB;
        int idAdd;
        // Parceque la fonction insert, insère avant la valeur.
        if (Utils::is_next_of(vhs, vhs[idA], vhs[idB]))
            idAdd = idB;
        else
            idAdd = idA;

        int idFirst;
        if (idAdd == idB)   idFirst = idA;
        if (idAdd == idA)   idFirst = idB;
        vhs[idFirst];
        // MyMesh::HalfedgeHandle heh1 = _mesh->halfedge_handle(eh, 0);
        // qDebug() << "\tidAdd : " << idAdd;

        _mesh->delete_face(fh, false);
        _mesh->garbage_collection();

        vector<VertexHandle>::iterator it;
        for(it = vhs.begin(); it!=vhs.end(); ++it)
        {
            VertexHandle vhTmp = *it;
            if (vhTmp.idx() == idAdd) break;
        }
        vhs.insert(it, vh);
        int cpt=0;
        qDebug() << "\tNew";
        for (auto v : vhs) {
            qDebug() << "\tvhs["<<cpt<<"] = idx()" << vhs[cpt].idx();
            cpt++;
        }
        if (numHeh == 0) vhs0 = vhs;
        if (numHeh == 1) vhs1 = vhs;
    }

    UtilsMesh::add_face(_mesh, vhs0);
    if (!vhs1.empty()) {
        //        vhs1 = Utils::invert_vector(vhs1);
        //        int cpt=0;
        //        qDebug() << "\tNew2";
        //        for (auto v : vhs) {
        //            qDebug() << "\tvhs["<<cpt<<"] = idx()" << vhs1[cpt].idx();
        //            cpt++;
        //        }
        UtilsMesh::add_face(_mesh, vhs1);
    }

    qDebug() << "</" << __FUNCTION__ << ">";
}

//void UtilsMesh::old_add_vertex_to_edge(MyMesh *_mesh, EdgeHandle eh, VertexHandle vh)
//{
//    qDebug() << "<" << __FUNCTION__ << ">";

//    FaceHandle fh;
//    VertexHandle vhA, vhB;
//    UtilsMesh::get_vh_of_edge(_mesh, eh.idx(), vhA, vhB);
//    qDebug() << "\tvhA.idx() =" << vhA.idx();
//    qDebug() << "\tvhB.idx() =" << vhB.idx();
//    vector<VertexHandle> vhs;
//    vector<VertexHandle> vhs0;
//    vector<VertexHandle> vhs1;
////    for (int numHeh = 0; numHeh<2; numHeh++)
////    {
////        MyMesh::HalfedgeHandle heh = _mesh->halfedge_handle(eh, numHeh);
////        if (!_mesh->is_valid_handle(heh) || _mesh->is_boundary(heh)) {
////            qDebug() << "\tBoundary!";
////            continue;
////        }
////    }

//    // Pour bien insérer dans les 2 faces autour de eh
//    for (int numHeh = 0; numHeh<2; numHeh++)
//    {
//        qDebug() << "\n\t/////////////////";
//        qDebug() << "\tBoucle" << numHeh;
//        vhs.clear();
//        MyMesh::HalfedgeHandle heh = _mesh->halfedge_handle(eh, numHeh);
//        if (!_mesh->is_valid_handle(heh) || _mesh->is_boundary(heh)) {
//            qDebug() << "\tBoundary!";
//            continue;
//        }

//        fh = _mesh->face_handle(heh);
//        if (numHeh==0)   _mesh->set_color(fh, MyMesh::Color(0, 255, 0));
//        if (numHeh==1)   _mesh->set_color(fh, MyMesh::Color(0, 0, 255));
//        qDebug() << "\tfh.idx() =" << fh.idx();
//        vhs = UtilsMesh::get_vh_of_face(_mesh, fh.idx());
//        int idA = Utils::get_idVal_in_vector(vhs, vhA);
//        int idB = Utils::get_idVal_in_vector(vhs, vhB);
//        qDebug() << "\tidA : " << idA;
//        qDebug() << "\tidB : " << idB;
//        int idAdd;
//        // Parceque la fonction insert, insère avant la valeur.
//        if (Utils::is_next_of(vhs, vhs[idA], vhs[idB]))
//            idAdd = idB;
//        else
//            idAdd = idA;

//        // qDebug() << "\tidAdd : " << idAdd;

////        _mesh->delete_face(fh, false);
////        _mesh->garbage_collection();

//        vector<VertexHandle>::iterator it;
//        for(it = vhs.begin(); it!=vhs.end(); ++it)
//        {
//            VertexHandle vhTmp = *it;
//            if (vhTmp.idx() == idAdd) break;
//        }
//        vhs.insert(it, vh);
//        int cpt=0;
//        qDebug() << "\tNew";
//        for (auto v : vhs) {
//            qDebug() << "\tvhs["<<cpt<<"] = idx()" << vhs[cpt].idx();
//            cpt++;
//        }
////        UtilsMesh::add_face(_mesh, vhs);
//    }
//    qDebug() << "</" << __FUNCTION__ << ">";
//}

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

    return UtilsMesh::middle_edge(_mesh, vh1.idx(), vh2.idx());
}

MyMesh::Point UtilsMesh::middle_edge(MyMesh *_mesh, int vertexID1, int vertexID2)
{
    VertexHandle vh1 = _mesh->vertex_handle(vertexID1);
    VertexHandle vh2 = _mesh->vertex_handle(vertexID2);
    MyMesh::Point p1 = _mesh->point(vh1);
    MyMesh::Point p2 = _mesh->point(vh2);

    float X = UtilsMesh::middle_edge_coord(p1[0], p2[0]);
    float Y = UtilsMesh::middle_edge_coord(p1[1], p2[1]);
    float Z = UtilsMesh::middle_edge_coord(p1[2], p2[2]);
    MyMesh::Point myP(X, Y, Z);
    return myP;
}

