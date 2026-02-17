#include "xquare.hpp"
#include <cmath>

shirokov::Xquare::Xquare(const point_t &center, double side) noexcept:
  center_(center),
  top_({center.x, center.y + side / std::sqrt(2)}),
  bottom_({center.x, center.y - side / std::sqrt(2)})
{}

shirokov::Xquare::Xquare(const point_t &top, const point_t &bottom) noexcept:
  center_({(top.x + bottom.x) / 2, (top.y + bottom.y) / 2}),
  top_(top),
  bottom_(bottom)
{}

double shirokov::Xquare::getArea() const noexcept
{
  double side = (top_.y - bottom_.y) / std::sqrt(2);
  return side * side;
}

shirokov::rectangle_t shirokov::Xquare::getFrameRect() const noexcept
{
  double width = top_.y - bottom_.y;
  return {width, width, center_};
}

void shirokov::Xquare::move(const point_t &target) noexcept
{
  point_t delta = {center_.x - target.x, center_.y - target.y};
  center_ = target;
  top_ = {top_.x - delta.x, top_.y - delta.y};
  bottom_ = {bottom_.x - delta.x, bottom_.y - delta.y};
}

void shirokov::Xquare::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;

  top_.x += dx;
  top_.y += dy;

  bottom_.x += dx;
  bottom_.y += dy;
}

void shirokov::Xquare::scale(double coefficient) noexcept
{
  bottom_.y = center_.y + coefficient * (bottom_.y - center_.y);
  top_.y = center_.y + coefficient * (top_.y - center_.y);
}
