#include "podshape.hpp"
#include <iostream>

void karpovich::scalefrompt(Shape* shapes[], size_t size, double k, point_t pt)
{
  if (k <= 0.0) {
    throw std::invalid_argument("err: k must be positive\n");
  }
  for (size_t i = 0; i < size; ++i) {
    shapes[i]->move(-pt.x, -pt.y);
    shapes[i]->uncheckedScale(k);
    shapes[i]->move(pt.x, pt.y);
  }
}
karpovich::rectangle_t karpovich::computeTotalFrame(const Shape* const shapes[], size_t size)
{
  if (size == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  karpovich::rectangle_t first = shapes[0]->getFrameRect();
  double left = first.pos.x - first.width / 2.0;
  double right = first.pos.x + first.width / 2.0;
  double bottom = first.pos.y - first.height / 2.0;
  double top = first.pos.y + first.height / 2.0;
  for (size_t i = 1; i < size; ++i) {
    karpovich::rectangle_t frame = shapes[i]->getFrameRect();
    double l = frame.pos.x - frame.width / 2.0;
    double r = frame.pos.x + frame.width / 2.0;
    double b = frame.pos.y - frame.height / 2.0;
    double t = frame.pos.y + frame.height / 2.0;
    left = std::min(l, left);
    right = std::max(r, right);
    bottom = std::min(b, bottom);
    top = std::max(t, top);
  }
  return {{right - left}, {top - bottom}, {(left + right) / 2.0, (bottom + top) / 2.0}};
}
void karpovich::output(const Shape* const shapes[], size_t size)
{
  double total_area = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shapes[i]->getArea();
    total_area += area;
    std::cout << area << "\n";
    outputRectangle(shapes[i]->getFrameRect());
  }
  rectangle_t total_frame = computeTotalFrame(shapes, size);
  std::cout << total_area << "\n";
  outputRectangle(total_frame);
}

void karpovich::outputRectangle(const rectangle_t& rect)
{
  std::cout << rect.pos.x << "\n";
  std::cout << rect.pos.y << "\n";
  std::cout << rect.width << "\n";
  std::cout << rect.height << "\n";
}
void karpovich::Shape::scale(double k)
{
  if (k <= 0.0) {
    throw std::invalid_argument("err: k must be positive\n");
  }
  doScale(k);
}
void karpovich::Shape::uncheckedScale(double k) noexcept
{
  doScale(k);
}
