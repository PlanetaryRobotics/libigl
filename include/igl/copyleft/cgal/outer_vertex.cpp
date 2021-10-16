// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2015 Qingnan Zhou <qnzhou@gmail.com>
// Copyright (C) 2021 Alec Jacobson <jacobson@cs.toronto.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "outer_vertex.h"
#include "outer_edge.h"
#include <iostream>
#include <vector>

template <
     typename DerivedV,
     typename DerivedF,
     typename DerivedI,
     typename IndexType,
     typename DerivedA
     >
IGL_INLINE void igl::copyleft::cgal::outer_vertex(
        const Eigen::PlainObjectBase<DerivedV> & V,
        const Eigen::PlainObjectBase<DerivedF> & F,
        const Eigen::PlainObjectBase<DerivedI> & I,
        IndexType & v_index,
        Eigen::PlainObjectBase<DerivedA> & A)
{
    // Algorithm:
    //    Find an outer vertex (i.e. vertex reachable from infinity)
    //    Return the vertex with the largest X value.
    //    If there is a tie, pick the one with largest Y value.
    //    If there is still a tie, pick the one with the largest Z value.
    //    If there is still a tie, then there are duplicated vertices within the
    //    mesh, which violates the precondition.
    typedef typename DerivedF::Scalar Index;
    const Index INVALID = std::numeric_limits<Index>::max();
    const size_t num_selected_faces = I.rows();
    std::vector<size_t> candidate_faces;
    Index outer_vid = INVALID;
    typename DerivedV::Scalar outer_val = 0;
    for (size_t i=0; i<num_selected_faces; i++)
    {
        size_t f = I(i);
        for (size_t j=0; j<3; j++)
        {
            Index v = F(f, j);
            auto vx = V(v, 0);
            if (outer_vid == INVALID || vx > outer_val)
            {
                outer_val = vx;
                outer_vid = v;
                candidate_faces = {f};
            } else if (v == outer_vid)
            {
                candidate_faces.push_back(f);
            } else if (vx == outer_val)
            {
                // Break tie.
                auto vy = V(v,1);
                auto vz = V(v, 2);
                auto outer_y = V(outer_vid, 1);
                auto outer_z = V(outer_vid, 2);
                assert(!(vy == outer_y && vz == outer_z));
                bool replace = (vy > outer_y) ||
                    ((vy == outer_y) && (vz > outer_z));
                if (replace)
                {
                    outer_val = vx;
                    outer_vid = v;
                    candidate_faces = {f};
                }
            }
        }
    }

    assert(outer_vid != INVALID);
    assert(candidate_faces.size() > 0);
    v_index = outer_vid;
    A.resize(candidate_faces.size());
    std::copy(candidate_faces.begin(), candidate_faces.end(), A.data());
}

#ifdef IGL_STATIC_LIBRARY
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
// Explicit template instantiation
#include <cstdint>
template void igl::copyleft::cgal::outer_vertex<Eigen::Matrix<CGAL::Epeck::FT, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, int64_t, Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<CGAL::Epeck::FT, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> > const&, int64_t&, Eigen::PlainObjectBase<Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >&);
template void igl::copyleft::cgal::outer_vertex<Eigen::Matrix<CGAL::Epeck::FT, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int64_t, -1, 1, 0, -1, 1>, int64_t, Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<CGAL::Epeck::FT, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> > const&, int64_t&, Eigen::PlainObjectBase<Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >&);
template void igl::copyleft::cgal::outer_vertex<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, int64_t, Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, int64_t&, Eigen::PlainObjectBase<Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >&);
template void igl::copyleft::cgal::outer_vertex<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, int64_t, Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> > const&, int64_t&, Eigen::PlainObjectBase<Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >&);
template void igl::copyleft::cgal::outer_vertex<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<int64_t, -1, 1, 0, -1, 1>, int64_t, Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> > const&, int64_t&, Eigen::PlainObjectBase<Eigen::Matrix<int64_t, -1, 1, 0, -1, 1> >&);
#ifdef WIN32

template void __cdecl igl::copyleft::cgal::outer_vertex<class Eigen::Matrix<class CGAL::Epeck::FT,-1,-1,0,-1,-1>,class Eigen::Matrix<int,-1,-1,0,-1,-1>,class Eigen::Matrix<long,-1,1,0,-1,1>,__int64,class Eigen::Matrix<__int64,-1,1,0,-1,1> >(class Eigen::PlainObjectBase<class Eigen::Matrix<class CGAL::Epeck::FT,-1,-1,0,-1,-1> > const &,class Eigen::PlainObjectBase<class Eigen::Matrix<int,-1,-1,0,-1,-1> > const &,class Eigen::PlainObjectBase<class Eigen::Matrix<long,-1,1,0,-1,1> > const &,__int64 &,class Eigen::PlainObjectBase<class Eigen::Matrix<__int64,-1,1,0,-1,1> > &);
template void __cdecl igl::copyleft::cgal::outer_vertex<class Eigen::Matrix<double,-1,3,0,-1,3>,class Eigen::Matrix<int,-1,3,0,-1,3>,class Eigen::Matrix<int,-1,1,0,-1,1>,__int64,class Eigen::Matrix<__int64,-1,1,0,-1,1> >(class Eigen::PlainObjectBase<class Eigen::Matrix<double,-1,3,0,-1,3> > const &,class Eigen::PlainObjectBase<class Eigen::Matrix<int,-1,3,0,-1,3> > const &,class Eigen::PlainObjectBase<class Eigen::Matrix<int,-1,1,0,-1,1> > const &,__int64 &,class Eigen::PlainObjectBase<class Eigen::Matrix<__int64,-1,1,0,-1,1> > &);
template void __cdecl igl::copyleft::cgal::outer_vertex<class Eigen::Matrix<double,-1,3,0,-1,3>,class Eigen::Matrix<int,-1,3,0,-1,3>,class Eigen::Matrix<long,-1,1,0,-1,1>,__int64,class Eigen::Matrix<__int64,-1,1,0,-1,1> >(class Eigen::PlainObjectBase<class Eigen::Matrix<double,-1,3,0,-1,3> > const &,class Eigen::PlainObjectBase<class Eigen::Matrix<int,-1,3,0,-1,3> > const &,class Eigen::PlainObjectBase<class Eigen::Matrix<long,-1,1,0,-1,1> > const &,__int64 &,class Eigen::PlainObjectBase<class Eigen::Matrix<__int64,-1,1,0,-1,1> > &);

#endif
#endif
