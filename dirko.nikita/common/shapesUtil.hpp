#ifndef SHAPES_UTIL
#define SHAPES_UTIL
#include <iosfwd>

namespace dirko
{
  struct point_t
  {
    double x, y;
  };
  struct rectangle_t
  {
    double w, h;
    point_t pos;
  };
  struct Shape
  {
    virtual ~Shape() noexcept = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t point) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    void doScale(double coef) noexcept;
    void doScaleSafe(double coef);

  private:
    virtual void scale_(double coef) noexcept = 0;
  };
  void scaleFromPoint(Shape **shps, size_t size, point_t point, double coef);
  rectangle_t getTotalFrame(const Shape *const *shps, size_t size);
  std::ostream &output(std::ostream &os, const Shape *const *shps, size_t size);
  std::ostream &outRect(std::ostream &os, rectangle_t frame);
}
#endif
