#include "shapes.hpp"
#include <stdexcept>

double dirko::getPolArea(const point_t *pts, size_t size)
{
  double area = 0;
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    area += pts[i].x * pts[j].y;
    area -= pts[j].x * pts[i].y;
  }
  area = std::abs(area) / 2;
  return area;
}
dirko::point_t dirko::getPolMid(const point_t *pts, size_t size)
{
  double area = getPolArea(pts, size);
  point_t mid{};
  double factor = 0.0;
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    factor = (pts[i].x * pts[j].y - pts[j].x * pts[i].y);
    mid.x += (pts[i].x + pts[j].x) * factor;
    mid.y += (pts[i].y + pts[j].y) * factor;
  }
  mid.x /= (6.0 * area);
  mid.y /= (6.0 * area);
  return mid;
}
dirko::Polygon::Polygon(size_t size, point_t *pts):
  pts_(size > 2 ? new point_t[size] : nullptr),
  size_(size),
  mid_(getPolMid(pts, size))
{
  if (size < 3) {
    throw std::logic_error("Cant create polygon");
  }
  for (size_t i = 0; i < size_; ++i) {
    pts_[i] = pts[i];
  }
}
dirko::Polygon::~Polygon() noexcept
{
  delete[] pts_;
}
dirko::Polygon::Polygon(const Polygon &other):
  pts_(other.size_ == 0 ? nullptr : new point_t[other.size_]),
  size_(other.size_),
  mid_(other.mid_)
{
  for (size_t i = 0; i < size_; ++i) {
    pts_[i] = other.pts_[i];
  }
}
dirko::Polygon::Polygon(Polygon &&r) noexcept:
  pts_(r.pts_),
  size_(r.size_),
  mid_(r.mid_)
{
  r.pts_ = nullptr;
}
dirko::Polygon &dirko::Polygon::operator=(const Polygon &other)
{
  point_t *tmp = new point_t[other.size_];
  delete[] pts_;
  pts_ = tmp;
  size_ = other.size_;
  mid_ = other.mid_;
  for (size_t i = 0; i < size_; ++i) {
    pts_[i] = other.pts_[i];
  }
  return *this;
}
dirko::Polygon &dirko::Polygon::operator=(Polygon &&r) noexcept
{
  delete[] pts_;
  pts_ = r.pts_;
  size_ = r.size_;
  mid_ = r.mid_;
  r.pts_ = nullptr;
  return *this;
}
double dirko::Polygon::getArea() const noexcept
{
  return getPolArea(pts_, size_);
}
dirko::rectangle_t dirko::Polygon::getFrameRect() const noexcept
{
  double maxx = pts_[0].x, minx = pts_[0].x;
  double maxy = pts_[0].y, miny = pts_[0].y;
  for (size_t i = 1; i < size_; ++i) {
    maxx = std::max(maxx, pts_[i].x);
    minx = std::min(minx, pts_[i].x);
    maxy = std::max(maxy, pts_[i].y);
    miny = std::min(miny, pts_[i].y);
  }
  double w = maxx - minx;
  double h = maxy - miny;
  return {w, h, {maxx - w / 2, maxy - h / 2}};
}
void dirko::Polygon::move(double dx, double dy) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    pts_[i].x += dx;
    pts_[i].y += dy;
  }
  mid_.x += dx;
  mid_.y += dy;
}
void dirko::Polygon::move(point_t point) noexcept
{
  double dx = point.x - mid_.x;
  double dy = point.y - mid_.y;
  move(dx, dy);
}
void dirko::Polygon::scale_(double coef) noexcept
{
  double dx = 0, dy = 0;
  for (size_t i = 0; i < size_; ++i) {
    dx = pts_[i].x - mid_.x;
    dy = pts_[i].y - mid_.y;
    pts_[i].x += dx * (coef - 1);
    pts_[i].y += dy * (coef - 1);
  }
}
