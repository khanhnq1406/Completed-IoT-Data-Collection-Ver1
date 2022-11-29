# IoT-Data-Collection-Ver1
This is an IoT Data Collection project using LoRa, NodeJS, MQTT and Firebase

## How to use repository

- We will code in `src` folder. You can create new folder in `src` for your task (such as website, ESP32, LoRa,...).
- **Note:** Remember to `pull` code before working and `push` code after coding.
- You can take note your solution to solve problem or everything in `notes` folder by _.md_ files.

## Install

1. Install NodeJS, [click here](https://nodejs.org/en/download/)

## Deploy Heroku using Docker

`docker build . -t khanhnq1406/iot-data-collection`
`heroku container:login`
`heroku container:push web`
`heroku container:release web`
