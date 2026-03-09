cmake_minimum_required(VERSION 3.20)
project(DSATracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Sources ───────────────────────────────────────────────────────────────────
file(GLOB_RECURSE SOURCES
    "src/**/*.cpp"
    "src/*.cpp"
)

add_executable(dsa_tracker ${SOURCES})

target_include_directories(dsa_tracker PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# ── SQLite (bundled) ──────────────────────────────────────────────────────────
# Drop sqlite3.h and sqlite3.c into vendor/sqlite/ and uncomment:
# target_sources(dsa_tracker PRIVATE vendor/sqlite/sqlite3.c)
# target_include_directories(dsa_tracker PRIVATE vendor/sqlite)

# ── Compiler flags ────────────────────────────────────────────────────────────
target_compile_options(dsa_tracker PRIVATE
    -Wall -Wextra -Wpedantic
    -O2
)

# ── Tests ─────────────────────────────────────────────────────────────────────
# enable_testing()
# add_subdirectory(tests)
