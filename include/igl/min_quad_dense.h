#ifndef IGL_MIN_QUAD_DENSE_H
#define IGL_MIN_QUAD_DENSE_H
#include "igl_inline.h"

#include <Eigen/Dense>

//// debug
//#include <matlabinterface.h>
//Engine *g_pEngine;


namespace igl
{
  // MIN_QUAD_WITH_FIXED Minimize quadratic energy Z'*A*Z + Z'*B + C
  // subject to linear constraints Aeq*Z = Beq
  //
  // Templates:
  //   T  should be a eigen matrix primitive type like float or double
  // Inputs:
  //   A  n by n matrix of quadratic coefficients
  //   B  n by 1 column of linear coefficients
  //   Aeq  m by n list of linear equality constraint coefficients
  //   Beq  m by 1 list of linear equality constraint constant values
  // Outputs:
  //   S  n by (n + m) "solve" matrix, such that S*[B', Beq'] is a solution
  // Returns true on success, false on error
  template <typename T>
  IGL_INLINE void min_quad_dense_precompute(
    const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& A,
    const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& Aeq,    
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& S);
}

#ifdef IGL_HEADER_ONLY
#  include "min_quad_dense.cpp"
#endif

#endif