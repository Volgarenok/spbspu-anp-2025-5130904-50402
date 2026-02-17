#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "shape.hpp"
namespace saldaev
{
  struct Rectangle final: Shape
  {
    Rectangle(point_t p, double w, double h);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t target) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t pos_;
    double width_, height_;
    void doScale(double coef) noexcept override;
  };
}
#endif
