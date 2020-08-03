# play-node-native-binding

## install C++ extension for vscode

It will be good if vscode supports c/cpp files, so we need to configure it at the beginning:

- install extension <https://code.visualstudio.com/docs/languages/cpp>
- config `minGW` <https://code.visualstudio.com/docs/cpp/config-mingw>
- or, just `msvc` <https://code.visualstudio.com/docs/cpp/config-msvc>

## install node-gyp globally

`node-gyp` is needed to generate the boilerplate code, and build native addons:

- install [windows-build-tools](https://github.com/felixrieseberg/windows-build-tools) if using windows, for example:

    ```bash
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

## build module

After all codes are prepared, we are ready to build code into native node addons. using `node-gyp`:

```bash
node-gyp build
```

Then in `./build/Release` directory, we got the `binding.node` file as node specific dynamic linking library module, which is ready for use like a normal node module.

We can use [node-bindings](https://www.npmjs.com/package/bindings) to help `require()` the produced native addon `.node` file.

## build for electron

It is easy to build a native binding for electron:

```bash
node-gyp rebuild --target=v<version> --dist-url=https://atom.io/download/electron
```

Basically it is the same as building for specific node version.

## What's more

Once there's a `binding.gyp` in the module root directory, the `npm`(and `yarn`) will automatically consider it as a native module, and run `node-gyp rebuild` everything after its installation. This is sometimes annoying, and seems there's no direct way to disable this behavior.

To bypass that, we can rename the `binding.gyp` as something else. But if we do that, `node-gyp` will not able to locate it, because this config file is [hardcoded](https://github.com/nodejs/node-gyp/blob/master/lib/configure.js#L332). What's more, there's no way to programmatically call `node-gyp` directly. These combination makes controlling it becomes really disgusting.

So if we really want to archive that, we had to rename the `binding.gyp` into something else, and write script to create the file back and then start a child process to run `node-gyp`.

## Todo

- [ ] create a dynamic link library and call it through native addon
- [ ] build for electron, packing & signing included

## references

- <https://nodejs.org/api/addons.html>
- <https://github.com/nodejs/nan>
- <https://github.com/nodejs/node-addon-api>
- <https://github.com/nodejs/node-addon-examples>
- <https://itnext.io/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32>
- <https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference>
- <https://stackoverflow.com/questions/61730307/node-js-native-addons-where-is-node-api-h-located>
- <https://github.com/nodejs/node-addon-api/blob/master/doc/callbackinfo.md>
- <https://iweiyun.github.io/2019/01/04/node-cpp-addon/>
- <https://github.com/nodejs/node-gyp/issues/1676>
- <https://github.com/nodejs/node-gyp/issues/682>
- <https://gyp.gsrc.io/docs/UserDocumentation.md>
