const fb = require("firebase");
const url = require("url");
class MonitorController {
  // [GET] /
  index(req, res, next) {
    if (req.session.loggedin) {
      const firebaseDB = fb.database();
      const firebaseRef = firebaseDB.ref("Temperature");
      let data = [];
      let dateSort = req.query.dateSort;
      let tempSort = req.query.tempSort;
      let dateSortSelect = req.query.dateSortSelect;

      firebaseRef.once("value", function (snapshot) {
        snapshot.forEach(function (element) {
          if (dateSort !== undefined) {
            if (element.key.includes(dateSort))
              data.push({ dateTime: element.key, temperature: element.val() });
          } else data.push({ dateTime: element.key, temperature: element.val() });
        });

        if (tempSort === "ascending") {
          data.sort((a, b) => {
            return Number(a.temperature) - Number(b.temperature);
          });
        } else if (tempSort === "descending") {
          data.sort((a, b) => {
            return Number(b.temperature) - Number(a.temperature);
          });
        } else if (dateSortSelect === "ascending") {
          data.reverse();
        }
        res.render("monitor", {
          pageData: data,
        });
        // res.send("ok");
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
  // [GET] /addTemp
  addTemperature(req, res, next) {
    const today = new Date();
    const date =
      today.getFullYear() +
      "-" +
      (today.getMonth() + 1) +
      "-" +
      today.getDate();
    const time =
      today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    const dateTime = date + " " + time;
    const firebaseDB = fb.database();
    const temperature = Math.random() * (99 - 10) + 10;
    firebaseDB
      .ref("Temperature")
      .child(dateTime)
      .set(Number(temperature.toFixed(2)));
    res.send("");
  }
}

module.exports = new MonitorController();
