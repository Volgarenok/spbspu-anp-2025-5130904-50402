#ifndef XQUARE_HPP
#define XQUARE_HPP
#include "pods.hpp"
#include "shape.hpp"

namespace shirokov
{
  struct Xquare final: Shape
  {
    Xquare(const point_t &center, double side) noexcept;
    Xquare(const point_t &top, const point_t &bottom) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &target) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double coefficient) noexcept override;

  private:
    point_t center_, top_, bottom_;
  };
}

#endif
