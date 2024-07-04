
# ESP32 Telegram, Line and NTP

This project demonstrates how to use ESP32 to send messages via Telegram and Line Notify while syncing time using NTP.

## Features

- Connects to WiFi and obtains time from NTP server.
- Sends messages to Telegram and Line Notify.
- Deep sleeps after setup to conserve power.

## Prerequisites

- ESP32 development environment set up.
- Arduino IDE or PlatformIO installed.
- WiFi credentials (`ssid` and `password`).
- Telegram bot token (`telegramToken`) and chat ID (`telegramChatId`).
- Line Notify token (`lineNotifyToken`).

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/esp32-telegram-line-and-ntp.git
   ```

2. Open the project in Arduino IDE or PlatformIO.

3. Configure `ssid`, `password`, `telegramToken`, `telegramChatId`, and `lineNotifyToken` in `main.cpp`.

4. Upload the sketch to your ESP32 board.

## Usage

1. Open the Serial Monitor to view debug messages (`115200` baud).
2. ESP32 connects to WiFi and obtains current time using NTP.
3. Sends a message to Telegram and Line Notify with current timestamp.
4. ESP32 enters deep sleep mode to conserve power.

## Notes

- Ensure your ESP32 has a stable WiFi connection for proper operation.
- Adjust `gmtOffset_sec` and `daylightOffset_sec` in `main.cpp` for your timezone.
- Replace the certificate placeholders (`TELEGRAM_CERTIFICATE` and `LINE_CERTIFICATE`) with actual certificates if needed.

## Resources

- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [Telegram Bot API](https://core.telegram.org/bots/api)
- [Line Notify API](https://notify-bot.line.me/doc/en/)
- [NTP Pool Project](https://www.pool.ntp.org/)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
