/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
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
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    MeshViewerWidget *displayWidget;
    QPushButton *pushButton_chargement;
    QPushButton *pushButton_spherizationSrc;
    QPushButton *pushButton_despherizationSrc;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_2;
    QPushButton *pushButton_generer;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QPushButton *pushButton_spherization;
    QPushButton *pushButton_despherization;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_3;
    MeshViewerWidget *displayWidget_2;
    QPushButton *pushButton_chargement_2;
    QPushButton *pushButton_spherizationTgt;
    QPushButton *pushButton_despherizationTgt;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1013, 610);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        displayWidget = new MeshViewerWidget(centralWidget);
        displayWidget->setObjectName(QStringLiteral("displayWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(displayWidget->sizePolicy().hasHeightForWidth());
        displayWidget->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(displayWidget);

        pushButton_chargement = new QPushButton(centralWidget);
        pushButton_chargement->setObjectName(QStringLiteral("pushButton_chargement"));
        pushButton_chargement->setMinimumSize(QSize(200, 0));

        verticalLayout_2->addWidget(pushButton_chargement);

        pushButton_spherizationSrc = new QPushButton(centralWidget);
        pushButton_spherizationSrc->setObjectName(QStringLiteral("pushButton_spherizationSrc"));

        verticalLayout_2->addWidget(pushButton_spherizationSrc);

        pushButton_despherizationSrc = new QPushButton(centralWidget);
        pushButton_despherizationSrc->setObjectName(QStringLiteral("pushButton_despherizationSrc"));

        verticalLayout_2->addWidget(pushButton_despherizationSrc);


        horizontalLayout->addLayout(verticalLayout_2);

        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(150);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        widget_2->setMinimumSize(QSize(150, 0));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(4);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout->addItem(verticalSpacer_2);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        pushButton_generer = new QPushButton(widget_2);
        pushButton_generer->setObjectName(QStringLiteral("pushButton_generer"));

        verticalLayout->addWidget(pushButton_generer);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        pushButton_spherization = new QPushButton(widget_2);
        pushButton_spherization->setObjectName(QStringLiteral("pushButton_spherization"));

        verticalLayout->addWidget(pushButton_spherization);

        pushButton_despherization = new QPushButton(widget_2);
        pushButton_despherization->setObjectName(QStringLiteral("pushButton_despherization"));

        verticalLayout->addWidget(pushButton_despherization);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        displayWidget_2 = new MeshViewerWidget(centralWidget);
        displayWidget_2->setObjectName(QStringLiteral("displayWidget_2"));
        sizePolicy.setHeightForWidth(displayWidget_2->sizePolicy().hasHeightForWidth());
        displayWidget_2->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(displayWidget_2);

        pushButton_chargement_2 = new QPushButton(centralWidget);
        pushButton_chargement_2->setObjectName(QStringLiteral("pushButton_chargement_2"));
        pushButton_chargement_2->setMinimumSize(QSize(200, 0));

        verticalLayout_3->addWidget(pushButton_chargement_2);

        pushButton_spherizationTgt = new QPushButton(centralWidget);
        pushButton_spherizationTgt->setObjectName(QStringLiteral("pushButton_spherizationTgt"));

        verticalLayout_3->addWidget(pushButton_spherizationTgt);

        pushButton_despherizationTgt = new QPushButton(centralWidget);
        pushButton_despherizationTgt->setObjectName(QStringLiteral("pushButton_despherizationTgt"));

        verticalLayout_3->addWidget(pushButton_despherizationTgt);


        horizontalLayout->addLayout(verticalLayout_3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1013, 22));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_chargement->setText(QApplication::translate("MainWindow", "Charger OBJ", nullptr));
        pushButton_spherizationSrc->setText(QApplication::translate("MainWindow", "Spherization", nullptr));
        pushButton_despherizationSrc->setText(QApplication::translate("MainWindow", "Despherization", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "TMP / Tests", nullptr));
        pushButton_generer->setText(QApplication::translate("MainWindow", "G\303\251n\303\251rer mesh", nullptr));
        label->setText(QApplication::translate("MainWindow", "Doubles transformations", nullptr));
        pushButton_spherization->setText(QApplication::translate("MainWindow", "Spherization", nullptr));
        pushButton_despherization->setText(QApplication::translate("MainWindow", "Despherization", nullptr));
        pushButton_chargement_2->setText(QApplication::translate("MainWindow", "Charger OBJ", nullptr));
        pushButton_spherizationTgt->setText(QApplication::translate("MainWindow", "Spherization", nullptr));
        pushButton_despherizationTgt->setText(QApplication::translate("MainWindow", "Despherization", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
