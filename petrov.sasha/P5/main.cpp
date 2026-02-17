#include <iostream>

namespace petrov
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  void printRectangle(const rectangle_t& rect) noexcept;

  class Shape
  {
  public:
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& pos) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    void scale(double k);
    void untestedScale(double k) noexcept;

  private:
    virtual void doScale(double k) noexcept = 0;
  };

  class Rectangle final: public Shape
  {
  public:
    Rectangle(const point_t& center, double width, double height);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& pos) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t center_;
    double width_;
    double height_;

    void doScale(double k) noexcept override;
  };

  class Diamond final: public Shape
  {
  public:
    Diamond(const point_t& center, double diag_h, double diag_v);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& pos) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t center_;
    double diag_h_;
    double diag_v_;
    void doScale(double k) noexcept override;
  };

  class ComplexQuad final: public Shape
  {
  public:
    ComplexQuad(const point_t& p1, const point_t& p2, const point_t& p3, const point_t& p4);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& pos) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t points_[4];
    point_t center_;

    void doScale(double k) noexcept override;
    point_t getCenter() const noexcept;
    point_t computeCenter(const point_t (&points)[4]) noexcept;
    double crossProduct(const point_t& o, const point_t& a, const point_t& b) const noexcept;
  };

  void scaleAll(Shape** shapes, size_t n, const point_t& center, double k);
  double totalArea(const Shape* const* shapes, size_t n) noexcept;
  rectangle_t overallFrame(const Shape* const* shapes, size_t n) noexcept;
  void printInfo(const Shape* const* shapes, size_t n, const char* title) noexcept;
}

int main()
{
  try
  {
    using namespace petrov;

    Rectangle rect({2.0, 3.0}, 4.0, 5.0);
    Diamond diamond({5.0, 5.0}, 6.0, 8.0);
    ComplexQuad quad({0.0, 0.0}, {4.0, 4.0}, {4.0, 0.0}, {0.0, 4.0});

    const size_t n = 3;
    Shape* shapes[n] = {std::addressof(rect), std::addressof(diamond), std::addressof(quad)};

    printInfo(shapes, n, "ДО МАСШТАБИРОВАНИЯ");

    point_t scaleCenter = {0.0, 0.0};
    double scaleCoeff = 0.0;

    std::cout << "Введите центр масштабирования (x y): ";
    if (!(std::cin >> scaleCenter.x >> scaleCenter.y))
    {
      std::cerr << "Неверный ввод координат центра\n";
      return 1;
    }

    std::cout << "Введите коэффициент масштабирования (положительное число): ";
    if (!(std::cin >> scaleCoeff))
    {
      std::cerr << "Неверный ввод коэффициента масштабирования\n";
      return 1;
    }
    scaleAll(shapes, n, scaleCenter, scaleCoeff);
    printInfo(shapes, n, "ПОСЛЕ МАСШТАБИРОВАНИЯ");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
  return 0;
}

void petrov::Shape::scale(double k)
{
  if (k <= 0.0)
  {
    throw std::invalid_argument("Коэффициент масштабирования должен быть положительным");
  }
  doScale(k);
}

void petrov::Shape::untestedScale(double k) noexcept
{
  doScale(k);
}

petrov::Rectangle::Rectangle(const point_t& center, double width, double height):
  center_(center),
  width_(width),
  height_(height)
{
  if (width_ <= 0.0 || height_ <= 0.0)
  {
    throw std::invalid_argument("Ширина и высота должны быть положительными");
  }
  if (width_ > 10000.0 || height_ > 10000.0)
  {
    throw std::logic_error("Размеры прямоугольника слишком велики");
  }
}

double petrov::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

petrov::rectangle_t petrov::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, center_};
}

void petrov::Rectangle::move(const point_t& pos) noexcept
{
  center_ = pos;
}

void petrov::Rectangle::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}

void petrov::Rectangle::doScale(double k) noexcept
{
  width_ *= k;
  height_ *= k;
}

petrov::Diamond::Diamond(const point_t& center, double diag_h, double diag_v):
  center_(center),
  diag_h_(diag_h),
  diag_v_(diag_v)
{
  if (diag_h_ <= 0.0 || diag_v_ <= 0.0)
  {
    throw std::invalid_argument("Диагонали должны быть положительными");
  }
  if (diag_h_ > 10000.0 || diag_v_ > 10000.0)
  {
    throw std::logic_error("Диагонали ромба слишком велики");
  }
}

double petrov::Diamond::getArea() const noexcept
{
  return (diag_h_ * diag_v_) / 2.0;
}

petrov::rectangle_t petrov::Diamond::getFrameRect() const noexcept
{
  return {diag_h_, diag_v_, center_};
}

void petrov::Diamond::move(const point_t& pos) noexcept
{
  center_ = pos;
}

void petrov::Diamond::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}

void petrov::Diamond::doScale(double k) noexcept
{
  diag_h_ *= k;
  diag_v_ *= k;
}

petrov::point_t petrov::ComplexQuad::computeCenter(const point_t (&points)[4]) noexcept
{
  double x1 = points[0].x, y1 = points[0].y;
  double x2 = points[2].x, y2 = points[2].y;
  double x3 = points[1].x, y3 = points[1].y;
  double x4 = points[3].x, y4 = points[3].y;

  double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  if (std::abs(denom) < 1e-12)
  {
    return {
      (x1 + x2 + x3 + x4) / 4.0,
      (y1 + y2 + y3 + y4) / 4.0
    };
  }
  else
  {
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    return {
      x1 + t * (x2 - x1),
      y1 + t * (y2 - y1)
    };
  }
}

petrov::ComplexQuad::ComplexQuad(const point_t& p1, const point_t& p2, const point_t& p3, const point_t& p4):
  points_{p1, p2, p3, p4},
  center_(computeCenter(points_))
{
  double area = getArea();
  if (area < 1e-12)
  {
    throw std::logic_error("Четырехугольник вырожденный или имеет нулевую площадь");
  }
}

double petrov::ComplexQuad::crossProduct(const point_t& o, const point_t& a, const point_t& b) const noexcept
{
  return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

double petrov::ComplexQuad::getArea() const noexcept
{
  double area1 = std::abs(crossProduct(points_[0], points_[1], points_[2])) / 2.0;
  double area2 = std::abs(crossProduct(points_[0], points_[2], points_[3])) / 2.0;
  return area1 + area2;
}

petrov::rectangle_t petrov::ComplexQuad::getFrameRect() const noexcept
{
  double min_x = points_[0].x;
  double max_x = points_[0].x;
  double min_y = points_[0].y;
  double max_y = points_[0].y;

  for (size_t i = 1; i < 4; ++i)
  {
    min_x = std::min(min_x, points_[i].x);
    max_x = std::max(max_x, points_[i].x);
    min_y = std::min(min_y, points_[i].y);
    max_y = std::max(max_y, points_[i].y);
  }

  double width = max_x - min_x;
  double height = max_y - min_y;
  point_t pos = {min_x + width / 2.0, min_y + height / 2.0};
  return {width, height, pos};
}

void petrov::ComplexQuad::move(const point_t& pos) noexcept
{
  double dx = pos.x - center_.x;
  double dy = pos.y - center_.y;
  move(dx, dy);
}

petrov::point_t petrov::ComplexQuad::getCenter() const noexcept
{
  return center_;
}

void petrov::ComplexQuad::move(double dx, double dy) noexcept
{
  for (size_t i = 0; i < 4; ++i)
  {
    points_[i].x += dx;
    points_[i].y += dy;
  }
  center_.x += dx;
  center_.y += dy;
}

void petrov::ComplexQuad::doScale(double k) noexcept
{
  for (size_t i = 0; i < 4; ++i)
  {
    points_[i].x = center_.x + (points_[i].x - center_.x) * k;
    points_[i].y = center_.y + (points_[i].y - center_.y) * k;
  }
}

void petrov::scaleAll(Shape** shapes, size_t n, const point_t& center, double k)
{
  if (k <= 0.0)
  {
    throw std::invalid_argument("Коэффициент масштабирования должен быть положительным");
  }
  for (size_t i = 0; i < n; ++i)
  {
    shapes[i]->move(-center.x, -center.y);
    shapes[i]->untestedScale(k);
    shapes[i]->move(center.x, center.y);
  }
}

double petrov::totalArea(const Shape* const* shapes, size_t n) noexcept
{
  double total = 0.0;
  for (size_t i = 0; i < n; ++i)
  {
    total += shapes[i]->getArea();
  }
  return total;
}

petrov::rectangle_t petrov::overallFrame(const Shape* const* shapes, size_t n) noexcept
{
  if (n == 0)
  {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  rectangle_t frame = shapes[0]->getFrameRect();
  double left = frame.pos.x - frame.width / 2.0;
  double right = frame.pos.x + frame.width / 2.0;
  double bottom = frame.pos.y - frame.height / 2.0;
  double top = frame.pos.y + frame.height / 2.0;

  for (size_t i = 1; i < n; ++i)
  {
    frame = shapes[i]->getFrameRect();
    double l = frame.pos.x - frame.width / 2.0;
    double r = frame.pos.x + frame.width / 2.0;
    double b = frame.pos.y - frame.height / 2.0;
    double t = frame.pos.y + frame.height / 2.0;

    left = std::min(left, l);
    right = std::max(right, r);
    bottom = std::min(bottom, b);
    top = std::max(top, t);
  }

  double width = right - left;
  double height = top - bottom;
  point_t pos = {left + width / 2.0, bottom + height / 2.0};
  return {width, height, pos};
}

void petrov::printInfo(const Shape* const* shapes, size_t n, const char* title) noexcept
{
  std::cout << title << '\n';
  for (size_t i = 0; i < n; ++i)
  {
    std::cout << "Фигура " << (i + 1) << ":\n";
    std::cout << "  Площадь: " << shapes[i]->getArea() << '\n';
    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << "  Ограничивающий прямоугольник: ";
    printRectangle(frame);
    std::cout << '\n';
  }
  std::cout << "Общая площадь: " << totalArea(shapes, n) << '\n';
  rectangle_t overall = overallFrame(shapes, n);
  std::cout << "Общий ограничивающий прямоугольник: ";
  printRectangle(overall);
  std::cout << '\n';
}

void petrov::printRectangle(const rectangle_t& rect) noexcept
{
  std::cout << "центр(" << rect.pos.x << ", " << rect.pos.y;
  std::cout << "), ширина: " << rect.width << ", высота: " << rect.height;
}
