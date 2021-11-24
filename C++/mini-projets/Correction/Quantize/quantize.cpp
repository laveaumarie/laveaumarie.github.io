#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

template<std::size_t dim>
struct point { double coords[dim]; };

template<std::size_t dim>
using points = std::vector<point<dim>>;

template<std::size_t dim>
point<dim> randomPoint() {
  point<dim> p;
  for (std::size_t i = 0; i != dim; ++i)
    p.coords[i] = double(std::rand()) / RAND_MAX;
  return p;
}

template<std::size_t dim>
std::ostream &operator<<(std::ostream &out, point<dim> const &p) {
  out << '(';
  for (std::size_t i = 0; i != dim; ++i) {
    if (i) out << ", ";
    out << p.coords[i];
  }
  out << ')';
  return out;
}

template<std::size_t dim>
points<dim> randomPoints(std::size_t n) {
  points<dim> pts;
  std::generate_n(std::back_inserter(pts), n, randomPoint<dim>);
  return pts;
}

template<std::size_t dim>
std::ostream &operator<<(std::ostream &out, points<dim> const &pts) {
  std::for_each(pts.begin(), pts.end(),
                [&out](auto &p) { out << "  " << p << '\n'; });
  return out;
}

template<std::size_t dim>
double dist(point<dim> const &p1, point<dim> const &p2) {
  double d = 0;
  for (std::size_t i = 0; i != dim; ++i) {
    double v = p2.coords[i] - p1.coords[i];
    d += v * v;
  }
  return sqrt(d);
}

template<std::size_t dim>
double pathLength(points<dim> const &pts) {
  if (pts.size() == 0) return 0;
  double d = 0;
  point<dim> prev = pts[pts.size() - 1];
  std::for_each(pts.begin(), pts.end(),
                [&d,&prev](auto const &curr)
                { d += dist(prev, curr); prev = curr; });
  return d;
}

template<std::size_t dim>
double pathLength2(points<dim> const &pts) {
  if (pts.size() == 0) return 0;
  point<dim> prev = pts[pts.size() - 1];
  return
  std::accumulate(pts.begin(), pts.end(), 0.,
                  [&prev](double d, auto const &curr)
                  { d += dist(prev, curr); prev = curr; return d; });
}

template<std::size_t dim>
point<dim> quant(point<dim> const &p1) {
  point<dim> p2;
  for (std::size_t i = 0; i != dim; ++i) {
    p2.coords[i] = (int)(p1.coords[i] * 10) / 10.;
  }
  return p2;
}

template<std::size_t dim>
bool lt(point<dim> const &p1, point<dim> const &p2) {
  point<dim> p3 = quant(p1), p4 = quant(p2);
  for (std::size_t i = 0; i != dim; ++i) {
    if (p3.coords[i] < p4.coords[i]) return true;
    if (p3.coords[i] > p4.coords[i]) return false;
  }
  return false;
}

template<std::size_t dim>
struct clt {
  bool operator()(point<dim> const &p1, point<dim> const &p2) const {
    return lt(p1, p2);
  }
};

template<std::size_t dim>
using point_set = std::set<point<dim>, clt<dim>>;

int main() {
  std::cout << "un point : " << randomPoint<3>() << '\n';
  points<2> pts = randomPoints<2>(4);
  std::cout << "un vecteur de points :\n" << pts;
  points<2> triangle = { {0.,0.}, {1.,0.}, {0.,1.} };
  std::cout << "périmètre du triangle : " << pathLength(triangle)
            << ' ' << pathLength2(triangle) << '\n';
  std::pair<point<2>,point<2>> ppts[4] =
    { {{0.15,0.72}, {0.43,0.09}},
      {{0.27,0.57}, {0.21,0.63}},
      {{0.34,0.72}, {0.36,0.73}},
      {{0.43,0.09}, {0.15,0.72}} };
  for (auto const &pp : ppts) {
    char const *s = lt(pp.first, pp.second) ? " <  " : " >= ";
    std::cout << pp.first << s << pp.second << '\n';
  }
  points<4> pts2 = randomPoints<4>(10000);
  point_set<4> pset;
  std::copy(pts2.begin(), pts2.end(), std::inserter(pset, pset.begin()));
  std::cout << "taille de l'ensemble : " << pset.size() << '\n';
}
