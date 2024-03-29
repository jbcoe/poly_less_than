#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <string>
#include "visitors/poly_less_than.h"

using visitors::PolyLessThan;

namespace {

struct Resident;
struct Student;
struct Teacher;

struct ResidentVisitor {
  virtual ~ResidentVisitor() = default;
  virtual void Visit(const Student& s) = 0;
  virtual void Visit(const Teacher& s) = 0;
};

struct Resident {
  virtual ~Resident() = default;
  virtual void Accept(ResidentVisitor& v) const = 0;
};

class Student : public Resident {
  std::string name_;

 public:
  Student(const char* name) : name_(name) {}
  void Accept(ResidentVisitor& v) const override { v.Visit(*this); }

  friend bool operator<(const Student& lhs, const Student& rhs) {
    return lhs.name_ < rhs.name_;
  }
};

class Teacher : public Resident {
  int id_;

 public:
  Teacher(int id) : id_(id) {}
  void Accept(ResidentVisitor& v) const override { v.Visit(*this); }

  friend bool operator<(const Teacher& lhs, const Teacher& rhs) {
    return lhs.id_ < rhs.id_;
  }
};

bool operator<(const Resident& lhs, const Resident& rhs) {
  return PolyLessThan<ResidentVisitor, Student, Teacher>()(&lhs, &rhs);
}

}  // end namespace

TEST_CASE("Different types are compared", "[PolyLessThan]") {
  Teacher t(101);
  Student s("Tommy");

  REQUIRE(s < t);
  REQUIRE_FALSE(t < s);
}

TEST_CASE("Same types are compared", "[PolyLessThan]") {
  Teacher t100(100);
  Teacher t1000(1000);

  REQUIRE(t100 < t1000);
  REQUIRE_FALSE(t1000 < t100);
  REQUIRE_FALSE(t1000 < t1000);
  REQUIRE_FALSE(t100 < t100);
}
