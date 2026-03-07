#ifndef SQUARE_HPP
#define SQUARE_HPP
#include "shape.hpp"
namespace saldaev
{
  struct Square final: Shape
  {
    Square(point_t p, double s);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t target) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t pos_;
    double side_;
    void doScale(double coef) noexcept override;
  };
}
#endif
