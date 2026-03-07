#include "rectangle.hpp"

saldaev::Rectangle::Rectangle(point_t p, double w, double h):
  pos_(p),
  width_(w),
  height_(h)
{}

double saldaev::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

saldaev::rectangle_t saldaev::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, pos_};
}

void saldaev::Rectangle::move(point_t target) noexcept
{
  pos_ = target;
}

void saldaev::Rectangle::move(double dx, double dy) noexcept
{
  pos_.x += dx;
  pos_.y += dy;
}

void saldaev::Rectangle::doScale(double coef) noexcept
{
  width_ *= coef;
  height_ *= coef;
}
