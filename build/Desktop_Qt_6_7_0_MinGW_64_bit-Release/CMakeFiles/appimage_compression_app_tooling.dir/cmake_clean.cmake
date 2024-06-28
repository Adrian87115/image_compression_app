file(REMOVE_RECURSE
  "image_compression_app/Main.qml"
  "image_compression_app/qml.qrc"
  "image_compression_app/resources/left_side/LeftSide.qml"
  "image_compression_app/resources/right_side/RightSide.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appimage_compression_app_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
