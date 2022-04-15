file(GLOB proto_files ${PROJECT_SOURCE_DIR}/example/*.proto)
foreach(proto_file ${proto_files})
    string(REGEX REPLACE "[^/]proto" "" proto_file_name ${proto_file})
    list(APPEND proto_src_files "${proto_file_name}.pb.cc")
    add_custom_command(
        OUTPUT "${proto_file_name}.pb.h" "${proto_file_name}.pb.cc"
        COMMAND protoc --proto_path=${PROJECT_SOURCE_DIR}/example
                --cpp_out=${PROJECT_SOURCE_DIR}/example/ ${proto_file}
        DEPENDS ${proto_file}
    )
endforeach(proto_file)

include_directories(${PROJECT_SOURCE_DIR}/example)

add_executable(client echo_client.cpp ${proto_src_files})
add_executable(server echo_server.cpp ${proto_src_files})

target_link_libraries(client ${CMAKE_PROJECT_NAME}_lib pthread protobuf)
target_link_libraries(server ${CMAKE_PROJECT_NAME}_lib pthread protobuf)