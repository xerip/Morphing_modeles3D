#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "utilsMesh.h"
#include "metamesh.h"
#include "supermesh.h"
#include "raycasting.h"

#include <vector>
#include <iostream>

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
    void on_pushButton_chargement_clicked();
    void on_pushButton_chargement_2_clicked();

    void on_pushButton_superMesh_clicked();

    //    void on_pushButton_spherization_clicked();
    void on_pushButton_spherizationSrc_clicked();
    void on_pushButton_spherizationTgt_clicked();

    void on_pushButton_fusion_clicked();
    void on_pushButton_despherizationSrc_clicked();
    void on_pushButton_despherizationTgt_clicked();

    void on_spinBox_valueChanged(int arg1);
    void on_add_vertex_src_clicked();
    void on_spinBox_2_valueChanged(int arg1);
    void on_add_vertex_tgt_clicked();

    void on_doubleSpinBox_pourcentage_valueChanged(double val);

private:
    // IMPORTANTS
    Ui::MainWindow *ui;
    MyMesh meshSrc;
    MyMesh meshSrcSphere;   bool b_srcSphere=false;
    MyMesh meshTgt;
    MyMesh meshTgtSphere;   bool b_tgtSphere=false;
    MyMesh::Point origin;
    float rayonGeneral;
    MetaMesh *myMetaSrc;
    MetaMesh *myMetaTgt;
    vector<int> list_vertex_control_src;
    vector<int> list_vertex_control_tgt;
    SuperMesh *mySM;
    MyMesh superM;

    float pourcentage=0.f;

    void modify_mesh_size(MyMesh* _mesh1, MyMesh *_mesh2);
    float rayon_Englobant(MyMesh* _mesh, MyMesh::Point &myOrigine);
    void deplacement_origine(MyMesh *_mesh);
    void spheriser(int windowNum, float rayonGeneral);
    void make_psm();


    // UTILS
    bool b_ptsCtrl = true;

    // TMP
    vector<MyMesh::Point> pointsBase;
    vector<float> distsPointsBase1;
    vector<float> distsPointsBase2;
    bool precision = true;
};

#endif // MAINWINDOW_H
