cmake_minimum_required(VERSION 3.5)

project(GraphSerializer VERSION 0.0 LANGUAGES CXX)

# Set sources and headers (headers optional - for some IDEs)
set(SOURCES main.cpp)
set(HEADERS )

# Add dirs...

add_executable(graph-serializer ${SOURCES} ${HEADERS})

target_link_libraries(graph-serializer tiny-fsm)
