const express = require('express');
const router = express.Router();


const monitorController = require('../app/controllers/MonitorController');
router.get('/addTemp', monitorController.addTemperature);
router.get('/', monitorController.index);

module.exports = router;