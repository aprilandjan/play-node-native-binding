const addon = require('./build/Release/binding.node');


//  just export what is needed
module.exports = addon.hello;