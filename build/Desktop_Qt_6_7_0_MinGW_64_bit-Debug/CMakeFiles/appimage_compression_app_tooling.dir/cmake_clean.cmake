file(REMOVE_RECURSE
  "image_compression_app/Main.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appimage_compression_app_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
