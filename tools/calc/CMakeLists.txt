set(sources
  main.cpp)

add_executable(crepl ${sources})

target_link_libraries(crepl calc)
