# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

from conans import ConanFile, tools, CMake
import os
from pathlib import Path


class QtConanError(Exception):
    pass


class QtDeviceUtilities(ConanFile):
    name = "qtdeviceutilities"
    version = "6.4.0"
    license = "GPL-3.0, Commercial Qt License Agreement"
    author = "The Qt Company <https://www.qt.io/contact-us>"
    url = "https://code.qt.io/cgit/qt/qtdeviceutilities.git/"
    description = "Qt Device Utilities provides functionality that is useful for controlling settings in embedded applications."
    topics = ("qt", "qt6", "deviceutilities", "device creation", "network")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False, "default"],
               "qt6": "ANY"}  # this is needed to model unique package_id for the Add-on build per used Qt6 version
    default_options = {"shared": "default",  # default: Use the value of the Qt build
                       "qt6": None}
    exports_sources = "*", "!conan*.*"
    # use commit ID as the RREV (recipe revision) if this is exported from .git repository
    revision_mode = "scm" if Path(Path(__file__).parent.resolve() / ".git").exists() else "hash"

    def source(self):
        # sources are installed next to recipe, no need to clone etc. sources here
        pass

    def _get_cmake_prefix_path(self):
        # 'QTDIR' provided as env variable in profile file which is part of the Qt essential binary
        # package(s). Installed under .conan/profiles
        cmake_prefix_path = os.environ.get("QTDIR")
        if not cmake_prefix_path:
            raise QtConanError("'QTDIR' not defined! The 'QTDIR' needs to point to Qt installation directory.")
        print(f"CMAKE_PREFIX_PATH for '{self.name}/{self.version}' build is: {cmake_prefix_path}")
        return cmake_prefix_path

    def _read_env(self, key):
        value = os.environ.get(key)
        if not value:
            raise QtConanError(f"{self.settings.os} build specified but '{key}' was not defined?")
        return value

    def _get_qtcmake(self):
        qt_install_path = self._get_cmake_prefix_path()
        ext = ".bat" if tools.os_info.is_windows else ""
        qtcmake = os.path.abspath(os.path.join(qt_install_path, "bin", "qt-cmake" + ext))
        if not os.path.exists(qtcmake):
            raise QtConanError(f"Unable to locate {qtcmake} from 'QTDIR': {qt_install_path}")
        return qtcmake

    def _get_cmake_tool(self):
        cmake = CMake(self, cmake_program=self._get_qtcmake())
        cmake.verbose = True

        # Qt modules need to be 'installed'.
        # We need to direct the 'make install' to some directory under Conan cache,
        # place it under the current build directory which is also under the Conan cache.
        # Note, the actual 'make install' is called in "package()".
        install_dir = os.path.join(os.getcwd(), "_install_tmp")
        cmake.definitions["CMAKE_INSTALL_PREFIX"] = install_dir

        # Use the value of the Qt build
        if self.options.shared.value == "default":
            del cmake.definitions["BUILD_SHARED_LIBS"]

        cmake_toolchain_file = os.environ.get("CMAKE_TOOLCHAIN_FILE")
        if cmake_toolchain_file:
            cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = cmake_toolchain_file

        if self.settings.os == "Android":
            cmake.definitions["ANDROID_SDK_ROOT"] = self._read_env('ANDROID_SDK_ROOT')
            cmake.definitions["ANDROID_NDK_ROOT"] = self._read_env('ANDROID_NDK_ROOT')

        if self.settings.os == "iOS":
            # Instead of Conan's auto-added 'Darwin', explicitly pass 'iOS'.
            cmake.definitions["CMAKE_SYSTEM_NAME"] = "iOS"

            # Remove the explicit sysroot, let CMake detect the sysroots, to ensure
            # that multi-arch builds work.
            del cmake.definitions["CMAKE_OSX_SYSROOT"]

            # Remove the conan provided architecture, instead rely on the architectures set
            # by the Qt toolchain file, which with official Qt packages most likely means
            # multi-arch iOS.
            del cmake.definitions["CMAKE_OSX_ARCHITECTURES"]

        return cmake

    def build(self):
        cmake = self._get_cmake_tool()
        self.run('%s "%s" %s' % (self._get_qtcmake(), self.source_folder, cmake.command_line))
        self.run('cmake --build . %s' % cmake.build_config)

    def package(self):
        install_dir = os.path.join(os.getcwd(), "_install_tmp")  # see 'CMAKE_INSTALL_PREFIX' above
        self.run('cmake --build . --target install')
        self.copy("*", src=install_dir, dst=".")

    def package_info(self):
        self.cpp_info.libs = ["Qt6NetworkSettings"]  # used for the actual library filename, Ordered list with the library names

    def deploy(self):
        self.copy("*")  # copy from current package
        self.copy_deps("*")  # copy from dependencies

