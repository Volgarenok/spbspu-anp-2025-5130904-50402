#include "Rectangle.hpp"

namespace pozdnyakov
{
  Rectangle::Rectangle(const point_t& center, double width, double height):
    frame_{ width, height, center }
  {
  }

  double Rectangle::getArea() const noexcept
  {
    return frame_.width * frame_.height;
  }

  rectangle_t Rectangle::getFrameRect() const noexcept
  {
    return frame_;
  }

  void Rectangle::move(const point_t& pos) noexcept
  {
    frame_.pos = pos;
  }

  void Rectangle::move(double dx, double dy) noexcept
  {
    frame_.pos.x += dx;
    frame_.pos.y += dy;
  }

  void Rectangle::doScale(double k) noexcept
  {
    frame_.width *= k;
    frame_.height *= k;
  }
}
