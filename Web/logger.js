/**
 * @brief MQTT related part
 */
var mqtt = require('mqtt');
var Topic = 'balcony/#';
var Broker_URL = 'mqtt://broker.hivemq.com:1883';

var options = {
    clientId: 'scraper',
    queueQoSZero: true,
    keepalive: 60
};

var client = mqtt.connect(Broker_URL, options);
client.on('connect', mqtt_connect);
client.on('reconnect', mqtt_reconnect);
client.on('error', mqtt_error);
client.on('message', mqtt_messsageReceived);
client.on('close', mqtt_close);

function mqtt_connect() {
    console.log("Connecting MQTT");
    client.subscribe(Topic, mqtt_subscribe);
};

function mqtt_subscribe(err, granted) {
    if (err) console.log(err);
};

function mqtt_reconnect(err) {
    console.log("Reconnect MQTT");
    if (err) console.log(err);
    client = mqtt.connect(Broker_URL, options);
};

function mqtt_error(err) {
    console.log("Error!");
    if (err) console.log(err);
};

function mqtt_messsageReceived(topic, message, packet) {
    var message_str = message.toString(); //convert byte array to string
    message_str = message_str.replace(/\n$/, ''); //remove new line
    if (message_str.length == 0) {
        console.log("Invalid payload");
    } else {
        insert_message(topic, message_str, packet);
    }
};

function mqtt_close() {
    console.log("MQTT Closed");
};

/**
 * @brief MySQL related part
 * 
 * @todo consider https://github.com/behrad/mqtt-nedb-store *
 */
var mysql = require('mysql');
var connection = mysql.createConnection({
    host: "localhost",
    user: "user",
    password: "password",
    database: "balcony"
});

connection.connect(function (err) {
    if (err) throw err;
    console.log("Database Connected!");
});

function insert_message(topic, message, packet) {
    // save data to db
    var sql = "INSERT INTO logs (??,??) VALUES (?,?)";
    var timestamp = 10000;
    var params = ['topic', 'msg', topic, message];
    sql = mysql.format(sql, params);

    connection.query(sql, function (error, results) {
        if (error) throw error;
    });
};
