const url = require("url");
const mqttPassword = "aio_qARB84XraZlVW9TCwXm6bZtqnVNt";
class ControlController {
  // [GET] /
  index(req, res, next) {
    if (req.session.loggedin) {
      console.log("In MQTT");
      var mqtt = require("mqtt");
      var client = mqtt.connect("mqtt://io.adafruit.com", {
        username: "khanhnq1406",
        password: mqttPassword,
      });
      var mqtttopic = `${client.options.username}/f/mqtt-server`;
      client.on("connect", function () {
        console.log("connected");
        client.subscribe(mqtttopic, function (err) {
          if (!err) {
            console.log("subscribed");
            client.publish(mqtttopic, "#checkStatus");
          }
        });
      });

      client.on("message", function (topic, message) {
        const messageString = message.toString();
        const hasTurnedOn = messageString.toLowerCase().split("~");
        console.log(messageString, hasTurnedOn);
        function checkStatus(hasTurnedOn) {
          return hasTurnedOn === "true";
        }
        const statusLight1 = checkStatus(hasTurnedOn[0]);
        const statusLight2 = checkStatus(hasTurnedOn[1]);
        console.log(statusLight1, statusLight2);
        if (messageString.includes("~")) {
          client.end();
          res.render("control", {
            statusLight1: statusLight1,
            statusLight2: statusLight2,
          });
        }
      });
    } else {
      res.redirect(
        url.format({
          pathname: "/",
          query: {
            notYetLogin: true,
          },
        })
      );
    }
  }

  turning(req, res, next) {
    var mqtt = require("mqtt");
    var client = mqtt.connect("mqtt://io.adafruit.com", {
      username: "khanhnq1406",
      password: mqttPassword,
    });
    var mqtttopic = `${client.options.username}/f/mqtt-server`;
    client.on("connect", function () {
      console.log("connected");
      client.subscribe(mqtttopic, function (err) {
        if (!err) {
          console.log("subscribed");
          client.publish(mqtttopic, "#checkStatusLight1");
        }
      });
    });

    client.on("message", function (topic, message) {
      console.log(message.toString());
      if (message.toString() == "#hasturnedOn") {
        client.end();
        res.redirect("turnOff");
      } else if (message.toString() == "#hasturnedOff") {
        client.end();
        res.redirect("turnOn");
      }
    });
  }
  turnOn(req, res, next) {
    // Setup MQTT Client
    // mqtt[s]://[username][:password]@host.domain[:port]
    var mqtt = require("mqtt");
    var client = mqtt.connect("mqtt://io.adafruit.com", {
      username: "khanhnq1406",
      password: mqttPassword,
    });
    var mqtttopic = `${client.options.username}/f/mqtt-server`;
    client.on("connect", function () {
      console.log("connected");
      client.subscribe(mqtttopic, function (err) {
        if (!err) {
          console.log("subscribed");
          client.publish(mqtttopic, "#turnOn");
        }
      });
    });

    client.on("message", function (topic, message) {
      console.log(message.toString());
      if (message.toString() == "#turnedOn") {
        client.end();
        res.redirect("/control");
      }
    });
  }

  turnOff(req, res, next) {
    // Setup MQTT Client
    // mqtt[s]://[username][:password]@host.domain[:port]
    var mqtt = require("mqtt");
    var client = mqtt.connect("mqtt://io.adafruit.com", {
      username: "khanhnq1406",
      password: mqttPassword,
    });
    var mqtttopic = `${client.options.username}/f/mqtt-server`;
    client.on("connect", function () {
      console.log("connected");
      client.subscribe(mqtttopic, function (err) {
        if (!err) {
          console.log("subscribed");
          client.publish(mqtttopic, "#turnOff");
        }
      });
    });
    console.log("IN fucntion turnOff");
    client.on("message", function (topic, message) {
      console.log(message.toString());
      if (message.toString() == "#turnedOff") {
        client.end();
        res.redirect("/control");
      }
    });
  }

  turningLight2(req, res, next) {
    var mqtt = require("mqtt");
    var client = mqtt.connect("mqtt://io.adafruit.com", {
      username: "khanhnq1406",
      password: mqttPassword,
    });
    var mqtttopic = `${client.options.username}/f/mqtt-server`;
    client.on("connect", function () {
      console.log("connected");
      client.subscribe(mqtttopic, function (err) {
        if (!err) {
          console.log("subscribed");
          client.publish(mqtttopic, "#checkStatusLight2");
        }
      });
    });

    client.on("message", function (topic, message) {
      console.log(message.toString());
      if (message.toString() == "#hasturnedOn") {
        client.end();
        res.redirect("turnOffLight2");
      } else if (message.toString() == "#hasturnedOff") {
        client.end();
        res.redirect("turnOnLight2");
      }
    });
  }
  turnOnLight2(req, res, next) {
    // Setup MQTT Client
    // mqtt[s]://[username][:password]@host.domain[:port]
    var mqtt = require("mqtt");
    var client = mqtt.connect("mqtt://io.adafruit.com", {
      username: "khanhnq1406",
      password: mqttPassword,
    });
    var mqtttopic = `${client.options.username}/f/mqtt-server`;
    client.on("connect", function () {
      console.log("connected");
      client.subscribe(mqtttopic, function (err) {
        if (!err) {
          console.log("subscribed");
          client.publish(mqtttopic, "#turnOnLight2");
        }
      });
    });
    client.on("message", function (topic, message) {
      console.log(message.toString());
      if (message.toString() == "#turnedOnLight2") {
        client.end();
        res.redirect("/control");
      }
    });
  }

  turnOffLight2(req, res, next) {
    // Setup MQTT Client
    // mqtt[s]://[username][:password]@host.domain[:port]
    var mqtt = require("mqtt");
    var client = mqtt.connect("mqtt://io.adafruit.com", {
      username: "khanhnq1406",
      password: mqttPassword,
    });
    var mqtttopic = `${client.options.username}/f/mqtt-server`;
    client.on("connect", function () {
      console.log("connected");
      client.subscribe(mqtttopic, function (err) {
        if (!err) {
          console.log("subscribed");
          client.publish(mqtttopic, "#turnOffLight2");
        }
      });
    });
    client.on("message", function (topic, message) {
      console.log(message.toString());
      if (message.toString() == "#turnedOffLight2") {
        client.end();
        res.redirect("/control");
      }
    });
  }
}

module.exports = new ControlController();
