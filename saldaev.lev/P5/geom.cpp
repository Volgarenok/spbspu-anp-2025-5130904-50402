#include "geom.hpp"

saldaev::point_t saldaev::operator-(point_t a, point_t b) noexcept
{
  return {a.x - b.x, a.y - b.y};
}

saldaev::point_t saldaev::operator+(point_t a, point_t b) noexcept
{
  return {a.x + b.x, a.y + b.y};
}

saldaev::point_t saldaev::operator*(point_t p, double coef) noexcept
{
  return {p.x * coef, p.y * coef};
}

saldaev::point_t saldaev::operator/(point_t p, double coef) noexcept
{
  return {p.x / coef, p.y / coef};
}

saldaev::point_t &saldaev::operator+=(point_t &a, point_t b) noexcept
{
  a.x += b.x;
  a.y += b.y;
  return a;
}

saldaev::point_t &saldaev::operator-=(point_t &a, point_t b) noexcept
{
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

saldaev::point_t &saldaev::operator*=(point_t &p, double coef) noexcept
{
  p.x *= coef;
  p.y *= coef;
  return p;
}

saldaev::point_t &saldaev::operator/=(point_t &p, double coef) noexcept
{
  p.x /= coef;
  p.y /= coef;
  return p;
}
