# HackHome v0.1 - セットアップ手順

## 必要なもの

### ハードウェア
- M5Stamp S3
- SG90 サーボ (青いやつ)
- USB-C ケーブル (電源 & 書き込み用)
- ジャンパーワイヤー 3本

### ソフトウェア
- [VSCode](https://code.visualstudio.com/) + [PlatformIO拡張](https://platformio.org/install/ide?install=vscode)
- [Node.js](https://nodejs.org/) (v18以上)

---

## 1. 配線

| SG90 ワイヤー | 接続先 (M5Stamp S3) |
| :--- | :--- |
| 赤 (VCC) | 5V |
| 茶 (GND) | GND |
| 橙 (Signal) | G1 |

> M5Stamp S3 は USB-C から 5V が取れるので、サーボの電源もそこから供給。

---

## 2. ファームウェア書き込み

### 2-1. WiFi設定を編集

`v0.1/firmware/include/config.h` を開いて自分のWiFi情報を入力:

```cpp
#define WIFI_SSID "自分のSSID"
#define WIFI_PASS "自分のパスワード"
```

### 2-2. ビルド & 書き込み

```bash
cd v0.1/firmware

# ビルド
pio run

# M5Stamp S3 を USB-C で接続してから書き込み
pio run -t upload

# シリアルモニターで動作確認 (任意)
pio device monitor
```

シリアルモニターに以下が表示されれば成功:
```
Connected! IP: 192.168.x.x
mDNS: http://hackhome.local
Server started
```

### 2-3. 動作確認 (curl)

```bash
# ステータス確認
curl http://hackhome.local/api/status

# サーボを押す
curl -X POST http://hackhome.local/api/servo/press

# 任意の角度に設定
curl -X POST http://hackhome.local/api/servo/angle -H "Content-Type: application/json" -d '{"angle": 45}'
```

> `hackhome.local` で繋がらない場合は、シリアルモニターに表示された IP アドレスを使う。

---

## 3. フロントエンド起動

```bash
cd v0.1/frontend

# 依存パッケージインストール (初回のみ)
npm install

# 開発サーバー起動
npm run dev
```

ブラウザで http://localhost:3000 を開く。

### 使い方
1. 画面上部の入力欄に ESP32 のアドレスを入力 (デフォルト: `hackhome.local`)
2. 緑のドットが表示されたら接続成功
3. **Press** ボタン → サーボがスイッチを押して戻る
4. **スライダー** → サーボを任意の角度に設定

---

## トラブルシューティング

| 症状 | 対処 |
| :--- | :--- |
| WiFi に繋がらない | SSID/PASS を確認。2.4GHz のみ対応 |
| `hackhome.local` に繋がらない | IP アドレスを直接入力 |
| フロントから操作できない | ESP32 と PC が同じ WiFi に接続されているか確認 |
| サーボが動かない | 配線確認 (G1 に Signal)。USB給電が弱い場合は別電源を試す |
| 書き込みできない | M5Stamp S3 の G0 ボタンを押しながら USB 接続 → BootMode に入る |
