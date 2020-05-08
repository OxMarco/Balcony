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

const express = require("express");
const app = express();
const port = process.env.PORT || "8000";

app.get("/", (req, res) => {
    res.status(200).send("WHATABYTE: Food For Devs");
});

app.listen(port, () => {
    console.log(`Listening to requests on http://localhost:${port}`);
});








    /* parse message
    try {
        const data = JSON.parse(message)
    } catch(err) {
        console.log(err)
    }
    */