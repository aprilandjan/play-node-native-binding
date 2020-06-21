// const addon = require('./build/Release/binding.node');
const addon = require('bindings')('binding.node');


//  just export what is needed
module.exports = addon.hello;