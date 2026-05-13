#include <Wire.h>
#include <Adafruit_MLX90640.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "[wi-fi_name]";
const char* password = "[wi-fi_Password]";
//Make sure that both the system and the display device are connected to the same Wi-Fi network.

Adafruit_MLX90640 mlx;
float frame[32 * 24];
WebServer server(80);

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Thermal Camera</title>
  <style>
    body { background:#111; color:white; font-family:Arial; text-align:center; margin:20px; }
    canvas { image-rendering:pixelated; border:2px solid #555; }
    #info { font-size:18px; margin:10px; }
    #fps { font-size:13px; color:#aaa; }
  </style>
</head>
<body>
  <h2>🌡️ MLX90640 Thermal Camera</h2>
  <canvas id="c" width="640" height="480"></canvas>
  <div id="info">Connecting...</div>
  <div id="fps"></div>
<script>
const canvas = document.getElementById('c');
const ctx = canvas.getContext('2d');
let lastTime = Date.now();
let frameCount = 0;

// Change canvas to larger size for smoother look
canvas.width = 640;
canvas.height = 480;

function getColor(t, min, max) {
  const r = Math.max(0, Math.min(1, (t - min) / (max - min)));
  
  // Ironbow/Inferno colormap
  // Black → Purple → Red → Orange → Yellow → White
  let R, G, B;
  
  if (r < 0.25) {
    // Black to Purple
    R = Math.round(r * 4 * 100);
    G = 0;
    B = Math.round(r * 4 * 150);
  } else if (r < 0.5) {
    // Purple to Red
    R = Math.round(100 + (r - 0.25) * 4 * 155);
    G = 0;
    B = Math.round(150 - (r - 0.25) * 4 * 150);
  } else if (r < 0.75) {
    // Red to Orange/Yellow
    R = 255;
    G = Math.round((r - 0.5) * 4 * 200);
    B = 0;
  } else {
    // Yellow to White
    R = 255;
    G = Math.round(200 + (r - 0.75) * 4 * 55);
    B = Math.round((r - 0.75) * 4 * 255);
  }
  
  return [R, G, B];
}
async function update() {
  try {
    const res = await fetch('/data');
    const d = await res.json();
    const temps = d.temps;
    let min = Math.min(...temps);
    let max = Math.max(...temps);

    // Minimum 2°C range to avoid false colors on empty scene
    if (max - min < 2.0) {
      const mid = (min + max) / 2;
      min = mid - 1;
      max = mid + 1;
    }

    // Draw small 32x24 to offscreen canvas
    const offscreen = document.createElement('canvas');
    offscreen.width = 32;
    offscreen.height = 24;
    const offCtx = offscreen.getContext('2d');
    const img = offCtx.createImageData(32, 24);

    for (let y = 0; y < 24; y++) {
      for (let x = 0; x < 32; x++) {
        const [R,G,B] = getColor(temps[y*32+x], min, max);
        const i = (y*32+x)*4;
        img.data[i]=R; img.data[i+1]=G; img.data[i+2]=B; img.data[i+3]=255;
      }
    }
    offCtx.putImageData(img, 0, 0);

    // Scale up with smoothing — this removes pixelation!
    ctx.imageSmoothingEnabled = true;
    ctx.imageSmoothingQuality = 'high';
    // Flip horizontally
    ctx.save();
    ctx.scale(-1, 1);
    ctx.drawImage(offscreen, -640, 0, 640, 480);
    ctx.restore();

    document.getElementById('info').innerHTML =
      'Min: <span style="color:#4af">' + min.toFixed(1) + '°C</span> &nbsp;' +
      'Max: <span style="color:#f84">' + max.toFixed(1) + '°C</span> &nbsp;' +
      'Ambient: <span style="color:#4f8">' + d.ambient.toFixed(1) + '°C</span>';

    frameCount++;
    const now = Date.now();
    if (now - lastTime > 1000) {
      document.getElementById('fps').textContent = frameCount + ' FPS';
      frameCount = 0;
      lastTime = now;
    }
  } catch(e) {
    document.getElementById('info').textContent = 'Error: ' + e.message;
  }
  setTimeout(update,0); // faster polling
}
update();
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", webpage);
}

void handleData() {
  if (mlx.getFrame(frame) != 0) {
    server.send(500, "text/plain", "Frame error");
    return;
  }
  
  // First pass - calculate average of valid pixels
  float sum = 0;
  int count = 0;
  for (int i = 0; i < 768; i++) {
    if (!isnan(frame[i])) {
      sum += frame[i];
      count++;
    }
  }
  float avg = count > 0 ? sum / count : 25.0;

  float ambient = mlx.getTa(false);
  String json = "{\"ambient\":" + String(ambient, 1) + ",\"temps\":[";
  for (int i = 0; i < 768; i++) {
    if (isnan(frame[i])) {
      json += String(avg, 1); // replace NaN with average
    } else {
      json += String((int)frame[i]); // no decimal = smaller/faster
    }
    if (i < 767) json += ",";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);
  Wire.setClock(1000000);

  if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found!");
    while (1);
  }
  Serial.println("MLX90640 found!");
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_16_HZ);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("Server started! Open browser to IP above.");
}

void loop() {
  server.handleClient();
}
