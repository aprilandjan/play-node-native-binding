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

## references

- <https://nodejs.org/api/addons.html>
- <https://github.com/nodejs/nan>
- <https://github.com/nodejs/node-addon-api>
- <https://github.com/nodejs/node-addon-examples>
- <https://itnext.io/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32>
