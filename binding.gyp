{
  "targets": [
    {
      "target_name": "chatglm",
      "sources": [
        "src/main.cpp",
        "src/libs/chat/build/lib/libchat.a"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src/libs/chat"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "conditions": [
        ["OS=='win'", {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1
            }
          }
        }],
        ["OS=='mac'", {
          "xcode_settings": {
            "CLANG_CXX_LIBRARY": "libc++",
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "MACOSX_DEPLOYMENT_TARGET": "10.7"
          }
        }]
      ]
    }
  ]
}
