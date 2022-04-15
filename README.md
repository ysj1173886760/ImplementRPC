Implement basic echo RPC example based on protobuf

Compile guide

```shell
mkdir build
cd build
cmake ..
make
```

Then you can run example/client and example/server

This tiny project is aimd to illustrate the procedure of a single RPC call and how it works

![20220415191202](https://picsheep.oss-cn-beijing.aliyuncs.com/pic/20220415191202.png)

This archtecture decoupled the message serializing/deserializing, service logic, server and communication

So you can use some high performance network lib to build channel/server module without interfering other parts

Once you've figured out how this works, next thing is do read source code of some real rpc frameworks and check how they can reach high performance. According what i've mentioned above, the one which brings you high performace must come from optimizing server/channel module.

Happy Coding.