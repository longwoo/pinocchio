//
// Copyright (c) 2018-2019 CNRS INRIA
//

#ifndef __pinocchio_autodiff_ccpad_hpp__
#define __pinocchio_autodiff_ccpad_hpp__

#include "pinocchio/math/fwd.hpp"

// Do not include this file directly.
// Copy and use directly the intructions from <cppad/example/cppad_eigen.hpp>
// to avoid redifinition of EIGEN_MATRIXBASE_PLUGIN for Eigen 3.3.0 and later
//#include <cppad/example/cppad_eigen.hpp>

#ifdef PINOCCHIO_CPPAD_REQUIRES_MATRIX_BASE_PLUGIN
  #define EIGEN_MATRIXBASE_PLUGIN <cppad/example/eigen_plugin.hpp>
#endif

#include <cppad/cppad.hpp>
#include <Eigen/Dense>

namespace boost
{
  namespace math
  {
    namespace constants
    {
      namespace detail
      {
        template<typename Scalar>
        struct constant_pi< CppAD::AD<Scalar> > : constant_pi<Scalar> {};
      }
    }
  }
}

namespace Eigen
{
  namespace internal
  {
    // Specialization of Eigen::internal::cast_impl for CppAD input types
    template<typename Scalar>
    struct cast_impl<CppAD::AD<Scalar>,Scalar>
    {
#if EIGEN_VERSION_AT_LEAST(3,2,90)
      EIGEN_DEVICE_FUNC
#endif
      static inline Scalar run(const CppAD::AD<Scalar> & x)
      {
        return CppAD::Value(x);
      }
    };
  }
} //namespace Eigen

/// Source from #include <cppad/example/cppad_eigen.hpp>
namespace Eigen
{
  template <class Base> struct NumTraits< CppAD::AD<Base> >
  {  // type that corresponds to the real part of an AD<Base> value
    typedef CppAD::AD<Base>   Real;
    // type for AD<Base> operations that result in non-integer values
    typedef CppAD::AD<Base>   NonInteger;
    //  type to use for numeric literals such as "2" or "0.5".
    typedef CppAD::AD<Base>   Literal;
    // type for nested value inside an AD<Base> expression tree
    typedef CppAD::AD<Base>   Nested;
    
    enum {
      // does not support complex Base types
      IsComplex             = 0 ,
      // does not support integer Base types
      IsInteger             = 0 ,
      // only support signed Base types
      IsSigned              = 1 ,
      // must initialize an AD<Base> object
      RequireInitialization = 1 ,
      // computational cost of the corresponding operations
      ReadCost              = 1 ,
      AddCost               = 2 ,
      MulCost               = 2
    };
    
    // machine epsilon with type of real part of x
    // (use assumption that Base is not complex)
    static CppAD::AD<Base> epsilon(void)
    {  return CppAD::numeric_limits< CppAD::AD<Base> >::epsilon(); }
    
    // relaxed version of machine epsilon for comparison of different
    // operations that should result in the same value
    static CppAD::AD<Base> dummy_precision(void)
    {  return 100. *
      CppAD::numeric_limits< CppAD::AD<Base> >::epsilon();
    }
    
    // minimum normalized positive value
    static CppAD::AD<Base> lowest(void)
    {  return CppAD::numeric_limits< CppAD::AD<Base> >::min(); }
    
    // maximum finite value
    static CppAD::AD<Base> highest(void)
    {  return CppAD::numeric_limits< CppAD::AD<Base> >::max(); }
    
    // number of decimal digits that can be represented without change.
    static int digits10(void)
    {  return CppAD::numeric_limits< CppAD::AD<Base> >::digits10; }
  };
} // namespace Eigen

/// Source from #include <cppad/example/cppad_eigen.hpp>
namespace CppAD
{
  // functions that return references
  template <class Base> const AD<Base>& conj(const AD<Base>& x)
  {  return x; }
  template <class Base> const AD<Base>& real(const AD<Base>& x)
  {  return x; }
  
  // functions that return values (note abs is defined by cppad.hpp)
  template <class Base> AD<Base> imag(const AD<Base>& /*x*/)
  {  return CppAD::AD<Base>(0.); }
  template <class Base> AD<Base> abs2(const AD<Base>& x)
  {  return x * x; }
} // namespace CppAD

namespace pinocchio
{
  template<typename Scalar>
  struct TaylorSeriesExpansion< CppAD::AD<Scalar> > : TaylorSeriesExpansion<Scalar>
  {
    typedef TaylorSeriesExpansion<Scalar> Base;
    using Base::precision;
  };
} // namespace pinocchio

#endif // #ifndef __pinocchio_autodiff_ccpad_hpp__
