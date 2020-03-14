#include "calc/ErrorOr.h"
#include <sstream>

using namespace calc;

void Error::print(std::ostream &OS) const { OS << Msg; }
