file(GLOB src_files "*.cpp")

# refering https://github.com/goyas/goya-rpc/blob/master/src/CMakeLists.txt
file(GLOB proto_files ${PROJECT_SOURCE_DIR}/src/*.proto)
foreach(proto_file ${proto_files})
    string(REGEX REPLACE "[^/]proto" "" proto_file_name ${proto_file})
    list(APPEND proto_src_files "${proto_file_name}.pb.cc")
    add_custom_command(
        OUTPUT "${proto_file_name}.pb.h" "${proto_file_name}.pb.cc"
        COMMAND protoc --proto_path=${PROJECT_SOURCE_DIR}/src
                --cpp_out=${PROJECT_SOURCE_DIR}/src/ ${proto_file}
        DEPENDS ${proto_file}
    )
endforeach(proto_file)

message(STATUS "proto source files: ${proto_src_files}")

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${src_files} ${proto_src_files})