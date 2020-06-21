const hello = require('./build/Release/binding.node');

console.log(hello);

console.log(hello.hello('Foo'));
