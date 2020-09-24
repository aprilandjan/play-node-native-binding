const assert = require('assert');
const hello = require('./entry');

assert(typeof hello === 'function', 'the module exports is a function');

assert(hello('Foo') === 'Hello, Foo!', 'the hello function correctly returned');
