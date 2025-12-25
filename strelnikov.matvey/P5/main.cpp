#include <cstdlib>
#include <iostream>
namespace strelnikov {
  constexpr double PI = 3.14;
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
  point_t get_poly_C(const strelnikov::point_t* data, size_t k);

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
  strelnikov::Rectangle::move({x, y});
}

void strelnikov::Rectangle::scale(double k)
{
  strelnikov::Rectangle::rec_.c_.x_ *= k;
  strelnikov::Rectangle::rec_.c_.y_ *= k;
}

double
strelnikov::get_tr_poly_S(strelnikov::point_t a, strelnikov::point_t b, strelnikov::point_t c)
{
  double res = a.x_ * (b.y_ - c.y_) + b.x_ * (c.y_ - a.y_) + c.x_ * (a.y_ - b.y_);
  if (res < 0) {
    res *= -1;
  }
  res *= 0.5;
  return res;
}

strelnikov::point_t
strelnikov::get_tr_poly_C(strelnikov::point_t a, strelnikov::point_t b, strelnikov::point_t c)
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

strelnikov::Polygon::Polygon(strelnikov::point_t* data, size_t size)
    : data_(data), size_(size), c_{0, 0}
{
  if (size < 3 || !data) {
    delete[] data_;
    throw std::logic_error("Bad poly");
  }
  c_ = strelnikov::get_poly_C(data, size);
}

double strelnikov::Polygon::getArea()
{
  double res = 0;
  for (size_t i = 1; i < (size_ - 1); ++i) {
    res += get_tr_poly_S(data_[i], data_[i - 1], data_[0]);
  }
  return res;
}

strelnikov::rectangle_t strelnikov::Polygon::getFrameRect()
{
  double min_x = data_[0].x_;
  double min_y = data_[0].y_;
  double max_y = data_[0].y_;
  double max_x = data_[0].x_;
  for (size_t i = 1; i < size_; ++i) {
    min_x = ((min_x < data_[i].x_) ? min_x : (data_[i].x_));
    min_y = ((min_y < data_[i].y_) ? min_y : (data_[i].y_));
    max_x = ((max_x > data_[i].x_) ? max_x : (data_[i].x_));
    max_y = ((max_y > data_[i].y_) ? max_y : (data_[i].y_));
  }
  return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2, (min_y + max_y) / 2}};
}

void strelnikov::Polygon::move(strelnikov::point_t p)
{
  for (size_t i = 0; i < size_; ++i) {
    data_[i].x_ += p.x_;
    data_[i].y_ += p.y_;
  }
  c_ = get_poly_C(data_, size_);
}

void strelnikov::Polygon::move(double x, double y)
{
  strelnikov::Polygon::move({x, y});
}

void strelnikov::Polygon::scale(double k)
{
  for (size_t i = 0; i < size_; ++i) {
    data_[i].x_ *= k;
    data_[i].y_ *= k;
  }
  c_ = get_poly_C(data_, size_);
}

strelnikov::Circle::Circle(point_t cen, double rad) : cen_(cen), rad_(rad)
{}

double strelnikov::Circle::getArea()
{
  return strelnikov::PI * rad_ * rad_;
}

strelnikov::rectangle_t strelnikov::Circle::getFrameRect()
{
  return {(2 * rad_), (2 * rad_), cen_};
}

void strelnikov::Circle::move(strelnikov::point_t p)
{
  cen_ = p;
}

void strelnikov::Circle::move(double x, double y)
{
  strelnikov::Circle::move({cen_.x_ + x, cen_.y_ + y});
}

void strelnikov::Circle::scale(double k)
{
  cen_.x_ *= k;
  cen_.y_ *= k;
}

int main()
{
  strelnikov::IShape** ishps = new strelnikov::IShape*[3];
  strelnikov::Circle* circle = new strelnikov::Circle({3, 3}, 3);
  strelnikov::Rectangle* rect = new strelnikov::Rectangle(4, 4, {0, 0});
  strelnikov::point_t data[] = {
      {3.5f, 2.0f}, {4.76f, 2.90f}, {4.27f, 4.44f}, {2.73f, 4.44f}, {2.24f, 2.90f}};
  strelnikov::Polygon* poly = new strelnikov::Polygon(data, 5);
  ishps[0] = circle;
  ishps[1] = rect;
  ishps[2] = poly;
  for (size_t i = 0; i < 3; ++i) {
    strelnikov::rectangle_t frame = ishps[i]->getFrameRect();
    std::cout << "Фигура до изменения:\n"
              << ishps[i]->getArea() << '\t' << "центр: (" << frame.c_.x_ << ' ' << frame.c_.y_
              << ") ширина: " << frame.width_ << " высота: " << frame.width_ << '\n';
    ishps[i]->move({1, 1});
    ishps[i]->scale(2.5);
    std::cout << "Фигура после изменения:\n"
              << ishps[i]->getArea() << '\t' << "центр: (" << frame.c_.x_ << ';' << frame.c_.y_
              << ") ширина: " << frame.width_ << " высота: " << frame.width_ << '\n';
  }
  return 0;
}