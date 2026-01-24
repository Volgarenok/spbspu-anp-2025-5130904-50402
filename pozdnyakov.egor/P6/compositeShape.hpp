#ifndef COMPOSITE_SHAPE_HPP
#define COMPOSITE_SHAPE_HPP

#include "Shape.hpp"
#include <cstddef>

namespace pozdnyakov
{
  class CompositeShape final: public Shape
  {
  public:
    CompositeShape();
    CompositeShape(const CompositeShape &other);
    CompositeShape(CompositeShape &&other) noexcept;
    CompositeShape &operator=(const CompositeShape &other);
    CompositeShape &operator=(CompositeShape &&other) noexcept;
    ~CompositeShape() override;

    void addShape(Shape *shape);
    void removeShape(size_t index);
    size_t getCount() const noexcept;

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) override;
    Shape *clone() const override;

  private:
    size_t count_;
    size_t capacity_;
    Shape **shapes_;

    void doScale(double k) noexcept override;
    void swap(CompositeShape &other) noexcept;
  };
}

#endif
