#include "shapesUtil.hpp"
#include <stdexcept>
#include <iostream>

void dirko::Shape::doScale(double coef) noexcept
{
  scale_(coef);
}
void dirko::Shape::doScaleSafe(double coef)
{
  if (coef <= 0) {
    throw std::invalid_argument("Non positive coef");
  }
  scale_(coef);
}
void dirko::scaleFromPoint(Shape **shps, size_t size, point_t point, double coef)
{
  if (coef <= 0) {
    throw std::invalid_argument("Negative coef");
  }
  for (size_t i = 0; i < size; ++i) {
    rectangle_t fr = shps[i]->getFrameRect();
    double dx = (fr.pos.x - point.x) * (coef - 1);
    double dy = (fr.pos.y - point.y) * (coef - 1);
    shps[i]->move(dx, dy);
    shps[i]->doScale(coef);
  }
}
dirko::rectangle_t dirko::getTotalFrame(const Shape *const *shps, size_t size)
{
  if (size == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  rectangle_t frame = shps[0]->getFrameRect();
  double left = frame.pos.x - frame.w / 2.0;
  double right = frame.pos.x + frame.w / 2.0;
  double bottom = frame.pos.y - frame.h / 2.0;
  double top = frame.pos.y + frame.h / 2.0;
  for (size_t i = 1; i < size; ++i) {
    frame = shps[i]->getFrameRect();
    double l = frame.pos.x - frame.w / 2.0;
    double r = frame.pos.x + frame.w / 2.0;
    double b = frame.pos.y - frame.h / 2.0;
    double t = frame.pos.y + frame.h / 2.0;
    left = std::min(l, left);
    right = std::max(r, right);
    bottom = std::min(b, bottom);
    top = std::max(t, top);
  }
  rectangle_t total;
  total.w = right - left;
  total.h = top - bottom;
  total.pos.x = (left + right) / 2.0;
  total.pos.y = (bottom + top) / 2.0;
  return total;
}
std::ostream &dirko::outRect(std::ostream &os, rectangle_t frame)
{
  os << "frame x: " << frame.pos.x << "\n";
  os << "frame y: " << frame.pos.y << "\n";
  os << "frame width: " << frame.w << "\n";
  os << "frame height: " << frame.h;
  return os;
}
std::ostream &dirko::output(std::ostream &os, const Shape *const *shps, size_t size)
{
  double totalArea = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shps[i]->getArea();
    totalArea += area;
    os << "shape " << i + 1 << " area: " << area << "\n";
    rectangle_t frame = shps[i]->getFrameRect();
    outRect(os, frame) << '\n';
  }
  rectangle_t totalFrame = getTotalFrame(shps, size);
  os << "Total area: " << totalArea << "\n";
  outRect(os, totalFrame) << '\n';
  return os;
}
