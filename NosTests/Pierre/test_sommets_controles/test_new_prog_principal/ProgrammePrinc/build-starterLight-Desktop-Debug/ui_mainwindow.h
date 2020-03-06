/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <meshviewerwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_chargement;
    QSpinBox *spinBox;
    QPushButton *add_vertex;
    QPushButton *pushButton_spherizationSrc;
    QPushButton *pushButton_despherizationSrc;
    MeshViewerWidget *displayWidget;
    QFrame *line_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_chargement_2;
    QSpinBox *spinBox_2;
    QPushButton *add_vertex_2;
    QPushButton *pushButton_spherizationTgt;
    QPushButton *pushButton_despherizationTgt;
    MeshViewerWidget *displayWidget_2;
    QFrame *line;
    QVBoxLayout *verticalLayout;
    MeshViewerWidget *displayWidget_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_generer;
    QPushButton *pushButton_spherization;
    QPushButton *pushButton_despherization;
    QPushButton *pushButton_superMesh;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(750, 438);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_4 = new QHBoxLayout(centralWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_chargement = new QPushButton(centralWidget);
        pushButton_chargement->setObjectName(QStringLiteral("pushButton_chargement"));
        pushButton_chargement->setMinimumSize(QSize(100, 0));

        horizontalLayout_2->addWidget(pushButton_chargement);

        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setEnabled(false);

        horizontalLayout_2->addWidget(spinBox);

        add_vertex = new QPushButton(centralWidget);
        add_vertex->setObjectName(QStringLiteral("add_vertex"));

        horizontalLayout_2->addWidget(add_vertex);

        pushButton_spherizationSrc = new QPushButton(centralWidget);
        pushButton_spherizationSrc->setObjectName(QStringLiteral("pushButton_spherizationSrc"));

        horizontalLayout_2->addWidget(pushButton_spherizationSrc);

        pushButton_despherizationSrc = new QPushButton(centralWidget);
        pushButton_despherizationSrc->setObjectName(QStringLiteral("pushButton_despherizationSrc"));

        horizontalLayout_2->addWidget(pushButton_despherizationSrc);


        verticalLayout_2->addLayout(horizontalLayout_2);

        displayWidget = new MeshViewerWidget(centralWidget);
        displayWidget->setObjectName(QStringLiteral("displayWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(displayWidget->sizePolicy().hasHeightForWidth());
        displayWidget->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(displayWidget);


        verticalLayout_4->addLayout(verticalLayout_2);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_chargement_2 = new QPushButton(centralWidget);
        pushButton_chargement_2->setObjectName(QStringLiteral("pushButton_chargement_2"));
        pushButton_chargement_2->setMinimumSize(QSize(100, 0));

        horizontalLayout_3->addWidget(pushButton_chargement_2);

        spinBox_2 = new QSpinBox(centralWidget);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));

        horizontalLayout_3->addWidget(spinBox_2);

        add_vertex_2 = new QPushButton(centralWidget);
        add_vertex_2->setObjectName(QStringLiteral("add_vertex_2"));

        horizontalLayout_3->addWidget(add_vertex_2);

        pushButton_spherizationTgt = new QPushButton(centralWidget);
        pushButton_spherizationTgt->setObjectName(QStringLiteral("pushButton_spherizationTgt"));

        horizontalLayout_3->addWidget(pushButton_spherizationTgt);

        pushButton_despherizationTgt = new QPushButton(centralWidget);
        pushButton_despherizationTgt->setObjectName(QStringLiteral("pushButton_despherizationTgt"));

        horizontalLayout_3->addWidget(pushButton_despherizationTgt);


        verticalLayout_3->addLayout(horizontalLayout_3);

        displayWidget_2 = new MeshViewerWidget(centralWidget);
        displayWidget_2->setObjectName(QStringLiteral("displayWidget_2"));
        sizePolicy.setHeightForWidth(displayWidget_2->sizePolicy().hasHeightForWidth());
        displayWidget_2->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(displayWidget_2);


        verticalLayout_4->addLayout(verticalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout_4);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        displayWidget_3 = new MeshViewerWidget(centralWidget);
        displayWidget_3->setObjectName(QStringLiteral("displayWidget_3"));
        sizePolicy.setHeightForWidth(displayWidget_3->sizePolicy().hasHeightForWidth());
        displayWidget_3->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(displayWidget_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_generer = new QPushButton(centralWidget);
        pushButton_generer->setObjectName(QStringLiteral("pushButton_generer"));

        horizontalLayout->addWidget(pushButton_generer);

        pushButton_spherization = new QPushButton(centralWidget);
        pushButton_spherization->setObjectName(QStringLiteral("pushButton_spherization"));

        horizontalLayout->addWidget(pushButton_spherization);

        pushButton_despherization = new QPushButton(centralWidget);
        pushButton_despherization->setObjectName(QStringLiteral("pushButton_despherization"));

        horizontalLayout->addWidget(pushButton_despherization);

        pushButton_superMesh = new QPushButton(centralWidget);
        pushButton_superMesh->setObjectName(QStringLiteral("pushButton_superMesh"));

        horizontalLayout->addWidget(pushButton_superMesh);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_4->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 750, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton_chargement->setText(QApplication::translate("MainWindow", "Charger OBJ", Q_NULLPTR));
        add_vertex->setText(QApplication::translate("MainWindow", "ajouter sommet", Q_NULLPTR));
        pushButton_spherizationSrc->setText(QApplication::translate("MainWindow", "Spherization", Q_NULLPTR));
        pushButton_despherizationSrc->setText(QApplication::translate("MainWindow", "Despherization", Q_NULLPTR));
        pushButton_chargement_2->setText(QApplication::translate("MainWindow", "Charger OBJ", Q_NULLPTR));
        add_vertex_2->setText(QApplication::translate("MainWindow", "ajouter sommet", Q_NULLPTR));
        pushButton_spherizationTgt->setText(QApplication::translate("MainWindow", "Spherization", Q_NULLPTR));
        pushButton_despherizationTgt->setText(QApplication::translate("MainWindow", "Despherization", Q_NULLPTR));
        pushButton_generer->setText(QApplication::translate("MainWindow", "G\303\251n\303\251rer mesh", Q_NULLPTR));
        pushButton_spherization->setText(QApplication::translate("MainWindow", "Spherization", Q_NULLPTR));
        pushButton_despherization->setText(QApplication::translate("MainWindow", "Despherization", Q_NULLPTR));
        pushButton_superMesh->setText(QApplication::translate("MainWindow", "SuperMesh", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
