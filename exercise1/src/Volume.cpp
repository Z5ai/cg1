// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "Volume.h"

#include <iostream>

float ComputeVolume(const HEMesh& m)
{
    float vol = 0;
    for (auto f_it=m.faces_begin(); f_it!=m.faces_end(); ++f_it){ // iterate all faces

        HEMesh::ConstFaceVertexIter fv_it = m.cfv_iter(*f_it); // vector_iter fh_it is created based on the current face *f_it
        OpenMesh::Vec3f vec0 = m.point(*(fv_it++)); // could this also be done by v = *f_it.vertices; TripleProduct = f(v[0],v[1],v[2]);?
        OpenMesh::Vec3f vec1 = m.point(*(fv_it++));
        OpenMesh::Vec3f vec2 = m.point(*(fv_it++));
        // probably throw an error or message, if one face has more vertices than 3
        // if calculation is done with cube: "Triangulating mesh." - message is displayed in console. afterwards the area and volume is different than before.
        auto triple_product = abs(OpenMesh::dot(vec0, OpenMesh::cross(vec1, vec2)));

        vol += triple_product;
    }
    return vol/6;
}