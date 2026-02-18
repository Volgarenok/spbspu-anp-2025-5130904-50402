#ifndef POLYGON_HPP
#define POLYGON_HPP
#include "shape.hpp"
namespace saldaev
{
  struct Polygon final: Shape
  {
    Polygon(const Polygon &other);
    Polygon(Polygon &&other) noexcept;
    Polygon(point_t *vs, size_t kk);
    ~Polygon() noexcept;
    Polygon &operator=(const Polygon &other);
    Polygon &operator=(Polygon &&other) noexcept;

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t target) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    size_t k_;
    point_t pos_;
    point_t *vertexes_;
    void doScale(double coef) noexcept override;
  };
}
#endif
