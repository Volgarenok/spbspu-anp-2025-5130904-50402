#include <iostream>
#include <limits>

namespace afanasev
{
  struct point_t
  {
    double x;
    double y;
  };

  bool operator==(const point_t & lhs, const point_t & rhs) noexcept;

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  class Shape
  {
  public:
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;

    virtual rectangle_t getFrameRect() const noexcept = 0;

    virtual void move(const point_t & point) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    void checkScale(double k);

    virtual void scale(double k) noexcept = 0;
  };

  class Rectangle final: public Shape
  {
  public:
    Rectangle(double w, double h, const point_t & pos);

    double getArea() const noexcept override;

    rectangle_t getFrameRect() const noexcept override;

    void move(const point_t & point) noexcept override;

    void move(double dx, double dy) noexcept override;

    void scale(double k) noexcept override;

  private:
    double width_;
    double height_;
    point_t pos_;
    point_t center_;
  };

  class Circle final: public Shape
  {
  public:
    Circle(double r, const point_t & pos);

    double getArea() const noexcept override;

    rectangle_t getFrameRect() const noexcept override;

    void move(const point_t & point) noexcept override;

    void move(double dx, double dy) noexcept override;

    void scale(double k) noexcept override;

  private:
    double radius_;
    point_t pos_;
    point_t center_;
  };

  class Rubber final: public Shape
  {
  public:
    Rubber(double r1, double r2, const point_t & pos1, const point_t & pos2);

    double getArea() const noexcept override;

    rectangle_t getFrameRect() const noexcept override;

    void move(const point_t & point) noexcept override;

    void move(double dx, double dy) noexcept override;

    void scale(double k) noexcept override;

  private:
    Circle c_min_;
    Circle c_max_;
    point_t center_;
  };

  rectangle_t calculateOverallFrameRect(const Shape * const * shapes, size_t cnt_shapes) noexcept;

  void printShapesInfo(const Shape * const * shapes, size_t cnt_shapes) noexcept;

  void scaleAllShapes(Shape ** shapes, size_t cnt_shapes, double x, double y, double k) noexcept;
}

int main()
{
  using afanasev::point_t;
  using afanasev::rectangle_t;
  using afanasev::Shape;
  using afanasev::Rectangle;
  using afanasev::Circle;
  using afanasev::Rubber;

  size_t cnt_shapes = 3;
  Shape ** shapes = nullptr;

  try
  {
    shapes = new Shape * [cnt_shapes];
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "bad alloc" << std::endl;
    return 1;
  }

  for (size_t i = 0; i < cnt_shapes; i++)
  {
    shapes[i] = nullptr;
  }

  try
  {
    shapes[0] = new Rectangle(2, 2, {2, 2});
    shapes[1] = new Circle(2, {2, 2});
    shapes[2] = new Rubber(2, 4, {5, 5}, {4, 4});

    printShapesInfo(shapes, cnt_shapes);


    std::cout << "\nМеняем фигуры:" << '\n';
    std::cout << "Введите x, y и коэффицент k через пробел:\n";
    double k = 0;
    double x = 0;
    double y = 0;
    std::cin >> x >> y;
    std::cin >> k;

    if (!std::cin)
    {
      throw std::logic_error("input error");
    }

    scaleAllShapes(shapes, cnt_shapes, x, y, k);

    std::cout << "\nМасштабирован в " << k << " раз, относительно {";
    std::cout << x << ", " << y << "}\n";

    printShapesInfo(shapes, cnt_shapes);
  }
  catch (const std::exception & e)
  {
    std::cerr << "Ошибка: " << e.what() << std::endl;

    for (size_t i = 0; i < cnt_shapes; i++)
    {
      delete shapes[i];
    }
    delete[] shapes;

    return 1;
  }

  for (size_t i = 0; i < cnt_shapes; i++)
  {
    delete shapes[i];
  }
  delete[] shapes;

  return 0;
}

bool afanasev::operator==(const point_t & a, const point_t & b) noexcept
{
  return a.x == b.x && a.y == b.y;
}

void afanasev::Shape::checkScale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("coefficient must be > 0");
  }
  scale(k);
}

afanasev::Rectangle::Rectangle(double w, double h, const point_t & pos):
  width_(w),
  height_(h),
  pos_(pos),
  center_(pos)
{
  if (w <= 0 || h <= 0)
  {
    throw std::invalid_argument("scale must be > 0");
  }
}
double afanasev::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}
afanasev::rectangle_t afanasev::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, pos_};
}
void afanasev::Rectangle::move(const point_t & point) noexcept
{
  center_ = point;
}
void afanasev::Rectangle::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}
void afanasev::Rectangle::scale(double k) noexcept
{
  width_ *= k;
  height_ *= k;

  double vx = pos_.x - center_.x;
  double vy = pos_.y - center_.y;

  pos_.x += -vx + vx * k;
  pos_.y += -vy + vy * k;
}

afanasev::Circle::Circle(double r, const point_t & pos):
  radius_(r),
  pos_(pos),
  center_(pos)
{
  if (r <= 0)
  {
    throw std::invalid_argument("radius must be > 0");
  }
}
double afanasev::Circle::getArea() const noexcept
{
  return radius_ * radius_ * 3.1415;
}
afanasev::rectangle_t afanasev::Circle::getFrameRect() const noexcept
{
  return {radius_ * 2, radius_ * 2, pos_};
}
void afanasev::Circle::move(const point_t & point) noexcept
{
  center_ = point;
}
void afanasev::Circle::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}
void afanasev::Circle::scale(double k) noexcept
{
  radius_ *= k;

  double vx = pos_.x - center_.x;
  double vy = pos_.y - center_.y;

  pos_.x += -vx + vx * k;
  pos_.y += -vy + vy * k;
}

afanasev::Rubber::Rubber(double r1, double r2, const point_t & pos1, const point_t & pos2):
  c_min_(r1, pos1),
  c_max_(r2, pos2),
  center_(pos1)
{
  if (r1 <= 0 || r2 <= 0 || r2 <= r1 || pos1 == pos2)
  {
    throw std::invalid_argument("incorrect input");
  }

  double dx = pos1.x - pos2.x;
  double dy = pos1.y - pos2.y;
  double distance_squared = dx * dx + dy * dy;

  if (distance_squared >= (r2 - r1) * (r2 - r1))
  {
    throw std::invalid_argument("circle is collision");
  }
}
double afanasev::Rubber::getArea() const noexcept
{
  return c_max_.getArea() - c_min_.getArea();
}
afanasev::rectangle_t afanasev::Rubber::getFrameRect() const noexcept
{
  return c_max_.getFrameRect();
}
void afanasev::Rubber::move(const point_t & point) noexcept
{
  center_ = point;
}
void afanasev::Rubber::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}
void afanasev::Rubber::scale(double k) noexcept
{
  c_min_.move(center_);
  c_max_.move(center_);

  c_min_.scale(k);
  c_max_.scale(k);
}

afanasev::rectangle_t afanasev::calculateOverallFrameRect(const Shape * const * shapes,
  size_t cnt_shapes) noexcept
{
  double x_min = std::numeric_limits<double>::max();
  double x_max = std::numeric_limits<double>::lowest();
  double y_min = std::numeric_limits<double>::max();
  double y_max = std::numeric_limits<double>::lowest();

  for (size_t i = 0; i < cnt_shapes; i++) {
    rectangle_t frame = shapes[i]->getFrameRect();
      
    double left = frame.pos.x - frame.width / 2;
    double right = frame.pos.x + frame.width / 2;
    double bottom = frame.pos.y - frame.height / 2;
    double top = frame.pos.y + frame.height / 2;

    x_min = std::min(left, x_min);
    x_max = std::max(right, x_max);
    y_min = std::min(bottom, y_min);
    y_max = std::max(top, y_max);
  }

  double width = x_max - x_min;
  double height = y_max - y_min;
  point_t center = {
    (x_min + x_max) / 2,
    (y_min + y_max) / 2
  };

  return {width, height, center};
}

void afanasev::printShapesInfo(const Shape * const * shapes, size_t cnt_shapes) noexcept
{
  std::cout << "\nСуществующие фигуры:" << '\n';

  double s_all = 0;

  for (size_t i = 0; i < cnt_shapes; i++)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    s_all += (shapes[i]->getArea());

    std::cout << i << ") S = " << shapes[i]->getArea();

    std::cout << "; Размер ограничивающего прямоугольника: {h = " << frame.height;
    std::cout << ", w = " << frame.width << '}';

    std::cout << "; Координаты фигуры: {" << frame.pos.x;
    std::cout << ", " << frame.pos.y << "}\n";
  }

  rectangle_t overall_frame = calculateOverallFrameRect(shapes, cnt_shapes);

  std::cout << "Общая площадь: " << s_all;
  std::cout << '\n';
  std::cout << "Общий ограничивающий: {h = ";
  std::cout << overall_frame.height << ", w = " << overall_frame.width << '}';
  std::cout << '\n';
}

void afanasev::scaleAllShapes(afanasev::Shape ** shapes, size_t cnt_shapes,
  double x, double y, double k) noexcept
{
  shapes[0]->move({x, y});
  shapes[0]->checkScale(k);

  for (size_t i = 1; i < cnt_shapes; i++)
  {
    shapes[i]->move({x, y});
    shapes[i]->scale(k);
  }
}
