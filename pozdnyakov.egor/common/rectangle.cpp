#include "rectangle.hpp"

pozdnyakov::Rectangle::Rectangle(const point_t &center, double width, double height) noexcept:
  frame_{width, height, center}
{}

double pozdnyakov::Rectangle::getArea() const noexcept
{
  return frame_.width * frame_.height;
}

pozdnyakov::rectangle_t pozdnyakov::Rectangle::getFrameRect() const noexcept
{
  return frame_;
}

void pozdnyakov::Rectangle::move(const point_t &pos) noexcept
{
  frame_.pos = pos;
}

void pozdnyakov::Rectangle::move(double dx, double dy) noexcept
{
  frame_.pos.x += dx;
  frame_.pos.y += dy;
}

pozdnyakov::Shape *pozdnyakov::Rectangle::clone() const
{
  return new Rectangle(*this);
}

void pozdnyakov::Rectangle::doScale(double k) noexcept
{
  frame_.width *= k;
  frame_.height *= k;
}
