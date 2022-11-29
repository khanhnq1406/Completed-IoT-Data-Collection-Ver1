const fb = require("firebase");
const generateString = require("../../utils/GenerateString");
const url = require("url");
const { emitKeypressEvents } = require("readline");
// require("firebase/firestore");
class HomeController {
  // [GET] /
  index(req, res, next) {
    console.log(req.session.loggedin);
    // req.session.loggedin = true;
    if (req.session.loggedin) res.render("home");
    else if (req.query.notYetLogin) {
      res.render("login", {
        notYetLogin: true,
      });
    } else {
      res.render("login");
    }
  }

  // [POST] /login
  login(req, res, next) {
    const formData = req.body;
    const username = formData.username;
    const password = formData.password;
    console.log(formData);
    const firebaseDB = fb.database();
    const firebaseRef = firebaseDB.ref("Users");
    firebaseRef.on("value", function (snapshot) {
      snapshot.forEach(function (element) {
        if (username === element.key && password === element.val()) {
          req.session.loggedin = true;
          req.session.username = username;
        }
      });
      if (req.session.loggedin) res.render("home");
      else
        res.render("login", {
          isLoginFail: true,
        });
    });
  }

  // [POST] /addUsers
  addUsers(req, res, next) {
    const username = req.body.username;
    const password = req.body.password;
    const retypePassword = req.body.retypePassword;
    if (password !== retypePassword) {
      res.render("signup", {
        isDiffPassword: true,
        username: username,
      });
    } else {
      // Add data
      const firestoreDB = fb.database();
      let isInvalid = false;
      const firebaseRef = firestoreDB.ref("Users");
      firebaseRef.on("value", function (snapshot) {
        snapshot.forEach(function (element) {
          console.log(element.key);
          if (username === element.key) {
            console.log("isInvalid set true");
            isInvalid = true;
          }
        });
      });
      if (isInvalid) {
        console.log("Username is invalid");
        res.redirect(
          url.format({
            pathname: "/signup",
            query: {
              isInvalid: true,
            },
          })
        );
      } else {
        firebaseRef.child(username).set(password);
        console.log("Add successfully");
        res.render("login", {
          justSignup: true,
        });
      }
    }
  }

  // [POST] /sendToFirebase
  sendToFirebase(req, res, next) {
    const number1 = req.body.number1;
    const number2 = req.body.number2;
    const firestoreDB = fb.database();
    if (number1 !== "") {
      firestoreDB.ref("Number 1").set(Number(number1));
    }
    if (number2 !== "") {
      firestoreDB.ref("Number 2").set(Number(number2));
    }
    res.redirect("/");
  }
  // [GET] /signup
  signup(req, res, next) {
    const username = req.query.username;
    if (req.query.isDiffPassword) {
      res.render("signup", {
        isDiffPassword: true,
        username: username,
      });
    } else if (req.query.isInvalid) {
      res.render("signup", {
        isInvalid: true,
      });
    } else {
      res.render("signup");
    }
  }

  logout(req, res, next) {
    req.session.loggedin = false;
    res.redirect("/");
  }
}
module.exports = new HomeController();
