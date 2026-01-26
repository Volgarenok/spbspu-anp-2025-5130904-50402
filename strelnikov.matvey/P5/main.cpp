#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace strelnikov {
  constexpr double PI = 3.14;
  struct point_t {
    double x, y;
  };
  struct rectangle_t {
    double width, height;
    point_t c;
  };
  class Shape {
  public:
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t) noexcept = 0;
    virtual void move(double, double) noexcept = 0;
    virtual void scale(double) noexcept = 0;
  };
  class Rectangle final: public Shape {
  public:
    Rectangle(double, double, point_t);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t) noexcept override;
    void move(double, double) noexcept override;
    void scale(double) noexcept override;

  private:
    rectangle_t rec_;
  };

  class Polygon final: public Shape {
  public:
    Polygon(point_t*, size_t);
    Polygon(const Polygon&);
    Polygon(Polygon&&) noexcept;
    ~Polygon() override;
    Polygon& operator=(const Polygon&);
    Polygon& operator=(Polygon&&) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t) noexcept override;
    void move(double, double) noexcept override;
    void scale(double) noexcept override;

  private:
    point_t* data_;
    size_t size_;
    point_t cen_;
  };

  class Circle final: public Shape {
  public:
    Circle(point_t, double);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t) noexcept override;
    void move(double, double) noexcept override;
    void scale(double) noexcept override;

  private:
    point_t cen_;
    double rad_;
  };

  double getTrPolyS(point_t a, point_t b, point_t c);
  point_t getTrPolyC(point_t a, point_t b, point_t c);
  point_t getPolyC(const point_t* data, size_t k);
  void scaleAll(Shape* const* ishps, size_t size, point_t move, double k);
  rectangle_t computeGlobalFrameRect(const Shape* const* ishps, size_t size);
  double computeTotalArea(const Shape* const* ishps, size_t size);
  void printRectangle(const rectangle_t rect);
  void printShapes(const Shape* const* ishps, size_t size);
};

strelnikov::Rectangle::Rectangle(double wdth, double hght, point_t cntr):
  rec_{wdth, hght, cntr}
{}

double strelnikov::Rectangle::getArea() const noexcept
{
  return rec_.height * rec_.width;
}

strelnikov::rectangle_t strelnikov::Rectangle::getFrameRect() const noexcept
{
  return rec_;
}

void strelnikov::Rectangle::move(point_t p) noexcept
{
  rec_.c = p;
}

void strelnikov::Rectangle::move(double x, double y) noexcept
{
  move({rec_.c.x + x, rec_.c.y + y});
}

void strelnikov::Rectangle::scale(double k) noexcept
{
  rec_.c.x *= k;
  rec_.c.y *= k;
  rec_.height *= k;
  rec_.width *= k;
}

double strelnikov::getTrPolyS(point_t a, point_t b, point_t c)
{
  double res = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
  if (res < 0) {
    res *= -1;
  }
  res *= 0.5;
  return res;
}

strelnikov::point_t strelnikov::getTrPolyC(point_t a, point_t b, point_t c)
{
  double x = (a.x + b.x + c.x) / 3.0;
  double y = (a.y + b.y + c.y) / 3.0;
  return {x, y};
}

strelnikov::point_t strelnikov::getPolyC(const point_t* data, size_t k)
{
  double sum_s = 0;
  double sum_x = 0;
  double sum_y = 0;
  for (size_t i = 1; i < (k - 1); ++i) {
    double cur_sum = getTrPolyS(data[i], data[i + 1], data[0]);
    sum_s += cur_sum;
    sum_x += ((getTrPolyC(data[i], data[i + 1], data[0])).x * cur_sum);
    sum_y += ((getTrPolyC(data[i], data[i + 1], data[0])).y * cur_sum);
  }

  if (sum_s == 0.0) {
    throw std::logic_error("Bad poly");
  }
  return {(sum_x / sum_s), (sum_y / sum_s)};
}

strelnikov::Polygon::Polygon(point_t* data, size_t size):
  data_(new point_t[size]),
  cen_(getPolyC(data_, size)),
  size_(size)
{
  if (size < 3) {
    delete[] data_;
    throw std::logic_error("Bad poly");
  }
  for (size_t i = 0; i < size; ++i) {
    data_[i] = data[i];
  }
}

strelnikov::Polygon::Polygon(const Polygon& p):
  data_(new point_t[p.size_]),
  size_(p.size_),
  cen_{p.cen_}
{
  if (size_ < 3) {
    delete[] data_;
    throw std::logic_error("Bad poly while in copy constructor");
  }
  for (size_t i = 0; i < size_; ++i) {
    data_[i] = p.data_[i];
  }
}

strelnikov::Polygon::Polygon(Polygon&& p) noexcept:
  data_(p.data_),
  size_(p.size_),
  cen_(p.cen_)
{
  p.data_ = nullptr;
  p.size_ = 0;
  p.cen_ = {0, 0};
}

strelnikov::Polygon::~Polygon()
{
  delete[] data_;
}

strelnikov::Polygon& strelnikov::Polygon::operator=(const Polygon& p)
{
  if (this == &p) {
    return *this;
  }
  point_t* tmp_data = nullptr;
  try {
    tmp_data = new point_t[p.size_];
  } catch (const std::bad_alloc &e) {
    return *this;
  }
  delete[] data_;
  data_ = tmp_data;
  size_ = p.size_;
  cen_ = p.cen_;
  for (size_t i = 0; i < size_; ++i) {
    data_[i] = p.data_[i];
  }
  return *this;
}

strelnikov::Polygon& strelnikov::Polygon::operator=(Polygon&& p) noexcept
{
  if (this == &p) {
    return *this;
  }
  delete[] data_;
  data_ = p.data_;
  size_ = p.size_;
  cen_ = p.cen_;
  p.data_ = nullptr;
  p.size_ = 0;
  p.cen_ = {0, 0};
  return *this;
}

double strelnikov::Polygon::getArea() const noexcept
{
  double res = 0;
  for (size_t i = 1; i < (size_ - 1); ++i) {
    res += getTrPolyS(data_[i], data_[i + 1], data_[0]);
  }
  return res;
}

strelnikov::rectangle_t strelnikov::Polygon::getFrameRect() const noexcept
{
  double min_x = data_[0].x;
  double min_y = data_[0].y;
  double maxy = data_[0].y;
  double maxx = data_[0].x;
  for (size_t i = 1; i < size_; ++i) {
    min_x = ((min_x < data_[i].x) ? min_x : (data_[i].x));
    min_y = ((min_y < data_[i].y) ? min_y : (data_[i].y));
    maxx = ((maxx > data_[i].x) ? maxx : (data_[i].x));
    maxy = ((maxy > data_[i].y) ? maxy : (data_[i].y));
  }
  return {maxx - min_x, maxy - min_y, {(min_x + maxx) / 2, (min_y + maxy) / 2}};
}

void strelnikov::Polygon::move(point_t p) noexcept
{
  point_t d = {p.x - cen_.x, p.y - cen_.y};
  for (size_t i = 0; i < size_; ++i) {
    data_[i].x += d.x;
    data_[i].y += d.y;
  }
  cen_ = p;
}

void strelnikov::Polygon::move(double x, double y) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    data_[i].x += x;
    data_[i].y += y;
  }
  cen_.x += x;
  cen_.y += y;
}

void strelnikov::Polygon::scale(double k) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    data_[i].x *= k;
    data_[i].y *= k;
  }
  cen_ = getPolyC(data_, size_);
}

strelnikov::Circle::Circle(point_t cen, double rad):
  cen_(cen),
  rad_(rad)
{}

double strelnikov::Circle::getArea() const noexcept
{
  return strelnikov::PI * rad_ * rad_;
}

strelnikov::rectangle_t strelnikov::Circle::getFrameRect() const noexcept
{
  return {(2 * rad_), (2 * rad_), cen_};
}

void strelnikov::Circle::move(point_t p) noexcept
{
  cen_ = p;
}

void strelnikov::Circle::move(double x, double y) noexcept
{
  move({cen_.x + x, cen_.y + y});
}

void strelnikov::Circle::scale(double k) noexcept
{
  cen_.x *= k;
  cen_.y *= k;
  rad_ *= k;
}

void strelnikov::scaleAll(Shape* const* ishps, size_t size, point_t move, double k)
{
  for (size_t i = 0; i < size; ++i) {
    ishps[i]->move(move);
    ishps[i]->scale(k);
  }
}

strelnikov::rectangle_t strelnikov::computeGlobalFrameRect(const Shape* const* ishps,
  const size_t size)
{
  rectangle_t first = ishps[0]->getFrameRect();
  double min_x = first.c.x - first.width / 2.0;
  double max_x = first.c.x + first.width / 2.0;
  double min_y = first.c.y - first.height / 2.0;
  double max_y = first.c.y + first.height / 2.0;

  for (size_t i = 1; i < size; ++i) {
    rectangle_t frame = ishps[i]->getFrameRect();
    double left = frame.c.x - frame.width / 2.0;
    double right = frame.c.x + frame.width / 2.0;
    double bottom = frame.c.y - frame.height / 2.0;
    double top = frame.c.y + frame.height / 2.0;

    min_x = std::min(min_x, left);
    max_x = std::max(max_x, right);
    min_y = std::min(min_y, bottom);
    max_y = std::max(max_y, top);
  }

  double width = max_x - min_x;
  double height = max_y - min_y;
  point_t center = {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0};

  return {width, height, center};
}

double strelnikov::computeTotalArea(const Shape* const* ishps, size_t size)
{
  double res = 0.0;
  for (size_t i = 0; i < size; ++i) {
    res += ishps[i]->getArea();
  }
  return res;
}

void strelnikov::printRectangle(const rectangle_t rect)
{
  std::cout << "центр=(" << rect.c.x << ", " << rect.c.y
    << ") ширина=" << rect.width << " высота=" << rect.height;
}

void strelnikov::printShapes(const Shape* const* ishps, size_t size)
{
  double area = computeTotalArea(ishps, size);
  rectangle_t total_frame = computeGlobalFrameRect(ishps, size);
  
  for (size_t i = 0; i < size; ++i) {
    std::cout << ishps[i]->getArea() << '\t';
    printRectangle(ishps[i]->getFrameRect());
    std::cout << '\n';
  }
  
  std::cout << "площадь всех фигур: " << area << '\n';
  std::cout << "Общая рамка: ";
  printRectangle(total_frame);
  std::cout << '\n';
}

int main()
{
  const int polyDataSize = 5;
  const int shapeSize = 3;
  strelnikov::point_t data[polyDataSize] = {
    {3.5, 2.0},
    {4.76, 2.90},
    {4.27, 4.44},
    {2.73, 4.44},
    {2.24, 2.90}
  };
  strelnikov::Shape** ishps = nullptr;
  strelnikov::Circle* circle = nullptr;
  strelnikov::Rectangle* rect = nullptr;
  strelnikov::Polygon* poly = nullptr;
  try {
    ishps = new strelnikov::Shape*[shapeSize];
    circle = new strelnikov::Circle({3, 3}, 3);
    rect = new strelnikov::Rectangle(4, 4, {0, 0});
    poly = new strelnikov::Polygon(data, polyDataSize);
  } catch (const std::bad_alloc &e) {
    delete circle;
    delete rect;
    delete poly;
    delete[] ishps;
    return 1;
  } catch (const std::logic_error &e) {
    delete circle;
    delete rect;
    delete poly;
    delete[] ishps;
    return 2;
  }
  ishps[0] = circle;
  ishps[1] = rect;
  ishps[2] = poly;

  std::cout << "Фигуры до изменения: \n";
  strelnikov::printShapes(ishps, 3);

  double x, y;
  std::cout << "Введите точку от которой нужно масштабироваться:\n";
  std::cin >> x >> y;
  if (!std::cin) {
    return 3;
  }
  strelnikov::point_t move{x, y};

  double k;
  std::cout << "Введите коэф. масштабирования:\n";
  std::cin >> k;
  if (!std::cin || k <= 0){
    for (size_t i = 0; i < shapeSize; ++i) {
      delete ishps[i];
    }
    delete[] ishps;
  }

  try {
    strelnikov::scaleAll(ishps, shapeSize, {x, y}, k);
  } catch (const std::logic_error &e) {
    for (size_t i = 0; i < shapeSize; ++i) {
      delete ishps[i];
    }
    delete[] ishps;
    return 4;
  }
  std::cout << "Фигуры после изменения: \n";
  strelnikov::printShapes(ishps, 3);
  for (size_t i = 0; i < shapeSize; ++i) {
    delete ishps[i];
  }
  delete[] ishps;
  return 0;
}
