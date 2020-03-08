#include "calc/ErrorOr.h"
#include <sstream>

using namespace calc;

void Error::print(std::ostream &OS) const { OS << Msg; }

std::ostream &calc::operator<<(std::ostream &OS, const Error &E) {
  E.print(OS);
  return OS;
}