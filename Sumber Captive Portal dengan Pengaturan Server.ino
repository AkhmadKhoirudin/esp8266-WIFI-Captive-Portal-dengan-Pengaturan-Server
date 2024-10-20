#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// Pengaturan DNS dan Wi-Fi
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);  // IP untuk jaringan captive portal
DNSServer dnsServer;
ESP8266WebServer webServer(80);

// Nama dan password jaringan Wi-Fi captive portal
const char* ssid = "ESP_Captive_Portal";
const char* password = "12345678";

// Voucher yang valid untuk login
const char* validVouchers[] = {"VOUCHER123", "VOUCHER456", "VOUCHER789"};
const int voucherCount = sizeof(validVouchers) / sizeof(validVouchers[0]);

void setup() {
  Serial.begin(115200);
  
  // Atur ESP8266 sebagai access point (AP)
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
  // Mulai DNS server untuk mengarahkan semua domain ke IP captive portal
  dnsServer.start(DNS_PORT, "*", apIP);

  // Atur routing untuk halaman captive portal
  webServer.on("/", handleRoot);  // Halaman utama (captive portal)
  webServer.on("/validate", HTTP_POST, handleValidate);  // Proses validasi kode voucher
  webServer.onNotFound(handleNotFound);  // Pengalihan jika halaman tidak ditemukan
  
  // Mulai web server
  webServer.begin();
  Serial.println("Captive portal aktif");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

// Halaman utama captive portal
void handleRoot() {
  const char* html = R"rawliteral(
  <!DOCTYPE html>
  <html lang="id">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Captive Portal Login</title>
      <style>
          body { font-family: Arial, sans-serif; background-color: #f4f4f4; text-align: center; padding: 50px; }
          h2 { color: #333; }
          input[type="text"], input[type="submit"] { padding: 10px; font-size: 16px; }
          .message { margin-top: 20px; font-size: 18px; color: red; }
      </style>
  </head>
  <body>
      <h2>Login ke Jaringan</h2>
      <form action="/validate" method="POST">
          <input type="text" name="voucher" placeholder="Masukkan kode voucher" required>
          <input type="submit" value="Login">
      </form>
      <div class="message"></div>
  </body>
  </html>
  )rawliteral";

  webServer.send(200, "text/html", html);
}

// Proses validasi voucher
void handleValidate() {
  if (webServer.method() == HTTP_POST) {
    String voucher = webServer.arg("voucher");
    bool isValid = false;

    // Memeriksa apakah voucher valid
    for (int i = 0; i < voucherCount; i++) {
      if (voucher == validVouchers[i]) {
        isValid = true;
        break;
      }
    }

    if (isValid) {
      // Voucher valid, redirect ke halaman sukses atau halaman lain
      webServer.send(200, "text/plain", "Voucher valid! Anda berhasil login.");
    } else {
      // Voucher tidak valid, kembali ke halaman login
      webServer.send(200, "text/plain", "Voucher tidak valid! Coba lagi.");
    }
  } else {
    webServer.send(405, "text/plain", "Method Not Allowed");
  }
}

// Mengalihkan pengguna jika halaman tidak ditemukan
void handleNotFound() {
  webServer.sendHeader("Location", "/", true);  // Redirect ke halaman root
  webServer.send(302, "text/plain", "");
}
