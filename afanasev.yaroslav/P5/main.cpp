#include <iostream>

namespace afanasev
{
  struct point_t
  {
    double x;
    double y;

    bool operator==(const point_t& other) const
    {
      return x == other.x && y == other.y;
    }
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

    virtual double getArea() const = 0;

    virtual rectangle_t getFrameRect() const = 0;

    virtual void move(const point_t& point) = 0;
    virtual void move(double dx, double dy) = 0;

    virtual void scale(double k) = 0;
  };

  class Rectangle : public Shape
  {
    double width_;
    double height_;
    point_t pos_;
    point_t center_;

  public:

    Rectangle(double w, double h, point_t pos)
    {
      if (w <= 0 || h <= 0)
      {
        throw std::invalid_argument("scale must be > 0");
      }
      width_ = w;
      height_ = h;
      // центр для масштабирования
      center_ = pos;
      // центр фигуры
      pos_ = pos;
    }

    double getArea() const override
    {
      return width_ * height_;
    }

    rectangle_t getFrameRect() const override
    {
      return {width_, height_, pos_};
    }

    void move(const point_t& point) override
    {
      center_ = point;
    }

    void move(double dx, double dy) override
    {
      center_.x += dx;
      center_.y += dy;
    }

    void scale(double k) override
    {
      if (k <= 0)
      {
        throw std::invalid_argument("coefficient must be > 0");
      }

      width_ *= k;
      height_ *= k;

      // Расстояние от "центра" до центра фигуры
      double vx = pos_.x - center_.x;
      double vy = pos_.y - center_.y;

      pos_.x += -vx + vx * k;
      pos_.y += -vy + vy * k;
    }
  };

  class Circle : public Shape
  {
    double radius;
    point_t pos_;
    point_t center_;

  public:

    Circle(double r, point_t pos)
    {
      if (r <= 0)
      {
        throw std::invalid_argument("radius must be > 0");
      }
      radius = r;
      // центр для масштабирования
      center_ = pos;
      // центр фигуры
      pos_ = pos;
    }

    double getArea() const override
    {
      return radius * radius * 3.1415;
    }

    rectangle_t getFrameRect() const override
    {
      return {radius * 2, radius * 2, pos_};
    }

    void move(const point_t& point) override
    {
      center_ = point;
    }

    void move(double dx, double dy) override
    {
      center_.x += dx;
      center_.y += dy;
    }

    void scale(double k) override
    {
      if (k <= 0)
      {
        throw std::invalid_argument("coefficient must be > 0");
      }

      radius *= k;

      // Расстояние от "центра" до центра фигуры
      double vx = pos_.x - center_.x;
      double vy = pos_.y - center_.y;

      pos_.x += -vx + vx * k;
      pos_.y += -vy + vy * k;
    }
  };

  class Rubber : public Shape
  {
    // Внутренний меньший
    double r_1;
    // Внешний больший
    double r_2;

    // Центр меньшего круга
    point_t pos_1;
    // Центр большего круга
    point_t pos_2;

    // Центр масштабирования
    point_t center_;

  public:

    Rubber(double r1, double r2, point_t pos1, point_t pos2)
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

      r_1 = r1;
      r_2 = r2;

      // центр для масштабирования
      center_ = pos1;
      // центр фигуры
      pos_1 = pos1;
      // центр большего круга
      pos_2 = pos2;
    }

    double getArea() const override
    {
      return (r_2 * r_2 - r_1 * r_1) * 3.1415;
    }

    rectangle_t getFrameRect() const override
    {
      return {r_2 * 2, r_2 * 2, pos_2};
    }

    void move(const point_t& point) override
    {
      center_ = point;
    }

    void move(double dx, double dy) override
    {
      center_.x += dx;
      center_.y += dy;
    }

    void scale(double k) override
    {
      if (k <= 0)
      {
        throw std::invalid_argument("coefficient must be > 0");
      }

      r_1 *= k;

      // Расстояние от "центра" до центра фигуры
      double vx = pos_1.x - center_.x;
      double vy = pos_1.y - center_.y;

      pos_1.x += -vx + vx * k;
      pos_1.y += -vy + vy * k;
    }
  };

  void printShapesInfo(Shape ** const shapes, size_t cnt_shapes)
  {
    std::cout << "Существующие фигуры:" << '\n';

    double s_all = 0;

    for (size_t i = 0; i < cnt_shapes; i++)
    {
      // площадь
      s_all += (shapes[i] -> getArea());

      std::cout << i << ") S = " << shapes[i] -> getArea();

      // Размер
      std::cout << "; Размер ограничивающего прямоугольника: {h = " << shapes[i] -> getFrameRect().height;
      std::cout << ", w = " << shapes[i] -> getFrameRect().width << '}';

      // Положение фигуры
      std::cout << "; Координаты фигуры: {" << shapes[i] -> getFrameRect().pos.x;
      std::cout << ", " << shapes[i] -> getFrameRect().pos.y << '}';

      std::cout << '\n';
    }
    
    std::cout << "Общая площадь: " << s_all;
    
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
          shapes[shape] -> move({x, y});
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
          shapes[shape] -> move(x, y);
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
          shapes[shape] -> scale(k);
          std::cout << "Масштаб изменён в " << k << " раз" << '\n';
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