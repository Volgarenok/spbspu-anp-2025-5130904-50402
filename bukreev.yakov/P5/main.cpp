#include <iostream>

namespace bukreev
{
  struct point_t
  {
    double x, y;
  };
  struct rectangle_t
  {
    point_t pos;
    double width, height;
  };

  struct Shape
  {
    virtual ~Shape() noexcept = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t newPos) noexcept = 0;
    virtual void move(double dX, double dY) noexcept = 0;
    void scaleWithCheck(double k);
    void scaleNoCheck(double k) noexcept;

  protected:
    virtual void scale(double k) noexcept = 0;
  };

  struct Rectangle final: Shape
  {
    Rectangle(point_t center, double width, double height) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t newPos) noexcept override;
    void move(double dX, double dY) noexcept override;

  private:
    void scale(double k) noexcept override;

  private:
    point_t m_Center;
    double m_Width, m_Height;
  };

  struct Xquare final: Shape
  {
    Xquare(point_t center, double diagWidth) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t newPos) noexcept override;
    void move(double dX, double dY) noexcept override;

  private:
    void scale(double k) noexcept override;

  private:
    point_t m_Center;
    double m_DiagWidth;
  };

  struct Square final: Shape
  {
    Square(point_t center, double width) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t newPos) noexcept override;
    void move(double dX, double dY) noexcept override;

  private:
    void scale(double k) noexcept override;

  private:
    point_t m_Center;
    double m_Width;
  };

  void scaleShapes(Shape* const* shapes, size_t shapeCount, point_t base, double k);
  void printShapes(std::ostream& out, const Shape* const* shapes, size_t shapeCount);
  rectangle_t getFrameRect(const Shape* const* shapes, size_t shapeCount);
  void printRectangle(std::ostream& out, rectangle_t rect);
}

int main()
{
  using namespace bukreev;

  Rectangle rect({0, 0}, 100, 80);
  Xquare xqr({20, 30}, 48);
  Square sqr({-7, 14}, 20);

  Shape* shapes[3];
  shapes[0] = std::addressof(rect);
  shapes[1] = std::addressof(xqr);
  shapes[2] = std::addressof(sqr);

  point_t base = {};
  double k = 1;

  std::cout << "Enter scaling base: ";
  std::cin >> base.x >> base.y;
  if (!std::cin)
  {
    std::cerr << "Invalid scaling base.\n";
    return 1;
  }

  std::cout << "Enter scaling rate: ";
  std::cin >> k;
  if (!std::cin)
  {
    std::cerr << "Invalid scaling rate.\n";
    return 1;
  }

  std::cout << "== BEFORE ==\n";
  printShapes(std::cout, shapes, 3);

  try
  {
    scaleShapes(shapes, 3, base, k);
  }
  catch (std::invalid_argument& e)
  {
    std::cerr << "Invalid scaling rate: " << e.what() << ".\n";
    return 1;
  }

  std::cout << "== AFTER ==\n";
  printShapes(std::cout, shapes, 3);
}

void bukreev::scaleShapes(Shape* const* shapes, size_t shapeCount, point_t base, double k)
{
  if (k < 0)
  {
    throw std::invalid_argument("Scaling rate should be non-negative");
  }

  for (size_t i = 0; i < shapeCount; i++)
  {
    Shape* shape = shapes[i];
    point_t pos = shape->getFrameRect().pos;

    double x = (pos.x - base.x) * k + base.x;
    double y = (pos.y - base.y) * k + base.x;
    shape->move({x, y});
    shape->scaleNoCheck(k);
  }
}

void bukreev::printShapes(std::ostream& out, const Shape* const* shapes, size_t shapeCount)
{
  double totalArea = 0;
  for (size_t i = 0; i < shapeCount; i++)
  {
    out << "SHAPE " << i << ":\n";

    double area = shapes[i]->getArea();
    totalArea += area;
    out << "Area: " << area << '\n';
    out << "Frame rect: " << '\n';
    printRectangle(out, shapes[i]->getFrameRect());
    out << '\n';
  }

  out << "Total area: " << totalArea << '\n';
  out << "General frame rect: " << '\n';
  printRectangle(out, getFrameRect(shapes, shapeCount));
  out << '\n';
}

void bukreev::printRectangle(std::ostream& out, rectangle_t rect)
{
  out << " Position: (" << rect.pos.x << ", " << rect.pos.y << ")\n";
  out << " Size: (" << rect.width << ", " << rect.height << ")\n";
}

bukreev::rectangle_t bukreev::getFrameRect(const Shape* const* shapes, size_t shapeCount)
{
  rectangle_t frame = shapes[0]->getFrameRect();

  double minLeft = frame.pos.x - frame.width / 2;
  double maxRight = frame.pos.x + frame.width / 2;
  double maxTop = frame.pos.y + frame.height / 2;
  double minBottom = frame.pos.y - frame.height / 2;

  for (size_t i = 1; i < shapeCount; i++)
  {
    frame = shapes[i]->getFrameRect();

    double left = frame.pos.x - frame.width / 2;
    double right = frame.pos.x + frame.width / 2;
    double top = frame.pos.y + frame.height / 2;
    double bottom = frame.pos.y - frame.height / 2;

    minLeft = std::min(minLeft, left);
    maxRight = std::max(maxRight, right);
    maxTop = std::max(maxTop, top);
    minBottom = std::min(minBottom, bottom);
  }

  double x = (minLeft + maxRight) / 2;
  double y = (minBottom + maxTop) / 2;
  double w = maxRight - minLeft;
  double h = maxTop - minBottom;
  return {{x, y}, w, h};
}

void bukreev::Shape::scaleWithCheck(double k)
{
  if (k < 0)
  {
    throw std::invalid_argument("Scaling rate should be non-negative");
  }

  scale(k);
}

void bukreev::Shape::scaleNoCheck(double k) noexcept
{
  scale(k);
}

bukreev::Rectangle::Rectangle(point_t center, double width, double height) noexcept:
  m_Center(center),
  m_Width(width),
  m_Height(height)
{}

double bukreev::Rectangle::getArea() const noexcept
{
  return m_Width * m_Height;
}
bukreev::rectangle_t bukreev::Rectangle::getFrameRect() const noexcept
{
  return {m_Center, m_Width, m_Height};
}
void bukreev::Rectangle::move(point_t newPos) noexcept
{
  m_Center = newPos;
}
void bukreev::Rectangle::move(double dX, double dY) noexcept
{
  m_Center.x += dX;
  m_Center.y += dY;
}
void bukreev::Rectangle::scale(double k) noexcept
{
  m_Width *= k;
  m_Height *= k;
}

bukreev::Xquare::Xquare(point_t center, double diagWidth) noexcept:
  m_Center(center),
  m_DiagWidth(diagWidth)
{}

double bukreev::Xquare::getArea() const noexcept
{
  return (m_DiagWidth * m_DiagWidth) / 2;
}
bukreev::rectangle_t bukreev::Xquare::getFrameRect() const noexcept
{
  return {m_Center, m_DiagWidth, m_DiagWidth};
}
void bukreev::Xquare::move(point_t newPos) noexcept
{
  m_Center = newPos;
}
void bukreev::Xquare::move(double dX, double dY) noexcept
{
  m_Center.x += dX;
  m_Center.y += dY;
}
void bukreev::Xquare::scale(double k) noexcept
{
  m_DiagWidth *= k;
}

bukreev::Square::Square(point_t center, double width) noexcept:
  m_Center(center),
  m_Width(width)
{}

double bukreev::Square::getArea() const noexcept
{
  return m_Width * m_Width;
}
bukreev::rectangle_t bukreev::Square::getFrameRect() const noexcept
{
  return {m_Center, m_Width, m_Width};
}
void bukreev::Square::move(point_t newPos) noexcept
{
  m_Center = newPos;
}
void bukreev::Square::move(double dX, double dY) noexcept
{
  m_Center.x += dX;
  m_Center.y += dY;
}
void bukreev::Square::scale(double k) noexcept
{
  m_Width *= k;
}
