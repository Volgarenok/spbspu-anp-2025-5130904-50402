#include "shapes.hpp"
#include <algorithm>

karpovich::Rubber::Rubber(double radius1, double radius2, point_t centr1, point_t centr2) noexcept:
  radius1_(radius1),
  radius2_(radius2),
  centr1_(centr1),
  centr2_(centr2)
{}
double karpovich::Rubber::getArea() const noexcept
{
  return PI * (radius1_ * radius1_ - radius2_ * radius2_);
}
karpovich::rectangle_t karpovich::Rubber::getFrameRect() const noexcept
{
  double left = std::min(centr1_.x - radius1_, centr2_.x - radius2_);
  double right = std::max(centr1_.x + radius1_, centr2_.x + radius2_);
  double bottom = std::min(centr1_.y - radius1_, centr2_.y - radius2_);
  double top = std::max(centr1_.y + radius1_, centr2_.y + radius2_);

  return {{right - left}, {top - bottom}, {(left + right) / 2.0, (bottom + top) / 2.0}};
}
void karpovich::Rubber::move(point_t p) noexcept
{
  double dx = p.x - centr2_.x;
  double dy = p.y - centr2_.y;
  centr1_.x += dx;
  centr1_.y += dy;
  centr2_ = p;
}
void karpovich::Rubber::move(double dx, double dy) noexcept
{
  centr1_.x += dx;
  centr1_.y += dy;
  centr2_.x += dx;
  centr2_.y += dy;
}
void karpovich::Rubber::doScale(double k) noexcept
{
  radius1_ *= k;
  radius2_ *= k;
}
