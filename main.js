/**
 * Created by gberger on 15/04/15.
 */
var root = require('./root');

Web = require(root.web+'/web');

var web = new Web({});

web.startServer();
