#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "pods.hpp"
#include "shape.hpp"

namespace shirokov
{
  struct Rectangle final: Shape
  {
    Rectangle(const point_t &center, double width, double height) noexcept;
    Rectangle(const point_t &bottomLeft, const point_t &topRight) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &target) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double coefficient) noexcept override;

  private:
    point_t center_, bottomLeft_, topRight_;
  };
}

#endif
