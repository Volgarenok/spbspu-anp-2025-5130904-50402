#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "geom.hpp"
namespace saldaev
{
  struct Shape
  {
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t target) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    void scale(double coef);
    void nsfwscale(double coef) noexcept;

  private:
    virtual void doScale(double coef) noexcept = 0;
  };
}
#endif
