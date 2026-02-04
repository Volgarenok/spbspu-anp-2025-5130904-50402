#include "storageActions.hpp"
#include <iostream>
#include <stdexcept>

void karpovich::output(StorageShape& shapes, size_t size)
{
  double total_area = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shapes.get(i).getArea();
    total_area += area;
    std::cout << area << "\n";
    outputRectangle(shapes.get(i).getFrameRect());
  }
  std::cout << total_area << "\n";
  outputRectangle(shapes.getFrameRect());
}
void karpovich::scalefrompt(StorageShape& shapes, size_t size, double k, point_t pt)
{
  if (k <= 0.0) {
    throw std::invalid_argument("err: k must be positive\n");
  }
  for (size_t i = 0; i < size; ++i) {
    shapes.get(i).move(-pt.x, -pt.y);
    shapes.get(i).uncheckedScale(k);
    shapes.get(i).move(pt.x, pt.y);
  }
}
