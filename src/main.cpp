// this `napi.h` is provided by node-api-addon module
#include <napi.h>
#include <string>
#include "hello.h"

//  The intermediate transformation for the original native function code
Napi::String _hello(const Napi::CallbackInfo & info) {
    Napi::Env env = info.Env();

    //  index `info` for function arguments
    //  cast argument into std::string type
    std::string name = (std::string) info[0].ToString();

    //  call function defined in other files
    //  make the params hard-coded for now
    std::string result = hello(name);

    //  return a new `Napi::String` value
    return Napi::String::New(env, result);
}

//  callback method when module is registered with node.js
Napi::Object _init(Napi::Env env, Napi::Object exports) {
    //  define property for `exports` object
    exports.Set(
        Napi::String::New(env, "hello"), // property name: "hello",
        Napi::Function::New(env, _hello) // property value: the function
    );

    //  always return this `exports` object
    return exports;
}

//  the declaration macro-function of node module
NODE_API_MODULE(hello, _init)