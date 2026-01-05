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
/*
    

    virtual rectangle_t getFrameRect() const = 0;

    virtual void move(const point_t& point) = 0;
    virtual void move(double x, double y) = 0;

    virtual void scale(double coefficient) = 0;
    */
  };

  class Rectangle : public Shape
  {
    double width_;
    double height_;
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
      center_ = pos;
    }

    double getArea() const override
    {
      return width_ * height_;
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
    Rectangle * rect1 = new Rectangle(4, 3, {8, 6});

    std::cout << rect1 -> getArea() << '\n';

    delete rect1;

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Ошибка: " << e.what() << std::endl;
    return 1;
  }
}