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
    Circle c_min;
    Circle c_max;

    // Центр масштабирования
    point_t center_;

  public:

    Rubber(double r1, double r2, point_t pos1, point_t pos2):
    c_min(r1, pos1),
    c_max(r2, pos2)
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

      // центр для масштабирования
      center_ = pos1;
    }

    double getArea() const override
    {
      return c_max.getArea() - c_min.getArea();
    }

    rectangle_t getFrameRect() const override
    {
      return c_max.getFrameRect();
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

      c_min.move(center_);
      c_max.move(center_);

      c_min.scale(k);
      c_max.scale(k);
    }
  };

  void printShapesInfo(Shape ** const shapes, size_t cnt_shapes)
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
      rectangle_t frame = shapes[i] -> getFrameRect();

      // площадь
      s_all += (shapes[i] -> getArea());

      std::cout << i << ") S = " << shapes[i] -> getArea();

      // Размер
      std::cout << "; Размер ограничивающего прямоугольника: {h = " << frame.height;
      std::cout << ", w = " << frame.width << '}';

      // Положение фигуры
      std::cout << "; Координаты фигуры: {" << frame.pos.x;
      std::cout << ", " << frame.pos.y << '}';

      std::cout << '\n';

      // Пересчитываем общий ограничивающий
      double left = frame.pos.x - frame.width / 2;
      double right = frame.pos.x + frame.width / 2;
      double bottom = frame.pos.y - frame.height / 2;
      double top = frame.pos.y + frame.height / 2;

      // Если это первая найденная фигура, инициализируем границы
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
        // Обновляем границы общего ограничивающего прямоугольника
        if (left < x_min)
        {
          x_min = left;
        }
        if (right > x_max)
        {
          x_max = right;
        }
        if (bottom < y_min)
        {
          y_min = bottom;
        }
        if (top > y_max)
        {
        y_max = top;
        }
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