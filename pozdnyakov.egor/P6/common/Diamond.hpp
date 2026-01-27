#ifndef DIAMOND_HPP
#define DIAMOND_HPP

#include "Shape.hpp"

namespace pozdnyakov
{
  class Diamond final: public Shape
  {
  public:
    Diamond(const point_t &center, double diag_h, double diag_v) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    Shape *clone() const override;

  private:
    point_t center_;
    double diag_h_;
    double diag_v_;
    void doScale(double k) noexcept override;
  };
}

#endif
