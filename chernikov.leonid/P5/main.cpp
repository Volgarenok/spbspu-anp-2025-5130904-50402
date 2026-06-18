#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>
namespace chernikov {
  struct point_t {
    double x;
    double y;
    point_t(double x_val = 0, double y_val = 0) : x(x_val), y(y_val) {}
  };
  struct rectangle_t {
    double w;
    double h;
    point_t pos;
  };
  struct Shape {
    virtual void scale(double ct) = 0;
    virtual rectangle_t getFrameRectangle() const noexcept = 0;
    virtual void move(point_t pt) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual double getArea() const noexcept = 0;
    virtual ~Shape() = default;
  };
  struct Rectangle final: Shape {
    Rectangle(double w, double h, point_t mid);
    void move(double dx, double dy) noexcept override;
    void move(point_t pt) noexcept override;
    double getArea() const noexcept override;
    rectangle_t getFrameRectangle()const noexcept override;
    void scale(double ct) noexcept override;
  private:
    double w_, h_;
    point_t mid_;
  };
  struct Polygon final: Shape {
    ~Polygon();
    Polygon(const Polygon& other);
    Polygon(Polygon&& other) noexcept;
    Polygon& operator=(const Polygon& other);
    Polygon& operator=(Polygon&& other) noexcept;

    Polygon(size_t size, point_t *points);
    void move(double dx, double dy) noexcept override;
    void move(point_t pt) noexcept override;
    double getArea() const noexcept override;
    rectangle_t getFrameRectangle() const noexcept override;
    void scale(double ct) noexcept override;
  private:
    double computeArea(point_t* pts, size_t size) const;
    point_t computeCentroid(point_t* pts, size_t size) const;
    size_t size_;
    point_t *pts_;
    point_t mid_;
  };
  struct Diamond final: Shape {
    Diamond(double rad, point_t mid);
    void move(double dx, double dy) noexcept override;
    void move(point_t pt) noexcept override;
    double getArea() const noexcept override;
    rectangle_t getFrameRectangle() const noexcept override;
    void scale(double ct) noexcept override;
  private:
    double rad_;
    point_t mid_;
  };
  std::ostream &output(std::ostream &os, const Shape *const *shps, size_t size);
  void scaleFromPoint(Shape **shps, size_t size, point_t point, double ct);
  rectangle_t getTotalFrame(const Shape *const *shps, size_t size);
}
int main()
{
  chernikov::Rectangle r(10, 2, chernikov::point_t(6, 8));
  chernikov::point_t points[4] = {
    chernikov::point_t(0, 1),
    chernikov::point_t(4, 6),
    chernikov::point_t(3, 3),
    chernikov::point_t(9.4, 3)
  };
  chernikov::Diamond d(5.2, chernikov::point_t(5, 5));
  try {
    chernikov::Polygon p(4, points);
    chernikov::Shape *shps[3] = {std::addressof(r), std::addressof(d), std::addressof(p)};
    chernikov::output(std::cout, shps, 3);
    chernikov::point_t point = {};
    double ct = 0;
    if (!(std::cin >> point.x >> point.y >> ct)) {
      std::cerr << "failed to count input" << "\n";
      return 1;
    }
    if (ct <= 0) {
      std::cerr << "Negative coef" << "\n";
      return 1;
    }
    chernikov::scaleFromPoint(shps, 3, point, ct);
    chernikov::output(std::cout, shps, 3);
    } catch (std::bad_alloc &e) {
      std::cerr << "failed to allocate memory" << "\n";
      return 2;
  }
  return 0;
}

chernikov::Rectangle::Rectangle(double w, double h, point_t mid) :
   w_(w),
   h_(h),
   mid_(mid)
{}
void chernikov::Rectangle::move(point_t pt)
{
  mid_ = pt;
}
void chernikov::Rectangle::move(double dx, double dy)
{
  mid_.x += dx;
  mid_.y += dy;
}
double chernikov::Rectangle::getArea() const
{
  return w_ * h_;
}
chernikov::rectangle_t chernikov::Rectangle::getFrameRectangle() const
{
  return {w_, h_, mid_};
}
void chernikov::Rectangle::scale(double ct)
{
  w_ = w_ * ct;
  h_ = h_ * ct;
}

chernikov::Diamond::Diamond(double rad, point_t mid) :
   rad_(rad),
   mid_(mid)
{}
void chernikov::Diamond::move(point_t pt)
{
  mid_ = pt;
}
void chernikov::Diamond::move(double dx, double dy)
{
  mid_.x += dx;
  mid_.y += dy;
}
double chernikov::Diamond::getArea() const
{
  return ((2 * rad_) * (2 * rad_)) / 2;
}
chernikov::rectangle_t chernikov::Diamond::getFrameRectangle() const
{
  return {2 * rad_, 2 * rad_, mid_};
}
void chernikov::Diamond::scale(double ct)
{
  rad_ = rad_ * ct;
}
chernikov::Polygon::~Polygon()
{
   delete[] pts_;
}
chernikov::Polygon::Polygon(const Polygon& other) :
  size_(other.size_),
  pts_(new point_t[other.size_]),
  mid_(other.mid_)
{
  for (size_t i = 0; i < size_; ++i) {
    pts_[i] = other.pts_[i];
  }
}

chernikov::Polygon::Polygon(Polygon&& other) noexcept :
  size_(other.size_),
  pts_(other.pts_),
  mid_(other.mid_)
{
  other.size_ = 0;
  other.pts_ = nullptr;
  other.mid_ = point_t{0, 0};
}

chernikov::Polygon& chernikov::Polygon::operator=(const Polygon& other)
{
  if (this != &other) {
    delete[] pts_;
    size_ = other.size_;
    pts_ = new point_t[size_];
    mid_ = other.mid_;
    for (size_t i = 0; i < size_; ++i) {
      pts_[i] = other.pts_[i];
    }
  }
  return *this;
}

chernikov::Polygon& chernikov::Polygon::operator=(Polygon&& other) noexcept
{
  if (this != &other) {
    delete[] pts_;
    size_ = other.size_;
    pts_ = other.pts_;
    mid_ = other.mid_;
    other.size_ = 0;
    other.pts_ = nullptr;
    other.mid_ = point_t{0, 0};
  }
  return *this;
}
chernikov::Polygon::Polygon(size_t size, point_t *points) :
   size_(size),
   pts_(size < 3 ? nullptr : new point_t[size]),
   mid_(computeCentroid(points, size))
{
  if (size < 3) {
    throw std::logic_error("Not a polygon");
  }
  for (size_t i = 0; i < size; ++i) {
        pts_[i] = points[i];
    }
}
void chernikov::Polygon::move(double dx, double dy)
{
  for (size_t i = 0; i < size_; ++i) {
    pts_[i].x += dx;
    pts_[i].y += dy;
  }
  mid_.x += dx;
  mid_.y += dy;
}
void chernikov::Polygon::move(point_t pt)
{
  double dx = pt.x - mid_.x;
  double dy = pt.y - mid_.y;
  move(dx, dy);
}
double chernikov::Polygon::getArea() const
{
  double area = 0;
  for (size_t i = 0; i < size_; ++i) {
    size_t j = (i + 1) % size_;
    area += pts_[i].x * pts_[j].y;
    area -= pts_[j].x * pts_[i].y;
  }
  area = std::abs(area) / 2;
  return area;
}
chernikov::rectangle_t chernikov::Polygon::getFrameRectangle() const
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
void chernikov::Polygon::scale(double ct)
{
  --ct;
  double dx = 0, dy = 0;
  for (size_t i = 0; i < size_; ++i) {
    dx = pts_[i].x - mid_.x;
    dy = pts_[i].y - mid_.y;
    pts_[i].x += dx * ct;
    pts_[i].y += dy * ct;
  }
}
std::ostream &chernikov::output(std::ostream &out, Shape **shps, size_t size)
{
  double totalArea = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shps[i]->getArea();
    totalArea += area;
  }
  rectangle_t totalFrame = getTotalFrame(shps, size);
  out << totalArea << "\n";
  out << totalFrame.pos.x << " " << totalFrame.pos.y << "\n";
  out << totalFrame.w << " " << totalFrame.h << "\n";
  return out;
}
void chernikov::scaleFromPoint(Shape **shps, size_t size, point_t point, double ct)
{
  for (size_t i = 0; i < size; ++i) {
    shps[i]->move(-point.x, -point.y);
    shps[i]->scale(ct);
    shps[i]->move(point.x, point.y);
  }
}
chernikov::rectangle_t chernikov::getTotalFrame(Shape **shps, size_t size)
{
  if (size == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  rectangle_t frame = shps[0]->getFrameRectangle();
  double left = frame.pos.x - frame.w / 2.0;
  double right = frame.pos.x + frame.w / 2.0;
  double bottom = frame.pos.y - frame.h / 2.0;
  double top = frame.pos.y + frame.h / 2.0;
  for (size_t i = 1; i < size; ++i) {
    frame = shps[i]->getFrameRectangle();
    double l = frame.pos.x - frame.w / 2.0;
    double r = frame.pos.x + frame.w / 2.0;
    double b = frame.pos.y - frame.h / 2.0;
    double t = frame.pos.y + frame.h / 2.0;
    left = std::min(left, l);
    right = std::max(right, r);
    bottom = std::min(bottom, b);
    top = std::max(top, t);
  }
  rectangle_t total;
  total.w = right - left;
  total.h = top - bottom;
  total.pos.x = (left + right) / 2.0;
  total.pos.y = (bottom + top) / 2.0;
  return total;
}
double chernikov::Polygon::computeArea(point_t* pts, size_t size) const
{
  double area = 0.0;
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    area += pts[i].x * pts[j].y - pts[j].x * pts[i].y;
  }
  return std::abs(area) / 2.0;
}
chernikov::point_t chernikov::Polygon::computeCentroid(point_t* pts, size_t size) const
{
  double area = computeArea(pts, size);
  point_t centroid{0, 0};
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    double factor = pts[i].x * pts[j].y - pts[j].x * pts[i].y;
    centroid.x += (pts[i].x + pts[j].x) * factor;
    centroid.y += (pts[i].y + pts[j].y) * factor;
  }
  centroid.x /= (6.0 * area);
  centroid.y /= (6.0 * area);
  return centroid;
}
//EOF
