// Used to provide typechecking in this file
#ifndef EIGEN_MATRIXBASE_PLUGIN
#include "Eigen/src/Core/MatrixBase.h"

using namespace Eigen;
class dummy_class : public Eigen::MatrixBase<dummy_class> {
#endif

static constexpr int ROWS = RowsAtCompileTime;
static constexpr int COLS = ColsAtCompileTime;

/**
 * Equivalent to multiplying this from the left
 * by the inverse of the divisor
 */
Matrix<Scalar, ROWS, COLS> divide_left(const Matrix<Scalar, ROWS, ROWS>& divisor)
{
    return divisor.colPivHouseholderQr().solve(*this);
}

/**
 * Equivalent to multiplying this from the right
 * by the inverse of the divisor
 */
Matrix<Scalar, ROWS, COLS> divide_right(const Matrix<Scalar, COLS, COLS>& divisor)
{
    return derived().transpose().divide_left(divisor.transpose()).transpose();
}


template <size_t ROWS_OTHER>
Matrix<Scalar, ROWS + ROWS_OTHER, COLS> concat_vertical(const Matrix<Scalar, ROWS_OTHER, COLS>& other)
{
    Matrix<Scalar, ROWS + ROWS_OTHER, COLS> result;
    result << *this, other;
    return result;
}


template <size_t COLS_OTHER>
Matrix<Scalar, ROWS, COLS + COLS_OTHER> concat_horizontal(const Matrix<Scalar, ROWS, COLS_OTHER>& other)
{
    Matrix<Scalar, ROWS, COLS + COLS_OTHER> result;
    result << *this, other;
    return result;
}


#ifndef EIGEN_MATRIXBASE_PLUGIN
};
#endif