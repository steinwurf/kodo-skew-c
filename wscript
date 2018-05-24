#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = 'kodo-slide-c'
VERSION = '2.0.0'

def build(bld):

    CXX = bld.env.get_flat("CXX")
    # Matches both g++ and clang++
    if 'g++' in CXX or 'clang' in CXX:
        # The -fPIC flag is required for all underlying static libraries that
        # will be included in the shared library
        bld.env.append_value('CXXFLAGS', '-fPIC')

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_KODO_SLIDE_C_VERSION="{}"'.format(VERSION))

    # Build the kodo-slide-c static library
    bld.stlib(
        source=bld.path.ant_glob('src/kodo_slide_c/*.cpp'),
        target='kodo_slide_c_static',
        name='kodo_slide_c_static',
        defines=['KODO_SLIDE_C_STATIC'],
        export_defines=['KODO_SLIDE_C_STATIC'],
        export_includes='src',
        use=['kodo_slide'])

    # Build the kodo-slide-c shared library
    bld.shlib(
        source=bld.path.ant_glob('src/kodo_slide_c/*.cpp'),
        target='kodo_slide_c',
        name='kodo_slide_c',
        defines=['KODO_SLIDE_C_DLL_EXPORTS'],
        install_path=None,
        export_includes='src',
        use=['kodo_slide'])

    if bld.is_toplevel():

        bld.recurse('test')

        # Install kodo_slide_c.h to the 'include' folder
        if bld.has_tool_option('install_path'):
            install_path = bld.get_tool_option('install_path')
            install_path = os.path.abspath(os.path.expanduser(install_path))
            start_dir = bld.path.find_dir('src')
            bld.install_files(
                os.path.join(install_path, 'include'),
                start_dir.ant_glob('**/*.h'),
                cwd=start_dir,
                relative_trick=True)
