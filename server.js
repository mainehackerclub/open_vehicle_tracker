var http = require('http');
var static = require('node-static');
var file = new(static.Server)('.');

// Create HTTP server
var serv = http.createServer(function (req, res) {
    console.log('HTTP Connection');
    req.addListener('end', function () {
    // Serve files!
        file.serve(req, res);
    });
}).listen(1350);

console.log('HTTP Server running at http://127.0.0.1:1350/');
