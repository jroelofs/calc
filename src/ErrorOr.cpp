#include "calc/ErrorOr.h"

using namespace calc;

void Error::print(std::ostream &OS) const {
    OS << Msg;
}

std::ostream &calc::operator<<(std::ostream &OS, const Error &E) {
  E.print(OS);
  return OS;
}