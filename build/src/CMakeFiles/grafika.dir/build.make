# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/js/cpp/grafika

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/js/cpp/grafika/build

# Include any dependencies generated for this target.
include src/CMakeFiles/grafika.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/grafika.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/grafika.dir/flags.make

src/CMakeFiles/grafika.dir/main.cpp.o: src/CMakeFiles/grafika.dir/flags.make
src/CMakeFiles/grafika.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/js/cpp/grafika/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/grafika.dir/main.cpp.o"
	cd /home/js/cpp/grafika/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grafika.dir/main.cpp.o -c /home/js/cpp/grafika/src/main.cpp

src/CMakeFiles/grafika.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grafika.dir/main.cpp.i"
	cd /home/js/cpp/grafika/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/js/cpp/grafika/src/main.cpp > CMakeFiles/grafika.dir/main.cpp.i

src/CMakeFiles/grafika.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grafika.dir/main.cpp.s"
	cd /home/js/cpp/grafika/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/js/cpp/grafika/src/main.cpp -o CMakeFiles/grafika.dir/main.cpp.s

# Object files for target grafika
grafika_OBJECTS = \
"CMakeFiles/grafika.dir/main.cpp.o"

# External object files for target grafika
grafika_EXTERNAL_OBJECTS =

src/grafika: src/CMakeFiles/grafika.dir/main.cpp.o
src/grafika: src/CMakeFiles/grafika.dir/build.make
src/grafika: src/CMakeFiles/grafika.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/js/cpp/grafika/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable grafika"
	cd /home/js/cpp/grafika/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grafika.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/grafika.dir/build: src/grafika

.PHONY : src/CMakeFiles/grafika.dir/build

src/CMakeFiles/grafika.dir/clean:
	cd /home/js/cpp/grafika/build/src && $(CMAKE_COMMAND) -P CMakeFiles/grafika.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/grafika.dir/clean

src/CMakeFiles/grafika.dir/depend:
	cd /home/js/cpp/grafika/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/js/cpp/grafika /home/js/cpp/grafika/src /home/js/cpp/grafika/build /home/js/cpp/grafika/build/src /home/js/cpp/grafika/build/src/CMakeFiles/grafika.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/grafika.dir/depend
