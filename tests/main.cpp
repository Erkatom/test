#include <iostream>
struct p_t {
  int x, y;
};

struct Pt {
  Pt(int x, int y);
  Pt(const Pt& pt):
   x(pt.x), y(pt.y)
  {
    std::cout << "Pt::Pt(const Pt&): " << this << "\n";
  }
  ~Pt() {
    std::cout << "Pt::~Pt(): " << this << "\n";
  }

  Pt& operator=(const Pt& rhs) {
    std::cout << "Pt::operator=(const Pt&): " << this << "\n";
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

 private:
  int x, y;
};

struct array {
  int* begin;
  int* end;
};

Pt::Pt(int xx, int yy):
 x(xx), y(yy)
{
  std::cout << "Pt::Pt(int, int): " << this << "\n";
}

int main()
{
  Pt a(10, 20);
  Pt b(30, 10);
  b = a = b;
  Pt c = a;
  Pt d(c);
  Pt f{d};
  Pt* ptr1 = new Pt(10, 20); // Pt(int, int)
  try {
    Pt* ptr2 = new Pt(*ptr1); // Pt(int, int)
    // ...
    delete ptr1;
    delete ptr2;
  } catch (...) {
    delete ptr1;
    throw;
  }
}
// b.~Pt()
// a.~Pt()
