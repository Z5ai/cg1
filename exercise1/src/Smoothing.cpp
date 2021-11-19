// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "Smoothing.h"

#include <random>

void SmoothUniformLaplacian(HEMesh& m, float lambda, unsigned int iterations)
{
	/*Task 1.2.4*/
    for(int index_iterations=0;index_iterations<iterations;index_iterations++)
    {
        HEMesh m_read = m; // read on copy m_read, write on original m

        for (auto vertex_selected : m_read.vertices()) {
            int vertexes_in_ring = 0;

            OpenMesh::Vec3f vector_sum{0, 0, 0};
            float count_vertexes_in_ring = 0;

            // vector_sum is accumulated by vectors from vertex_selected to the vertices of its one-ring
            for (auto vertex_ring: m_read.vv_range(vertex_selected)) {
                count_vertexes_in_ring++;
                vector_sum += (m_read.point(vertex_ring) - m_read.point(vertex_selected));
            }

            OpenMesh::Vec3f vertex_new = m_read.point(vertex_selected) + vector_sum.vectorize(lambda / count_vertexes_in_ring);
            m.point(vertex_selected) = vertex_new;
        }
    }
}

void AddNoise(HEMesh& m, OpenMesh::MPropHandleT<Viewer::BBoxType> bbox_prop)
{
	std::mt19937 rnd;
	std::normal_distribution<float> dist;

	for (auto v : m.vertices())
	{
		OpenMesh::Vec3f n;
		m.calc_vertex_normal_correct(v, n);
		const auto diag = m.property(bbox_prop).diagonal();
		const float base_diag = std::min(diag.x(), std::min(diag.y(), diag.z())) / 20.f;
		float base_nb=0, nb_num=0;
		for (auto vnb : m.vv_range(v))
		{
			base_nb += (m.point(v) - m.point(vnb)).norm();
			nb_num++;
		}
		base_nb /= 4.f * nb_num;

		m.point(v) += std::min(base_diag, base_nb) * dist(rnd) * n.normalized();
	}
}
