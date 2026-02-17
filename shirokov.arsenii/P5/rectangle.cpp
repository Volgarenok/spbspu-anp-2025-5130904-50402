#include "rectangle.hpp"

shirokov::Rectangle::Rectangle(const point_t &center, double width, double height) noexcept:
  center_(center),
  bottomLeft_({center.x - width / 2, center.y - height / 2}),
  topRight_({center.x + width / 2, center.y + height / 2})
{}

shirokov::Rectangle::Rectangle(const point_t &bottomLeft, const point_t &topRight) noexcept:
  center_({(topRight.x + bottomLeft.x) / 2, (topRight.y + bottomLeft.y) / 2}),
  bottomLeft_(bottomLeft),
  topRight_(topRight)
{}

double shirokov::Rectangle::getArea() const noexcept
{
  return (topRight_.x - bottomLeft_.x) * (topRight_.y - bottomLeft_.y);
}

shirokov::rectangle_t shirokov::Rectangle::getFrameRect() const noexcept
{
  return {topRight_.x - bottomLeft_.x, topRight_.y - bottomLeft_.y, center_};
}

void shirokov::Rectangle::move(const point_t &target) noexcept
{
  point_t delta = {center_.x - target.x, center_.y - target.y};
  center_ = target;
  topRight_ = {topRight_.x - delta.x, topRight_.y - delta.y};
  bottomLeft_ = {bottomLeft_.x - delta.x, bottomLeft_.y - delta.y};
}

void shirokov::Rectangle::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;

  topRight_.x += dx;
  topRight_.y += dy;

  bottomLeft_.x += dx;
  bottomLeft_.y += dy;
}

void shirokov::Rectangle::scale(double coefficient) noexcept
{
  double tempX = center_.x + coefficient * (bottomLeft_.x - center_.x);
  double tempY = center_.y + coefficient * (bottomLeft_.y - center_.y);
  bottomLeft_ = {tempX, tempY};

  tempX = center_.x + coefficient * (topRight_.x - center_.x);
  tempY = center_.y + coefficient * (topRight_.y - center_.y);
  topRight_ = {tempX, tempY};
}
