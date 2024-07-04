/**
 * 
 * ESP32 Telegram, Line and NTP

 * This project demonstrates how to use ESP32 to send messages via Telegram and Line Notify while syncing time using NTP.
 *
 * https://github.com/aos92/ESP32-TELEGRAM-LINE-AND-NTP
 *
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "time.h"

// WiFi Credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

//Telegram Token and Chat ID
const char telegramToken[] = "YOUR_TELEGRAM_NOTIFY_TOKEN";
const char telegramChatId[] = "YOUR_CHAT_ID";
// Line Token
const char lineNotifyToken[] = "YOUR_LINE_NOTIFY_TOKEN";

// NTP Server Details
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200; // Waktu GMT+7 (Waktu Indonesia Barat)
const int   daylightOffset_sec = 0; // Tidak ada penyesuaian daylight saving time

struct tm timeinfo;

// Root CA Certificate for Telegram API
const char TELEGRAM_CERTIFICATE[] =R"=====(
-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx
EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT
EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp
ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAwMFoXDTM3MTIzMTIz
NTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH
EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8GA1UE
AxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIw
DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKD
E6bFIEMBO4Tx5oVJnyfq9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH
/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD+qK+ihVqf94Lw7YZFAXK6sOoBJQ7Rnwy
DfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutdfMh8+7ArU6SSYmlRJQVh
GkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMlNAJWJwGR
tDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEA
AaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYE
FDqahQcQZyi27/a9BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmX
WWcDYfF+OwYxdS2hII5PZYe096acvNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu
9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r5N9ss4UXnT3ZJE95kTXWXwTr
gIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYVN8Gb5DKj7Tjo
2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO
LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI
4uJEvlz36hz1
-----END CERTIFICATE-----
)=====";

// Root CA Certificate for Line Notify
const char LINE_CERTIFICATE[] = R"=====(
-----BEGIN CERTIFICATE-----
MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G
A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp
Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4
MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG
A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8
RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT
gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm
KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd
QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ
XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw
DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o
LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU
RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp
jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK
6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX
mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs
Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH
WD9f
-----END CERTIFICATE-----
)=====";


void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Configure time via NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Print the current time
  printLocalTime();

  // Send message to Telegram
  String telegramMessage = "Hello World [" + getCurrentTimestamp() + "]";
  String telegramResponse = sendTelegramMessage(telegramToken, telegramChatId, telegramMessage);
  Serial.println("Telegram Response: " + telegramResponse);

  // Send message to Line Notify
  String lineNotifyMessage = "Hello World [" + getCurrentTimestamp() + "]";
  String lineNotifyResponse = sendLineNotify(lineNotifyMessage);
  Serial.println("Line Notify Response: " + lineNotifyResponse);

  // Delay and then deep sleep or perform other actions
  delay(1000);
  esp_deep_sleep_start();
}

void loop() {
  // Empty, as we are using deep sleep after setup
}

void printLocalTime() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

String getCurrentTimestamp() {
  if (!getLocalTime(&timeinfo)) {
    return "Failed to obtain time";
  }

  char timestamp[50];
  sprintf(timestamp, "%02d-%02d-%02d %02d:%02d", 
          timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year % 100,
          timeinfo.tm_hour, timeinfo.tm_min);

  return String(timestamp);
}


String sendTelegramMessage(const char* token, const char* chat_id, String message) {
  WiFiClientSecure client;
  client.setCACert(TELEGRAM_CERTIFICATE);

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Failed to connect to Telegram");
    return "Connection failed";
  }

  String url = "/bot";
url += token;
url += "/sendMessage?chat_id=";
url += chat_id;
url += "&text=";
url += message;
    

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "Connection: close\r\n\r\n");

  delay(500);

  String response = "";
  while (client.available()) {
    String line = client.readStringUntil('\r');
    response += line;
  }

  return response;
}

String sendLineNotify(String message) {
  WiFiClientSecure client;
  client.setCACert(LINE_CERTIFICATE);

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("Failed to connect to Line Notify");
    return "Connection failed";
  }

  String payload = "message=" + message;
  String headers = "POST /api/notify HTTP/1.1\r\n";
headers += "Host: notify-api.line.me\r\n";
headers += "Authorization: Bearer ";
headers += String(lineNotifyToken);
headers += "\r\n";
headers += "Content-Type: application/x-www-form-urlencoded\r\n";
headers += "Content-Length: ";
headers += String(payload.length());
headers += "\r\n\r\n";

  client.print(headers);
  client.print(payload);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String response = "";
  while (client.available()) {
    response += client.readString();
  }

  return response;
}
