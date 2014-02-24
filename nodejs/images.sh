#! /bin/sh
rrdtool graph /home/lars/huset/nodejs/public/rrd_images/RSSI_24H.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="RSSI 24H" \
--alt-y-grid \
--slope-mode --start -24h \
--upper-limit 0 --lower-limit -110 \
DEF:a=/home/lars/RRD/RadioRSSIStorage.rrd:rssi:AVERAGE \
LINE:a#ff1c1c:"Storage room\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/RSSI_weekly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="RSSI weekly" \
--alt-y-grid \
--slope-mode --start -7d \
--upper-limit 0 --lower-limit -110 \
DEF:a=/home/lars/RRD/RadioRSSIStorage.rrd:rssi:AVERAGE \
LINE:a#ff1c1c:"Storage room\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Humidity_weekly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Humidity weekly" \
--slope-mode --start -7d \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/DHTHumidityStorage.rrd:humidity:AVERAGE \
DEF:b=/home/lars/RRD/DHTHumidityHallway.rrd:humidity:AVERAGE \
DEF:c=/home/lars/RRD/DHTHumidityRoof.rrd:humidity:AVERAGE \
LINE:a#ff1c1c:"Storage room\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#ffc000:"Ceiling\t\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Humidity_yearly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Humidity yearly" \
--slope-mode --start -1y \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/DHTHumidityStorage.rrd:humidity:AVERAGE \
DEF:b=/home/lars/RRD/DHTHumidityHallway.rrd:humidity:AVERAGE \
DEF:c=/home/lars/RRD/DHTHumidityRoof.rrd:humidity:AVERAGE \
LINE:a#ff1c1c:"Storage room\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#ffc000:"Ceiling\t\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Humidity_monthly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Humidity monthly" \
--slope-mode --start -1mon \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/DHTHumidityStorage.rrd:humidity:AVERAGE \
DEF:b=/home/lars/RRD/DHTHumidityHallway.rrd:humidity:AVERAGE \
DEF:c=/home/lars/RRD/DHTHumidityRoof.rrd:humidity:AVERAGE \
LINE:a#ff1c1c:"Storage room\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#ffc000:"Ceiling\t\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Temperature_weekly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Temperature weekly" \
--alt-y-grid --slope-mode --start -7d \
--upper-limit 40 --lower-limit -10 \
DEF:a=/home/lars/RRD/DHTTemperatureStorage.rrd:temperature:AVERAGE \
DEF:b=/home/lars/RRD/DHTTemperatureHallway.rrd:temperature:AVERAGE \
DEF:c=/home/lars/RRD/DS1820TemperatureStorage.rrd:temperature:AVERAGE \
DEF:d=/home/lars/RRD/DS1820TemperaturePorch.rrd:temperature:AVERAGE \
DEF:e=/home/lars/RRD/DHTTemperatureRoof.rrd:temperature:AVERAGE \
DEF:f=/home/lars/RRD/RadioTemperatureStorage.rrd:temperature:AVERAGE \
LINE:a#ff1c1c:"Storage room (DHT)\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway (DHT)\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#3df325:"Storage room (DS1820)\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n" \
LINE:d#df60ff:"Porch (DS1820)\t\t" \
GPRINT:d:LAST:"Nu\:%6.2lf%s" \
GPRINT:d:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:d:MAX:"Max\:%6.2lf%s" \
GPRINT:d:MIN:"Min\:%6.2lf%s\n" \
LINE:e#ffc000:"Ceiling (DHT)\t\t" \
GPRINT:e:LAST:"Nu\:%6.2lf%s" \
GPRINT:e:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:e:MAX:"Max\:%6.2lf%s" \
GPRINT:e:MIN:"Min\:%6.2lf%s\n" \
LINE:f#fcc00f:"Storage room (radio)\t" \
GPRINT:f:LAST:"Nu\:%6.2lf%s" \
GPRINT:f:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:f:MAX:"Max\:%6.2lf%s" \
GPRINT:f:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Temperature_yearly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Temperature yearly" \
--alt-y-grid --slope-mode --start -1y \
--upper-limit 40 --lower-limit -10 \
DEF:a=/home/lars/RRD/DHTTemperatureStorage.rrd:temperature:AVERAGE \
DEF:b=/home/lars/RRD/DHTTemperatureHallway.rrd:temperature:AVERAGE \
DEF:c=/home/lars/RRD/DS1820TemperatureStorage.rrd:temperature:AVERAGE \
DEF:d=/home/lars/RRD/DS1820TemperaturePorch.rrd:temperature:AVERAGE \
DEF:e=/home/lars/RRD/DHTTemperatureRoof.rrd:temperature:AVERAGE \
DEF:f=/home/lars/RRD/RadioTemperatureStorage.rrd:temperature:AVERAGE \
LINE:a#ff1c1c:"Storage room (DHT)\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway (DHT)\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#3df325:"Storage room (DS1820)\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n" \
LINE:d#df60ff:"Porch (DS1820)\t\t" \
GPRINT:d:LAST:"Nu\:%6.2lf%s" \
GPRINT:d:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:d:MAX:"Max\:%6.2lf%s" \
GPRINT:d:MIN:"Min\:%6.2lf%s\n" \
LINE:e#ffc000:"Ceiling (DHT)\t\t" \
GPRINT:e:LAST:"Nu\:%6.2lf%s" \
GPRINT:e:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:e:MAX:"Max\:%6.2lf%s" \
GPRINT:e:MIN:"Min\:%6.2lf%s\n" \
LINE:f#fcc00f:"Storage room (radio)\t" \
GPRINT:f:LAST:"Nu\:%6.2lf%s" \
GPRINT:f:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:f:MAX:"Max\:%6.2lf%s" \
GPRINT:f:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Temperature_monthly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Temperature monthly" \
--alt-y-grid --slope-mode --start -1mon \
--upper-limit 40 --lower-limit -10 \
DEF:a=/home/lars/RRD/DHTTemperatureStorage.rrd:temperature:AVERAGE \
DEF:b=/home/lars/RRD/DHTTemperatureHallway.rrd:temperature:AVERAGE \
DEF:c=/home/lars/RRD/DS1820TemperatureStorage.rrd:temperature:AVERAGE \
DEF:d=/home/lars/RRD/DS1820TemperaturePorch.rrd:temperature:AVERAGE \
DEF:e=/home/lars/RRD/DHTTemperatureRoof.rrd:temperature:AVERAGE \
DEF:f=/home/lars/RRD/RadioTemperatureStorage.rrd:temperature:AVERAGE \
LINE:a#ff1c1c:"Storage room (DHT)\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway (DHT)\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#3df325:"Storage room (DS1820)\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n" \
LINE:d#df60ff:"Porch (DS1820)\t\t" \
GPRINT:d:LAST:"Nu\:%6.2lf%s" \
GPRINT:d:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:d:MAX:"Max\:%6.2lf%s" \
GPRINT:d:MIN:"Min\:%6.2lf%s\n" \
LINE:e#ffc000:"Ceiling (DHT)\t\t" \
GPRINT:e:LAST:"Nu\:%6.2lf%s" \
GPRINT:e:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:e:MAX:"Max\:%6.2lf%s" \
GPRINT:e:MIN:"Min\:%6.2lf%s\n" \
LINE:f#fcc00f:"Storage room (radio)\t" \
GPRINT:f:LAST:"Nu\:%6.2lf%s" \
GPRINT:f:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:f:MAX:"Max\:%6.2lf%s" \
GPRINT:f:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Humidity_24H.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Humidity 24H" \
--slope-mode --start -24h \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/DHTHumidityStorage.rrd:humidity:AVERAGE \
DEF:b=/home/lars/RRD/DHTHumidityHallway.rrd:humidity:AVERAGE \
DEF:c=/home/lars/RRD/DHTHumidityRoof.rrd:humidity:AVERAGE \
LINE:a#ff1c1c:"Storage room\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#ffc000:"Ceiling\t\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Temperature_24H.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Temperature 24H" \
--alt-y-grid --slope-mode --start -24h \
--upper-limit 40 --lower-limit -10 \
DEF:a=/home/lars/RRD/DHTTemperatureStorage.rrd:temperature:AVERAGE \
DEF:b=/home/lars/RRD/DHTTemperatureHallway.rrd:temperature:AVERAGE \
DEF:c=/home/lars/RRD/DS1820TemperatureStorage.rrd:temperature:AVERAGE \
DEF:d=/home/lars/RRD/DS1820TemperaturePorch.rrd:temperature:AVERAGE \
DEF:e=/home/lars/RRD/DHTTemperatureRoof.rrd:temperature:AVERAGE \
DEF:f=/home/lars/RRD/RadioTemperatureStorage.rrd:temperature:AVERAGE \
LINE:a#ff1c1c:"Storage room (DHT)\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway (DHT)\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#3df325:"Storage room (DS1820)\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n" \
LINE:d#df60ff:"Porch (DS1820)\t\t" \
GPRINT:d:LAST:"Nu\:%6.2lf%s" \
GPRINT:d:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:d:MAX:"Max\:%6.2lf%s" \
GPRINT:d:MIN:"Min\:%6.2lf%s\n" \
LINE:e#ffc000:"Ceiling (DHT)\t\t" \
GPRINT:e:LAST:"Nu\:%6.2lf%s" \
GPRINT:e:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:e:MAX:"Max\:%6.2lf%s" \
GPRINT:e:MIN:"Min\:%6.2lf%s\n" \
LINE:f#fcc00f:"Storage room (radio)\t" \
GPRINT:f:LAST:"Nu\:%6.2lf%s" \
GPRINT:f:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:f:MAX:"Max\:%6.2lf%s" \
GPRINT:f:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Power_weekly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Power weekly" \
--slope-mode --start -7d \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/ElectricPower.rrd:power:AVERAGE \
DEF:b=/home/lars/RRD/HeatPower.rrd:heatpower:AVERAGE \
LINE:a#0000FF:"Electric\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#FF0000:"Heat\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Power_monthly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Power monthly" \
--slope-mode --start -1mon \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/ElectricPower.rrd:power:AVERAGE \
DEF:b=/home/lars/RRD/HeatPower.rrd:heatpower:AVERAGE \
LINE:a#0000FF:"Electric\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#FF0000:"Heat\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Power_24H.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Power 24H" \
--slope-mode --start -24h \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/ElectricPower.rrd:power:AVERAGE \
DEF:b=/home/lars/RRD/HeatPower.rrd:heatpower:AVERAGE \
LINE:a#0000FF:"Electric\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#FF0000:"Heat\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Humidity_2H.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Humidity 2H" \
--slope-mode --start -2h \
--upper-limit 100 --lower-limit 20 \
DEF:a=/home/lars/RRD/DHTHumidityStorage.rrd:humidity:AVERAGE \
DEF:b=/home/lars/RRD/DHTHumidityHallway.rrd:humidity:AVERAGE \
DEF:c=/home/lars/RRD/DHTHumidityRoof.rrd:humidity:AVERAGE \
LINE:a#ff1c1c:"Storage room\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#ffc000:"Ceiling\t\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Temperature_2H.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Temperature 2H" \
--alt-y-grid --slope-mode --start -2h \
--upper-limit 40 --lower-limit -10 \
DEF:a=/home/lars/RRD/DHTTemperatureStorage.rrd:temperature:AVERAGE \
DEF:b=/home/lars/RRD/DHTTemperatureHallway.rrd:temperature:AVERAGE \
DEF:c=/home/lars/RRD/DS1820TemperatureStorage.rrd:temperature:AVERAGE \
DEF:d=/home/lars/RRD/DS1820TemperaturePorch.rrd:temperature:AVERAGE \
DEF:e=/home/lars/RRD/DHTTemperatureRoof.rrd:temperature:AVERAGE \
DEF:f=/home/lars/RRD/RadioTemperatureStorage.rrd:temperature:AVERAGE \
LINE:a#ff1c1c:"Storage room (DHT)\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#6082ff:"Hallway (DHT)\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n" \
LINE:c#3df325:"Storage room (DS1820)\t" \
GPRINT:c:LAST:"Nu\:%6.2lf%s" \
GPRINT:c:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:c:MAX:"Max\:%6.2lf%s" \
GPRINT:c:MIN:"Min\:%6.2lf%s\n" \
LINE:d#df60ff:"Porch (DS1820)\t\t" \
GPRINT:d:LAST:"Nu\:%6.2lf%s" \
GPRINT:d:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:d:MAX:"Max\:%6.2lf%s" \
GPRINT:d:MIN:"Min\:%6.2lf%s\n" \
LINE:e#ffc000:"Ceiling (DHT)\t\t" \
GPRINT:e:LAST:"Nu\:%6.2lf%s" \
GPRINT:e:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:e:MAX:"Max\:%6.2lf%s" \
GPRINT:e:MIN:"Min\:%6.2lf%s\n" \
LINE:f#fcc00f:"Storage room (radio)\t" \
GPRINT:f:LAST:"Nu\:%6.2lf%s" \
GPRINT:f:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:f:MAX:"Max\:%6.2lf%s" \
GPRINT:f:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Power_2H.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Power 2H" \
--slope-mode --start -2h \
--upper-limit 100 --lower-limit 0 \
DEF:a=/home/lars/RRD/ElectricPower.rrd:power:AVERAGE \
DEF:b=/home/lars/RRD/HeatPower.rrd:heatpower:AVERAGE \
LINE:a#0000FF:"Electric\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#FF0000:"Heat\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n"

rrdtool graph /home/lars/huset/nodejs/public/rrd_images/Power_yearly.png \
--height=350 \
--width=900 \
COMMENT:"\n" \
--vertical-label="Power yearly" \
--slope-mode --start -1y \
--upper-limit 100 --lower-limit 0 \
DEF:a=/home/lars/RRD/ElectricPower.rrd:power:AVERAGE \
DEF:b=/home/lars/RRD/HeatPower.rrd:heatpower:AVERAGE \
LINE:a#0000FF:"Electric\t" \
GPRINT:a:LAST:"Nu\:%6.2lf%s" \
GPRINT:a:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:a:MAX:"Max\:%6.2lf%s" \
GPRINT:a:MIN:"Min\:%6.2lf%s\n" \
LINE:b#FF0000:"Heat\t\t" \
GPRINT:b:LAST:"Nu\:%6.2lf%s" \
GPRINT:b:AVERAGE:"Medel\:%6.2lf%s" \
GPRINT:b:MAX:"Max\:%6.2lf%s" \
GPRINT:b:MIN:"Min\:%6.2lf%s\n"

# ncftpput -u englund -p tomtas vm1.i4s.se CAN_data /home/lars/huset/nodejs/public/rrd_images/*

# cp /home/lars/huset/nodejs/public/rrd_images/* /home/lars/.gvfs/FTP\ as\ englund\ on\ vm1.i4s.se/
#cd /var/www
#wput CAN_data ftp://englund:tomtas@vm1.i4s.se

#HOST='vm1.i4s.se'
#USER='englund'
#PASSWD='tomtas'
#ftp -n -v $HOST << EOT
#user $USER $PASSWD
#cd CAN_data
#mput *.*
#bye
#EOT

