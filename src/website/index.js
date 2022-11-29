const path = require("path");
const express = require("express");
// const morgan = require('morgan');
const methodOverride = require("method-override");
const { engine } = require("express-handlebars");
const session = require("express-session");
const app = express();
let port = process.env.PORT || 3000 || 80;
let server_host = "localhost" || "0.0.0.0";
const route = require("./routes");
const db = require("./config/db/configDB");

//Process static file (add image)
app.use(express.static(path.join(__dirname, "public")));

app.use(methodOverride("_method"));

//HTTP logger
// app.use(morgan('combined'));

//use midleware to get data
app.use(
  express.urlencoded({
    extended: true,
  })
);
app.use(express.json());

app.use(
  session({
    secret: "secret",
    resave: true,
    saveUninitialized: true,
  })
);
//Template engine
app.engine(
  "hbs",
  engine({
    extname: ".hbs", //config name of file .handlebars
    helpers: {
      sum: (a, b) => a + b,
    },
  })
);
app.set("view engine", "hbs");
app.set("views", path.join(__dirname, "resources", "views"));

// Routes init
route(app);

app.listen(port, "0.0.0.0", () => {
  console.log(`App listening on port ${port}`);
});
