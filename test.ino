

const int sensorPin = A3;       // 첫 번째 센서가 연결된 핀 (3번 아날로그 핀)
const int tempSensorPin = A2;   // 온도 센서가 연결된 핀 (2번 아날로그 핀)
const int pressureSensorPin = A4; // 압력 센서가 연결된 핀 (4번 아날로그 핀)
const int humiditySensorPin = A5; // 습도 센서가 연결된 핀 (5번 아날로그 핀)
const int soundSensorPin = A1;  // 소리 감지 센서가 연결된 핀 (1번 아날로그 핀)

const int ledPin = 9;           // 첫 번째 LED가 연결된 핀 (PWM 제어가 가능한 핀)
const int ledPin2 = 8;          // 두 번째 LED가 연결된 핀 (온도에 따라 켜질 LED)
const int ledPin3 = 7;          // 세 번째 LED가 연결된 핀 (습도에 따라 켜질 LED)

// 세그먼트 핀 정의
const int segmentPins[7] = {2, 3, 4, 5, 6, 10, 11};  // a, b, c, d, e, f, g 핀

// 변수 선언
int sensorValue = 0;        // 첫 번째 센서 값 저장
int ledBrightness = 0;      // 첫 번째 LED 밝기 값 저장
int tempValue = 0;          // 온도 센서 값 저장
float temperature = 0.0;    // 계산된 온도 값 저장
int pressureValue = 0;      // 압력 센서 값 저장
int humidityValue = 0;      // 습도 센서 값 저장
int soundValue = 0;         // 소리 감지 센서 값 저장
int dB = 0;                 // 계산된 데시벨 값 저장

// 세그먼트 디스플레이 숫자 정의 (0~9)
const byte segmentDigits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1}   // 9
};

void setup() {
  // 핀 모드 설정
  pinMode(ledPin, OUTPUT);    // 첫 번째 LED 핀을 출력 모드로 설정
  pinMode(ledPin2, OUTPUT);   // 두 번째 LED 핀을 출력 모드로 설정
  pinMode(ledPin3, OUTPUT);   // 세 번째 LED 핀을 출력 모드로 설정
  pinMode(sensorPin, INPUT);  // 첫 번째 센서 핀을 입력 모드로 설정
  pinMode(tempSensorPin, INPUT); // 온도 센서 핀을 입력 모드로 설정
  pinMode(pressureSensorPin, INPUT); // 압력 센서 핀을 입력 모드로 설정
  pinMode(humiditySensorPin, INPUT); // 습도 센서 핀을 입력 모드로 설정
  pinMode(soundSensorPin, INPUT); // 소리 감지 센서 핀을 입력 모드로 설정

  // 세그먼트 디스플레이 핀을 출력 모드로 설정
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // 시리얼 통신 시작 (디버깅용)
  Serial.begin(9600);
}

void loop() {
  // 첫 번째 센서 값을 읽기 (0부터 1023까지의 아날로그 값)
  sensorValue = analogRead(sensorPin);
  
  // 첫 번째 센서 값을 LED 밝기로 매핑 (0부터 1023을 0부터 255의 밝기로 변환)
  ledBrightness = map(sensorValue, 0, 1023, 0, 255);

  // 첫 번째 LED 밝기 설정
  analogWrite(ledPin, ledBrightness);

  // 온도 센서 값을 읽기 (0부터 1023까지의 아날로그 값)
  tempValue = analogRead(tempSensorPin);
  temperature = (tempValue * 5.0 * 100.0) / 1024.0;

  // 온도가 30도 이상이면 두 번째 LED 켜기
  if (temperature >= 30) {
    digitalWrite(ledPin2, HIGH);  // 두 번째 LED 켜기
  } else {
    digitalWrite(ledPin2, LOW);   // 두 번째 LED 끄기
  }

  // 압력 센서 값을 읽기 (0부터 1023까지의 아날로그 값)
  pressureValue = analogRead(pressureSensorPin);

  // 습도 센서 값을 읽기 (0부터 1023까지의 아날로그 값)
  humidityValue = analogRead(humiditySensorPin);

  // 습도가 일정 수준 이상이면 세 번째 LED 켜기
  if (humidityValue > 600) {
    digitalWrite(ledPin3, HIGH);  // 세 번째 LED 켜기
  } else {
    digitalWrite(ledPin3, LOW);   // 세 번째 LED 끄기
  }

  // 소리 감지 센서 값을 읽고 데시벨 값으로 변환
  soundValue = analogRead(soundSensorPin);
  dB = map(soundValue, 0, 1023, 0, 9);  // 0~1023 값을 0~9 데시벨로 매핑
  
  // 세그먼트 디스플레이에 데시벨 값 표시
  displayDigit(dB);

  // 시리얼 모니터에 값 출력
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" - LED Brightness: ");
  Serial.print(ledBrightness);
  Serial.print(" - Temperature: ");
  Serial.print(temperature);
  Serial.print(" - Humidity: ");
  Serial.print(humidityValue);
  Serial.print(" - dB: ");
  Serial.println(dB);




  // 10ms 대기
  delay(10);
}

// 세그먼트 디스플레이에 숫자를 출력하는 함수
void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], segmentDigits[num][i]);
  }
}
