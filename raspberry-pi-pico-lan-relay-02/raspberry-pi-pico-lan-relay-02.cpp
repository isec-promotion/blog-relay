#include <SPI.h>
#include <EthernetENC.h>
#include <EthernetUdp.h>
// ====== ピン設定 ======
static const int PIN_CS = 17;
// リレー出力用（8ch）
static const int NUM_CH = 8;
static const int GPIO_PINS[NUM_CH] = {0, 1, 2, 3, 4, 5, 6, 7};
// ====== ネットワーク設定 ======
byte mac[] = {0x02, 0xAA, 0xBB, 0xCC, 0xDD, 0x01};
IPAddress ip(192, 168, 40, 50);
IPAddress dnsIP(192, 168, 40, 1);
IPAddress gateway(192, 168, 40, 1);
IPAddress subnet(255, 255, 255, 0);
const unsigned int LOCAL_PORT = 5005;
EthernetUDP Udp;
void setup() {
  // リレー制御用GPIOの初期化（すべてLOWに設定）
  for(int i = 0; i < NUM_CH; i++){
    pinMode(GPIO_PINS[i], OUTPUT);
    digitalWrite(GPIO_PINS[i], LOW);
  }
  // デバッグ用シリアル通信の開始（USB経由）
  Serial.begin(115200);
  // SPIピンの明示的なルーティング (ENC28J60接続用)
  SPI.setSCK(18);
  SPI.setTX(19);  // MOSI
  SPI.setRX(16);  // MISO
  SPI.begin();
  // ENC28J60の初期化とネットワーク起動
  Ethernet.init(PIN_CS);
  Ethernet.begin(mac, ip, dnsIP, gateway, subnet);
  Udp.begin(LOCAL_PORT);
  Serial.println("=================================");
  Serial.println(" Pico LAN Control Started");
  Serial.print(" IP Address: ");
  Serial.println(Ethernet.localIP());
  Serial.println(" Waiting for UDP packets...");
  Serial.println("=================================");
}
void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize) {
    char buf[128];
    int len = Udp.read(buf, 127);
    buf[len] = 0; // null終端
    // 受信データをString型に変換して改行コード等を除去
    String cmd = String(buf);
    cmd.trim();
    // USB経由のシリアルモニタへ受信結果を出力
    Serial.print("\n[Received Command]: ");
    Serial.println(cmd);
    // ======= GPIOのON/OFF制御ロジック =======
    // 1. 全チャンネルON
    if(cmd == "ALLON") {
      for(int i = 0; i < NUM_CH; i++){
        digitalWrite(GPIO_PINS[i], HIGH);
      }
      Serial.println(" -> Action: ALL GPIO (GP0-GP7) turned HIGH");
    }
    // 2. 全チャンネルOFF
    else if(cmd == "ALLOFF") {
      for(int i = 0; i < NUM_CH; i++){
        digitalWrite(GPIO_PINS[i], LOW);
      }
      Serial.println(" -> Action: ALL GPIO (GP0-GP7) turned LOW");
    }
    // 3. 個別チャンネル制御（例: "CH0_ON", "CH7_OFF"）
    else if(cmd.startsWith("CH") && cmd.indexOf("_") > 0) {
      // "CH"と"_"の間にある数字を抽出してインデックス化
      int chIndex = cmd.substring(2, cmd.indexOf("_")).toInt();
      // 指定されたチャンネル番号が範囲内かチェック
      if(chIndex >= 0 && chIndex < NUM_CH) {
        if(cmd.endsWith("_ON")) {
          digitalWrite(GPIO_PINS[chIndex], HIGH);
          Serial.print(" -> Action: GP");
          Serial.print(GPIO_PINS[chIndex]);
          Serial.println(" turned HIGH");
        }
        else if(cmd.endsWith("_OFF")) {
          digitalWrite(GPIO_PINS[chIndex], LOW);
          Serial.print(" -> Action: GP");
          Serial.print(GPIO_PINS[chIndex]);
          Serial.println(" turned LOW");
        } else {
          Serial.println(" -> Error: Invalid suffix (Use _ON or _OFF)");
        }
      } else {
        Serial.println(" -> Error: Channel out of range (Use CH0 - CH7)");
      }
    }
    // 4. 未知のコマンド
    else {
      Serial.println(" -> Action: Unknown Command");
    }
  }
}