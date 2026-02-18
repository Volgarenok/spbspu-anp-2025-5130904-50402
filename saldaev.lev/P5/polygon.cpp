#include "polygon.hpp"
#include <cmath>
#include <stdexcept>

namespace
{
  saldaev::point_t calculateCenter(const saldaev::point_t *vs, size_t kk)
  {
    if (kk < 3) {
      throw std::logic_error("Invalid polygon vertices");
    }
    saldaev::point_t centre = {0, 0};
    double t_area = 0;
    double sum_area = 0;
    saldaev::point_t ab = {0, 0};
    saldaev::point_t ac = {0, 0};
    for (size_t i = 1; i < kk - 1; ++i) {
      ab = vs[i] - vs[0];
      ac = vs[i + 1] - vs[0];
      t_area = (ab.x * ac.y - ab.y * ac.x) / 2.0;
      centre += ((vs[0] + vs[i] + vs[i + 1]) / 3.0) * t_area;
      sum_area += t_area;
    }

    if (sum_area == 0.0) {
      throw std::logic_error("Invalid polygon vertices");
    }
    return centre / sum_area;
  }
}

saldaev::Polygon::Polygon(point_t *vs, size_t kk):
  k_(kk),
  pos_(calculateCenter(vs, kk)),
  vertexes_(new point_t[kk])
{
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] = vs[i];
  }
}

saldaev::Polygon::~Polygon()
{
  delete[] vertexes_;
}

saldaev::Polygon::Polygon(const Polygon &other):
  k_(other.k_),
  pos_(other.pos_),
  vertexes_(new point_t[other.k_])
{
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] = other.vertexes_[i];
  }
}

saldaev::Polygon &saldaev::Polygon::operator=(const Polygon &other)
{
  if (&other == this) {
    return *this;
  }

  point_t *newVertexes = new point_t[other.k_];
  for (size_t i = 0; i < k_; ++i) {
    newVertexes[i] = other.vertexes_[i];
  }

  delete[] vertexes_;
  vertexes_ = newVertexes;
  k_ = other.k_;
  pos_ = other.pos_;
  return *this;
}

saldaev::Polygon::Polygon(Polygon &&other) noexcept:
  k_(other.k_),
  pos_(other.pos_),
  vertexes_(other.vertexes_)
{
  other.vertexes_ = nullptr;
}

saldaev::Polygon &saldaev::Polygon::operator=(Polygon &&other) noexcept
{
  if (&other == this) {
    return *this;
  }

  k_ = other.k_;
  pos_ = other.pos_;

  delete[] vertexes_;
  vertexes_ = other.vertexes_;
  other.vertexes_ = nullptr;

  return *this;
}

double saldaev::Polygon::getArea() const noexcept
{
  double area = 0;
  size_t j = 0;
  for (size_t i = 0; i < k_; ++i) {
    j = (i + 1) % k_;
    area += vertexes_[i].x * vertexes_[j].y - vertexes_[j].x * vertexes_[i].y;
  }
  return std::abs(area) * 0.5;
}

saldaev::rectangle_t saldaev::Polygon::getFrameRect() const noexcept
{
  double maxx = vertexes_[0].x;
  double minx = maxx;
  double maxy = vertexes_[0].y;
  double miny = maxy;

  for (size_t i = 1; i < k_; ++i) {
    maxx = std::max(maxx, vertexes_[i].x);
    minx = std::min(minx, vertexes_[i].x);
    maxy = std::max(maxy, vertexes_[i].y);
    miny = std::min(miny, vertexes_[i].y);
  }

  double dx = maxx - minx;
  double dy = maxy - miny;
  return {dx, dy, {minx + dx / 2, miny + dy / 2}};
}

void saldaev::Polygon::move(point_t target) noexcept
{
  point_t dp = target - pos_;
  move(dp.x, dp.y);
}

void saldaev::Polygon::move(double dx, double dy) noexcept
{
  pos_ += {dx, dy};
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] += {dx, dy};
  }
}

void saldaev::Polygon::doScale(double coef) noexcept
{
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] += (vertexes_[i] - pos_) * (coef - 1);
  }
}
