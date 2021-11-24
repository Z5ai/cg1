// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "Stripification.h"

#include <random>
#include "sample_set.h"




unsigned int ExtractTriStrips(HEMesh& mesh, OpenMesh::FPropHandleT<int> perFaceStripIdProperty, unsigned int nTrials)
{
	//prepare random engine
	std::mt19937 eng;            //properly initialize later

	//initialize strip index to -1 for each face
	for (auto f : mesh.faces())
		mesh.property(perFaceStripIdProperty, f) = -1;

	int nStrips = 0;

	/*Task 1.2.5*/

    std::vector<OpenMesh::FaceHandle> remaining_faces;
    std::vector<OpenMesh::HalfedgeHandle> trial;
    std::vector<OpenMesh::HalfedgeHandle> trial_max;
    OpenMesh::FaceHandle starting_face;
    OpenMesh::HalfedgeHandle current_he;

    OpenMesh::HPropHandleT<int> perHalfedgeParityProperty;

    for(auto h : mesh.halfedges()) {
        mesh.property(perHalfedgeParityProperty, h) = -1;
    }

    for(auto f : mesh.faces()){
        remaining_faces.push_back(f);
    }

    while(!remaining_faces.empty()){
        trial_max.clear();
        //grab 20 random faces out of pool
        for(int i = 0; i < nTrials; i++) {
            trial.clear();
            std::uniform_int_distribution<> randomNumber{0, static_cast<int>((remaining_faces.size()-1))};
            starting_face = remaining_faces[randomNumber(eng)];

            HEMesh::ConstFaceHalfedgeIter cfh_it = mesh.cfh_iter(starting_face);
            OpenMesh::HalfedgeHandle starting_he = *cfh_it;
            trial.push_back(starting_he);
            mesh.property(perHalfedgeParityProperty, starting_he) = 0;

            current_he = starting_he;
            //iterate forwards
            bool valid = true;
            while(valid){
            switch(mesh.property(perHalfedgeParityProperty, current_he)) {
                case 0:
                    //current_he = prev(inv(current_he));
                    current_he = mesh.opposite_halfedge_handle(current_he);
                    current_he = mesh.prev_halfedge_handle(current_he);
                    //add current to list
                    mesh.property(perHalfedgeParityProperty,current_he) = 1;
                    trial.push_back(current_he);
                    break;
                case 1:
                    //current_he = next(inv(current_he));
                    current_he = mesh.opposite_halfedge_handle(current_he);
                    current_he = mesh.next_halfedge_handle(current_he);
                    //add current to list
                    mesh.property(perHalfedgeParityProperty,current_he) = 0;
                    trial.push_back(current_he);
                    break;
            }
            }

            current_he = starting_he;
            //iterate backwards
            switch(mesh.property(perHalfedgeParityProperty, current_he)) {
                case 0:
                    //current_he = prev(inv(current_he));
                    current_he = mesh.prev_halfedge_handle(current_he);
                    current_he = mesh.opposite_halfedge_handle(current_he);
                    //add current to list
                    mesh.property(perHalfedgeParityProperty,current_he) = 1;
                    trial.push_back(current_he);
                    break;
                case 1:
                    //current_he = next(inv(current_he));
                    current_he = mesh.next_halfedge_handle(current_he);
                    current_he = mesh.opposite_halfedge_handle(current_he);
                    //add current to list
                    mesh.property(perHalfedgeParityProperty,current_he) = 0;
                    trial.push_back(current_he);
                    break;
            }
        if(trial.size() > trial_max.size()) {
            trial_max = trial;
        }



        }

    }

    //stripification pseudocode
    //k from the lecture = nTrials

   //while(faces with id -1 exist){
    //for(int i = 0; i < nTrials; i++){
        //randomly select a halfedge hi with attribute id -1
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