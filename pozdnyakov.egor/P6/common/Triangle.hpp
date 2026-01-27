#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"

namespace pozdnyakov
{
  class Triangle final: public Shape
  {
  public:
    Triangle(const point_t &a, const point_t &b, const point_t &c) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    Shape *clone() const override;

  private:
    point_t p1_;
    point_t p2_;
    point_t p3_;
    void doScale(double k) noexcept override;
  };
}

#endif
