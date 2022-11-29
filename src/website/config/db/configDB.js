const fb = require("firebase");
const express = require("express");
const app = express();

const appInit = {
  apiKey: "AIzaSyC_qSBpZtTtC2WJPDzZYyrkhs4ZKMCnqTM",
  authDomain: "iot-project-ffcf1.firebaseapp.com",
  databaseURL: "https://iot-project-ffcf1-default-rtdb.firebaseio.com",
  projectId: "iot-project-ffcf1",
  storageBucket: "iot-project-ffcf1.appspot.com",
  messagingSenderId: "660977884125",
  appId: "1:660977884125:web:0166c572f6ff41f9bd7434",
  measurementId: "G-J4TKHJE6KV",
};

const appInit2 = {
  apiKey: "AIzaSyCCMle_JkmnDA6A0aKU_7cZt5zZE905ggI",
  authDomain: "iot-data-collection-5d7a4.firebaseapp.com",
  //databaseURL: "https://iot-data-collection-5d7a4-default-rtdb.firebaseio.com",
  databaseURL:
    "https://iot-data-collection-5d7a4-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "iot-data-collection-5d7a4",
  storageBucket: "iot-data-collection-5d7a4.appspot.com",
  messagingSenderId: "362954911615",
  appId: "1:362954911615:web:79fd295a9168908112f44a",
  measurementId: "G-F2GDXMVXLY",
};
fb.initializeApp(appInit2);

const firebaseDB = fb.database();
// firebaseDB.ref("theText").set("hello");
console.log(`Connect successfully`);
module.exports = { firebaseDB };
