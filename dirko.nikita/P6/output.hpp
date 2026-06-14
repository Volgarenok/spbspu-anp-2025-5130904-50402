#ifndef OUTPUT_HPP
#define OUTPUT_HPP
#include <shapesUtil.hpp>
#include "shape_vec.hpp"

namespace dirko
{
  void scaleFromPoint(Shape_vec &shps, point_t point, double coef);
  std::ostream &output(std::ostream &os, const Shape_vec &shps);
}

#endif
