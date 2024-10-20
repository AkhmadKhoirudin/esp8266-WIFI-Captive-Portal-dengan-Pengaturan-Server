# esp8266-WIFI-Captive-Portal-dengan-Pengaturan-Server
Penjelasan Kode:
Wi-Fi Access Point:

WiFi.softAP(ssid, password) digunakan untuk membuat jaringan Wi-Fi captive portal dengan nama dan password yang ditentukan.
IP captive portal diatur menggunakan WiFi.softAPConfig().
DNS Server:

DNSServer digunakan untuk mengarahkan semua permintaan domain ke IP captive portal, sehingga pengguna selalu diarahkan ke halaman login.
Web Server:

ESP8266WebServer digunakan untuk membuat server web yang menangani halaman captive portal.
Terdapat dua endpoint: halaman utama ("/") dan validasi voucher ("/validate").
Jika halaman yang diminta tidak ada, pengguna akan diarahkan kembali ke halaman utama.
Validasi Voucher:

Server memeriksa apakah kode voucher yang dimasukkan pengguna cocok dengan daftar voucher yang valid.
Jika valid, pengguna mendapatkan pesan sukses, dan jika tidak valid, pesan kesalahan akan ditampilkan.
