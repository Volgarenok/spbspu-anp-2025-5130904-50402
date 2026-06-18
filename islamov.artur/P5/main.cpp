#include <iostream>
#include <iomanip>
namespace islamov
{
  struct point_t
  {
    double x;
    double y;
  };
  struct rect_t
  {
    double w;
    double h;
    point_t pos;
  };
  class Shape
  {
  public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual rect_t frame() const = 0;
    virtual void move(const point_t& p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };
  class Rectangle : public Shape
  {
  public:
    Rectangle(const point_t& c, double w, double h);
    double area() const override;
    rect_t frame() const override;
    void move(const point_t& p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    point_t c_;
    double w_;
    double h_;
  };
  Rectangle::Rectangle(const point_t& c, double w, double h):
    c_(c),
    w_(w),
    h_(h)
  {
    if (w <= 0.0 || h <= 0.0)
    {
      std::cerr << "Error: width and height must be positive\n";
      exit(2);
    }
  }
  double Rectangle::area() const
  {
    return w_ * h_;
  }
  rect_t Rectangle::frame() const
  {
    return {w_, h_, c_};
  }
  void Rectangle::move(const point_t& p)
  {
    c_ = p;
  }
  void Rectangle::move(double dx, double dy)
  {
    c_.x += dx;
    c_.y += dy;
  }
  void Rectangle::scale(double k)
  {
    if (k <= 0.0)
    {
      std::cerr << "Error: scale coefficient must be positive\n";
      exit(2);
    }
    w_ *= k;
    h_ *= k;
  }
  class Triangle : public Shape
  {
  public:
    Triangle(const point_t& v1, const point_t& v2, const point_t& v3);
    double area() const override;
    rect_t frame() const override;
    void move(const point_t& p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    point_t v_[3];
    point_t c_;
    void calcCenter();
    double triArea(const point_t& a, const point_t& b, const point_t& c) const;
  };
  Triangle::Triangle(const point_t& v1, const point_t& v2, const point_t& v3):
    v_{v1, v2, v3}
  {
    calcCenter();
  }
  void Triangle::calcCenter()
  {
    c_.x = (v_[0].x + v_[1].x + v_[2].x) / 3.0;
    c_.y = (v_[0].y + v_[1].y + v_[2].y) / 3.0;
  }
  double Triangle::triArea(const point_t& a, const point_t& b, const point_t& c) const
  {
    double ar = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    if (ar < 0.0) ar = -ar;
    return ar / 2.0;
  }
  double Triangle::area() const
  {
    return triArea(v_[0], v_[1], v_[2]);
  }
  rect_t Triangle::frame() const
  {
    double minX = v_[0].x;
    double maxX = v_[0].x;
    double minY = v_[0].y;
    double maxY = v_[0].y;
    for (int i = 1; i < 3; ++i)
    {
      if (v_[i].x < minX) minX = v_[i].x;
      if (v_[i].x > maxX) maxX = v_[i].x;
      if (v_[i].y < minY) minY = v_[i].y;
      if (v_[i].y > maxY) maxY = v_[i].y;
    }
    rect_t f;
    f.w = maxX - minX;
    f.h = maxY - minY;
    f.pos.x = minX + f.w / 2.0;
    f.pos.y = minY + f.h / 2.0;
    return f;
  }
  void Triangle::move(const point_t& p)
  {
    double dx = p.x - c_.x;
    double dy = p.y - c_.y;
    move(dx, dy);
  }
  void Triangle::move(double dx, double dy)
  {
    for (int i = 0; i < 3; ++i)
    {
      v_[i].x += dx;
      v_[i].y += dy;
    }
    c_.x += dx;
    c_.y += dy;
  }
  void Triangle::scale(double k)
  {
    if (k <= 0.0)
    {
      std::cerr << "Error: scale coefficient must be positive\n";
      exit(2);
    }
    for (int i = 0; i < 3; ++i)
    {
      v_[i].x = c_.x + (v_[i].x - c_.x) * k;
      v_[i].y = c_.y + (v_[i].y - c_.y) * k;
    }
  }
  void scaleAll(Shape** s, size_t n, const point_t& c, double k)
  {
    if (k <= 0.0)
    {
      std::cerr << "Error: scale coefficient must be positive\n";
      exit(1);
    }
    for (size_t i = 0; i < n; ++i)
    {
      s[i]->move(-c.x, -c.y);
      s[i]->scale(k);
      s[i]->move(c.x, c.y);
    }
  }
  double totalArea(Shape** s, size_t n)
  {
    double t = 0.0;
    for (size_t i = 0; i < n; ++i)
    {
      t += s[i]->area();
    }
    return t;
  }
  rect_t overallFrame(Shape** s, size_t n)
  {
    if (n == 0)
    {
      return {0.0, 0.0, {0.0, 0.0}};
    }
    rect_t r = s[0]->frame();
    double minX = r.pos.x - r.w / 2.0;
    double maxX = r.pos.x + r.w / 2.0;
    double minY = r.pos.y - r.h / 2.0;
    double maxY = r.pos.y + r.h / 2.0;
    for (size_t i = 1; i < n; ++i)
    {
      rect_t r = s[i]->frame();
      double rxMin = r.pos.x - r.w / 2.0;
      double rxMax = r.pos.x + r.w / 2.0;
      double ryMin = r.pos.y - r.h / 2.0;
      double ryMax = r.pos.y + r.h / 2.0;
      if (rxMin < minX) minX = rxMin;
      if (rxMax > maxX) maxX = rxMax;
      if (ryMin < minY) minY = ryMin;
      if (ryMax > maxY) maxY = ryMax;
    }
    rect_t o;
    o.w = maxX - minX;
    o.h = maxY - minY;
    o.pos.x = minX + o.w / 2.0;
    o.pos.y = minY + o.h / 2.0;
    return o;
  }
  void printInfo(Shape** s, size_t n, const char* t)
  {
    std::cout << t << '\n';
    for (size_t i = 0; i < n; ++i)
    {
      std::cout << "Shape " << (i + 1) << ":\n";
      std::cout << "  Area: " << std::fixed << std::setprecision(2) << s[i]->area() << '\n';
      rect_t f = s[i]->frame();
      std::cout << "  Frame rectangle: center(" << f.pos.x << ", " << f.pos.y << "), width: " << f.w << ", height: " << f.h << '\n';
    }
    std::cout << "Total area: " << std::fixed << std::setprecision(2) << totalArea(s, n) << '\n';
    rect_t o = overallFrame(s, n);
    std::cout << "Overall frame rectangle: center(" << o.pos.x << ", " << o.pos.y << "), width: " << o.w << ", height: " << o.h << '\n';
  }
}
int main()
{
  islamov::Rectangle r1({2.0, 3.0}, 4.0, 5.0);
  islamov::Triangle t1({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
  const size_t n = 2;
  islamov::Shape* s[n] = {&r1, &t1};
  islamov::printInfo(s, n, "BEFORE SCALING");
  islamov::point_t sc = {1.0, 1.0};
  double k = 2.0;
  islamov::scaleAll(s, n, sc, k);
  islamov::printInfo(s, n, "AFTER SCALING");
  return 0;
}