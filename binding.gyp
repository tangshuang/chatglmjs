{
  "targets": [
    {
      "target_name": "chatglm",
      "sources": [
        "main.cpp",
        "chat.cpp",
        "libs/chatglm.cpp",
        "libs/chatglm.h",
        "libs/ggml-backend.h",
        "libs/ggml-meta.h",
        "libs/ggml.h",
        "libs/sentencepiece_processor.h"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'conditions': [
        ['OS=="win"', {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1
            }
          }
        }],
        ['OS=="mac"', {
          "xcode_settings": {
            "CLANG_CXX_LIBRARY": "libc++",
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.7'
          }
        }]
      ]
    }
  ]
}
