AR = '/usr/bin/ar'
ARFLAGS = 'rcs'
BINDIR = '/usr/local/bin'
CC_VERSION = ('4', '4', '5')
COMPILER_CXX = 'g++'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/g++']
CXXFLAGS = ['-g', '-O3', '-Wall', '-std=c++0x']
CXXFLAGS_MACBUNDLE = ['-fPIC']
CXXFLAGS_cxxshlib = ['-fPIC']
CXXLNK_SRC_F = ''
CXXLNK_TGT_F = ['-o', '']
CXX_NAME = 'gcc'
CXX_SRC_F = ''
CXX_TGT_F = ['-c', '-o', '']
DEFINES = []
DEFINES_ST = '-D%s'
DEST_BINFMT = 'elf'
DEST_CPU = 'x86_64'
DEST_OS = 'linux'
INCLUDES = ['/usr/include', '/usr/include/SDL', '/usr/include/GL']
LIB = ['SDL', 'SDL_image', 'GL', 'GLU', 'glut', 'GLEW']
LIBDIR = '/usr/local/lib'
LIBPATH = ['/usr/lib']
LIBPATH_ST = '-L%s'
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_cxxshlib = ['-shared']
LINKFLAGS_cxxstlib = ['-Wl,-Bstatic']
LINK_CXX = ['/usr/bin/g++']
PREFIX = '/usr/local'
RPATH_ST = '-Wl,-rpath,%s'
SHLIB_MARKER = '-Wl,-Bdynamic'
SONAME_ST = '-Wl,-h,%s'
STLIBPATH_ST = '-L%s'
STLIB_MARKER = '-Wl,-Bstatic'
STLIB_ST = '-l%s'
cfg_files = ['/home/compendium/Code/Pew-Pew/src/config.h']
cxxprogram_PATTERN = '%s'
cxxshlib_PATTERN = 'lib%s.so'
cxxstlib_PATTERN = 'lib%s.a'
define_key = []
macbundle_PATTERN = '%s.bundle'
