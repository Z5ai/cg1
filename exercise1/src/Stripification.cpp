// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "Stripification.h"

#include <random>
#include "sample_set.h"




unsigned int ExtractTriStrips(HEMesh& mesh, OpenMesh::FPropHandleT<int> perFaceStripIdProperty, unsigned int nTrials)
{
	//prepare random engine
	std::mt19937 eng;

	//initialize strip index to -1 for each face
	for (auto f : mesh.faces())
		mesh.property(perFaceStripIdProperty, f) = -1;

	int nStrips = 0;

	/*Task 1.2.5*/


    stripification pseudocode
    k from the lecture = nTrials

    while(faces with id -1 exist){
    for(int i = 0; i < nTrials; i++){
        randomly select a halfedge hi with attribute id -1
        make a strip out of it, forwards and backwards
            hi.parity = 0       //give current halfedge parity 0
            add hi to list of halfedges
            iterate through halfedges (see slide 14)
            save them all to list, if not in it already
            if we find one thats already in the list, break
        save pointer to that list to "list of lists" (trail list)
    }

    select longest of the nTrials strips, go with that one
    assign nStrips to all faces making up the strips
    nStrips+=1;
    }

	return nStrips;
}