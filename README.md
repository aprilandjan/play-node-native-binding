# play-node-native-binding

## install C++ extension for vscode

It will be good if vscode supports c/cpp files, so we need to configure it at the beginning:

- install extension <https://code.visualstudio.com/docs/languages/cpp>
- config `minGW` <https://code.visualstudio.com/docs/cpp/config-mingw>
- or, just `msvc` <https://code.visualstudio.com/docs/cpp/config-msvc>

## install node-gyp globally

`node-gyp` is needed to generate the boilerplate code, and build native addons:

- install [windows-build-tools](https://github.com/felixrieseberg/windows-build-tools) if using windows, for example:
    
    ```
    yarn global add windows-build-tools --python-mirror=https://npm.taobao.org/mirrors/python/
    ```

- it's better set python related environments <https://datatofish.com/add-python-to-windows-path/>

- install [node-gyp](https://github.com/nodejs/node-gyp) globally

## gyp - generate your project

Time to start some coding. Create C files and make some functions, for example: 

```cpp
#include <string>

// just output hello
std::string hello(std::string name)
{
    return "hello, " + name + "!";
}
```

This is completely C++ code. To make it usable for node.js, we need to write some intermediate codes using ABI(abstract binary interface) provided by node.js.

Firstly, install `node-addon-api`:

```bash
yarn add --dev node-addon-api
```

This module provides `napi.h` while writing intermediate codes. If we want `vscode c++ intellisense` works well, we need add these headers into c++ project configurations in `./vscode/c_cpp_properties.json`:

```json
{
    "includePath": [
        "${workspaceFolder}/src/**",
        "${workspaceFolder}/node_modules/node-addon-api/**",
        "C:/Users/<user>/.node-gyp/<version>/include/node/**"
    ],
}
```

Now we are ready to write the intermediate codes. It is very likely to write a `module.exports` in C++, using `Napi` namespace things. The detailed implementations are in [main.cpp](./src/main.cpp).

## Building module

After all codes are prepared, we are ready to build code into native node addons. using `node-gyp`:

```bash
node-gyp build
```

Then in `./build/Release` directory, we got the `binding.node` file as node specific dynamic linking library module, which is ready for use like a normal node module.

## references

- <https://nodejs.org/api/addons.html>
- <https://github.com/nodejs/nan>
- <https://github.com/nodejs/node-addon-api>
- <https://github.com/nodejs/node-addon-examples>
- <https://itnext.io/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32>
- <https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference>
- <https://stackoverflow.com/questions/61730307/node-js-native-addons-where-is-node-api-h-located>