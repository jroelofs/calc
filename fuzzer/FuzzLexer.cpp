#include "calc/Lexer.h"

#include <sstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  std::stringstream SS(std::string(reinterpret_cast<const char*>(Data), Size));
  calc::IOSLexer L(SS);
  while (!L.empty()) {
    L.peek();
    L.pop();
  }
  return 0;
}
