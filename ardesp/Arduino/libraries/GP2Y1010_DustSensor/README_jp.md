# GP2Y1010_DustSensor  
Arduino IDE 用の SHARP GP2Y1010AU0F ほこりセンサ用のライブラリです。  
version 0.4

# ライセンス  
License MIT

# 使い方

* このライブラリを Arduino IDE にインクルードしてください。
* SHARP GP2Y1010AU0F を Arduino や ESP-WROOM-02/32 に配線してください。

    左から  
    GP2Y1010AU0F 青 → 150Ω抵抗経由 Arduino Vout(5V or 3.3V), 220μFコンデンサ経由 Arduino GND  
    GP2Y1010AU0F 緑 → Arduino GND  
    GP2Y1010AU0F 白 → Arduino Digital pin  
    GP2Y1010AU0F 黄 → Arduino GND  
    GP2Y1010AU0F 黒 → Arduino Analog pin (ESP-WROOM-02の場合は TOUT) ※下記注意事項参照  
    GP2Y1010AU0F 赤 → Arduino Vout(5V or 3.3V)  
    
    ※注意事項  
      - ESP-WROOM-02 を使用する場合、analog TOUTピンは1Vまでしか対応しておりません。そのため、分圧回路にて5V or 3.3Vを1Vまで降圧させてください。  
      - 青/赤(Vin)の電圧と、黒(Analog out)の電圧は合わせてください。(5V入力、3.3V出力は不可)  
      - ESP32を利用する場合、ADCは3.3Vまでの対応です。そのため、青/赤(Vin)は3.3Vに接続し、黒(analog out)はそのままADC対応のGPIOに接続する方法が楽です。  

* Arduino IDE からスケッチ例を参照してください。

# 注意事項

* このライブラリは GP2Y101**4**AU0F には対応していません。

# GP2Y1010_DustSensor ライブラリ説明

## コンストラクタ

* GP2Y1010_DustSensor  
  素のコンストラクタ。こちらを使って変数定義した場合は、事後に begin関数を呼び出してピンを設定してください。

* GP2Y1010_DustSensor(int ledPin, int measurePin)  
  使用するピンを設定できるコンストラクタ。  
    ledPin = 白の配線で接続したピン  
    measurePin = 黒の配線で接続したピン  

## public関数

* void begin(int ledPin, int measurePin)  
  使用するピンを設定します。  
    ledPin = 白の配線で接続したピン  
    measurePin = 黒の配線で接続したピン  

* void setADCbit(int bit)  
  使用する機器のADCのビット数を設定します。  
  ESP-WROOM-02/Arduino(AVR)は10(bit)、ESP-WROOM-32は12(bit) です。  
  デフォルト値は 10 (bit)です。
  
* int  getADCbit();
  設定してあるADCのビット数を取得します。  
  
* void setInputVolts(float volts)  
  入力電圧(5V or 3,3V)を設定します。  
  入力電圧を5Vに設定すると、Analog出力も5Vで出力されます。  
  Analog出力を3.3Vにしたい場合は、入力電圧も3.3Vにするか、分圧回路で降圧してください。  
  デフォルト値は 5 (V)です。
  
* float getInputVolts();
  設定してある入力電圧(5V or 3,3V)を取得します。

* float getDustDensity()  
  ほこりの濃度を取得します。単位は μg/m3 です。


# リンク
* GP2Y1010AU0F Data sheet  
  http://www.sharp.co.jp/products/device/doc/opto/gp2y1010au_j.pdf

* GP2Y1010AU0F 説明資料  
  http://www.sharp.co.jp/products/device/doc/opto/gp2y1010au_appl_j.pdf 

## history
* ver. 0.1: 非公開
* ver. 0.2: 初回公開
* ver. 0.3: ESP-WROOM-32対応、ADCbit, InputVolts関係の関数を追加
* ver. 0.31: GP2Y1014AU0F非対応の表示を追加、ほこり濃度がマイナスにならないように修正
* ver. 0.4: Arduino AVR 環境で動作しなかったバグを修正。あわせて getDustDensityAvg() と setDustDensityHistoryMaxSize() 関数を削除。
