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
class ErrorOr {
public:
  ErrorOr(const T &rhs) : containsError(false) { new (&value) T(rhs); }
  ErrorOr(const ErrorOr<T> &rhs) : containsError(rhs.containsError) {
    if (containsError) {
      new (&error) Error(rhs.error);
    } else {
      new (&value) T(rhs.value);
    }
  }
  explicit ErrorOr(T &&rhs) : containsError(false) {
    new (&value) T(std::forward<T>(rhs));
  }
  ErrorOr(const Error &err) : containsError(true) { new (&error) Error(err); }

  ~ErrorOr() {
    if (containsError) {
      error.~Error();
    } else {
      value.~T();
    }
  }

  ErrorOr &operator=(ErrorOr rhs) {
    this->~ErrorOr();

    containsError = rhs.containsError;

    if (containsError) {
      new (&error) Error(rhs.error);
    } else {
      new (&value) T(rhs.value);
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
ErrorOr<T> operator*(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs * *rhs;
}

template<typename T>
ErrorOr<T> operator/(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs / *rhs;
}

template<typename T>
ErrorOr<T> operator+(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs + *rhs;
}

template<typename T>
ErrorOr<T> operator-(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs - *rhs;
}

template<typename T>
bool operator==(const ErrorOr<T> &lhs, const T &rhs) {
  return lhs.hasValue() && *lhs == rhs;
}

template<typename T>
bool operator==(const T &lhs, const ErrorOr<T> &rhs) {
  return rhs.hasValue() && lhs == *rhs;
}

template<typename T>
bool operator==(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return rhs.hasError();
  }

  if (rhs.hasError()) {
    return false;
  }

  return *lhs == *rhs;
}

} // namespace calc

#endif