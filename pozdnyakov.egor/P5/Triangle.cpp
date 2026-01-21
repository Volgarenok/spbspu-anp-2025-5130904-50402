#include "Triangle.hpp"
#include <algorithm>
#include <cmath>

namespace pozdnyakov {
  Triangle::Triangle(const point_t& a, const point_t& b, const point_t& c) :
    p1_(a),
    p2_(b),
    p3_(c)
  {
  }

  double Triangle::getArea() const noexcept {
    double term1 = (p2_.x - p1_.x) * (p3_.y - p1_.y);
    double term2 = (p3_.x - p1_.x) * (p2_.y - p1_.y);
    return 0.5 * std::abs(term1 - term2);
  }

  rectangle_t Triangle::getFrameRect() const noexcept {
    double min_x = std::min(p1_.x, std::min(p2_.x, p3_.x));
    double max_x = std::max(p1_.x, std::max(p2_.x, p3_.x));
    double min_y = std::min(p1_.y, std::min(p2_.y, p3_.y));
    double max_y = std::max(p1_.y, std::max(p2_.y, p3_.y));

    double width = max_x - min_x;
    double height = max_y - min_y;

    point_t rect_pos = { min_x + width / 2.0, min_y + height / 2.0 };

    return { width, height, rect_pos };
  }

  void Triangle::move(const point_t& pos) noexcept {
    point_t current_center = getCenter();
    double dx = pos.x - current_center.x;
    double dy = pos.y - current_center.y;
    move(dx, dy);
  }

  void Triangle::move(double dx, double dy) noexcept {
    p1_.x += dx;
    p1_.y += dy;
    p2_.x += dx;
    p2_.y += dy;
    p3_.x += dx;
    p3_.y += dy;
  }

  void Triangle::doScale(double k) noexcept {
    point_t center = getCenter();
    p1_.x = center.x + (p1_.x - center.x) * k;
    p1_.y = center.y + (p1_.y - center.y) * k;
    p2_.x = center.x + (p2_.x - center.x) * k;
    p2_.y = center.y + (p2_.y - center.y) * k;
    p3_.x = center.x + (p3_.x - center.x) * k;
    p3_.y = center.y + (p3_.y - center.y) * k;
  }

  point_t Triangle::getCenter() const noexcept {
    return { (p1_.x + p2_.x + p3_.x) / 3.0, (p1_.y + p2_.y + p3_.y) / 3.0 };
  }
}
