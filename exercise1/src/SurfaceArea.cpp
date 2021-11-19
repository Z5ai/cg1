// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "SurfaceArea.h"

#include <iostream>

float triangleArea(OpenMesh::VectorT<float, 3> a, OpenMesh::VectorT<float, 3> b, OpenMesh::VectorT<float, 3> c){
    float area = 0;
    OpenMesh::VectorT<float, 3> ab = { b[0]-a[0], b[1]-a[1], b[2]-a[2]};
    OpenMesh::VectorT<float, 3> ac = { c[0]-a[0], c[1]-a[1], c[2]-a[2]};

    area = 0.5 * ab.cross(ac).length();

    return area;
}


float ComputeSurfaceArea(const HEMesh& m)
{
    float area = 0;
    /* Task 1.2.2 */

    OpenMesh::Vec3f pointA,pointB,pointC;
    std::vector<OpenMesh::Vec3f> points = {};
    HEMesh::ConstFaceVertexIter cfv_It;

    for (HEMesh::FaceIter face_it=m.faces_begin(); face_it!=m.faces_end(); ++face_it){

        cfv_It = m.cfv_iter(*face_it);
        for (cfv_It = m.cfv_iter(*face_it); cfv_It!=m.cfv_iter(*face_it); (++cfv_It).handle()){
            points.push_back(m.point(*cfv_It));
        }

        pointA = m.point(*cfv_It);
        pointB = m.point((++cfv_It).handle());
        pointC = m.point((++cfv_It).handle());

        if(points.size() == 3) {
            area = area + triangleArea(pointA,pointB,pointC);
        } else {

        }

        points.clear();
    }

    std::cout << "Area computation is not implemented." << std::endl;

    return area;
}