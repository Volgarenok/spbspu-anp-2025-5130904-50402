#ifndef PODSHAPE_HPP
#define PODSHAPE_HPP
#include <iosfwd>
namespace karpovich
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
  struct Shape
  {
    virtual ~Shape() noexcept = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t p) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    void scale(double k);
    void uncheckedScale(double k) noexcept;
    protected:
      virtual void doScale(double k) noexcept = 0;
  };
  void scalefrompt(Shape* shapes[], size_t size, double k, point_t pt);
  rectangle_t computeTotalFrame(const Shape* const shapes[], size_t size);
  void output(const Shape* const shapes[], size_t size);
  void outputRectangle(const rectangle_t& rect);
}
#endif
