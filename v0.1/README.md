# HackHome v0.1 - デモ版セットアップガイド

v0.1 はデモ版です。手持ちの M5Stamp S3 + SG90 サーボ + USB給電で、
「Web UIからサーボを動かして物理スイッチを押す」という基本動作を確認します。

---

## 目次

1. [必要なもの](#1-必要なもの)
2. [配線](#2-配線)
3. [ファームウェアのセットアップ](#3-ファームウェアのセットアップ)
4. [ファームウェアの動作確認](#4-ファームウェアの動作確認)
5. [フロントエンドのセットアップ](#5-フロントエンドのセットアップ)
6. [全体の動作確認](#6-全体の動作確認)
7. [API リファレンス](#7-api-リファレンス)
8. [ピンアサイン・設定値一覧](#8-ピンアサイン設定値一覧)
9. [RGB LED の色の意味](#9-rgb-led-の色の意味)
10. [カスタマイズ](#10-カスタマイズ)
11. [トラブルシューティング](#11-トラブルシューティング)

---

## 1. 必要なもの

### ハードウェア

| 部品 | 数量 | 備考 |
| :--- | :---: | :--- |
| M5Stamp S3 | 1 | ESP32-S3搭載。PIN 2.54mm 版でも PIN 1.27mm 版でもOK |
| SG90 サーボ | 1 | 青い小型サーボ。SG90互換品でも可 |
| USB-C ケーブル | 1 | データ通信対応のもの (充電専用ケーブルは不可) |
| ジャンパーワイヤー | 3本 | オス-メスまたはオス-オス (ピンヘッダーの種類に合わせて) |
| PC | 1 | Windows / Mac / Linux |

### ソフトウェア

| ツール | 用途 | インストール |
| :--- | :--- | :--- |
| VSCode | コードエディタ | https://code.visualstudio.com/ |
| PlatformIO (VSCode拡張) | ファームウェアのビルド・書き込み | VSCode拡張機能で「PlatformIO IDE」を検索してインストール |
| Node.js (v18以上) | フロントエンド実行 | https://nodejs.org/ (LTS推奨) |
| Git | ソースコード管理 | https://git-scm.com/ |

---

## 2. 配線

### SG90 サーボのワイヤー色

SG90 から出ている3本のワイヤーを M5Stamp S3 に接続します。

```
SG90 サーボ                M5Stamp S3
┌──────────┐              ┌──────────┐
│ 赤 (VCC) │ ──────────── │ 5V       │
│ 茶 (GND) │ ──────────── │ GND      │
│ 橙 (SIG) │ ──────────── │ G1       │
└──────────┘              └──────────┘
```

| SG90 ワイヤー | 色 | 接続先 (M5Stamp S3) | 説明 |
| :--- | :---: | :--- | :--- |
| VCC (電源) | 赤 | **5V** | サーボの電源。USBから供給される5V |
| GND (グランド) | 茶 | **GND** | 共通グランド |
| Signal (信号) | 橙 | **G1** (GPIO 1) | PWM信号でサーボの角度を制御 |

### 注意事項
- M5Stamp S3 は USB-C から 5V が出ているので、サーボの電源もそこから取れます
- ただし USB ポートの電流供給能力に依存するため、サーボに高負荷がかかると電力不足になる可能性があります。その場合は外部5V電源を別途用意してください
- SG90 のワイヤー色はメーカーによって異なる場合があります。茶色が黒の場合もあります

---

## 3. ファームウェアのセットアップ

### 3-1. リポジトリをクローン (まだの場合)

```bash
git clone https://github.com/tkg8610/HackHome.git
cd HackHome
```

### 3-2. WiFi 設定を編集

`v0.1/firmware/include/config.h` を開いて、自分のWiFi情報を入力します。

```cpp
// 変更前
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_PASSWORD"

// 変更後 (例)
#define WIFI_SSID "MyHomeWiFi"
#define WIFI_PASS "my_password_123"
```

> **重要:** ESP32-S3 は **2.4GHz の WiFi のみ** 対応しています。5GHz には接続できません。
> ルーターの設定で 2.4GHz バンドの SSID を確認してください。

### 3-3. PlatformIO でビルド

**方法A: VSCode (GUI)**

1. VSCode で `v0.1/firmware` フォルダを開く
2. 左下の PlatformIO アイコンをクリック
3. `Build` をクリック → 初回はライブラリのダウンロードが走るので数分かかります
4. `SUCCESS` と表示されればOK

**方法B: コマンドライン**

```bash
cd v0.1/firmware

# ビルド (初回はライブラリ自動ダウンロード)
pio run
```

成功すると以下のように表示されます:
```
Linking .pio/build/m5stamp-s3/firmware.elf
Building .pio/build/m5stamp-s3/firmware.bin
========================= [SUCCESS] Took XX.XXs =========================
```

### 3-4. M5Stamp S3 に書き込み

1. M5Stamp S3 を USB-C ケーブルで PC に接続
2. 書き込みを実行

**方法A: VSCode (GUI)**
- PlatformIO メニューから `Upload` をクリック

**方法B: コマンドライン**
```bash
pio run -t upload
```

> **書き込みに失敗する場合:**
> M5Stamp S3 の **G0 ボタンを押しながら** USB を接続すると、ダウンロードモード (Boot Mode) に入ります。
> その状態で再度 `Upload` を実行してください。

### 3-5. シリアルモニターで確認

書き込み完了後、シリアルモニターを開いて動作を確認します。

**方法A: VSCode (GUI)**
- PlatformIO メニューから `Monitor` をクリック

**方法B: コマンドライン**
```bash
pio device monitor
```

以下のように表示されれば成功です:

```
Connecting to MyHomeWiFi.......
Connected! IP: 192.168.1.42
mDNS: http://hackhome.local
Server started
```

**この IP アドレス (例: `192.168.1.42`) をメモしてください。** フロントエンドから接続する際に使います。

---

## 4. ファームウェアの動作確認

フロントエンドを起動する前に、curl や ブラウザで API が動いているか確認できます。

### ステータス確認

```bash
curl http://hackhome.local/api/status
```

レスポンス例:
```json
{
  "angle": 0,
  "pressing": false,
  "rssi": -45,
  "ip": "192.168.1.42"
}
```

### サーボを「押す」動作

```bash
curl -X POST http://hackhome.local/api/servo/press
```

サーボが 0° → 90° → 0° と動きます (押して戻る)。レスポンス:
```json
{"ok": true}
```

### サーボを任意の角度に設定

```bash
curl -X POST http://hackhome.local/api/servo/angle \
  -H "Content-Type: application/json" \
  -d '{"angle": 45}'
```

レスポンス:
```json
{"ok": true, "angle": 45}
```

### 物理ボタンで動作確認

M5Stamp S3 本体の **G0 ボタン** を押すと、サーボが「押す」動作をします。
API を使わなくても手元で動作確認ができます。

> **mDNS (`hackhome.local`) で繋がらない場合:**
> シリアルモニターに表示された IP アドレスを直接使ってください。
> 例: `curl http://192.168.1.42/api/status`

---

## 5. フロントエンドのセットアップ

### 5-1. 依存パッケージのインストール

```bash
cd v0.1/frontend

# パッケージインストール (初回のみ、数分かかる場合があります)
npm install
```

### 5-2. 開発サーバーの起動

```bash
npm run dev
```

以下が表示されます:
```
  ▲ Next.js 16.x.x
  - Local:        http://localhost:3000
  - Network:      http://192.168.x.x:3000

 ✓ Ready in X.Xs
```

### 5-3. ブラウザで開く

http://localhost:3000 にアクセスします。

スマホからアクセスする場合は、`Network` に表示されたアドレス (例: `http://192.168.1.100:3000`) を使います。
**PC と スマホ が同じ WiFi に接続されている必要があります。**

---

## 6. 全体の動作確認

1. ブラウザで http://localhost:3000 を開く
2. 画面上部の入力欄に ESP32 のアドレスを入力
   - デフォルトは `hackhome.local`
   - 繋がらない場合はシリアルモニターで確認した IP アドレスを入力 (例: `192.168.1.42`)
3. **緑のドット** が表示されたら接続成功
4. **「Press」ボタン** をクリック → サーボが 0° → 90° → 0° と動く
5. **スライダー** を動かす → サーボがリアルタイムで追従する

---

## 7. API リファレンス

ESP32 上で動作する REST API の仕様です。

### `GET /api/status`

デバイスの現在の状態を取得します。

**レスポンス:**

| フィールド | 型 | 説明 |
| :--- | :--- | :--- |
| `angle` | number | サーボの現在角度 (0〜180) |
| `pressing` | boolean | 現在「押す」動作中かどうか |
| `rssi` | number | WiFi 信号強度 (dBm)。-30 で最強、-90 で弱い |
| `ip` | string | ESP32 の IP アドレス |

### `POST /api/servo/press`

サーボで「スイッチを押す」動作を実行します。
0° → 90° に移動し、500ms 保持した後、0° に戻ります。

**リクエストボディ:** 不要

**レスポンス:** `{"ok": true}`

### `POST /api/servo/angle`

サーボを指定した角度に設定します。

**リクエストボディ:**
```json
{"angle": 45}
```

| フィールド | 型 | 範囲 | 説明 |
| :--- | :--- | :--- | :--- |
| `angle` | number | 0〜180 | 設定する角度 |

**レスポンス:** `{"ok": true, "angle": 45}`

**エラー (範囲外):** `{"error": "invalid angle"}` (HTTP 400)

---

## 8. ピンアサイン・設定値一覧

`v0.1/firmware/include/config.h` で定義されています。

### ピンアサイン

| 定数名 | GPIO | 用途 |
| :--- | :---: | :--- |
| `PIN_SERVO` | 1 | サーボの PWM 信号 |
| `PIN_BUTTON` | 0 | M5Stamp S3 内蔵ボタン |
| `PIN_LED` | 21 | M5Stamp S3 内蔵 RGB LED (WS2812) |

### サーボ設定

| 定数名 | デフォルト値 | 説明 |
| :--- | :---: | :--- |
| `SERVO_MIN_ANGLE` | 0 | サーボの最小角度 |
| `SERVO_MAX_ANGLE` | 180 | サーボの最大角度 |
| `SERVO_PRESS_ANGLE` | 90 | 「押す」ときの角度 |
| `SERVO_REST_ANGLE` | 0 | 待機時の角度 |
| `SERVO_PRESS_DELAY` | 500 | 「押す」状態を保持する時間 (ms) |

### その他

| 定数名 | デフォルト値 | 説明 |
| :--- | :---: | :--- |
| `MDNS_HOSTNAME` | `"hackhome"` | mDNS ホスト名 → `hackhome.local` でアクセス可能 |
| `DEBOUNCE_MS` | 200 | 物理ボタンのデバウンス時間 (ms) |

---

## 9. RGB LED の色の意味

M5Stamp S3 内蔵の RGB LED で、デバイスの状態がわかります。

| 色 | 状態 |
| :--- | :--- |
| 赤 (点灯) | 起動直後 / WiFi 切断 |
| 青 (点滅) | WiFi 接続中 |
| 緑 (点灯) | WiFi 接続済み・正常動作中 |
| 黄 (点灯) | サーボ「押す」動作中 |

---

## 10. カスタマイズ

### サーボの角度を調整したい

実際のスイッチに取り付けたとき、90° では強すぎる/弱すぎる場合があります。
`config.h` の以下を変更してください:

```cpp
#define SERVO_PRESS_ANGLE 90   // ← この値を調整 (例: 70 や 110)
#define SERVO_REST_ANGLE  0    // ← 待機位置も必要に応じて調整
```

### mDNS のホスト名を変えたい

複数台のデバイスを使う場合、名前を変えると区別できます:

```cpp
#define MDNS_HOSTNAME "hackhome-bedroom"  // → hackhome-bedroom.local
```

### 「押す」動作の保持時間を変えたい

```cpp
#define SERVO_PRESS_DELAY 500  // ← ミリ秒。1000 にすると1秒間押し続ける
```

---

## 11. トラブルシューティング

| 症状 | 原因 | 対処 |
| :--- | :--- | :--- |
| ビルドエラー | PlatformIO 未インストール or ライブラリ不足 | VSCode に PlatformIO 拡張を入れ直す。`pio run` で自動DLされる |
| 書き込みできない | ポートが認識されていない | G0 ボタンを押しながら USB 接続で Boot Mode に入る。ドライバが必要な場合は [Espressif USB ドライバ](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/establish-serial-connection.html) を参照 |
| シリアルモニターに何も出ない | USB CDC 未有効 | `platformio.ini` の `build_flags` に `-DARDUINO_USB_CDC_ON_BOOT=1` があるか確認 |
| WiFi に繋がらない (青点滅が続く) | SSID/PASS 間違い or 5GHz | `config.h` を確認。**2.4GHz の SSID** を使う |
| `hackhome.local` に繋がらない | mDNS 非対応環境 | IP アドレスを直接使う。Windows では mDNS が不安定な場合がある |
| フロントから操作できない | CORS エラー or 別ネットワーク | ブラウザの開発者ツール (F12) → Console でエラーを確認。ESP32 と PC が同じ WiFi か確認 |
| サーボが動かない | 配線ミス or 電力不足 | 赤→5V、茶→GND、橙→G1 を確認。USB ハブ経由だと電力不足になりやすい。PC の USB ポートに直接挿す |
| サーボがガタガタ震える | 電力不足 | USB ポートの電流が足りていない。外部 5V 電源を試す |
| Press しても角度が足りない | SERVO_PRESS_ANGLE が合っていない | `config.h` の `SERVO_PRESS_ANGLE` を調整して再書き込み |
