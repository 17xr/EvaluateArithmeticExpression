cmake_minimum_required(VERSION 3.28)
project(main)

set(CMAKE_CXX_STANDARD 23)

add_executable(main main.cpp EvalArithmeticExpr.hpp)