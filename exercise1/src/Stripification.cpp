// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "Stripification.h"

#include <random>
#include "sample_set.h"


bool check (HEMesh& mesh, OpenMesh::HalfedgeHandle h, std::vector<OpenMesh::HalfedgeHandle> trial){
    if(!mesh.is_valid_handle(h)){
        return false;
    }
    if(find(trial.begin(), trial.end(), h) != trial.end()){
        return false;
    }
    return true;
}

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
    mesh.add_property(perHalfedgeParityProperty);

    //initialize halfedge parities with -1
    for(auto h : mesh.halfedges()) {
        mesh.property(perHalfedgeParityProperty, h) = -1;
    }

    //put all faces of the mesh into a pool of faces
    for(auto f : mesh.faces()){
        remaining_faces.push_back(f);
    }

    while(!remaining_faces.empty()){
        trial_max.clear();
        //grab 20/nTrials random faces out of pool
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
                        //first check if this halfedge a) exists and b) is not in our trial list yet
                        //if either of those conditions are met, the strip ends
                        if(check(mesh, mesh.opposite_halfedge_handle(current_he), trial)) {
                                current_he = mesh.opposite_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }
                        if(check(mesh, mesh.prev_halfedge_handle(current_he), trial)) {
                                current_he = mesh.prev_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }

                        //add current halfedge to list
                        mesh.property(perHalfedgeParityProperty,current_he) = 1;
                        trial.push_back(current_he);
                        break;

                    case 1:
                        //current_he = next(inv(current_he));
                        if(check(mesh, mesh.opposite_halfedge_handle(current_he),trial)) {
                            current_he = mesh.opposite_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }
                        if(check(mesh, mesh.next_halfedge_handle(current_he), trial)) {
                            current_he = mesh.next_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }

                        //add current to list
                        mesh.property(perHalfedgeParityProperty,current_he) = 0;
                        trial.push_back(current_he);
                        break;
                }
            }

            current_he = starting_he;
            //iterate backwards
            valid = true;
            while(valid) {
                switch (mesh.property(perHalfedgeParityProperty, current_he)) {
                    case 0:
                        //current_he = prev(inv(current_he));
                        if(check(mesh, mesh.prev_halfedge_handle(current_he), trial)) {
                            current_he = mesh.prev_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }
                        if(check(mesh, mesh.opposite_halfedge_handle(current_he), trial)) {
                            current_he = mesh.opposite_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }
                        //add current to list
                        mesh.property(perHalfedgeParityProperty, current_he) = 1;
                        trial.push_back(current_he);
                        break;

                    case 1:
                        //current_he = next(inv(current_he));
                        if(check(mesh, mesh.next_halfedge_handle(current_he), trial)) {
                            current_he = mesh.next_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }
                        if(check(mesh, mesh.opposite_halfedge_handle(current_he), trial)) {
                            current_he = mesh.opposite_halfedge_handle(current_he);
                        } else {
                            valid = false;
                            break;
                        }
                        //add current to list
                        mesh.property(perHalfedgeParityProperty, current_he) = 0;
                        trial.push_back(current_he);
                        break;
                }

            }
        if(trial.size() > trial_max.size()) {
            trial_max = trial;
        }

        } //for over the trials

        //remove the faces of the longest strip from the pool of remaining faces
        OpenMesh::FaceHandle f;

        for (OpenMesh::HalfedgeHandle he : trial_max){
            f = mesh.face_handle(he);
            //find the index of f in the vector to then erase it
            int index = 0;


            if(remaining_faces.size() == 0){ break; }

            auto it = find(remaining_faces.begin(), remaining_faces.end(), f);
            index = std::distance(remaining_faces.begin(), it);

            mesh.property(perFaceStripIdProperty, f) = nStrips;     //set the Strip ID before removing
            if(index == remaining_faces.size()){
                remaining_faces.pop_back();
            } else {
                remaining_faces.erase(it);
            }

            //mesh.property(perFaceStripIdProperty, f) = nStrips;
            //remove(remaining_faces.begin(), remaining_faces.end(), f);

        }

        //erase all the parities again
        for(auto face : remaining_faces) {
            HEMesh::ConstFaceHalfedgeIter cfh = mesh.cfh_iter(face);
            OpenMesh::HalfedgeHandle help = *cfh;
            mesh.property(perHalfedgeParityProperty, help) = -1;
            help = cfh++;
            mesh.property(perHalfedgeParityProperty, help) = -1;
            help = cfh++;
            mesh.property(perHalfedgeParityProperty, help) = -1;
        }

        //after every found strip, increase the number of strips
        nStrips++;

    } //while(!remaining_faces.empty())


	return nStrips;
}