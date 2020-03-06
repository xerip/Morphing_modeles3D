#include "supermesh.h"

SuperMesh::SuperMesh(MyMesh &_meshSrc, MyMesh &_meshTgt, MyMesh::Point &_origin) :
    meshSrc(_meshSrc), meshTgt(_meshTgt), origin(_origin)
{
    ;
}

void SuperMesh::alignement(std::vector<int> list1, std::vector<int> list2)
{
    std::vector<MyMesh::Point> vect_deplacement;
    MyMesh::Point deplacement;
    MyMesh* mesh=&meshSrc;
    for(int i=0;i<min(list1.size(),list2.size());i++)
    {
        qDebug()<< "x = " << mesh->point(1)[0];
        /*
        deplacement=MyMesh::Point(meshSrc->point(list1[i])[0]-meshTgt->point(list2[i])[0],
                meshSrc->point(list1[i])[1]-meshTgt->point(list2[i])[1],
                meshSrc->point(list1[i])[2]-meshTgt->point(list2[i])[2];)

                */
        //vect_deplacement.push_back(meshSrc->point(*list1[i])[0]-meshTgt->point(*list2[i])[0],meshSrc->point(*list1[i])[1]-meshTgt->point(*list2[i])[1],meshSrc->point(*list1[i])[2]-meshTgt->point(*list2[i])[2]);

        // modifieur=MyMesh::Point(_mesh->point(*curVert)[0]-origine[0],_mesh->point(*curVert)[1]-origine[1],_mesh->point(*curVert)[2]-origine[2]);
        //_mesh->set_point(curVert, modifieur);

        //_mesh->point(*vh)[0]

        //list1[i].x()-list2[i].x(),list1[i].y()-list2[i].y(),list1[i].z()-list2[i].z());
    }
}

