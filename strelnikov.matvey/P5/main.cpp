#include <cstdlib>
#include <iostream>
namespace strelnikov {
  struct point_t {
    double x_, y_;
  };
  struct rectangle_t {
    double width_, height_;
    point_t c_;
  };
  class IShape {
  public:
    virtual double getArea() = 0;
    virtual rectangle_t getFrameRect() = 0;
    virtual void move(point_t) = 0;
    virtual void move(double, double) = 0;
    virtual void scale(double) = 0;
    virtual ~IShape() = default;
  };
  class Rectangle : public IShape {
  public:
    Rectangle(double, double, point_t);
    double getArea() override;
    rectangle_t getFrameRect() override;
    void move(point_t) override;
    void move(double, double) override;
    void scale(double) override;
    ~Rectangle() override = default;

  private:
    rectangle_t rec_;
  };

  class Polygon : public IShape {
  public:
    Polygon(point_t*, size_t);
    double getArea() override;
    rectangle_t getFrameRect() override;
    void move(point_t) override;
    void move(double, double) override;
    void scale(double) override;
    ~Polygon() override = default;

  private:
    point_t* data_;
    size_t size_;
    point_t c_;
  };

  class Circle : public IShape {
  public:
    Circle(point_t, double);
    double getArea() override;
    rectangle_t getFrameRect() override;
    void move(point_t) override;
    void move(double, double) override;
    void scale(double) override;
    ~Circle() override = default;

  private:
    point_t cen_;
    double rad_;
  };
}; // namespace strelnikov

strelnikov::Rectangle::Rectangle(double wdth, double hght, point_t cntr) : rec_{wdth, hght, cntr}
{}

double strelnikov::Rectangle::getArea()
{
  return rec_.height_ * rec_.width_;
}

strelnikov::rectangle_t strelnikov::Rectangle::getFrameRect()
{
  return rec_;
}

void strelnikov::Rectangle::move(strelnikov::point_t p)
{
  rec_.c_.y_ += p.y_;
  rec_.c_.x_ += p.x_;
}

void strelnikov::Rectangle::move(double x, double y)
{
  strelnikov::Rectangle::move({x,y});
}

void strelnikov::Rectangle::scale(double k)
{
  strelnikov::Rectangle::rec_.c_.x_ *= k;
  strelnikov::Rectangle::rec_.c_.y_ *= k;
}

int main()
{}