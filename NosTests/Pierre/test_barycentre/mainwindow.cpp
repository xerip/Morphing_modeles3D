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
    b_ptsCtrl = false;
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
        if (list_num_vertex[i] < (_mesh->n_vertices()) )
        {
            VertexHandle vh = _mesh->vertex_handle(list_num_vertex[i]);
            _mesh->set_color(vh, MyMesh::Color(255, 255, 0));
            _mesh->data(vh).thickness = 15;
        }
    }
}

void MainWindow::deplacement_origine(MyMesh *_mesh)
{
    MyMesh::Point origine, modifieur;
    rayon_Englobant(_mesh, origine);

    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        modifieur=MyMesh::Point(_mesh->point(*curVert)[0]-origine[0],
                                _mesh->point(*curVert)[1]-origine[1],
                                _mesh->point(*curVert)[2]-origine[2]);
        _mesh->set_point(curVert, modifieur);
    }
    rayon_Englobant(_mesh, origine);
}

void MainWindow::spheriser(int windowNum, float rayon)
{
    MyMesh *_mesh;
    MetaMesh *_metaMesh;
    vector<float> dp;
    if (windowNum == WINDOW_1) {
        _mesh = &meshSrc;
        _metaMesh = myMetaSrc;
    }
    else if (windowNum >= WINDOW_2){
        _mesh = &meshTgt;
        _metaMesh = myMetaTgt;
    }
//    // Verif
//    if (!_metaMesh->verif_rayon()) {
//        qDebug() << "<\t" << __FUNCTION__ << ">";
//        qDebug() << "\t\trayon trop court -> " << rayon;
//        qDebug() << "\t</" << __FUNCTION__ << ">";
//        return;
//    }
    _metaMesh->spherization(rayon);
    if (windowNum == WINDOW_1) {
        distsPointsBase1 = _metaMesh->_distPointsBase();
    }
    else if (windowNum >= WINDOW_2) {
        distsPointsBase2 = _metaMesh->_distPointsBase();
    }

    displayMesh(_mesh, windowNum);
}

void MainWindow::algo()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    qDebug() << "</" << __FUNCTION__ << ">";
}



////////////////////////////////////    IHM / Signaux    ////////////////////////////////////

void MainWindow::on_pushButton_my_test_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";
    mySM = new SuperMesh(meshSrc, origin);

    vector<int> sommetsRajoutes;
    int numFace = 0;
    mySM->my_split(numFace, sommetsRajoutes);
    meshSrc.update_normals();
    qDebug() << "\tmeshSrc.n_faces() = " << meshSrc.n_faces();

    //resetAllColorsAndThickness(&meshSrc);
    displayMesh(&meshSrc, WINDOW_1);
    displayMesh(&meshSrc, WINDOW_1);
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
    displayMesh(&meshSrc, WINDOW_1);

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
    displayMesh(&meshTgt, WINDOW_2);

    qDebug() << "</" << __FUNCTION__ << ">";
}

/*float dot(QVector3D v1, QVector3D v2)
{
    return QVector3D::dotProduct(v1, v2);
}*/

void test_barycentre(QVector3D A, QVector3D B, QVector3D C, QVector3D P)
{
    float d = (QVector3D::dotProduct(C-A,C-A)*QVector3D::dotProduct(B-A,B-A))-(QVector3D::dotProduct(C-A,B-A)*QVector3D::dotProduct(B-A,C-A));
    float beta = ((QVector3D::dotProduct(B-A,B-A)*QVector3D::dotProduct(P-A,C-A))-(QVector3D::dotProduct(B-A,C-A)*QVector3D::dotProduct(P-A,B-A)))/d;
    float gamma = ((QVector3D::dotProduct(C-A,C-A)*QVector3D::dotProduct(P-A,B-A))-(QVector3D::dotProduct(C-A,B-A)*QVector3D::dotProduct(P-A,C-A)))/d;
    std::cout << "alpha : " << 1-(beta+gamma) << std::endl;
    std::cout << "beta : " << beta << std::endl;
    std::cout << "gamma : " << gamma << std::endl;
}

// exemple pour construire un mesh face par face
void MainWindow::on_pushButton_generer_clicked()
{
    qDebug() << "<" << __FUNCTION__ << ">";

    MyMesh &mesh = meshSrc;

    // on construit une liste de sommets
    MyMesh::VertexHandle sommets[11];
    sommets[0] = mesh.add_vertex(MyMesh::Point(-1.f, 0.5f,  1.f));
    sommets[1] = mesh.add_vertex(MyMesh::Point( 1.f, 0.5f,  1.f));
    sommets[2] = mesh.add_vertex(MyMesh::Point( 0.f,  0.f,  1.f));
    sommets[3] = mesh.add_vertex(MyMesh::Point(-1.f,  -0.5f,  1.f));
    sommets[4] = mesh.add_vertex(MyMesh::Point(0.f,  -1.f,  1.f));
    sommets[5] = mesh.add_vertex(MyMesh::Point( 0.f, 1.f, 1.f));
    sommets[6] = mesh.add_vertex(MyMesh::Point(1.f,  -0.5f,  1.f));

    sommets[7] = mesh.add_vertex(MyMesh::Point(-1.5f,  0.f,  1.f));
    sommets[8] = mesh.add_vertex(MyMesh::Point(-1.5,  1.f,  1.f));

    sommets[9] = mesh.add_vertex(MyMesh::Point(1.5f,  0.f,  1.f));
    sommets[10] = mesh.add_vertex(MyMesh::Point(1.5f,  1.f,  1.f));

    // on construit des faces à partir des sommets
    mesh.add_face(sommets[0], sommets[1], sommets[2]);
    mesh.add_face(sommets[0], sommets[2], sommets[3]);
    mesh.add_face(sommets[2], sommets[4], sommets[3]);
    mesh.add_face(sommets[0], sommets[5], sommets[1]);
    mesh.add_face(sommets[1], sommets[6], sommets[2]);
    mesh.add_face(sommets[2], sommets[6], sommets[4]);

    mesh.add_face(sommets[0], sommets[7], sommets[8]);
    mesh.add_face(sommets[1], sommets[9], sommets[10]);

    mesh.update_normals();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);
//    mesh.set_color(sommets[2], MyMesh::Color(255, 100, 50));
//    mesh.data(sommets[2]).thickness = 8;
//    mesh.set_color(sommets[3], MyMesh::Color(255, 100, 50));
//    mesh.data(sommets[3]).thickness = 8;
//    mesh.set_color(sommets[4], MyMesh::Color(255, 100, 50));
//    mesh.data(sommets[4]).thickness = 8;

    // on affiche le maillage
    displayMesh(&mesh, WINDOW_1);   
    test_barycentre(QVector3D(3,5,3), QVector3D(0,2,-0.54), QVector3D(0,0,3), QVector3D(1,1,1));
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

    if (numFenetre == WINDOW_1) {
        ui->displayWidget->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);
    }
//    else if (numFenetre == WINDOW_2) {
//        ui->displayWidget_2->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);
//    }
//    else if (numFenetre >= WINDOW_3) {
//        ui->displayWidget_3->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);
//    }

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

    if (numFenetre == WINDOW_1) {
        ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);
    }
//    else if (numFenetre == WINDOW_2) {
//        ui->displayWidget_2->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);
//    }
//    else if (numFenetre == WINDOW_3) {
//        ui->displayWidget_3->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);
//    }

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

    if (numFenetre == WINDOW_1) {
        ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);
    }
//    else if (numFenetre == WINDOW_2) {
//        ui->displayWidget_2->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);
//    }
//    else if (numFenetre == WINDOW_3) {
//        ui->displayWidget_3->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);
//    }

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}


