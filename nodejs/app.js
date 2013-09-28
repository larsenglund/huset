var express = require('express');
var app = express();
var port = 3700;
var io = require('socket.io').listen(app.listen(port));
console.log('Listening on port ' + port);


var rrd = require('rrd');
var rrd_file_root = '/home/lars/RRD/'
var rrd_filename = __dirname + '/test.rrd';
function now() { return Math.ceil((new Date).getTime() / 1000); }
//var now = Math.ceil((new Date).getTime() / 1000);

/*rrd.create(rrd_filename, 60, now(), 
    ["DS:temperature:GAUGE:600:-40:100", 
    "RRA:AVERAGE:0.5:1:10000", 
    "RRA:AVERAGE:0.5:10:1200000", 
    "RRA:MIN:0.5:40:300000", 
    "RRA:MAX:0.5:40:300000"],
    //["DS:busy:GAUGE:120:0:U", "RRA:LAST:0.5:1:60"],
    function (error) { 
    if (error) console.log("Error:", error);
    rrd.info(rrd_filename, function (info) {
        console.log(info.filename, info.rrd_version);
    }); 
});*/

/*
var RRDTool = require('node-rrdtool');
var rrd = new RRDTool();
var filename = __dirname + '/test.rrd';
var now = Math.ceil((new Date).getTime() / 1000);
rrd.version(function (err, data) {
    console.log('Version: ' + data);
});
*/

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
            var str = data.toString().trim();
            if (str.length == 0) {
                return;
            }
            console.log('serial received ' + data.toString().length + ' bytes:' +
                        ' trimmed string [' + str + ']');
            var parts = str.split(' ');
            if (parts[1] == 'ds18x20') { // && parts[3] == '16') {
                var sensor = appConfig.nodes['id' + parts[0]].ds18x20['pin' + parts[2] + 'addr' + parts[3]];
                //console.log(sensor);
                var temperature = parseFloat(parts[4]);
                console.log('Got new ds18x20 temperature ' + temperature + ' for ' + sensor.name + ', logging to ' + sensor.rrd.temperature);
                sensor.lastValue = temperature;
                //console.log(appConfig.nodes['id' + parts[0]].ds18x20['pin' + parts[2] + 'addr' + parts[3]]);
                rrd.update(rrd_file_root + sensor.rrd.temperature, 'temperature', [ [now(), temperature].join(':') ], function (error) {
                    console.log(error === null ? '.' : error);
                });
            }
            else if (parts[1] == 'dht11') {
                var sensor = appConfig.nodes['id' + parts[0]].dht11['pin' + parts[2]];
                var temperature = parseFloat(parts[3]);
                var humidity = parseFloat(parts[4]);
                console.log('Got new dht11 temperature ' + temperature + ' and humidity ' + humidity + 'for ' + sensor.name + ', logging to ' + sensor.rrds.temperature + ' and ' + sensor.rrds.humidity);
                sensor.lastValues.temperature = temperature;
                sensor.lastValues.humidity = humidity;
                rrd.update(rrd_file_root + sensor.rrds.temperature, 'temperature', [ [now(), temperature].join(':') ], function (error) {
                    console.log(error === null ? '.' : error);
                });
                rrd.update(rrd_file_root + sensor.rrds.humidity, 'humidity', [ [now(), humidity].join(':') ], function (error) {
                    console.log(error === null ? '.' : error);
                });
            }
            else if (parts[1] == 'radio') {
                var node = appConfig.nodes['id' + parts[0]];
                var temperature = parseInt(parts[2]);
                console.log('Got new radio temperature ' + temperature + ' for ' + node.name + ', logging to ' + node.rrds.temperature);
                node.lastValues.temperature = temperature;
                rrd.update(rrd_file_root + node.rrds.temperature, 'temperature', [ [now(), temperature].join(':') ], function (error) {
                    console.log(error === null ? '.' : error);
                });
            }
            else if (parts[0] == 'debug') {
                var node = appConfig.nodes['id' + parts[1]];
                var rssi = parseInt(parts[2]);
                console.log('Got new radio rssi ' + rssi + ' for ' + node.name + ', logging to ' + node.rrds.rssi);
                node.lastValues.rssi = rssi;
                rrd.update(rrd_file_root + node.rrds.rssi, 'rssi', [ [now(), rssi].join(':') ], function (error) {
                    console.log(error === null ? '.' : error);
                });
            }
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
