#ifndef STORAGE_SHAPE_HPP
#define STORAGE_SHAPE_HPP
#include <podshape.hpp>
namespace karpovich
{
  struct StorageShape final: Shape
  {
    StorageShape() noexcept;
    StorageShape(const StorageShape& other) noexcept;
    StorageShape(StorageShape&& other) noexcept;
    ~StorageShape();
    StorageShape& operator=(StorageShape&& other) noexcept;
    StorageShape& operator=(const StorageShape& other) noexcept;

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t p) noexcept override;
    void move(double dx, double dy) noexcept override;
    void append(Shape* app);
    void preappend(Shape* app);
    void add(Shape* app, size_t idx);
    Shape& first() noexcept;
    Shape& last() noexcept;
    const Shape& firstConst() const noexcept;
    const Shape& lastConst() const noexcept;
    Shape& at(size_t idx);
    Shape& get(size_t idx) noexcept;
    const Shape& atConst(size_t idx) const;
    const Shape& getConst(size_t idx) const noexcept;
    void remove(size_t k);
    void dropFirst();
    void dropLast();
    void clear() noexcept;
    size_t size() noexcept;
    bool empty() noexcept;
    size_t capacity() const noexcept;
    void shrink();
    void reserve(size_t k);
    Shape* clone() const override;
    void swap(StorageShape& other) noexcept;

    private:
      Shape** shapes_;
      size_t size_;
      size_t cap_;
      void doScale(double k) noexcept override;
  };
}
#endif
