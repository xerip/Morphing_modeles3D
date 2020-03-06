#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////    Constructeur    ////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    origin = MyMesh::Point(0.f, 0.f, 0.f);
    // myMetaM = new MetaMesh(mesh, origin);

    // TEST
    b_ptsCtrl = true;
    if (b_ptsCtrl)
    {
        listPtsCtrl.push_back(1);
        listPtsCtrl.push_back(3);
        listPtsCtrl.push_back(5);
        listPtsCtrl.push_back(200);
        listPtsCtrl.push_back(17);
        listPtsCtrl.push_back(21);
        listPtsCtrl.push_back(31);
        listPtsCtrl.push_back(51);
        listPtsCtrl.push_back(81);
        listPtsCtrl.push_back(126);
        listPtsCtrl.push_back(6);
        listPtsCtrl.push_back(12);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete myMetaSrc;
    delete myMetaTgt;
    delete mySM;
}


////////////////////////////////////    Méthodes    ////////////////////////////////////

float MainWindow::rayon_Englobant(MyMesh* _mesh, MyMesh::Point &myOrigine)
{
    MyMesh::VertexIter vh = _mesh->vertices_begin();
    float minx, miny,minz, maxx,maxy,maxz;
    minx=_mesh->point(*vh)[0];
    miny=_mesh->point(*vh)[1];
    minz=_mesh->point(*vh)[2];
    maxx=_mesh->point(*vh)[0];
    maxy=_mesh->point(*vh)[1];
    maxz=_mesh->point(*vh)[2];

    for (MyMesh::VertexIter curvert = _mesh->vertices_begin(); curvert != _mesh->vertices_end(); curvert++)
    {
        if (_mesh->point(*curvert)[0]<minx)
            minx=_mesh->point(*curvert)[0];
        if (_mesh->point(*curvert)[1]<miny)
            miny=_mesh->point(*curvert)[1];
        if (_mesh->point(*curvert)[2]<minz)
            minz=_mesh->point(*curvert)[2];

        if (_mesh->point(*curvert)[0]>maxx)
            maxx=_mesh->point(*curvert)[0];
        if (_mesh->point(*curvert)[1]>maxy)
            maxy=_mesh->point(*curvert)[1];
        if (_mesh->point(*curvert)[2]>maxz)
            maxz=_mesh->point(*curvert)[2];
    }
    // qDebug() << "done";
    float centreX, centreY, centreZ;
    centreX = (minx+maxx)/2;
    centreY = (miny+maxy)/2;
    centreZ = (minz+maxz)/2;
    myOrigine = MyMesh::Point(centreX, centreY, centreZ);

    return sqrt((maxx-centreX)*(maxx-centreX)+(maxy-centreY)*(maxy-centreY)+(maxz-centreZ)*(maxz-centreZ));
}

void MainWindow::display_vertex_control(MyMesh *_mesh, std::vector<int> list_num_vertex)
{
    unsigned int t=list_num_vertex.size();
    for (unsigned int i=0; i<t; i++)
    {
        if (list_num_vertex[i]<(_mesh->n_vertices()))
        {
            VertexHandle vh = _mesh->vertex_handle(list_num_vertex[i]);
            _mesh->set_color(vh, MyMesh::Color(255, 255, 0));
            _mesh->data(vh).thickness = 15;
        }
    }
}

void MainWindow::spheriser(int meshType, float rayon)
{
    MyMesh *_mesh;
    MetaMesh *_metaMesh;
    vector<float> dp;
    if (meshType == MESH_SRC) {
        _mesh = &meshSrc;
        _metaMesh = myMetaSrc;
    }
    else if (meshType >= MESH_TGT){
        _mesh = &meshTgt;
        _metaMesh = myMetaTgt;
    }
    // Verif
    if (!_metaMesh->verif_rayon(origin, rayon)) {
        qDebug() << "rayon trop court -> " << rayon;
        return;
    }
    _metaMesh->spherization(rayon);
    if (meshType == MESH_SRC) {
        distsPointsBase1 = _metaMesh->_distPointsBase();
    }
    else if (meshType >= MESH_TGT){
        distsPointsBase2 = _metaMesh->_distPointsBase();
    }

    displayMesh(_mesh, meshType);
}

void MainWindow::algo()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    qDebug() << "</" << __FUNCTION__ << ">";
}



////////////////////////////////////    IHM / Signaux    ////////////////////////////////////

/*----------------- Super Mesh -----------------*/

void MainWindow::on_pushButton_superMesh_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    mySM = new SuperMesh(meshSrc, meshTgt, origin);
    qDebug() << "</" << __FUNCTION__ << ">";
}


/*----------------- Sphérisation -----------------*/

void MainWindow::deplacement_origine(MyMesh *_mesh)
{
    MyMesh::Point origine, modifieur;
    float rayon1, rayon2;
    rayon1=rayon_Englobant(_mesh, origine);

    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        modifieur=MyMesh::Point(_mesh->point(*curVert)[0]-origine[0],_mesh->point(*curVert)[1]-origine[1],_mesh->point(*curVert)[2]-origine[2]);
        _mesh->set_point(curVert, modifieur);
    }
    rayon2=rayon_Englobant(_mesh, origine);
}
void MainWindow::on_pushButton_spherization_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    MyMesh::Point myOriginSrc;
    deplacement_origine(&meshSrc);
    float rayonSrc = rayon_Englobant(&meshSrc, myOriginSrc);
    qDebug() << "\t rayonSrc = " << rayonSrc;
    MyMesh::Point myOriginTgt;
    deplacement_origine(&meshTgt);
    float rayonTgt = rayon_Englobant(&meshTgt, myOriginTgt);
    qDebug() << "\t rayonTgt = " << rayonTgt;
    if (rayonSrc >= rayonTgt) {
        rayonGeneral = rayonSrc;
        //origin = myOriginSrc;
    }
    else {
        rayonGeneral = rayonTgt;
        //origin = myOriginTgt;
    }

    qDebug() << "\t rayon = " << rayonGeneral;

    // Sphériser Src
    myMetaSrc = new MetaMesh(meshSrc, origin);
    spheriser(MESH_SRC, rayonGeneral);

    // Sphériser Tgt
    myMetaTgt = new MetaMesh(meshTgt, origin);
    spheriser(MESH_TGT, rayonGeneral);

    qDebug() << "</" << __FUNCTION__ << ">";
}

void MainWindow::on_pushButton_spherizationSrc_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    MyMesh::Point myO;
    float myRayon = rayon_Englobant(&meshSrc, myO);
    myMetaSrc = new MetaMesh(meshSrc, origin);
    spheriser(MESH_SRC, myRayon);
    qDebug() << "</" << __FUNCTION__ << ">";
}

void MainWindow::on_pushButton_spherizationTgt_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    MyMesh::Point myO;
    float myRayon = rayon_Englobant(&meshTgt, myO);
    myMetaTgt = new MetaMesh(meshTgt, origin);
    spheriser(MESH_TGT, myRayon);
    qDebug() << "</" << __FUNCTION__ << ">";

}

void MainWindow::on_pushButton_despherization_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    // myMetaSrc->despherization();
    // myMetaTgt->despherization();

    float coef = 1.f;

    for (unsigned i=0; i<distsPointsBase1.size(); i++)
    {
        distsPointsBase1[i] = distsPointsBase1[i] * coef;
    }
    myMetaSrc->despherization(distsPointsBase1);
    displayMesh(&meshSrc, 0);

    for (unsigned i=0; i<distsPointsBase2.size(); i++)
    {
        distsPointsBase2[i] = distsPointsBase2[i] * coef;
    }
    myMetaTgt->despherization(distsPointsBase2);
    displayMesh(&meshTgt, 1);

    qDebug() << "</" << __FUNCTION__ << ">";
}

void MainWindow::on_pushButton_despherizationSrc_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    float coef = 1.f;

    for (unsigned i=0; i<distsPointsBase1.size(); i++)
    {
        distsPointsBase1[i] = distsPointsBase1[i] * coef;
    }
    myMetaSrc->despherization(distsPointsBase1);
    displayMesh(&meshSrc, 0);
    qDebug() << "</" << __FUNCTION__ << ">";
}

void MainWindow::on_pushButton_despherizationTgt_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    float coef = 1.f;
    for (unsigned i=0; i<distsPointsBase2.size(); i++)
    {
        distsPointsBase2[i] = distsPointsBase2[i] * coef;
    }
    myMetaTgt->despherization(distsPointsBase2);
    displayMesh(&meshTgt, 1);
    qDebug() << "</" << __FUNCTION__ << ">";
}


/*----------------- Chargements -----------------*/

// exemple pour charger un fichier .obj
void MainWindow::on_pushButton_chargement_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "../../MeshFiles/", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(meshSrc, fileName.toUtf8().constData());

    meshSrc.update_normals();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&meshSrc);

    if (b_ptsCtrl) {
        display_vertex_control(&meshSrc, listPtsCtrl);
    }

    // on affiche le maillage
    displayMesh(&meshSrc, MESH_SRC);

    qDebug() << "</" << __FUNCTION__ << ">";
}

void MainWindow::on_pushButton_chargement_2_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "../../MeshFiles/", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(meshTgt, fileName.toUtf8().constData());

    meshTgt.update_normals();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&meshTgt);

    if (b_ptsCtrl) {
        display_vertex_control(&meshTgt, listPtsCtrl);
    }

    // on affiche le maillage
    displayMesh(&meshTgt, MESH_TGT);

    qDebug() << "</" << __FUNCTION__ << ">";
}

// exemple pour construire un mesh face par face
void MainWindow::on_pushButton_generer_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    MyMesh &mesh = meshSrc;

    // on construit une liste de sommets
    MyMesh::VertexHandle sommets[8];
    sommets[0] = mesh.add_vertex(MyMesh::Point(-1, -1,  1));
    sommets[1] = mesh.add_vertex(MyMesh::Point( 1, -1,  1));
    sommets[2] = mesh.add_vertex(MyMesh::Point( 1,  1,  1));
    sommets[3] = mesh.add_vertex(MyMesh::Point(-1,  1,  1));
    sommets[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
    sommets[5] = mesh.add_vertex(MyMesh::Point( 1, -1, -1));
    sommets[6] = mesh.add_vertex(MyMesh::Point( 1,  1, -1));
    sommets[7] = mesh.add_vertex(MyMesh::Point(-1,  1, -1));


    // on construit des faces à partir des sommets

    std::vector<MyMesh::VertexHandle> uneNouvelleFace;

    uneNouvelleFace.clear();
    uneNouvelleFace.push_back(sommets[0]);
    uneNouvelleFace.push_back(sommets[1]);
    uneNouvelleFace.push_back(sommets[2]);
    uneNouvelleFace.push_back(sommets[3]);
    mesh.add_face(uneNouvelleFace);

    uneNouvelleFace.clear();
    uneNouvelleFace.push_back(sommets[7]);
    uneNouvelleFace.push_back(sommets[6]);
    uneNouvelleFace.push_back(sommets[5]);
    uneNouvelleFace.push_back(sommets[4]);
    mesh.add_face(uneNouvelleFace);

    uneNouvelleFace.clear();
    uneNouvelleFace.push_back(sommets[1]);
    uneNouvelleFace.push_back(sommets[0]);
    uneNouvelleFace.push_back(sommets[4]);
    uneNouvelleFace.push_back(sommets[5]);
    mesh.add_face(uneNouvelleFace);

    uneNouvelleFace.clear();
    uneNouvelleFace.push_back(sommets[2]);
    uneNouvelleFace.push_back(sommets[1]);
    uneNouvelleFace.push_back(sommets[5]);
    uneNouvelleFace.push_back(sommets[6]);
    mesh.add_face(uneNouvelleFace);

    uneNouvelleFace.clear();
    uneNouvelleFace.push_back(sommets[3]);
    uneNouvelleFace.push_back(sommets[2]);
    uneNouvelleFace.push_back(sommets[6]);
    uneNouvelleFace.push_back(sommets[7]);
    mesh.add_face(uneNouvelleFace);

    uneNouvelleFace.clear();
    uneNouvelleFace.push_back(sommets[0]);
    uneNouvelleFace.push_back(sommets[3]);
    uneNouvelleFace.push_back(sommets[7]);
    uneNouvelleFace.push_back(sommets[4]);
    mesh.add_face(uneNouvelleFace);

    mesh.update_normals();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);

    // on affiche le maillage
    displayMesh(&mesh, 0);

    qDebug() << "</" << __FUNCTION__ << ">";
}


////////////////////////////////////    Fonctions supplémentatires    ////////////////////////////////////

// permet d'initialiser les couleurs et les épaisseurs des élements du maillage
void MainWindow::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

// charge un objet MyMesh dans l'environnement OpenGL
void MainWindow::displayMesh(MyMesh* _mesh, int numFenetre, bool isTemperatureMap, float mapRange)
{
    GLuint* triIndiceArray = new GLuint[_mesh->n_faces() * 3];
    GLfloat* triCols = new GLfloat[_mesh->n_faces() * 3 * 3];
    GLfloat* triVerts = new GLfloat[_mesh->n_faces() * 3 * 3];

    int i = 0;

    if(isTemperatureMap)
    {
        QVector<float> values;

        if(mapRange == -1)
        {
            for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
                values.append(fabs(_mesh->data(*curVert).value));
            qSort(values);
            mapRange = values.at(values.size()*0.8);
            qDebug() << "mapRange" << mapRange;
        }

        float range = mapRange;
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;

        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }
    else
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }

    if (numFenetre == 0) {
        ui->displayWidget->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);
    }
    else {
        ui->displayWidget_2->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);
    }

    delete[] triIndiceArray;
    delete[] triCols;
    delete[] triVerts;

    GLuint* linesIndiceArray = new GLuint[_mesh->n_edges() * 2];
    GLfloat* linesCols = new GLfloat[_mesh->n_edges() * 2 * 3];
    GLfloat* linesVerts = new GLfloat[_mesh->n_edges() * 2 * 3];

    i = 0;
    QHash<float, QList<int> > edgesIDbyThickness;
    for (MyMesh::EdgeIter eit = _mesh->edges_begin(); eit != _mesh->edges_end(); ++eit)
    {
        float t = _mesh->data(*eit).thickness;
        if(t > 0)
        {
            if(!edgesIDbyThickness.contains(t))
                edgesIDbyThickness[t] = QList<int>();
            edgesIDbyThickness[t].append((*eit).idx());
        }
    }
    QHashIterator<float, QList<int> > it(edgesIDbyThickness);
    QList<QPair<float, int> > edgeSizes;
    while (it.hasNext())
    {
        it.next();

        for(int e = 0; e < it.value().size(); e++)
        {
            int eidx = it.value().at(e);

            MyMesh::VertexHandle vh1 = _mesh->to_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh1)[0];
            linesVerts[3*i+1] = _mesh->point(vh1)[1];
            linesVerts[3*i+2] = _mesh->point(vh1)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;

            MyMesh::VertexHandle vh2 = _mesh->from_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh2)[0];
            linesVerts[3*i+1] = _mesh->point(vh2)[1];
            linesVerts[3*i+2] = _mesh->point(vh2)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;
        }
        edgeSizes.append(qMakePair(it.key(), it.value().size()));
    }

    if (numFenetre == 0) {
        ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);
    }
    else {
        ui->displayWidget_2->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);
    }

    delete[] linesIndiceArray;
    delete[] linesCols;
    delete[] linesVerts;

    GLuint* pointsIndiceArray = new GLuint[_mesh->n_vertices()];
    GLfloat* pointsCols = new GLfloat[_mesh->n_vertices() * 3];
    GLfloat* pointsVerts = new GLfloat[_mesh->n_vertices() * 3];

    i = 0;
    QHash<float, QList<int> > vertsIDbyThickness;
    for (MyMesh::VertexIter vit = _mesh->vertices_begin(); vit != _mesh->vertices_end(); ++vit)
    {
        float t = _mesh->data(*vit).thickness;
        if(t > 0)
        {
            if(!vertsIDbyThickness.contains(t))
                vertsIDbyThickness[t] = QList<int>();
            vertsIDbyThickness[t].append((*vit).idx());
        }
    }
    QHashIterator<float, QList<int> > vitt(vertsIDbyThickness);
    QList<QPair<float, int> > vertsSizes;

    while (vitt.hasNext())
    {
        vitt.next();

        for(int v = 0; v < vitt.value().size(); v++)
        {
            int vidx = vitt.value().at(v);

            pointsVerts[3*i+0] = _mesh->point(_mesh->vertex_handle(vidx))[0];
            pointsVerts[3*i+1] = _mesh->point(_mesh->vertex_handle(vidx))[1];
            pointsVerts[3*i+2] = _mesh->point(_mesh->vertex_handle(vidx))[2];
            pointsCols[3*i+0] = _mesh->color(_mesh->vertex_handle(vidx))[0];
            pointsCols[3*i+1] = _mesh->color(_mesh->vertex_handle(vidx))[1];
            pointsCols[3*i+2] = _mesh->color(_mesh->vertex_handle(vidx))[2];
            pointsIndiceArray[i] = i;
            i++;
        }
        vertsSizes.append(qMakePair(vitt.key(), vitt.value().size()));
    }

    if (numFenetre == 0) {
        ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);
    }
    else {
        ui->displayWidget_2->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);
    }

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}


