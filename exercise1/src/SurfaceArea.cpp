// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "SurfaceArea.h"

#include <iostream>

float ComputeSurfaceArea(const HEMesh& m)
{
    float area = 0;
    for (auto f_it=m.faces_begin(); f_it!=m.faces_end(); ++f_it){ // iterate all faces
        float face_area = 0;

        HEMesh::ConstFaceHalfedgeIter fh_it = m.cfh_iter(*f_it); // halfedge_iter fh_it is created based on the current face *f_it
        for(; fh_it.is_valid(); ++fh_it) { // iterate all halfedges on one face
            OpenMesh::SmartVertexHandle vh0 = fh_it->from();
            OpenMesh::SmartVertexHandle vh1 = fh_it->to();
            OpenMesh::Vec3f vec0 = m.point(vh0);
            OpenMesh::Vec3f vec1 = m.point(vh1);
            OpenMesh::Vec3f cross_product = OpenMesh::cross(vec0, vec1);
            face_area += OpenMesh::sqrnorm(cross_product);
        }

        area += abs(face_area) / 2;
    }
    return area;
}