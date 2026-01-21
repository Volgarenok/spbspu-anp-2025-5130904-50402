#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include "Shape.hpp"

namespace pozdnyakov {
  void scaleShapeAtPoint(Shape* shape, const point_t& target, double k);
  void scaleShapes(Shape** shapes, size_t count, const point_t& target, double k);
  void clearShapes(Shape** shapes, size_t count);
  void printShapesInfo(const Shape* const* shapes, size_t count);
}

#endif
