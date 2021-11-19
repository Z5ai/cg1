// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "SurfaceArea.h"

#include <iostream>

float triangleArea(std::vector<OpenMesh::Vec3f> points){
    float area = 0;
    OpenMesh::VectorT<float, 3> ab = { points[1][0]-points[0][0], points[1][1]-points[0][1], points[1][2]-points[0][2]};
    OpenMesh::VectorT<float, 3> ac = { points[2][0]-points[0][0], points[2][1]-points[0][1], points[2][2]-points[0][2]};

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
        for (cfv_It = m.cfv_iter(*face_it); cfv_It.is_valid(); (++cfv_It).handle()){
            points.push_back(m.point(*cfv_It));
        }

        //pointA = m.point(*cfv_It);
        //pointB = m.point((++cfv_It).handle());
        //pointC = m.point((++cfv_It).handle());

        if(points.size() == 3) {
            area = area + triangleArea(points);
        } else {

        }

        points.clear();
    }

    std::cout << "Area computation is not implemented." << std::endl;

    return area;
}