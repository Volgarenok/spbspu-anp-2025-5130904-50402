#include "output.hpp"
#include <iostream>

void dirko::scaleFromPoint(Shape_vec &shps, point_t point, double coef)
{
  if (coef <= 0) {
    throw std::invalid_argument("Negative coef");
  }
  for (size_t i = 0; i < shps.size(); ++i) {
    rectangle_t fr = shps.get(i).getFrameRect();
    double dx = (fr.pos.x - point.x) * (coef - 1);
    double dy = (fr.pos.y - point.y) * (coef - 1);
    shps.get(i).move(dx, dy);
    shps.get(i).doScale(coef);
  }
}

std::ostream &dirko::output(std::ostream &os, const Shape_vec &shps)
{
  for (size_t i = 0; i < shps.size(); ++i) {
    double area = shps.getConst(i).getArea();
    os << "shape " << i + 1 << " area: " << area << "\n";
    rectangle_t frame = shps.getConst(i).getFrameRect();
    outRect(os, frame) << '\n';
  }
  os << "Total area: " << shps.getArea() << "\n";
  outRect(os, shps.getFrameRect()) << '\n';
  return os;
}
