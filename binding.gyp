{
  "targets": [
    {
      "target_name": "binding",
      "sources": [
        "./src/hello.cpp",
        "./src/main.cpp",
        "./src/OCRunLoopContext.mm",
      ],
      "link_settings": {
        "libraries": [
          "-framework Foundation"
        ]
      },
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "xcode_settings": {
        "GCC_OPTIMIZATION_LEVEL": "s",
        "COPY_PHASE_STRIP": "YES",
        "DEAD_CODE_STRIPPING": "YES",
        "STRIP_INSTALLED_PRODUCT": "YES",
        "DEBUG_INFORMATION_FORMAT": "dwarf-with-dsym",
        "DEPLOYMENT_POSTPROCESSING": "YES",
        "OTHER_CPLUSPLUSFLAGS" : ['-ObjC++', '-std=c++11', '-stdlib=libc++',  '-fvisibility=hidden','-frtti'],
        "OTHER_LDFLAGS": [ "-stdlib=libc++"],
      },
    }
  ]
}
