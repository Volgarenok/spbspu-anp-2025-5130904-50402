#ifndef COMPOSITE_SHAPE_HPP
#define COMPOSITE_SHAPE_HPP

#include "shape.hpp"

namespace pozdnyakov
{
  class CompositeShape final: public Shape
  {
  public:
    CompositeShape() noexcept;
    CompositeShape(const CompositeShape &other);
    CompositeShape(CompositeShape &&other) noexcept;
    CompositeShape &operator=(const CompositeShape &other);
    CompositeShape &operator=(CompositeShape &&other) noexcept;
    ~CompositeShape() override;

    Shape *operator[](unsigned int index);
    const Shape *operator[](unsigned int index) const;

    void addShape(Shape *shape);
    void removeShape(unsigned int index);
    unsigned int size() const noexcept;

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) override;
    Shape *clone() const override;

  private:
    unsigned int count_;
    unsigned int capacity_;
    Shape **shapes_;

    void doScale(double k) noexcept override;
    void swap(CompositeShape &other) noexcept;
  };
}

#endif
