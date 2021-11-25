// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "ShellExtraction.h"

#include <queue>

bool common_edge(OpenMesh::SmartFaceHandle f1,OpenMesh::SmartFaceHandle f2)
{
    //PRE-CONDITIONS
    if(f1==f2) return false;
    //PRE-CONDITIONS

    //JOB
    bool connected = false;
    for(auto vertice1 : f1.vertices())
    {
        for(auto vertice2 : f1.vertices())
        {
            if(vertice1==vertice2)
            {
                connected = true;
            }
        }
    }
    //JOB

    //POST-CONDITIONS
    if(connected == false) return false;
    if(connected == true)  return true;
    return false;
    //POST-CONDITIONS
}

unsigned int ExtractShells(HEMesh& m, OpenMesh::FPropHandleT<int> perFaceShellIndex)
{
    //PRE-CONDITION
	//reset the shell indices to -1 for every face
	for (auto f : m.faces())
		m.property(perFaceShellIndex, f) = -1;
    //PRE-CONDITON

    //JOB
	/*Task 1.2.3*/
    int groups_number_temporary = 0;
    int groups_id = 0;

    for (auto f1 : m.faces())
    {
        for (auto f2 : m.faces())
        {
            //TEAM-WARNING [BLUME]: unsicher ob das idx ist - ich brauche halt den Parameter nach f1 oder f2,
            //                      der ne Halbordnung generiert - also die größer/kleiner Operation gestattet
            if(f1.idx()>f2.idx())
            {
                if(common_edge(f1,f2))
                {
                    if(  (m.property(perFaceShellIndex, f1) == -1)
                       &&(m.property(perFaceShellIndex, f2) == -1))
                    {
                        groups_number_temporary++;
                        groups_id++;
                        m.property(perFaceShellIndex, f1) = groups_id;
                        m.property(perFaceShellIndex, f2) = groups_id;
                    }
                    else
                    {
                        if (m.property(perFaceShellIndex, f1) == -1)
                              m.property(perFaceShellIndex, f1) = m.property(perFaceShellIndex, f2);
                        else
                        if (m.property(perFaceShellIndex, f2) == -1)
                              m.property(perFaceShellIndex, f2) = m.property(perFaceShellIndex, f1);
                        else
                        {

                          int littler_index,replace_index;
                          if(m.property(perFaceShellIndex, f1)<m.property(perFaceShellIndex, f2))
                          {
                              littler_index = m.property(perFaceShellIndex, f1);
                              replace_index = m.property(perFaceShellIndex, f2);
                          }
                          else
                          {
                              littler_index = m.property(perFaceShellIndex, f2);
                              replace_index = m.property(perFaceShellIndex, f1);
                          }
                          bool replacement_exists = false;
                          for (auto f3 : m.faces())
                          {
                              if(m.property(perFaceShellIndex, f3) == replace_index)
                              {
                                  m.property(perFaceShellIndex, f3) = littler_index;
                                  replacement_exists = true;
                              }
                          }
                          if(replacement_exists)
                              groups_number_temporary--;
                        }
                    }
                }
                else{

                    if(m.property(perFaceShellIndex, f1) == -1)
                    {
                        groups_number_temporary++;
                        groups_id++;
                        m.property(perFaceShellIndex, f1) = groups_id;
                    }
                    if(m.property(perFaceShellIndex, f2) == -1)
                    {
                        groups_number_temporary++;
                        groups_id++;
                        m.property(perFaceShellIndex, f2) = groups_id;
                    }
                }
            }
        }
    }

    //erase integer gaps, e.g. groups 1 3 4 8 should get 1 2 3 4
    int groups_number = 0;
    for(int groups_number_temporary_index = 0; groups_number_temporary_index<groups_number_temporary;groups_number_temporary++)
    {
        bool groups_number_temporary_index_no_gap = false;
        for (auto f : m.faces())
        {
            if((!groups_number_temporary_index_no_gap)&&(m.property(perFaceShellIndex, f) == groups_number_temporary_index))
            {
                groups_number++;
                groups_number_temporary_index_no_gap = false;
            }
            if(groups_number_temporary_index_no_gap)
                m.property(perFaceShellIndex, f) = groups_number;
        }
    }
    //JOB

    //POST-CONDTION
    if(groups_number_temporary>0) return groups_number_temporary;
	return -1;
    //POST-CONDTION
}

