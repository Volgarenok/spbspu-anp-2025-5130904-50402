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

  double get_tr_poly_S(point_t a, point_t b, point_t c);
  point_t get_tr_poly_C(point_t a, point_t b, point_t c);
  point_t get_poly_C();

};

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
  strelnikov::Rectangle::move({x, y});
}

void strelnikov::Rectangle::scale(double k)
{
  strelnikov::Rectangle::rec_.c_.x_ *= k;
  strelnikov::Rectangle::rec_.c_.y_ *= k;
}

double strelnikov::get_tr_poly_S(strelnikov::point_t a, strelnikov::point_t b, strelnikov::point_t c)
{
  double res = a.x_ * (b.y_ - c.y_) + b.x_ * (c.y_ - a.y_) + c.x_ * (a.y_ - b.y_);
  if (res < 0) {
    res *= -1;
  }
  res *= 0.5;
  return res;
}

strelnikov::point_t strelnikov::get_tr_poly_C(strelnikov::point_t a, strelnikov::point_t b, strelnikov::point_t c)
{
  double x = (a.x_ + b.x_ + c.x_) / 3.0;
  double y = (a.y_ + b.y_ + c.y_) / 3.0;
  return {x, y};
}

strelnikov::point_t strelnikov::get_poly_C(const strelnikov::point_t* data, size_t k)
{
  double sum_s = 0;
  double sum_x = 0;
  double sum_y = 0;
  for (size_t i = 1; i < (k - 1); ++i) {
    double cur_sum = strelnikov::get_tr_poly_S(data[i], data[i + 1], data[0]);
    sum_s += cur_sum;
    sum_x += ((strelnikov::get_tr_poly_C(data[i], data[i + 1], data[0])).x_ * cur_sum);
    sum_y += ((strelnikov::get_tr_poly_C(data[i], data[i + 1], data[0])).y_ * cur_sum);
  }

  if (sum_s == 0.0) {
    throw std::logic_error("Bad poly");
  }
  return {(sum_x / sum_s), (sum_y / sum_s)};
}



int main()
{}