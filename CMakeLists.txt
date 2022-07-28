cmake_minimum_required(VERSION 3.10)
project (MWM_Auction_Comparison)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

find_package(Boost 1.79.0 COMPONENTS headers)

add_subdirectory(Application/src)
