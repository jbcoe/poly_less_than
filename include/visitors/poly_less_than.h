#pragma once

namespace visitors {
template <class TVisitorBase, class... TArgs>
class PolyLessThan {
 public:
  template <class T1, class T2>
  bool operator()(const T1 *pt1, const T2 *pt2) const {
    auto polyCompare = Impl<1, TArgs...>();
    pt1->Accept(polyCompare);
    pt2->Accept(polyCompare);
    return polyCompare.result;
  }

 private:
  template <int N, class... TInnerArgs>
  struct Impl : TVisitorBase {
    bool result = false;

   protected:
    int n = 0;
    const void *pt = nullptr;
  };

  template <int N, class TItem, class... TInnerArgs>
  struct Impl<N, TItem, TInnerArgs...> : Impl<N + 1, TInnerArgs...> {
    void Visit(const TItem &t) override final {
      if (this->n == 0) {
        this->n = N;
        this->pt = static_cast<const void *>(&t);
      } else if (this->n < N) {
        this->result = true;
      } else if (N < this->n) {
        this->result = false;
      } else {
        this->result = *static_cast<const TItem *>(this->pt) < t;
      }
    }
  };

  static_assert(!std::is_abstract<Impl<1, TArgs...>>::value,
                "Cannot compile polymorphic comparator: "
                "no concrete implementation for one or more Visit functions");
};
}  // namespace visitors
