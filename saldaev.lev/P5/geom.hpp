#ifndef GEOM_HPP
#define GEOM_HPP
#include <cstddef>
namespace saldaev
{
  struct point_t
  {
    double x, y;
  };

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  point_t operator+(point_t a, point_t b) noexcept;
  point_t operator-(point_t a, point_t b) noexcept;
  point_t operator*(point_t p, double coef) noexcept;
  point_t operator/(point_t p, double coef) noexcept;

  point_t &operator+=(point_t &a, point_t b) noexcept;
  point_t &operator-=(point_t &a, point_t b) noexcept;
  point_t &operator*=(point_t &p, double coef) noexcept;
  point_t &operator/=(point_t &p, double coef) noexcept;
}
#endif
