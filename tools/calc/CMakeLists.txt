set(sources
  main.cpp)

add_executable(calc ${sources})

target_link_libraries(calc libcalc)
