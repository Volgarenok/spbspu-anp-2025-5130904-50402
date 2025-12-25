#include <cstdlib>

namespace strelnikov {
  struct point_t {
    float x_, y_;
  };
  struct rectangle_t {
    float width_, height_;
    point_t c_;
  };
  class IShape {
  public:
    virtual float getArea() = 0;
    virtual rectangle_t getFrameRect() = 0;
    virtual void move(point_t) = 0;
    virtual void move(float, float) = 0;
    virtual void scale(float) = 0;
    virtual ~IShape() = default;
  };
  class Rectangle : public IShape {
  public:
    Rectangle(float, float, point_t);
    float getArea() override;
    rectangle_t getFrameRect() override;
    void move(point_t) override;
    void move(float, float) override;
    void scale(float) override;
    ~Rectangle() override = default;

  private:
    rectangle_t rec_;
  };

  class Polygon : public IShape {
  public:
    Polygon(point_t*, size_t);
    float getArea() override;
    rectangle_t getFrameRect() override;
    void move(point_t) override;
    void move(float, float) override;
    void scale(float) override;
    ~Polygon() override = default;

  private:
    point_t* data_;
    size_t size_;
  };

  class Circle : public IShape {
  public:
    Circle(point_t, float);
    float getArea() override;
    rectangle_t getFrameRect() override;
    void move(point_t) override;
    void move(float, float) override;
    void scale(float) override;
    ~Circle() override = default;

  private:
    point_t cen_;
    float rad_;
  };

};

int main()
{}