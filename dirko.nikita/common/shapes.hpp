#ifndef SHAPES
#define SHAPES
#include <cmath>
#include <shapesUtil.hpp>
namespace dirko
{
  const double PI = std::acos(-1.0);
  struct Rectangle final: Shape
  {
    Rectangle(double w, double h, point_t mid) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t point) noexcept override;

  private:
    double w_, h_;
    point_t mid_;
    void scale_(double coef) noexcept override;
  };
  struct Polygon final: Shape
  {
    Polygon(const Polygon &);
    Polygon(Polygon &&) noexcept;
    Polygon(size_t size, point_t *pts);
    ~Polygon() noexcept override;
    Polygon &operator=(const Polygon &);
    Polygon &operator=(Polygon &&) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t point) noexcept override;

  private:
    point_t *pts_;
    size_t size_;
    point_t mid_;
    void scale_(double coef) noexcept override;
  };
  struct Bubble final: Shape
  {
    Bubble(double r, point_t dot) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t point) noexcept override;

  private:
    double r_;
    point_t dot_;
    void scale_(double coef) noexcept override;
  };
  double getPolArea(const point_t *pts, size_t size);
  point_t getPolMid(const point_t *pts, size_t size);
}
#endif
