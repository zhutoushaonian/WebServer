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
CMAKE_SOURCE_DIR = /home/luhuitong/CmakeHttpServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luhuitong/CmakeHttpServer/bulid

# Include any dependencies generated for this target.
include src/CMakeFiles/httpserver.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/httpserver.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/httpserver.dir/flags.make

src/CMakeFiles/httpserver.dir/buffer.cpp.o: src/CMakeFiles/httpserver.dir/flags.make
src/CMakeFiles/httpserver.dir/buffer.cpp.o: ../src/buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luhuitong/CmakeHttpServer/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/httpserver.dir/buffer.cpp.o"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/httpserver.dir/buffer.cpp.o -c /home/luhuitong/CmakeHttpServer/src/buffer.cpp

src/CMakeFiles/httpserver.dir/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpserver.dir/buffer.cpp.i"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luhuitong/CmakeHttpServer/src/buffer.cpp > CMakeFiles/httpserver.dir/buffer.cpp.i

src/CMakeFiles/httpserver.dir/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpserver.dir/buffer.cpp.s"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luhuitong/CmakeHttpServer/src/buffer.cpp -o CMakeFiles/httpserver.dir/buffer.cpp.s

src/CMakeFiles/httpserver.dir/http_conn.cpp.o: src/CMakeFiles/httpserver.dir/flags.make
src/CMakeFiles/httpserver.dir/http_conn.cpp.o: ../src/http_conn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luhuitong/CmakeHttpServer/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/httpserver.dir/http_conn.cpp.o"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/httpserver.dir/http_conn.cpp.o -c /home/luhuitong/CmakeHttpServer/src/http_conn.cpp

src/CMakeFiles/httpserver.dir/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpserver.dir/http_conn.cpp.i"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luhuitong/CmakeHttpServer/src/http_conn.cpp > CMakeFiles/httpserver.dir/http_conn.cpp.i

src/CMakeFiles/httpserver.dir/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpserver.dir/http_conn.cpp.s"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luhuitong/CmakeHttpServer/src/http_conn.cpp -o CMakeFiles/httpserver.dir/http_conn.cpp.s

src/CMakeFiles/httpserver.dir/locker.cpp.o: src/CMakeFiles/httpserver.dir/flags.make
src/CMakeFiles/httpserver.dir/locker.cpp.o: ../src/locker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luhuitong/CmakeHttpServer/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/httpserver.dir/locker.cpp.o"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/httpserver.dir/locker.cpp.o -c /home/luhuitong/CmakeHttpServer/src/locker.cpp

src/CMakeFiles/httpserver.dir/locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpserver.dir/locker.cpp.i"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luhuitong/CmakeHttpServer/src/locker.cpp > CMakeFiles/httpserver.dir/locker.cpp.i

src/CMakeFiles/httpserver.dir/locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpserver.dir/locker.cpp.s"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luhuitong/CmakeHttpServer/src/locker.cpp -o CMakeFiles/httpserver.dir/locker.cpp.s

src/CMakeFiles/httpserver.dir/lst_timer.cpp.o: src/CMakeFiles/httpserver.dir/flags.make
src/CMakeFiles/httpserver.dir/lst_timer.cpp.o: ../src/lst_timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luhuitong/CmakeHttpServer/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/httpserver.dir/lst_timer.cpp.o"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/httpserver.dir/lst_timer.cpp.o -c /home/luhuitong/CmakeHttpServer/src/lst_timer.cpp

src/CMakeFiles/httpserver.dir/lst_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpserver.dir/lst_timer.cpp.i"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luhuitong/CmakeHttpServer/src/lst_timer.cpp > CMakeFiles/httpserver.dir/lst_timer.cpp.i

src/CMakeFiles/httpserver.dir/lst_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpserver.dir/lst_timer.cpp.s"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luhuitong/CmakeHttpServer/src/lst_timer.cpp -o CMakeFiles/httpserver.dir/lst_timer.cpp.s

src/CMakeFiles/httpserver.dir/server.cpp.o: src/CMakeFiles/httpserver.dir/flags.make
src/CMakeFiles/httpserver.dir/server.cpp.o: ../src/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/luhuitong/CmakeHttpServer/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/httpserver.dir/server.cpp.o"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/httpserver.dir/server.cpp.o -c /home/luhuitong/CmakeHttpServer/src/server.cpp

src/CMakeFiles/httpserver.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpserver.dir/server.cpp.i"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luhuitong/CmakeHttpServer/src/server.cpp > CMakeFiles/httpserver.dir/server.cpp.i

src/CMakeFiles/httpserver.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpserver.dir/server.cpp.s"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luhuitong/CmakeHttpServer/src/server.cpp -o CMakeFiles/httpserver.dir/server.cpp.s

# Object files for target httpserver
httpserver_OBJECTS = \
"CMakeFiles/httpserver.dir/buffer.cpp.o" \
"CMakeFiles/httpserver.dir/http_conn.cpp.o" \
"CMakeFiles/httpserver.dir/locker.cpp.o" \
"CMakeFiles/httpserver.dir/lst_timer.cpp.o" \
"CMakeFiles/httpserver.dir/server.cpp.o"

# External object files for target httpserver
httpserver_EXTERNAL_OBJECTS =

../bin/httpserver: src/CMakeFiles/httpserver.dir/buffer.cpp.o
../bin/httpserver: src/CMakeFiles/httpserver.dir/http_conn.cpp.o
../bin/httpserver: src/CMakeFiles/httpserver.dir/locker.cpp.o
../bin/httpserver: src/CMakeFiles/httpserver.dir/lst_timer.cpp.o
../bin/httpserver: src/CMakeFiles/httpserver.dir/server.cpp.o
../bin/httpserver: src/CMakeFiles/httpserver.dir/build.make
../bin/httpserver: src/CMakeFiles/httpserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/luhuitong/CmakeHttpServer/bulid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable ../../bin/httpserver"
	cd /home/luhuitong/CmakeHttpServer/bulid/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/httpserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/httpserver.dir/build: ../bin/httpserver

.PHONY : src/CMakeFiles/httpserver.dir/build

src/CMakeFiles/httpserver.dir/clean:
	cd /home/luhuitong/CmakeHttpServer/bulid/src && $(CMAKE_COMMAND) -P CMakeFiles/httpserver.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/httpserver.dir/clean

src/CMakeFiles/httpserver.dir/depend:
	cd /home/luhuitong/CmakeHttpServer/bulid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luhuitong/CmakeHttpServer /home/luhuitong/CmakeHttpServer/src /home/luhuitong/CmakeHttpServer/bulid /home/luhuitong/CmakeHttpServer/bulid/src /home/luhuitong/CmakeHttpServer/bulid/src/CMakeFiles/httpserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/httpserver.dir/depend

