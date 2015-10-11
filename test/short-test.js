var util = require('util');

var pseudoFS = require('../index.js');

var path = "/proc/meminfo";
console.log('test reading a procfs thing: ' + path);
pseudoFS.readPseudo(path,function(err,data){
	console.log("readPseudo callback...");
	if(err) {
		console.error("Error: " + util.inspect(err));
	} else
	    console.log("output: " + data);
});

path = "/proc/meminfo2"; // non existant
console.log('test reading a procfs thing: ' + path);
pseudoFS.readPseudo(path,function(err,data){
	console.log("readPseudo callback (error call)...");
	if(err) {
		console.error("Expected Error (ok): " + util.inspect(err));
	} else
	    console.log("output: " + data);
});
