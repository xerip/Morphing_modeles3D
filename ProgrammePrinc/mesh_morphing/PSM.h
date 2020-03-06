/*-------------------------------------------------------------
 * Module de création de pseudo SuperMesh
 *
 * Contient les structures MyVertex, MyFace et MyData
 * permettant de stocker des données du maillage pouvant
 * être utilisées par la suite pour faire des opérations
 * dessus.
 *
 * Contient la clasee PSM représentant le pseudo super mesh
 * -----------------------------------------------------------*/

#ifndef PSM_H
#define PSM_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyConnectivity.hh>

#include <QVector3D>

#include "utils.h"
#include "utilsMesh.h"
#include "raycasting.h"

#include <QDebug>

using namespace std;

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

enum enum_typeVertex {VERTEX_ORIGIN, VERTEX_SPLIT};
enum enum_typeFace {FACE_ORIGIN, FACE_SPLIT};

struct MyVertex
{
    int type;
    VertexHandle vh;
    MyMesh::Point pos={0.f, 0.f, 0.f};
    MyMesh::Point posSrc={0.f, 0.f, 0.f};
    MyMesh::Point posTgt={0.f, 0.f, 0.f};

    MyVertex();
    MyVertex(VertexHandle _vh) : vh(_vh)   {;}

    MyVertex(VertexHandle _vh, MyMesh::Point _pos,
             MyMesh::Point _posSrc, MyMesh::Point _posTgt, int _type) :
        vh(_vh), pos(_pos), posSrc(_posSrc), posTgt(_posTgt), type(_type)
    {;}
    MyVertex(VertexHandle _vh, MyMesh::Point _pos, int _type) :
        vh(_vh), pos(_pos), type(_type)
    {;}

    void add_pos_src(MyMesh::Point _posSrc) {   posSrc = _posSrc;  }
    void add_pos_tgt(MyMesh::Point _posTgt) {   posTgt = _posTgt;  }

    void display()
    {
        qDebug() << "vh" << vh.idx();
        if (type == VERTEX_ORIGIN)
            qDebug() << "\ttype = VERTEX_ORIGIN";
        else
            qDebug() << "\ttype = VERTEX_SPLIT";
        qDebug() << "\tposSrc("<<posSrc[0]<<","<<posSrc[1]<<","<<posSrc[2]<<")";
        qDebug() << "\tposTgt("<<posTgt[0]<<","<<posTgt[1]<<","<<posTgt[2]<<")";
    }
};

struct MyFace
{
    int id=-1;  // Id d'identification
    vector<unsigned> vertices;   // Contient positions (id) d'un tableau de sommets
    int numFaceOrigin = 0;
    // Pour savoir si la face est d'origine ou a été splitée
    int type = FACE_ORIGIN;

    MyFace(){type = FACE_ORIGIN;}
    MyFace(int _id, int _numFaceOrigin, vector<unsigned> _vertices,
           int _originalFace = FACE_ORIGIN) :
        id(_id),
        numFaceOrigin(_numFaceOrigin),
        vertices(_vertices),
        type(_originalFace)
    {;}
    MyFace(int _numFaceOrigin, vector<unsigned> _vertices,
           int _originalFace = FACE_ORIGIN) :
        numFaceOrigin(_numFaceOrigin),
        vertices(_vertices),
        type(_originalFace)
    {id = -1;}

    /*----------------------------------------------------------------------
     * Ajoute l'index @idV d'un sommet, à la position @pos dans @vertices
     * --------------------------------------------------------------------*/
    void add_vertex(int idV, int pos)
    {
        vector<unsigned>::iterator it;
        for(it = vertices.begin(); it!=vertices.end(); ++it)
        {
            int idTmp = *it;
            if (idTmp == pos) break;
        }
        vertices.insert(it, idV);
    }

    void display()
    {
        qDebug() << "face id =" << id;
        qDebug() << "face numOrigin =" << numFaceOrigin;
        if (type == FACE_ORIGIN)
            qDebug() << "\ttype = FACE_ORIGIN";
        else
            qDebug() << "\ttype = FACE_SPLIT";

        for (auto v: vertices) {
            qDebug() << "\t\tvertex " << v;
        }
    }

    unsigned size() {    return vertices.size();   }
};

struct MyData
{
    unsigned currentFace;
    int nb_original_vertices;
    int idFace=0;   // Un id à incrémenter pour donner des id aux faces
    vector<MyVertex> vhs;
    vector<MyFace> fhs;

    MyData() {;}
    MyData(int nb_vertices) :
        nb_original_vertices(nb_vertices)
    {
        ;
    }

    void display()
    {
        for (unsigned i=0; i<fhs.size(); i++) {
            fhs[i].display();
        }
    }

    void display_vertices()
    {
        for (unsigned i=0; i<vhs.size(); i++) {
            vhs[i].display();
        }
    }

    void add_vertex(VertexHandle vh)
    {
        MyVertex myVh(vh);
        vhs.push_back(myVh);
    }
    void add_vertex(MyVertex myVh)
    {
        vhs.push_back(myVh);
    }

    void add_face(MyFace _myFh)
    {
        _myFh.id = idFace++;
        fhs.push_back(_myFh);
    }
    void add_face(int numFace, vector<unsigned> numVertices, int _originalFace=FACE_ORIGIN)
    {
        MyFace myFh(numFace, numVertices, _originalFace);
        add_face(myFh);
    }
    void add_face(int numFace, vector<VertexHandle> _vhs, bool _originalFace=false)
    {
        vector<unsigned> numVertices;
        for (auto v : _vhs) {
            numVertices.push_back(v.idx());
        }
        add_face(numFace, numVertices, _originalFace);
    }

    void update_face(int numFace, MyFace myFh)
    {
        fhs[numFace] = myFh;
    }
    void update_face(int numFace, vector<unsigned> numVertices, int _originalFace=FACE_ORIGIN)
    {
        int id = fhs[numFace].id;
        MyFace myFh(id, numFace, numVertices, _originalFace);
        update_face(numFace, myFh);
    }
    void erase_face(unsigned posFace)
    {
        MyFace _myF = fhs[posFace];
        unsigned cpt=0;
        vector<MyFace>::iterator it;
        for(it = fhs.begin(); it!=fhs.end(); ++it)
        {
            if (cpt==posFace) {
                break;
            }
            cpt++;
        }
        fhs.erase(it);
    }

    int get_pos_of_vh(VertexHandle vh)
    {
        unsigned pos = -1;
        for (unsigned i=0; i<vhs.size(); i++)
        {
            // if (vhs[i].type == VERTEX_SPLIT)    continue;
            if (vhs[i].vh == vh) {
                pos = i;
                break;
            }
        }
        return pos;
    }

    vector<unsigned> get_faces_by_original_id(int numFace)
    {
        vector<unsigned> ids;
        for (unsigned i=0; i<fhs.size(); i++)
        {
            if (fhs[i].numFaceOrigin == numFace) {
                ids.push_back(i);
            }
        }
        return ids;
    }

    int get_common_edge(MyMesh *_mesh, unsigned posF1, unsigned posF2)
    {
        int res = -1;
        MyFace myF1 = fhs[posF1];
        MyFace myF2 = fhs[posF2];

        FaceHandle fh1 = _mesh->face_handle(myF1.numFaceOrigin);
        FaceHandle fh2 = _mesh->face_handle(myF2.numFaceOrigin);
        EdgeHandle eh;

        vector<EdgeHandle> ehs;
        for (MyMesh::FaceEdgeCWIter fe_it = _mesh->fe_cwiter(fh1); fe_it.is_valid(); fe_it++)
        {
            EdgeHandle ehTmp = *fe_it;
            ehs.push_back(ehTmp);
        }
        for (MyMesh::FaceEdgeCWIter fe_it = _mesh->fe_cwiter(fh2); fe_it.is_valid(); fe_it++)
        {
            EdgeHandle ehTmp = *fe_it;
            if (Utils::is_in_vector(ehs, ehTmp)) {
                eh = ehTmp;
                res = eh.idx();
                break;
            }
        }
        return res;
    }

    vector<unsigned> get_pos_of_faces_neigh(MyMesh *_mesh, int numFace)
    {
        vector<unsigned> posFacesNeigh;
        FaceHandle fh = _mesh->face_handle(numFace);
        for (MyMesh::FaceFaceCWIter ff_it = _mesh->ff_cwiter(fh); ff_it.is_valid(); ff_it++)
        {
            FaceHandle fhTmp = *ff_it;
            for (unsigned i=0; i<fhs.size(); i++)
            {
                // On ajoute pas aux voisins les faces splitées puisqu'elles sont déjà à jour.
                if (fhs[i].numFaceOrigin == fhTmp.idx()
                        &&  fhs[i].type==FACE_ORIGIN)
                {
                    posFacesNeigh.push_back(i);
                }
            }
        }
        return posFacesNeigh;
    }

    /*------------------------------------------------------------------------------
     * Renvoie si elle existe, la position d'un point entre @posVA et @posVB dans
     * le tableau de sommets de @myF
     * ----------------------------------------------------------------------------*/
    unsigned get_pos_of_middle_vertex_in_face(MyFace myF, unsigned posvA, unsigned posVB)
    {
        unsigned posM = -1;
        if (Utils::is_next_of(myF.vertices, posvA, posVB)
                ||  Utils::is_pred_of(myF.vertices, posvA, posVB)) {
            return -1;  // pas de vertex entre posVA et posVB.
        }

        posM = Utils::get_next_of(myF.vertices, posvA);
        if (vhs[posM].type==VERTEX_SPLIT
                &&    Utils::get_next_of(myF.vertices, posM) == posVB) {
            return posM;
        }

        posM = Utils::get_pred_of(myF.vertices, posvA);
        if (vhs[posM].type==VERTEX_SPLIT
                &&    Utils::get_pred_of(myF.vertices, posM) == posVB) {
            return posM;
        }

        return -1;
    }

    /*------------------------------------------------------------------------------
     * Met à jour les arêtes/faces des faces voisines de @numFace.
     * ----------------------------------------------------------------------------*/
    void update_faces_neigh(MyMesh *_mesh, int numFace)
    {
        int posFace = -1;
        for (unsigned i=0; i<fhs.size(); i++)
        {
            if (fhs[i].numFaceOrigin == numFace
                    &&  fhs[i].type == FACE_ORIGIN) {
                posFace = i;
            }
        }
        if (posFace<0) {
            qDebug() << "in " << __FUNCTION__ << ": no face original for @numFace=" << numFace;
            qDebug() << "</" << __FUNCTION__ << ">";
            return;
        }

        MyFace myF1 = fhs[posFace];

        vector<unsigned> posFacesNeigh = get_pos_of_faces_neigh(_mesh, numFace);
        for (auto pFN : posFacesNeigh)
        {
            int numEh = get_common_edge(_mesh, posFace, pFN);
            if (numEh<0) {
                qDebug() << "pb in:" << __FUNCTION__;
                continue;
            }
            VertexHandle vhA, vhB;
            UtilsMesh::get_vh_of_edge(_mesh, numEh, vhA, vhB);
            int posVA = get_pos_of_vh(vhA);
            int posVB = get_pos_of_vh(vhB);
            if (posVA<0 ||  posVB<0) {
                qWarning() << "Error in" << __FUNCTION__
                           << ": posVA or posVB not in MyData struct";
                exit(1);
            }

            MyFace myF2 = fhs[pFN];
            int posF1VA = Utils::get_pos_of_val_in_vector(myF1.vertices, static_cast<unsigned>(posVA));
            int posF1VB = Utils::get_pos_of_val_in_vector(myF1.vertices, static_cast<unsigned>(posVB));
            int posF2VA = Utils::get_pos_of_val_in_vector(myF2.vertices, static_cast<unsigned>(posVA));
            int posF2VB = Utils::get_pos_of_val_in_vector(myF2.vertices, static_cast<unsigned>(posVB));
            if (posF1VA<0 ||  posF1VB<0 ||  posF2VA<0 ||  posF2VB<0) {
                qWarning() << "Error in" << __FUNCTION__
                           << ": posFaces are not logics in MyData struct";
                exit(1);
            }

            int posMidVF1 = get_pos_of_middle_vertex_in_face(myF1,
                                                             static_cast<unsigned>(posVA),
                                                             static_cast<unsigned>(posVB));
            int posMidVF2 = get_pos_of_middle_vertex_in_face(myF2,
                                                             static_cast<unsigned>(posVA),
                                                             static_cast<unsigned>(posVB));

            if (posMidVF1>=0    &&  posMidVF2>=0) {
                continue;  // Rien à faire car déjà ajoutés.
            }
            else if (posMidVF1<0 && posMidVF2<0) {
                continue;  // Rien à faire car y a pas de point entre ces 2 positions.
            }
            else if (posMidVF1>=0)
            {
                if (Utils::is_next_of(myF2.vertices,
                                      static_cast<unsigned>(posVA),
                                      static_cast<unsigned>(posVB))) {
                    myF2.add_vertex(posMidVF1, posVB);  // Insère avant posF2VB (et non après)
                    update_face(pFN, myF2);
                }
                // Si précédent
                else if (Utils::is_pred_of(myF2.vertices,
                                           static_cast<unsigned>(posVA),
                                           static_cast<unsigned>(posVB))) {
                    myF2.add_vertex(posMidVF1, posVA);  // Insère avant posF2VA (et non après)
                    update_face(pFN, myF2);
                }
            }
        }
    }

    void trianguler_face_4V(MyFace myF)
    {
        vector<unsigned> idVTmp;
        MyFace myFTmp;
        for (auto v : myF.vertices)
        {
            if (vhs[v].type == VERTEX_SPLIT)
            {
                unsigned v1 = Utils::get_pred_of(myF.vertices, v);
                unsigned v2 = Utils::get_pred_of(myF.vertices, v1);
                idVTmp = {v, v1, v2};
                myFTmp = MyFace(myF.numFaceOrigin, idVTmp, FACE_SPLIT);
                add_face(myFTmp);

                unsigned v3 = Utils::get_next_of(myF.vertices, v);
                unsigned v4 = Utils::get_next_of(myF.vertices, v3);
                idVTmp = {v, v3, v4};
                myFTmp = MyFace(myF.numFaceOrigin, idVTmp, FACE_SPLIT);
                add_face(myFTmp);

                return;
            }
        }
        qWarning() << "Error in" << __FUNCTION__
                   << ": no vertex split";
        exit(1);
    }

    void trianguler_face_5V(MyFace myF)
    {
        for (auto v : myF.vertices)
        {
            if (vhs[v].type == VERTEX_ORIGIN)
            {
                unsigned v1 = Utils::get_pred_of(myF.vertices, v);
                unsigned v2 = Utils::get_next_of(myF.vertices, v);
                if ( (vhs[v1].type==VERTEX_ORIGIN &&  vhs[v2].type==VERTEX_SPLIT)
                     ||  (vhs[v2].type==VERTEX_ORIGIN &&  vhs[v1].type==VERTEX_SPLIT))
                {
                    vector<unsigned> idVTmp = {v, v1, v2};
                    MyFace myFTmp (myF.numFaceOrigin, idVTmp, FACE_SPLIT);
                    add_face(myFTmp);

                    idVTmp.clear();
                    for (auto vTmp : myF.vertices) {
                        if (vTmp == v)  continue;
                        idVTmp.push_back(vTmp);
                    }
                    myFTmp = MyFace(myF.numFaceOrigin, idVTmp, FACE_SPLIT);
                    trianguler_face_4V(myFTmp);
                    return;
                }
            }
        }
        qWarning() << "Error in" << __FUNCTION__
                   << ": no vertex split";
        exit(1);
    }

    void trianguler_face_6V(MyFace myF)
    {
        vector<unsigned> vSplit;
        vector<unsigned> idVTmp;
        MyFace myFTmp;
        for (auto v : myF.vertices)
        {
            if (vhs[v].type == VERTEX_ORIGIN)
            {
                unsigned v1 = Utils::get_pred_of(myF.vertices, v);
                unsigned v2 = Utils::get_next_of(myF.vertices, v);
                if (vhs[v1].type!=VERTEX_SPLIT
                        ||  vhs[v2].type!=VERTEX_SPLIT) {
                    qWarning() << "Error in" << __FUNCTION__
                               << ": v1 or v2 vertexOrigin not permitted";
                    myF.display();
                    exit(1);
                }
                idVTmp = {v, v1, v2};
                myFTmp = MyFace(myF.numFaceOrigin, idVTmp, FACE_SPLIT);
                add_face(myFTmp);
            }
            else vSplit.push_back(v);
        }
        if (vSplit.size() > 3) {
            qWarning() << "Error in" << __FUNCTION__
                       << ": + of 3 split vertices";
            exit(1);
        }
        myFTmp = MyFace(myF.numFaceOrigin, vSplit, FACE_SPLIT);
        add_face(myFTmp);
    }

    /*------------------------------------------------------------------------------
     * Renvoie vrai, si la face a eu besoin d'être retriangulée.
     * Attention la face originale (d'avant le split) n'est pas effacée.
     * ----------------------------------------------------------------------------*/
    bool trianguler_face(unsigned posFace)
    {
        MyFace myF = fhs[posFace];

        if (myF.vertices.size() == 6)
            trianguler_face_6V(myF);
        else if (myF.vertices.size() == 5)
            trianguler_face_5V(myF);
        else if (myF.vertices.size() == 4)
            trianguler_face_4V(myF);
        else {
            return false;
        }
        // erase_face(posFace);
    }

    /*------------------------------------------------------------------------------
     * Retriangule toutes les faces (si besoin)
     * ----------------------------------------------------------------------------*/
    void trianguler()
    {
        vector<unsigned> myIds;
        for (unsigned i=0; i<fhs.size(); i++) {
            if (trianguler_face(i)) {
                myIds.push_back(fhs[i].id);
            }
        }
        unsigned pos=0;
        for (auto id : myIds)
        {
            pos=0;
            for (auto f : fhs)
            {
                if (static_cast<unsigned>(f.id) == id) {
                    erase_face(pos);
                    break;
                }
                pos++;
            }
        }
    }

};




class PSM
{
public:

    MyData myData;

    PSM();
    PSM(MyMesh &mesh, MyMesh::Point &_origin);
    PSM(MyMesh &_meshSrc, MyMesh &_meshTgt,
              MyMesh &_meshSrcSphere, MyMesh &_meshTgt_Sphere,
              MyMesh &_superM, MyMesh::Point &_origin);

    MyMesh get_superM();    // Obsolète

    void build();
    void clear_mesh();
    void generate_by_myData();
    void my_split(int numFace);

    MyMesh::Point my_coords_barycentrique(MyMesh *_mesh, unsigned numFace, MyMesh::Point p);
    QVector3D coords_barycentre(QVector3D A, QVector3D B, QVector3D C, QVector3D P);
    VertexHandle get_plus_proche_vh_of_triangle(MyMesh *_mesh,
                                                VertexHandle A, VertexHandle B, VertexHandle C,
                                                MyMesh::Point p, vector<bool> vertex_used);
    VertexHandle get_plus_proche_vh_of_triangle_bis(MyMesh *_mesh, int numFace, vector<bool> vertex_used);

    void add_pos_vh_split_Tgt_to_Src();

    void fusion(float t);

private:
    MyMesh meshSrc;
    MyMesh meshSrcSphere;
    MyMesh meshTgt;
    MyMesh meshTgtSphere;
    MyMesh &superM;
    MyMesh::Point origin;
    float rayon;

    void build_my_data();
    void handle_intersection(unsigned numFace, MyMesh::Point p, MyMesh::Point pIntersect, vector<bool> &vertex_used);

    unsigned create_new_point_middle_edge(unsigned posF, unsigned posV, unsigned posV2);

    MyMesh::Point my_linear_interpolation(MyMesh::Point p0, MyMesh::Point p1, float t);
    bool my_intersect(MyMesh *_mesh, int face, MyMesh::Point O, MyMesh::Point P, MyMesh::Point &resP);

    vector<FaceHandle> get_3_neighbour_faces(int numFace,
                                                EdgeHandle eh1, EdgeHandle eh2, EdgeHandle eh3);
};



#endif // PSM_H



