#include <iostream>

namespace afanasev
{
  struct point_t
  {
    double x;
    double y;

    bool operator==(const point_t& other) const noexcept;
  };

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

    virtual void move(const point_t& point) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    virtual void scale(double k) = 0;
  };

  class Rectangle final: public Shape
  {
  public:
    Rectangle(double w, double h, const point_t & pos);

    double getArea() const noexcept override;

    rectangle_t getFrameRect() const noexcept override;

    void move(const point_t & point) noexcept override;

    void move(double dx, double dy) noexcept override;

    void scale(double k) override;

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

    void scale(double k) override;

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

    void scale(double k) override;

  private:
    Circle c_min_;
    Circle c_max_;
    point_t center_;
  };

  void printShapesInfo(Shape const * const * const shapes, size_t cnt_shapes)
  {
    std::cout << "Существующие фигуры:" << '\n';

    double s_all = 0;

    bool first_shape_found = false;

    double x_max = 0;
    double x_min = 0;
    double y_max = 0;
    double y_min = 0;

    for (size_t i = 0; i < cnt_shapes; i++)
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

      if (!first_shape_found)
      {
        x_min = left;
        x_max = right;
        y_min = bottom;
        y_max = top;
        first_shape_found = true;
      }
      else
      {
        x_min = (left < x_min) ? left : x_min;
        x_max = (right > x_max) ? right : x_max;
        y_min = (bottom < y_min) ? bottom : y_min;
        y_max = (top > y_max) ? top : y_max;
      }
    }

    std::cout << "Общая площадь: " << s_all;
    std::cout << '\n';
    std::cout << "Общий ограничивающий: {h = " << y_max - y_min << ", w = " << x_max - x_min << '}';
    std::cout << '\n';
  }
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
  catch (const std::bad_alloc&)
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

    int n = 0;
    size_t shape = 0;

    while (n != -1)
    {
      printShapesInfo(shapes, cnt_shapes);

      std::cout << "\nВведите номер фигуры:\n";

      std::cin >> shape;
      if (!std::cin)
      {
        throw std::logic_error("input error");
      }
      if (shape >= cnt_shapes)
      {
        std::cout << "Нет фигуры с таким номером" << '\n';
      }
      else
      {
        std::cout << "\nДействия:\n";
        std::cout << "0) переместить центр масштабирования на x и y (move(x, y)):" << '\n';
        std::cout << "1) сместить центр масштабирования на dx и dy (move(dx, dy)):" << '\n';
        std::cout << "2) масштабировать на коофицент k относительно центра масштабирования (scale(k)):" << '\n';
        std::cout << "3) УЛЬТРА МЕГА ФУНКЦИЯ КОМБО 3000!!!! масштабировать на коофицент k относительно x и y:" << '\n';
        std::cout << "введите номер нужного действия" << '\n';

        std::cin >> n;
        if (!std::cin)
        {
          throw std::logic_error("input error");
        }

        if (!n)
        {
          std::cout << "\nВведите x и y точки для перемещения центра:\n";
          double x = 0;
          double y = 0;
          std::cin >> x >> y;
          if (!std::cin)
          {
            throw std::logic_error("input error");
          }
          shapes[shape]->move({x, y});
          std::cout << "Центр перемещён в {" << x << ", " << y << "}\n";
        }
        else if (n == 1)
        {
          std::cout << "\nВведите dx и dy точки для смещения центра:\n";
          double x = 0;
          double y = 0;
          std::cin >> x >> y;
          if (!std::cin)
          {
            throw std::logic_error("input error");
          }
          shapes[shape]->move(x, y);
          std::cout << "Центр смещён на {" << x << ", " << y << "}\n";
        }
        else if (n == 2)
        {
          std::cout << "\nВведите k для масштабирования относительно центра:\n";
          double k = 0;
          std::cin >> k;
          if (!std::cin)
          {
            throw std::logic_error("input error");
          }
          shapes[shape]->scale(k);
          std::cout << "Масштаб изменён в " << k << " раз" << '\n';
        }
        else if (n == 3)
        {
          std::cout << "\nВведите k для масштабирования относительно центра:\n";
          double k = 0;
          std::cin >> k;
          if (!std::cin)
          {
            throw std::logic_error("input error");
          }

          std::cout << "\nВведите x и y центра для масштабирования:\n";
          double x = 0;
          double y = 0;
          std::cin >> x >> y;
          if (!std::cin)
          {
            throw std::logic_error("input error");
          }

          shapes[shape]->move({x, y});
          shapes[shape]->scale(k);

          std::cout << "Масштабирован в " << k << " раз, относительно {" << x << ", " << y << "}\n";
        }
        else
        {
          std::cout << "несуществующее действие" << '\n';
        }
      }

      std::cout << '\n';
    }
  }
  catch (const std::exception& e)
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


// point_t
bool afanasev::point_t::operator==(const point_t& other) const noexcept
{
  return x == other.x && y == other.y;
}


// Rectangle
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

void afanasev::Rectangle::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("coefficient must be > 0");
  }

  width_ *= k;
  height_ *= k;

  double vx = pos_.x - center_.x;
  double vy = pos_.y - center_.y;

  pos_.x += -vx + vx * k;
  pos_.y += -vy + vy * k;
}


// Circle
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

void afanasev::Circle::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("coefficient must be > 0");
  }

  radius_ *= k;

  double vx = pos_.x - center_.x;
  double vy = pos_.y - center_.y;

  pos_.x += -vx + vx * k;
  pos_.y += -vy + vy * k;
}


// Rubber
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

void afanasev::Rubber::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("coefficient must be > 0");
  }

  c_min_.move(center_);
  c_max_.move(center_);

  c_min_.scale(k);
  c_max_.scale(k);
}
