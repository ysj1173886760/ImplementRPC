Implement basic echo RPC example based on protobuf

generate protobuf
```sh
cd src
protoc --proto_path=./ --cpp_out=./ rpc_meta.proto
```