const express = require("express");
const router = express.Router();

const controlController = require("../app/controllers/ControlController");
router.get("/turnOn", controlController.turnOn);
router.get("/turnOff", controlController.turnOff);
router.get("/turning", controlController.turning);
router.get("/turningLight2", controlController.turningLight2);
router.get("/turnOnLight2", controlController.turnOnLight2);
router.get("/turnOffLight2", controlController.turnOffLight2);
router.get("/", controlController.index);

module.exports = router;
