const express = require("express");
const router = express.Router();

const homeController = require("../app/controllers/HomeController");
router.post("/login", homeController.login);
router.post("/addUsers", homeController.addUsers);
router.post("/sendToFirebase", homeController.sendToFirebase);
router.get("/logout", homeController.logout);
router.get("/signup", homeController.signup);
// router.get('/signup/fail', homeController.signup);
router.get("/", homeController.index);

module.exports = router;
