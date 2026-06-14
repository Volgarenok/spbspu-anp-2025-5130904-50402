#ifndef SHAPE_VEC
#define SHAPE_VEC
#include <cstddef>
#include <shapesUtil.hpp>
namespace dirko
{
  struct Shape_vec final: Shape
  {
    Shape_vec();
    Shape_vec(const Shape_vec &);
    Shape_vec(Shape_vec &&) noexcept;
    ~Shape_vec() noexcept override;
    Shape_vec &operator=(const Shape_vec &);
    Shape_vec &operator=(Shape_vec &&) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t point) noexcept override;
    void move(double dx, double dy) noexcept override;
    Shape_vec *clone() const override;
    void doScale(double coef) noexcept;
    void doScaleSafe(double coef);
    void append(const Shape *elem);
    void preappend(const Shape *elem);
    void add(const Shape *elem, size_t index);
    Shape &last() noexcept;
    Shape &first() noexcept;
    const Shape &lastConst() const noexcept;
    const Shape &firstConst() const noexcept;
    Shape &at(size_t index);
    const Shape &atConst(size_t index) const;
    Shape &get(size_t index) noexcept;
    const Shape &getConst(size_t index) const noexcept;
    void remove(size_t index);
    void dropFirst();
    void dropLast();
    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    size_t capasity() const noexcept;
    void shrink();
    void reserve(size_t newCap);
    void swap(Shape_vec &other) noexcept;

  private:
    Shape **shps_;
    size_t size_;
    size_t cap_;
    void scale_(double coef) noexcept override;
  };
}
#endif
