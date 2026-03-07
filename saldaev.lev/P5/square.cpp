#include "square.hpp"

saldaev::Square::Square(point_t p, double s):
  pos_(p),
  side_(s)
{}

double saldaev::Square::getArea() const noexcept
{
  return side_ * side_;
}

saldaev::rectangle_t saldaev::Square::getFrameRect() const noexcept
{
  return {side_, side_, pos_};
}

void saldaev::Square::move(point_t target) noexcept
{
  pos_ = target;
}

void saldaev::Square::move(double dx, double dy) noexcept
{
  pos_.x += dx;
  pos_.y += dy;
}

void saldaev::Square::doScale(double coef) noexcept
{
  side_ *= coef;
}
