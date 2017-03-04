solution 'bzt'
   configurations 'release'
   language 'C'

   -- Compiler options.
   buildoptions {
      '-Wall',
      '-Werror', 
      '-s',
      '-pedantic',
      '-Wextra',
      '-Wno-error=switch',
      '-Wno-error=unused-parameter',
      '-Wno-unused',
   }

   flags {
      'Symbols',
   }

   includedirs {
      '.',
      'src',
      'src/lib',
   }

   project 'bzt'
      location 'build/bzt'
      kind 'ConsoleApp'

      linkoptions {
         '-ldl',
      }

      links {
         'lib',
         'dl',
      }

      buildoptions {
         '-std=c99',
      }

      -- Source files.
      files {
         'src/*.c',
         'src/rcon/*.c',
      }

   -- Administration module.
   project 'admin_module'
      location 'build/admin'
      kind 'SharedLib'
      targetdir 'modules/admin'
      targetname 'admin'
      targetprefix ''

      includedirs {
         'src/access/c',
      }

      files {
         'src/access/c/*.c',
         'modules/admin/src/admin.c',
      }

      buildoptions {
         '-std=c99',
      }

   -- luk module.
   project 'luk_module'
      location 'build/luk'
      kind 'SharedLib'
      targetdir 'modules/luk'
      targetname 'luk'
      targetprefix ''

      includedirs {
         'modules/luk/src/lib',
         'src/access/c',
      }

      files {
         'src/access/c/*.c',
         'modules/luk/src/*.c',
         'modules/luk/src/lib/*.c',
      }

      buildoptions {
         '-std=c99',
      }

   -- Library files.
   project 'lib'
      location 'build/lib'
      kind 'StaticLib'
      targetdir 'build/lib'

      files {
         'src/lib/*.c',
         'src/lib/conf/*.c',
         'src/lib/dir/*.c',
         'src/lib/usock/usock_linux.c',
         'src/lib/huffman/*.c',
         'src/lib/md5/md5.c',
         'src/lib/dlib/dlib_linux.c',
      }

      buildoptions {
         '-std=c99',
      }