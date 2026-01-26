#include "shapes.hpp"

karpovich::Rectangle::Rectangle(double width, double height, point_t centr) noexcept:
  width_(width),
  height_(height),
  centr_(centr)
{}
double karpovich::Rectangle::getArea() const noexcept
{
  return height_ * width_;
}
karpovich::rectangle_t karpovich::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, centr_};
}
void karpovich::Rectangle::move(point_t p) noexcept
{
  centr_ = p;
}
void karpovich::Rectangle::move(double dx, double dy) noexcept
{
  centr_.x += dx;
  centr_.y += dy;
}
void karpovich::Rectangle::doScale(double k) noexcept
{
  width_ *= k;
  height_ *= k;
}
