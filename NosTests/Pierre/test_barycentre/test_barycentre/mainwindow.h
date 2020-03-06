#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "utilsMesh.h"
#include "metamesh.h"
#include "supermesh.h"

#include <vector>
#include <iostream>
#include <QVector3D>
using namespace std;

namespace Ui {
class MainWindow;
}

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

enum window_type {WINDOW_1, WINDOW_2, WINDOW_3};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayMesh(MyMesh *_mesh, int numFenetre, bool isTemperatureMap = false, float mapRange = -1);
    void resetAllColorsAndThickness(MyMesh* _mesh);

    // Mes fonctions
    void algo();
    void display_vertex_control(MyMesh* _mesh, std::vector<int> list_num_vertex);

private slots:

    void on_pushButton_my_test_clicked();

    void on_pushButton_chargement_clicked();
    void on_pushButton_chargement_2_clicked();
    void on_pushButton_generer_clicked();


private:
    // IMPORTANTS
    Ui::MainWindow *ui;
    MyMesh meshSrc;
    MyMesh meshTgt;
    MyMesh::Point origin;
    float rayonGeneral;
    MetaMesh *myMetaSrc;
    MetaMesh *myMetaTgt;
    vector<int> listPtsCtrl;
    SuperMesh *mySM;

    float rayon_Englobant(MyMesh* _mesh, MyMesh::Point &myOrigine);
    void deplacement_origine(MyMesh *_mesh);
    void spheriser(int windowNum, float rayonGeneral);


    // UTILS
    bool b_ptsCtrl = false;

    // TMP
    vector<MyMesh::Point> pointsBase;
    vector<float> distsPointsBase1;
    vector<float> distsPointsBase2;
    bool precision = true;
};

#endif // MAINWINDOW_H