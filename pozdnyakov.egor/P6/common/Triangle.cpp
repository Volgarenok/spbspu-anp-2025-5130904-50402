#include "Triangle.hpp"
#include <algorithm>
#include <cmath>

pozdnyakov::Triangle::Triangle(const point_t &a, const point_t &b, const point_t &c) noexcept:
  p1_(a),
  p2_(b),
  p3_(c)
{}

double pozdnyakov::Triangle::getArea() const noexcept
{
  double term1 = (p2_.x - p1_.x) * (p3_.y - p1_.y);
  double term2 = (p3_.x - p1_.x) * (p2_.y - p1_.y);
  return 0.5 * std::abs(term1 - term2);
}

pozdnyakov::rectangle_t pozdnyakov::Triangle::getFrameRect() const noexcept
{
  double min_x = std::min({p1_.x, p2_.x, p3_.x});
  double max_x = std::max({p1_.x, p2_.x, p3_.x});
  double min_y = std::min({p1_.y, p2_.y, p3_.y});
  double max_y = std::max({p1_.y, p2_.y, p3_.y});

  return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
}

void pozdnyakov::Triangle::move(const point_t &pos) noexcept
{
  point_t center = getFrameRect().pos;
  double dx = pos.x - center.x;
  double dy = pos.y - center.y;
  move(dx, dy);
}

void pozdnyakov::Triangle::move(double dx, double dy) noexcept
{
  p1_.x += dx;
  p1_.y += dy;
  p2_.x += dx;
  p2_.y += dy;
  p3_.x += dx;
  p3_.y += dy;
}

pozdnyakov::Shape *pozdnyakov::Triangle::clone() const
{
  return new Triangle(*this);
}

void pozdnyakov::Triangle::doScale(double k) noexcept
{
  point_t center = getFrameRect().pos;
  p1_.x = center.x + (p1_.x - center.x) * k;
  p1_.y = center.y + (p1_.y - center.y) * k;
  p2_.x = center.x + (p2_.x - center.x) * k;
  p2_.y = center.y + (p2_.y - center.y) * k;
  p3_.x = center.x + (p3_.x - center.x) * k;
  p3_.y = center.y + (p3_.y - center.y) * k;
}
