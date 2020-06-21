const assert = require('assert');
const hello = require('./entry');

console.log(hello);
assert(typeof hello === 'function', 'the module exports is a function');

console.log(hello('Foo'));
assert(hello('Foo') === 'Hello, Foo!', 'the hello function correctly returned');
