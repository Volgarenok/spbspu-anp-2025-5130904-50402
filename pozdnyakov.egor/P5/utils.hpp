#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <iosfwd>
#include "Shape.hpp"

namespace pozdnyakov
{
  void scaleShapeAtPoint(Shape *shape, const point_t &target, double k);
  void scaleShapes(Shape **shapes, size_t count, const point_t &target, double k);
  void clearShapes(Shape **shapes, size_t count);
  void printFrameRect(std::ostream &out, const rectangle_t &frame);
  void printShapesInfo(const Shape *const *shapes, size_t count);
}

#endif
