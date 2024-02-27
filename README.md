# Smart_Shoe_Bot

# Smart Shoe Beacon using Consentium Cloud as Backend

Enhance your safety with the Smart Shoe Tracker, a project designed to prevent injuries, provide location tracking through smart beacons, prevent harassment, and offer quick support calls.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Technologies Used](#technologies-used)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The Smart Shoe Tracker is a system that utilizes Arduino and various sensors to enhance user safety. It integrates with the Telegram messaging platform for real-time alerts and notifications. The mobile app is built with Expo, providing a seamless experience for users.

## Features

- **Prevent Injury:** Advanced tracking analyzes walking patterns to prevent potential injuries.
- **Smart Beacon:** Utilizes smart beacon technology for accurate location tracking and emergency alerts.
- **Anti-Harassment:** Built-in sensors detect unusual movements and activate an alert system to prevent harassment.
- **Quick Support Call:** In case of emergencies, initiate a quick support call directly from your Smart Shoe Tracker.

## Getting Started

### Prerequisites

- Arduino IDE installed
- Telegram account and BOT_TOKEN
- ESP32 or compatible Arduino board
- Necessary sensors (e.g., knock sensor)
- Expo CLI installed for mobile app development

### Installation

1. Clone the repository.
2. Configure your Wi-Fi credentials and Telegram BOT_TOKEN in the Arduino sketch.
3. Connect the necessary sensors to your Arduino board.
4. Upload the sketch to your ESP32 board.
5. Install Expo CLI globally using `npm install -g expo-cli`.
6. Navigate to the mobile app directory and run `npm install` to install dependencies.

## Usage

1. Wear the Smart Shoe Tracker.
2. In case of an emergency or triggering the sensors, the system will send your location to Telegram.

## Technologies Used

- **Arduino:** Open-source electronics platform for prototyping.
- **Universal Telegram Bot:** Library for integrating Telegram Bots with Arduino.
- **ESP32:** Powerful microcontroller with built-in Wi-Fi and Bluetooth capabilities.
- **Expo:** Framework for building cross-platform mobile apps with React Native.
- **Various Sensors:** Utilized for detecting knocks, movements, etc.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

## License

This project is licensed under the [MIT License](LICENSE).
