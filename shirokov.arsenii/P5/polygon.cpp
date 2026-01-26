#include "polygon.hpp"
#include "pods.hpp"
#include <algorithm>
#include <cstdlib>

shirokov::Polygon::Polygon(const point_t *vertices, size_t s):
  s_(s),
  vertices_(new point_t[s])
{
  for (size_t i = 0; i < s; ++i)
  {
    vertices_[i] = vertices[i];
  }
}

double shirokov::Polygon::getArea() const noexcept
{
  double area = 0;
  for (size_t i = 0; i < s_; ++i)
  {
    double xi = vertices_[i].x;
    double yi = vertices_[i].y;
    size_t j = (i + 1) % s_;
    double xj = vertices_[j].x;
    double yj = vertices_[j].y;
    double cross = xi * yj - xj * yi;
    area += cross;
  }
  area *= 0.5;
  return std::abs(area);
}

shirokov::point_t shirokov::Polygon::getCenter()
{
  double cx = 0, cy = 0;
  double signedDoubleArea = 0;
  for (size_t i = 0; i < s_; ++i)
  {
    double xi = vertices_[i].x;
    double yi = vertices_[i].y;
    size_t j = (i + 1) % s_;
    double xj = vertices_[j].x;
    double yj = vertices_[j].y;
    double cross = xi * yj - xj * yi;
    signedDoubleArea += cross;
    cx += (xi + xj) * cross;
    cy += (yi + yj) * cross;
  }
  cx /= 6 * signedDoubleArea;
  cy /= 6 * signedDoubleArea;

  return {cx, cy};
}

shirokov::rectangle_t shirokov::Polygon::getFrameRect() const noexcept
{
  double maxx = vertices_[0].x, minx = vertices_[0].x, maxy = vertices_[0].y, miny = vertices_[0].y;
  for (size_t i = 0; i < s_; ++i)
  {
    maxx = std::max(maxx, vertices_[i].x);
    minx = std::min(minx, vertices_[i].x);
    maxy = std::max(maxy, vertices_[i].y);
    miny = std::min(miny, vertices_[i].y);
  }
  double width = maxx - minx;
  double height = maxy - miny;
  point_t pos = {(minx + maxx) / 2, (miny + maxy) / 2};
  return {width, height, pos};
}

void shirokov::Polygon::move(const point_t &target) noexcept
{
  const point_t center_ = getCenter();
  point_t delta = {center_.x - target.x, center_.y - target.y};
  for (size_t i = 0; i < s_; ++i)
  {
    vertices_[i] = {vertices_[i].x - delta.x, vertices_[i].y - delta.y};
  }
}

void shirokov::Polygon::move(double dx, double dy) noexcept
{
  for (size_t i = 0; i < s_; ++i)
  {
    vertices_[i].x += dx;
    vertices_[i].y += dy;
  }
}

void shirokov::Polygon::scale(double coefficient) noexcept
{
  const point_t center_ = getCenter();
  for (size_t i = 0; i < s_; ++i)
  {
    double tempX = center_.x + coefficient * (vertices_[i].x - center_.x);
    double tempY = center_.y + coefficient * (vertices_[i].y - center_.y);
    vertices_[i] = {tempX, tempY};
  }
}

shirokov::Polygon::~Polygon() noexcept
{
  delete[] vertices_;
}
