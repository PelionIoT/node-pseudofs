# This is a generated file, modify: generate/templates/binding.gyp.ejs.
{
  "targets": [
    {
      "target_name": "pseudofs",

      "sources": [
        "pseudo_fs.cc",
        "error-common.cc"
      ],

     "include_dirs": [
		"deps/build/include",
		"node_modules/nan",
		"<!(node -e \"require('nan')\")"    # note, this is needed b/c with 'npm install' it places 'nan' in a different location - this may break in Yocto bb recipe
      ],

      "cflags": [
        "-Wall",
        "-std=c++11",
        "-D_ERRCMN_ADD_CONSTS"
      ],

      "conditions": [
        [
          "OS=='linux'", {
          "configurations" : {
            "Release" : {
            },
            "Debug" : {
              "defines" : [ "ERRCMN_DEBUG_BUILD", "PSEUDOFS_DEBUG_BUILD" ]
            }
          }
          }
        ],        
        [
          "OS=='mac'", {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "WARNING_CFLAGS": [
                "-Wno-unused-variable",
              ],
            }
          }
        ]
      ]
    },
  ]
}
