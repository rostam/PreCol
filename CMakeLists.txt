cmake_minimum_required(VERSION 3.2.2)
project(precol)
find_package(Boost REQUIRED  COMPONENTS unit_test_framework)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp  -D_GLIBCXX_PARALLEL")
include_directories(${Boost_INCLUDE_DIRS})
file(GLOB ALG_SRC "Algorithms/*.cpp")
file(GLOB Application "Application/*.cpp")
file(GLOB PRECONDITIONING_SRC "Application/Preconditioning/*.cpp")
file(GLOB COLUMN_GAIN_GRAPH "Application/ColumnGainGraph/*.cpp")
file(GLOB GRAPH_SRC "Graph/*.cpp")
file(GLOB ORD_SRC "Orderings/*.cpp")
file(GLOB BoostTestSrc "BoostTest/*.cpp")
file(GLOB MainSRC "Main/*.cpp")
file(GLOB InputOutput "InputOutput/*.cpp")
add_executable(BoostTest ${BoostTestSrc} ${ALG_SRC} ${GRAPH_SRC} ${ORD_SRC})
add_executable(Application ${Application} ${InputOutput} ${PRECONDITIONING_SRC} ${ALG_SRC} ${GRAPH_SRC} ${ORD_SRC})
add_executable(PreCol ${MainSRC} ${InputOutput}  ${ALG_SRC} ${GRAPH_SRC} ${ORD_SRC})
add_executable(ColumnGain ${COLUMN_GAIN_GRAPH} ${ALG_SRC} ${GRAPH_SRC} ${ORD_SRC})
target_link_libraries(Application ${Boost_LIBRARIES})
target_link_libraries(BoostTest ${Boost_LIBRARIES})
target_link_libraries(PreCol ${Boost_LIBRARIES})
#target_link_libraries(ColumnGain ${Boost_LIBRARIES})
