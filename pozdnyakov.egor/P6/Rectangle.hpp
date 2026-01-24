#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Shape.hpp"

namespace pozdnyakov
{
  class Rectangle final: public Shape
  {
  public:
    Rectangle(const point_t &center, double width, double height) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    Shape *clone() const override;

  private:
    rectangle_t frame_;
    void doScale(double k) noexcept override;
  };
}

#endif
