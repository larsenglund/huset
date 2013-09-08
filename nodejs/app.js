var express = require('express');
var app = express();
var port = 3700;
var io = require('socket.io').listen(app.listen(port));
console.log('Listening on port ' + port);

/*
var rrd = require('rrd');
var filename = __dirname + '/test.rrd';
var now = Math.ceil((new Date).getTime() / 1000);

rrd.create(filename, 60, now, ["DS:busy:GAUGE:120:0:U", "RRA:LAST:0.5:1:60"], function (error) { 
    if (error) console.log("Error:", error);
    rrd.info(filename, function (info) {
        console.log(info.filename, info.rrd_version);
    }); 
});
*/

var RRDTool = require('node-rrdtool');
var rrd = new RRDTool();
var filename = __dirname + '/test.rrd';
var now = Math.ceil((new Date).getTime() / 1000);
rrd.version(function (err, data) {
    console.log('Version: ' + data);
});

var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var mySerialPort;

serialport.list(function (err, ports) {
    console.log('Available serial ports;');
    ports.forEach(function(port) {
        console.log(' comName: ' + port.comName);
        console.log(' pnpId: ' + port.pnpId);
        console.log(' manufacturer: ' + port.manufacturer);
    });
    console.log('Using first port ' + ports[0].comName);
    mySerialPort = new SerialPort(ports[0].comName, {
        baudrate: 115200,
        parser: serialport.parsers.readline("\n")
    });

    mySerialPort.on("open", function () {
        console.log('open');
        mySerialPort.on('data', function(data) {
            console.log('data received: (' + data.toString().length);
            console.log(data);
            console.log(')');
            var str = data.toString().trim();
            //io.sockets.emit('flip', {flipid: 'lamp1', state: (str == '1' ? 'on' : 'off')});
        });  
        /*serialPort.write("ls\n", function(err, results) {
            console.log('err ' + err);
            console.log('results ' + results);
        });*/
    });
});



app.set('views', __dirname + '/views');
app.set('view engine', "jade");
app.engine('jade', require('jade').__express);
app.use(express.static(__dirname + '/public'));

app.get('/', function(req, res){
    res.render('index', appConfig);
});

count = 0;
io.sockets.on('connection', function (socket) {
    socket.on('flip', function (data) {
        console.log(data);
        if (data.flipid == 'lamp2') {
            mySerialPort.write((data.state == 'on' ? '1' : '0'), function(err, results) {
                console.log('err ' + err);
                console.log('results ' + results);
            }); 
        }
        var arr = data.flipid.split("_");
        var idx = parseInt(arr[1],10);
        console.log("IDX: " + idx)
        var my_switch = appConfig.remoteReceivers[idx];
        var my_remote;
        var my_button;

        for (var remote in appConfig.remotes) {
            if (appConfig.remotes[remote].id_name == my_switch.remote) {
                my_remote = appConfig.remotes[remote];
                break;
            }
        }
        console.log("Switch: " + my_switch.name);
        console.log("Remote: " + my_remote.name);
        console.log(my_switch);
        var cmd_str;
        if (my_remote.type == 'new') {
            cmd_str = '433 new ' + my_remote.period + ' ' + my_remote.address + ' ' +
                my_remote.buttons[my_switch.buttons] + ' ' + data.state + "\n";
        }
        else if (my_remote.type == 'old') {
            cmd_str = '433 old ' + my_remote.period + ' ' + my_remote.buttons[my_switch.buttons][data.state] + "\n";
        }
        console.log("Sending: " + cmd_str);
        mySerialPort.write(cmd_str, function(err, results) {
            console.log('err ' + err);
            console.log('results ' + results);
        });
        appConfig.remoteReceivers[idx].lastState = data.state;
        socket.broadcast.emit('flip', data);
    });
    socket.on('getPageState', function (data) {
        console.log('getPageState for page ' + data.pageId);
        socket.emit();
    });
});

/*setInterval(function () {
        count++;
        console.log('Flip ' + 1 + ' to ' + (count%2));
        io.sockets.emit('flip', {flipid: 'lamp1', state: (count%2 ? 'on' : 'off')});
        io.sockets.emit('temp', {tempid: 'temp1', value: count});
    }, 5000);
*/
//Save last values to JSON file (only save json object on exit?)
//lastValues = {};



var appConfig;
var fs = require('fs');
var file = __dirname + '/app_config.json';

fs.readFile(file, 'utf8', function (err, data) {
    if (err) {
        console.log('Error: ' + err);
        return;
    }

    appConfig = JSON.parse(data);

    //console.dir(data);
    //console.dir(data.remoteReceivers);
    appConfig.remoteReceivers.forEach(function (receiver) {
        if (receiver.type == 'switch') {
            console.log(receiver.name);
        }
    });

    // Code to save the json appconfig
    /*var outputFilename = __dirname + '/app_config.json';

    fs.writeFile(outputFilename, JSON.stringify(appConfig, null, 4), function(err) {
        if(err) {
         console.log(err);
        } 
        else {
            console.log("JSON saved to " + file);
        }
    }); */
});
