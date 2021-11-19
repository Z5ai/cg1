// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "ShellExtraction.h"

#include <queue>

unsigned int ExtractShells(HEMesh& m, OpenMesh::FPropHandleT<int> perFaceShellIndex)
{
	//reset the shell indices to -1 for every face
	for (auto f : m.faces())
		m.property(perFaceShellIndex, f) = -1;

    /*Task 1.2.3*/

    /* region-grow pseudocode
    unsigned int num_components = 0;
    std::vector<OpenMesh::FaceHandle> all_faces; //pool of all faces with id -1
    std::vector<OpenMesh::FaceHandle> neighbors; //pool of neighboring faces

     foreach(face){
        add to all_faces
     }

     if(all_faces.empty(){
        return 0;
     }

    while(all_faces not empty){
    current_face = take one out of all_faces (pop?)
     do {
        assign num_components to current_face (as shell_id)
        foreach(current_face adjacent faces)        //add its neighbors with id -1 to neighbors (if not already there)
            if(shell_id == -1 && not in neighbors yet) {
                add to neighbors
                remove from all_faces
            }
        remove current_face from neighbors
    } while(!neighbors.empty())

    num_components+=1;
    } */


	return num_components;
}