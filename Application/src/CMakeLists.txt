set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(app 
    AuctionArray.cpp
    Auction.cpp
    BipartiteGraph.cpp
    Main.cpp
    MaximumWeightedMatching.cpp
)

target_include_directories(app PRIVATE ../include)
target_link_libraries(app Boost::headers)
