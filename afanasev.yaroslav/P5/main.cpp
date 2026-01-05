#include <iostream>

namespace afanasev
{
  struct point_t
  {
    double x;
    double y;
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
    virtual void move(const double dx, const double dy) = 0;

    virtual void scale(const double k) = 0;
  };

  class Rectangle : public Shape
  {
  public:
    double width_;
    double height_;
    point_t pos_;
    point_t center_;

    Rectangle(const double w, const double h, const point_t pos)
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

    void move(const double dx, const double dy) override
    {
      center_.x += dx;
      center_.y += dy;
    }

    void scale(const double k) override
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


  void printShapesInfo()
  {
    std::cout << "f" << '\n';
  }
}



int main()
{
  using afanasev::point_t;
  using afanasev::rectangle_t;
  using afanasev::Rectangle;


  try
  {
    Rectangle * rect1 = new Rectangle(2, 2, {2, 2});
    std::cout << "прямоугольник 2 на 2:" << '\n';
    std::cout << "площадь:" << '\n';
    std::cout << rect1 -> getArea() << '\n';

    std::cout << "положение до увеличения:" << '\n';
    std::cout << rect1 -> getFrameRect().pos.x << '\n';
    std::cout << rect1 -> getFrameRect().pos.y << '\n';

    rect1 -> move(-2, -2);

    rect1 -> scale(2);
    
    std::cout << "размер после увеличения:" << '\n';
    std::cout << rect1 -> height_ << '\n';
    std::cout << rect1 -> width_ << '\n';

    std::cout << "положение после увеличения:" << '\n';
    std::cout << rect1 -> getFrameRect().pos.x << '\n';
    std::cout << rect1 -> getFrameRect().pos.y << '\n';


    delete rect1;

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Ошибка: " << e.what() << std::endl;
    return 1;
  }
}