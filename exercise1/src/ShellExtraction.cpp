// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "ShellExtraction.h"

#include <queue>

unsigned int ExtractShells(HEMesh& m, OpenMesh::FPropHandleT<int> perFaceShellIndex)
{
    //PRE-CONDITION
	//reset the shell indices to -1 for every face
	for (auto f : m.faces())
		m.property(perFaceShellIndex, f) = -1;
    //PRE-CONDITON

    //JOB
	/*Task 1.2.3*/
    int temporary_group = 0;

    int count_faces = temporary_group;
    for (auto f1 : m.faces())
    {
        for (auto f2 : m.faces())
        {
            if(f1.idx()>f2.idx())
            {
                if(common_edge(f1,f2))
                {
                    if(  (m.property(perFaceShellIndex, f1) == -1)
                       &&(m.property(perFaceShellIndex, f2) == -1))
                    {
                        temporary_group++;
                        m.property(perFaceShellIndex, f1) = temporary_group;
                        m.property(perFaceShellIndex, f2) = temporary_group;
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
                          bool replacement_exists = 0;
                          for (auto f3 : m.faces())
                          {
                              if(m.property(perFaceShellIndex, f3) == replace_index)
                              {
                                  m.property(perFaceShellIndex, f3) = littler_index;
                                  replacement_exists = 1;
                              }
                          }
                          if(replacement_exists == 1)
                              temporary_group--;
                        }
                    }
                }
                else{

                    if(m.property(perFaceShellIndex, f1) == -1)
                    {
                        temporary_group++;
                        m.property(perFaceShellIndex, f1) = temporary_group;
                    }
                    if(m.property(perFaceShellIndex, f2) == -1)
                    {
                        temporary_group++;
                        m.property(perFaceShellIndex, f2) = temporary_group;
                    }
                }
            }
        }
    }
    //JOB

    //POST-CONDTION
    if(temporary_group>0) return temporary_group;
	return -1;
    //POST-CONDTION
}