#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

void pozdnyakov::scaleShapeAtPoint(Shape *shape, const point_t &target, double k)
{
  point_t center = shape->getFrameRect().pos;
  double dx = (center.x - target.x) * (k - 1.0);
  double dy = (center.y - target.y) * (k - 1.0);

  shape->move(dx, dy);
  shape->unsafeScale(k);
}

void pozdnyakov::scaleShapes(Shape **shapes, size_t count, const point_t &target, double k)
{
  if (k < 0.0) {
    throw std::invalid_argument("Scaling coefficient must be non-negative");
  }

  for (size_t i = 0; i < count; ++i) {
    if (shapes[i]) {
      scaleShapeAtPoint(shapes[i], target, k);
    }
  }
}

void pozdnyakov::clearShapes(Shape **shapes, size_t count)
{
  for (size_t i = 0; i < count; ++i) {
    delete shapes[i];
  }
  delete[] shapes;
}

void pozdnyakov::printFrameRect(std::ostream &out, const rectangle_t &frame)
{
  out << "FrameCenter=(" << frame.pos.x << ", " << frame.pos.y << ")"
      << ", w=" << frame.width << ", h=" << frame.height;
}

void pozdnyakov::printShapesInfo(const Shape *const *shapes, size_t count)
{
  std::cout << std::fixed << std::setprecision(1);

  if (count == 0) {
    return;
  }

  double totalArea = 0.0;

  rectangle_t frame0 = shapes[0]->getFrameRect();
  double half_w0 = frame0.width / 2.0;
  double half_h0 = frame0.height / 2.0;

  double global_min_x = frame0.pos.x - half_w0;
  double global_max_x = frame0.pos.x + half_w0;
  double global_min_y = frame0.pos.y - half_h0;
  double global_max_y = frame0.pos.y + half_h0;

  for (size_t i = 0; i < count; ++i) {
    if (!shapes[i]) {
      continue;
    }

    double area = shapes[i]->getArea();
    totalArea += area;
    rectangle_t frame = shapes[i]->getFrameRect();

    std::cout << "Shape " << i << ": Area=" << area << ", ";
    printFrameRect(std::cout, frame);
    std::cout << "\n";

    double half_w = frame.width / 2.0;
    double half_h = frame.height / 2.0;
    double left = frame.pos.x - half_w;
    double right = frame.pos.x + half_w;
    double bottom = frame.pos.y - half_h;
    double top = frame.pos.y + half_h;

    global_min_x = std::min(global_min_x, left);
    global_max_x = std::max(global_max_x, right);
    global_min_y = std::min(global_min_y, bottom);
    global_max_y = std::max(global_max_y, top);
  }

  std::cout << "Total Area: " << totalArea << "\n";

  double global_w = global_max_x - global_min_x;
  double global_h = global_max_y - global_min_y;
  double global_cx = global_min_x + global_w / 2.0;
  double global_cy = global_min_y + global_h / 2.0;

  rectangle_t globalFrame = {global_w, global_h, {global_cx, global_cy}};

  std::cout << "Global FrameRect: ";
  printFrameRect(std::cout, globalFrame);
  std::cout << "\n";
}
