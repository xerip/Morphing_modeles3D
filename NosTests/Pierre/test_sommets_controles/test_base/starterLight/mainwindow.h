#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <iostream>

namespace Ui {
class MainWindow;
}

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

struct MyTraits : public OpenMesh::DefaultTraits
{
    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    // store the previous halfedge
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    // use face normals face colors
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    EdgeAttributes( OpenMesh::Attributes::Color | OpenMesh::Attributes::Status );
    // vertex thickness
    VertexTraits{float thickness; float value;};
    // edge thickness
    EdgeTraits{float thickness;};
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayMesh(MyMesh *_mesh, bool isTemperatureMap = false, float mapRange = -1);
    void displayMesh_2(MyMesh *_mesh, bool isTemperatureMap = false, float mapRange = -1);
    void resetAllColorsAndThickness(MyMesh* _mesh);
    void affiche_vertex_control(MyMesh* _mesh, std::vector<int> list_num_vertex);

private slots:
    void on_pushButton_chargement_clicked();
    void on_pushButton_chargement_2_clicked();

    void on_pushButton_generer_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_add_Vertex_clicked();

    void on_spinBox_2_valueChanged(int arg1);


    void on_add_vertex_2_clicked();

private:

    MyMesh mesh_src;
    MyMesh mesh_tgt;
    std::vector<int> list_vertex_control_src;
    std::vector<int> list_vertex_control_tgt;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
