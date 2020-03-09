#ifndef CALC_ERROROR_H
#define CALC_ERROROR_H

#include <string>

namespace calc {

using SLoc = std::pair<int, int>;

class Error {
public:
  Error(SLoc Loc, const std::string &Msg) : Loc(Loc), Msg(Msg) {}
  Error(SLoc Loc, std::string &&Msg) : Loc(Loc), Msg(std::move(Msg)) {}
  Error(const Error &E) : Loc(E.Loc), Msg(E.Msg) {}
  Error(Error &&E) : Loc(E.Loc), Msg(std::move(E.Msg)) {}

  void print(std::ostream &OS) const;
  void dump() const;
  friend std::ostream &operator<<(std::ostream &OS, const Error &E);

  SLoc Loc;
  std::string Msg;
};

template <typename T>
class [[nodiscard]] ErrorOr {
public:
  ErrorOr(const T &RHS) : containsError(false) { new (&value) T(RHS); }
  ErrorOr(const ErrorOr<T> &RHS) : containsError(RHS.containsError) {
    if (containsError) {
      new (&error) Error(RHS.error);
    } else {
      new (&value) T(RHS.value);
    }
  }
  explicit ErrorOr(T &&RHS) : containsError(false) {
    new (&value) T(std::forward<T>(RHS));
  }
  ErrorOr(const Error &err) : containsError(true) { new (&error) Error(err); }

  ~ErrorOr() {
    if (containsError) {
      error.~Error();
    } else {
      value.~T();
    }
  }

  ErrorOr &operator=(ErrorOr RHS) {
    this->~ErrorOr();

    containsError = RHS.containsError;

    if (containsError) {
      new (&error) Error(RHS.error);
    } else {
      new (&value) T(RHS.value);
    }

    return *this;
  }

  bool hasValue() const { return !containsError; }
  bool hasError() const { return containsError; }
  explicit operator bool() const { return !containsError; }
  const Error &getError() const { assert(containsError); return error; }

  void print(std::ostream &OS) const {
    if (containsError) {
      OS << error;
    } else {
      OS << value;
    }
  }
  void dump() const;

  T &operator*() {
    assert(!containsError);
    return value;
  }
  const T &operator*() const {
    assert(!containsError);
    return value;
  }

  T *operator->() {
    assert(!containsError);
    return &value;
  }
  const T *operator->() const {
    assert(!containsError);
    return &value;
  }

  union {
    T value;
    Error error;
  };
  bool containsError;
};

template<typename T>
std::ostream &operator<<(std::ostream &OS, ErrorOr<T> &E) {
  E.print(OS);
  return OS;
}

template<typename T>
ErrorOr<T> operator*(const ErrorOr<T> &LHS, const ErrorOr<T> &RHS) {
  if (LHS.hasError())
    return LHS;

  if (RHS.hasError())
    return RHS;

  return *LHS * *RHS;
}

template<typename T>
ErrorOr<T> operator/(const ErrorOr<T> &LHS, const ErrorOr<T> &RHS) {
  if (LHS.hasError())
    return LHS;

  if (RHS.hasError())
    return RHS;

  return *LHS / *RHS;
}

template<typename T>
ErrorOr<T> operator+(const ErrorOr<T> &LHS, const ErrorOr<T> &RHS) {
  if (LHS.hasError())
    return LHS;

  if (RHS.hasError())
    return RHS;

  return *LHS + *RHS;
}

template<typename T>
ErrorOr<T> operator-(const ErrorOr<T> &LHS, const ErrorOr<T> &RHS) {
  if (LHS.hasError())
    return LHS;

  if (RHS.hasError())
    return RHS;

  return *LHS - *RHS;
}

template<typename T>
ErrorOr<T> operator+(const ErrorOr<T> &Val) {
  return Val;
}

template<typename T>
ErrorOr<T> operator-(const ErrorOr<T> &Val) {
  if (Val.hasError())
    return Val;

  return -*Val;
}

template<typename T>
ErrorOr<T> operator!(const ErrorOr<T> &Val) {
  if (Val.hasError())
    return Val;

  return !*Val;
}

template<typename T>
bool operator==(const ErrorOr<T> &LHS, const T &RHS) {
  return LHS.hasValue() && *LHS == RHS;
}

template<typename T>
bool operator==(const T &LHS, const ErrorOr<T> &RHS) {
  return RHS.hasValue() && LHS == *RHS;
}

template<typename T>
bool operator==(const ErrorOr<T> &LHS, const ErrorOr<T> &RHS) {
  if (LHS.hasError())
    return RHS.hasError();

  if (RHS.hasError())
    return false;

  return *LHS == *RHS;
}

} // namespace calc

#endif