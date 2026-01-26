#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "pods.hpp"

namespace shirokov
{
  struct Shape
  {
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t &target) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    void safeScale(double coefficient);
    virtual void scale(double coefficient) noexcept = 0;
  };
}

#endif
