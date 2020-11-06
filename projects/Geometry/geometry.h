// !!! I haven't finished or tested it, it is just a draft

//  geometry.h
//  Speedwaagonnnnnnnm
//
//  Created by Александр Фреик on 18.04.2020.
//  Copyright © 2020 Freik. All rights reserved.
//

#ifndef geometry_h
#define geometry_h

#include <cassert>
#include <math.h>
#include <unordered_map>
#include <cmath>

class Line;

template<typename T = double>
bool is_equal(T num1, T num2, double precision) {
    assert(precision >= 0);
    // to prevent overload of type
    if (num1 < precision != num2 < precision) return false;
    T diff = num1 - num2;
    return diff < precision && -diff < precision;
}
struct Point {
    static constexpr double precision{0.0000001};
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
    Point& rotate(Point const &center, double angle);  //
    Point& reflex(Point const &center);  //
    Point& reflex(Line const &axis);  //
    Point& scale(Point const &center, double coefficient);  //
};
class Line {
public:
    // A, B, C define lines (Ax + By + C = 0), C == 1 or 0
    static constexpr double precision{0.0000001};
    double A, B;
    uint16_t C;
    Line(Point const &point1, Point const &point2);
    Line(Point const &point, double k);
    Line(double k, double shift);
};
//---------------------------------
//--------------Point--------------
//---------------------------------

//------------methods-------------
Point& Point::rotate(Point const &center, double angle) {
    double oldX = this->x, pi = 3.14159265359;
    this->x = this->x * cos(angle / 180 * pi) - this->y * sin(angle / 180 * pi);
    this->y = oldX * sin(angle / 180 * pi) + this->y * cos(angle / 180 * pi);
    return *this;
}
Point& Point::reflex(Point const &center) {
    this->x = center.x + center.x - this->x;
    this->y = center.y + center.y - this->y;
    return *this;
}
Point& Point::reflex(Line const &axis) {
    // p = p + 2tn, n = (A, B)
    // t = (n_x (x0 - x) + n_y (y0 - y)) / sumAB
    // x0 = A C / sumAB, y0 = B C / sumAB
    // sumAB = A^2 + B^2
    double sumAB = axis.A * axis.A + axis.B * axis.B;
    assert(is_equal<double>(sumAB, 0, precision));
    double x0 = axis.A * axis.C / sumAB, y0 =axis.B * axis.C / sumAB;
    double t = (axis.A * (x0 - this->x) + axis.B * (y0 - this->y)) / sumAB;
    this->x += 2 * t * axis.A;
    this->y += 2 * t * axis.B;
    return *this;
}
Point& Point::scale(Point const &center, double coefficient) {
    assert(coefficient > 0);
    this->x = center.x + coefficient * (this->x - center.x);
    this->y = center.y + coefficient * (this->y - center.y);
    return *this;
}

//--------------operators--------------
bool operator==(Point const &point1, Point const &point2) {
    return is_equal(point1.x, point2.x, point1.precision)  &&  is_equal(point1.y, point2.y, point1.precision);
}  // check
bool operator!=(Point const &point1, Point const &point2) {
    return !(point1 == point2);
}  // check

//------------functions-------------
double distance(Point const &point1, Point const &point2) {
    return sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}
double scalar(std::pair<Point, Point> const &directed_segment1, std::pair<Point, Point> const &directed_segment2) {
    return
    (directed_segment1.second.x - directed_segment1.first.x) *
    (directed_segment2.second.x - directed_segment2.first.x) +
    (directed_segment1.second.y - directed_segment1.first.y) *
    (directed_segment2.second.y - directed_segment2.first.y);
}

//--------------------------------
//--------------Line--------------
//--------------------------------

//------------constructors-------------
Line::Line(Point const &point1, Point const &point2) {
    double c = point1.x * (point1.y - point2.y) + point1.y * (point2.x - point1.x);
    if (is_equal<double>(c, 0, point1.precision)) {
        this->A = point2.y - point1.y;
        this->B = point1.x - point2.x;
        this->C = 0;
    } else {
        this->A = (point2.y - point1.y) / c;
        this->B = (point1.x - point2.x) / c;
        this->C = 1;
    }
}
Line::Line(Point const &point, double k) {
    double c = -point.x * k + point.y;
    if (is_equal<double>(c, 0, point.precision)) {
        this->A = k;
        this->B = -1;
        this->C = 0;
    } else {
        this->A = k / c;
        this->B = -1 / c;
        this->C = 1;
    }
}
Line::Line(double k, double shift) {
    if (is_equal<double>(shift, 0, this->precision)) {
        this->A = k;
        this->B = -1;
        this->C = 0;
    } else {
        this->A = k / shift;
        this->B = -1 / shift;
        this->C = 1;
    }
}

//--------------operators--------------
bool operator==(Line const &line1, Line const &line2) {
    return is_equal(line1.A, line2.A, line1.precision)  &&  is_equal(line1.B, line2.B, line1.precision) &&
    line1.C == line2.C;
}
bool operator!=(Line const &line1, Line const &line2) {
    return !(line1 == line2);
}


class Shape {
    virtual bool is_equivalent(const Shape& other) const = 0;

  protected:
    static constexpr double precision{0.0000001};
    
  public:
    bool operator==(const Shape& other) const;
    bool isCongruentTo(const Shape& other);
    bool isSimilarTo(const Shape& other);
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool containsPoint(Point const &point) const = 0;
    // actions with shape
    virtual Shape& rotate(Point const &center, double angle) = 0;
    virtual Shape& reflex(Point const &center) = 0;
    virtual Shape& reflex(Line const &axis) = 0;
    virtual Shape& scale(Point const &center, double coefficient) = 0;
};

bool Shape::operator==(const Shape& other) const {
    // make sure the types are the same
    if (typeid(*this) != typeid(other))
        return false;
    // if they are the same, then compare them
    return is_equivalent(other);
}
    
class Polygon : public Shape {
    bool is_equivalent(const Shape &) const override;  //
    
  protected:
    std::vector<Point> vertices;
    // thay correct (or undefefined) after every method with class
    mutable std::shared_ptr<double> minX, maxX, minY, maxY;
    mutable bool edges_correct{false};
    void set_edges() const;
    
  public:
    Polygon(std::vector<Point> const &pvec) : vertices(pvec) {}
    Polygon(std::initializer_list<Point> init_list) : vertices(init_list) {}
    // methods of all Shapes
    double perimeter() const override;  //
    double area() const override;  //
    bool containsPoint(Point const &point) const override;
    // actions with shape
    Polygon& rotate(Point const &center, double angle) override;  //
    Polygon& reflex(Point const &center) override;  //
    Polygon& reflex(Line const &axis) override;  //
    Polygon& scale(Point const &center, double coefficient) override;  //
    
    // methods only for poligons
    size_t verticesCount() { return vertices.size(); }
    std::vector<Point> getVertices() { return vertices; }
    bool isConvex();  //
};
bool Polygon::is_equivalent(const Shape& other) const {
    const Polygon *another = nullptr;
    // check type of other. After we sure that it's type is Polygon
    try {
        another = static_cast<const Polygon *>(&other);
    } catch (...) {
        assert(false);
        return false;
    }
    assert(!this->vertices.empty() && !another->vertices.empty());
    if (this->vertices.size() != another->vertices.size()) return false;
    size_t vert_counter = 0, size = this->vertices.size();
    for (; vert_counter != size; ++vert_counter)
        if ( this->vertices[vert_counter] == another->vertices[0]) break;
    if (vert_counter == size) return false;
    size_t cnt2 = 1;
    for (; cnt2 != size; ++cnt2)
        if (this->vertices[(cnt2 + vert_counter) % size] != another->vertices[cnt2]) break;
    if (cnt2 == size) return true;
    for (cnt2 = 1; cnt2 != size; ++cnt2)
        if (this->vertices[(size + vert_counter - cnt2) % size] != another->vertices[cnt2]) break;
    if (cnt2 == size) return true;
    return false;
}  // little check
void Polygon::set_edges() const {
    assert(!this->vertices.empty());
    if (this->edges_correct) return;
    if (!minX) {
        this->minX = std::make_shared<double>(this->vertices[0].x);
        this->maxX = std::make_shared<double>(this->vertices[0].x);
        this->minY = std::make_shared<double>(this->vertices[0].y);
        this->maxY = std::make_shared<double>(this->vertices[0].y);
    } else {
        *minX = this->vertices[0].x;
        *maxX = this->vertices[0].x;
        *minY = this->vertices[0].y;
        *maxY = this->vertices[0].y;
    }
    for (auto point : vertices) {
        if (*minX > point.x) *minX = point.x;
        else if (*maxX < point.x) *maxX = point.x;
        if (*minY > point.y) *minY = point.y;
        else if (*maxX < point.y) *maxX = point.y;
    }
    this->edges_correct = true;
}
double Polygon::perimeter() const {
    assert(!this->vertices.empty());
    double ans{0};
    for (auto first = vertices.cend() - 1, second = vertices.cbegin();
                                second != vertices.cend(); first = second++)
         ans += distance(*first, *second);
    return ans;
}
double Polygon::area() const {
    assert(!this->vertices.empty());
    double ans{0};
    if (!this->edges_correct) this->set_edges();
    // to increase precidion
    double middleX = (*this->maxX + *this->minX) / 2, middleY = (*this->maxY + *this->minY) / 2;
    if (middleX < middleY)
        for (auto first = vertices.cend() - 1, second = vertices.cbegin();
                                    second != vertices.cend(); first = second++)
            ans += (first->x - middleX + second->x - middleX) * (first->y - second->y) / 2;
    else
        for (auto first = vertices.cend() - 1, second = vertices.cbegin();
                                    second != vertices.cend(); first = second++)
            ans += (first->y - middleY + second->y - middleY) * (first->x - second->x) / 2;
    return abs(ans);
}
/// Define wether point inside the Polygon or not. The algorithm is implemented based on the ray method.
/// A horizontal beam is launched and algorythm count number of intersections with polygon faces.
/// @param point actually the point for which it is checked whether it is inside or outside.
bool Polygon::containsPoint(Point const &point) const {
    assert(!this->vertices.empty());
    if (!edges_correct) set_edges();
    double x = point.x, y = point.y;
    if (x < *this->minX || x > *this->maxX || y < *this->minY || y > *this->maxY)
        return false;
    bool ans = false;
    for (auto first = vertices.cend() - 1, second = vertices.cbegin();
                                second != vertices.cend(); first = second++) {
        if (first.x)
        if ((y <= first->y && y >= second->y) || (y >= first->y && y <= second->y))
            
    }
    return 0;
}
Polygon& Polygon::rotate(Point const &center, double angle) {
    assert(!this->vertices.empty());
    for (auto &point : this->vertices)
        point.rotate(center, angle);
    this->edges_correct = false;
    return *this;
}
Polygon& Polygon::reflex(Point const &center) {
    assert(!this->vertices.empty());
    for (auto &point : this->vertices)
        point.reflex(center);
    this->edges_correct = false;
    return *this;
}
Polygon& Polygon::reflex(Line const &axis) {
    assert(!this->vertices.empty());
    for (auto &point : this->vertices)
        point.reflex(axis);
    this->edges_correct = false;
    return *this;
}
Polygon& Polygon::scale(Point const &center, double coefficient) {
    assert(!this->vertices.empty() && coefficient > 0);
    for (auto &point : this->vertices)
        point.scale(center, coefficient);
    this->edges_correct = false;
    return *this;
}

bool Polygon::isConvex() {
    assert(!this->vertices.empty());
    if (this->vertices.size() < 4) return true;
    for (size_t index = 2; index < this->vertices.size(); ++index)
        if (scalar({this->vertices[index], this->vertices[index - 1]},
        {this->vertices[index - 1], this->vertices[index - 2]}) < precision)
            return false;
    if (scalar({*this->vertices.cbegin(), *this->vertices.crbegin()},
    {*this->vertices.crbegin(), *(this->vertices.crbegin() + 1)}) < precision)
        return false;
    return true;
}

#endif /* geometry_h */

