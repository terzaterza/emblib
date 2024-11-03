// Used to provide typechecking in this file
#ifndef EIGEN_QUATERNION_PLUGIN
#include "Eigen/Geometry"

using namespace Eigen;
class dummy_class : public Eigen::Quaternion<float> {
#endif

Vector<Scalar, 3> rotate_vector(const Vector<Scalar, 3>& vector) noexcept
{
    return this->_transformVector(vector);
}

#ifndef EIGEN_QUATERNION_PLUGIN
};
#endif