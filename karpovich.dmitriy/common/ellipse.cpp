#include "shapes.hpp"

karpovich::Ellipse::Ellipse(double semiax1, double semiax2, point_t centr) noexcept:
  semiax1_(semiax1),
  semiax2_(semiax2),
  centr_(centr)
{}
double karpovich::Ellipse::getArea() const noexcept
{
  return PI * semiax1_ * semiax2_;
}
karpovich::rectangle_t karpovich::Ellipse::getFrameRect() const noexcept
{
  return {2 * semiax1_, 2 * semiax2_, centr_};
}
void karpovich::Ellipse::move(point_t p) noexcept
{
  centr_ = p;
}
void karpovich::Ellipse::move(double dx, double dy) noexcept
{
  centr_.x += dx;
  centr_.y += dy;
}
void karpovich::Ellipse::doScale(double k) noexcept
{
  semiax1_ *= k;
  semiax2_ *= k;
}
