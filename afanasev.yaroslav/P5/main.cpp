#include <iostream>

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

    virtual void scale(double k) = 0;
  };

  class ScalableShape : public Shape
  {
  public:
    void scale(double k) final override;

  protected:
    virtual void doScale(double k) noexcept = 0;
  };

  class Rectangle final: public ScalableShape
  {
  public:
    Rectangle(double w, double h, const point_t & pos);

    double getArea() const noexcept override;

    rectangle_t getFrameRect() const noexcept override;

    void move(const point_t & point) noexcept override;

    void move(double dx, double dy) noexcept override;

  protected:
    void doScale(double k) noexcept override;

  private:
    double width_;
    double height_;
    point_t pos_;
    point_t center_;
  };

  class Circle final: public ScalableShape
  {
  public:
    Circle(double r, const point_t & pos);

    double getArea() const noexcept override;

    rectangle_t getFrameRect() const noexcept override;

    void move(const point_t & point) noexcept override;

    void move(double dx, double dy) noexcept override;

  protected:
    void doScale(double k) noexcept override;

  private:
    double radius_;
    point_t pos_;
    point_t center_;
  };

  class Rubber final: public ScalableShape
  {
  public:
    Rubber(double r1, double r2, const point_t & pos1, const point_t & pos2);

    double getArea() const noexcept override;

    rectangle_t getFrameRect() const noexcept override;

    void move(const point_t & point) noexcept override;

    void move(double dx, double dy) noexcept override;

  protected:
    void doScale(double k) noexcept override;

  private:
    Circle c_min_;
    Circle c_max_;
    point_t center_;
  };

  void printShapesInfo(const Shape ** shapes, size_t cnt_shapes) noexcept;

  void scaleAllShapes(Shape ** shapes, size_t cnt_shapes);
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

    printShapesInfo(const_cast<const Shape**>(shapes), cnt_shapes);

    scaleAllShapes(shapes, cnt_shapes);

    printShapesInfo(const_cast<const Shape**>(shapes), cnt_shapes);
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

void afanasev::ScalableShape::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("coefficient must be > 0");
  }
  doScale(k);
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
void afanasev::Rectangle::doScale(double k) noexcept
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
void afanasev::Circle::doScale(double k) noexcept
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
void afanasev::Rubber::doScale(double k) noexcept
{
  c_min_.move(center_);
  c_max_.move(center_);

  c_min_.scale(k);
  c_max_.scale(k);
}

void afanasev::printShapesInfo(const Shape ** shapes, size_t cnt_shapes) noexcept
{
  std::cout << "\nСуществующие фигуры:" << '\n';

  double s_all = 0;

  double x_max = 0;
  double x_min = 0;
  double y_max = 0;
  double y_min = 0;

  rectangle_t first_frame = shapes[0]->getFrameRect();
  x_min = first_frame.pos.x - first_frame.width / 2;
  x_max = first_frame.pos.x + first_frame.width / 2;
  y_min = first_frame.pos.y - first_frame.height / 2;
  y_max = first_frame.pos.y + first_frame.height / 2;

  std::cout << "0) S = " << shapes[0]->getArea();
  std::cout << "; Размер ограничивающего прямоугольника: {h = " << first_frame.height;
  std::cout << ", w = " << first_frame.width << '}';
  std::cout << "; Координаты фигуры: {" << first_frame.pos.x;
  std::cout << ", " << first_frame.pos.y << '}';
  std::cout << '\n';

  s_all += shapes[0]->getArea();

  for (size_t i = 1; i < cnt_shapes; i++)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    s_all += (shapes[i]->getArea());

    std::cout << i << ") S = " << shapes[i]->getArea();

    std::cout << "; Размер ограничивающего прямоугольника: {h = " << frame.height;
    std::cout << ", w = " << frame.width << '}';

    std::cout << "; Координаты фигуры: {" << frame.pos.x;
    std::cout << ", " << frame.pos.y << '}';

    std::cout << '\n';

    double left = frame.pos.x - frame.width / 2;
    double right = frame.pos.x + frame.width / 2;
    double bottom = frame.pos.y - frame.height / 2;
    double top = frame.pos.y + frame.height / 2;

    x_min = std::min(left, x_min);
    x_max = std::max(right, x_max);
    y_min = std::min(bottom, y_min);
    y_max = std::max(top, y_max);
  }

  std::cout << "Общая площадь: " << s_all;
  std::cout << '\n';
  std::cout << "Общий ограничивающий: {h = ";
  std::cout << y_max - y_min << ", w = " << x_max - x_min << '}';
  std::cout << '\n';
}

void afanasev::scaleAllShapes(afanasev::Shape ** shapes, size_t cnt_shapes)
{
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

  for (size_t i = 0; i < cnt_shapes; i++)
  {
    shapes[i]->move({x, y});
    shapes[i]->scale(k);
  }

  std::cout << "\nМасштабирован в " << k << " раз, относительно {";
  std::cout << x << ", " << y << "}\n";
}
