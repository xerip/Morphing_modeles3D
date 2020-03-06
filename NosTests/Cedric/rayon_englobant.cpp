

float MainWindow::rayon_Englobant(MyMesh* _mesh, QVector3D &myOrigine)
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
    qDebug() << "done";
    float centreX, centreY, centreZ;
    centreX = (minx+maxx)/2;
    centreY = (miny+maxy)/2;
    centreZ = (minz+maxz)/2;
    myOrigine = QVector3D(centreX, centreY, centreZ);

    return sqrt((maxx-centreX)*(maxx-centreX)+(maxy-centreY)*(maxy-centreY)+(maxz-centreZ)*(maxz-centreZ));
}
