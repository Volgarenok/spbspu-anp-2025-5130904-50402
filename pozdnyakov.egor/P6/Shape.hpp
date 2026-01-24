#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "baseTypes.hpp"

namespace pozdnyakov
{
  class Shape
  {
  public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t &pos) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    virtual void scale(double k);
    virtual Shape *clone() const = 0;

    void unsafeScale(double k) noexcept;

  private:
    virtual void doScale(double k) noexcept = 0;
  };
}

#endif
