# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app\build\Desktop_Qt_6_7_0_MinGW_64_bit-Debug

# Utility rule file for appimage_compression_app_qmlimportscan.

# Include any custom commands dependencies for this target.
include CMakeFiles/appimage_compression_app_qmlimportscan.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/appimage_compression_app_qmlimportscan.dir/progress.make

CMakeFiles/appimage_compression_app_qmlimportscan: .qt_plugins/Qt6_QmlPlugins_Imports_appimage_compression_app.cmake

.qt_plugins/Qt6_QmlPlugins_Imports_appimage_compression_app.cmake: C:/Qt/6.7.0/mingw_64/bin/qmlimportscanner.exe
.qt_plugins/Qt6_QmlPlugins_Imports_appimage_compression_app.cmake: .rcc/qmake_image_compression_app.qrc
.qt_plugins/Qt6_QmlPlugins_Imports_appimage_compression_app.cmake: .rcc/appimage_compression_app_raw_qml_0.qrc
.qt_plugins/Qt6_QmlPlugins_Imports_appimage_compression_app.cmake: C:/Users/adria/Desktop/Adrian/projects/Qt/image_compression_app/Main.qml
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app\build\Desktop_Qt_6_7_0_MinGW_64_bit-Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running qmlimportscanner for appimage_compression_app"
	cd /d C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app && call C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app\build\Desktop_Qt_6_7_0_MinGW_64_bit-Debug\.qt\bin\qt_setup_tool_path.bat C:/Qt/6.7.0/mingw_64/./bin/qmlimportscanner.exe @C:/Users/adria/Desktop/Adrian/projects/Qt/image_compression_app/build/Desktop_Qt_6_7_0_MinGW_64_bit-Debug/.qt_plugins/Qt6_QmlPlugins_Imports_appimage_compression_app.rsp

appimage_compression_app_qmlimportscan: .qt_plugins/Qt6_QmlPlugins_Imports_appimage_compression_app.cmake
appimage_compression_app_qmlimportscan: CMakeFiles/appimage_compression_app_qmlimportscan
appimage_compression_app_qmlimportscan: CMakeFiles/appimage_compression_app_qmlimportscan.dir/build.make
.PHONY : appimage_compression_app_qmlimportscan

# Rule to build all files generated by this target.
CMakeFiles/appimage_compression_app_qmlimportscan.dir/build: appimage_compression_app_qmlimportscan
.PHONY : CMakeFiles/appimage_compression_app_qmlimportscan.dir/build

CMakeFiles/appimage_compression_app_qmlimportscan.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\appimage_compression_app_qmlimportscan.dir\cmake_clean.cmake
.PHONY : CMakeFiles/appimage_compression_app_qmlimportscan.dir/clean

CMakeFiles/appimage_compression_app_qmlimportscan.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app\build\Desktop_Qt_6_7_0_MinGW_64_bit-Debug C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app\build\Desktop_Qt_6_7_0_MinGW_64_bit-Debug C:\Users\adria\Desktop\Adrian\projects\Qt\image_compression_app\build\Desktop_Qt_6_7_0_MinGW_64_bit-Debug\CMakeFiles\appimage_compression_app_qmlimportscan.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/appimage_compression_app_qmlimportscan.dir/depend

