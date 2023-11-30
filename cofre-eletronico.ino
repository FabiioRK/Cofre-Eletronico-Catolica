#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#define tamanhoSenha 7
#define echoPin A3 //Pino ECHO do sensor no pino analógica A1
#define trigPin A2 //Pino TRIG do sensor no pino analógica A0

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#define col 16
#define lin  2
#define ende 0x27
LiquidCrystal_I2C lcd(ende,col,lin);
//LiquidCrystal_I2C lcd(0x20,16,2);

const byte ROWS = 4; //4 linhas
const byte COLS = 4; //4 colunas
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {A0, A1, 11, 10};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo servo;

char senhaDigitada[tamanhoSenha];
char senha[tamanhoSenha] = "3C6A9B";
byte contador = 0;
int buzzer = 3;
byte controle = 0;
long duracao;
long HR_dist=0;
int minimumRange=1;
int maximumRange=200;

void setup()
{
  pinMode(buzzer,OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  servo.attach(5);
  servo.write(0);
  lcd.init();    // configura LCD de 16 colunas de 2 linhas
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  displayInicial();

  pinMode(trigPin, OUTPUT); //define o pino TRIG como saída
  pinMode(echoPin, INPUT); //define o pino ECHO como entrada
}

void loop()
{
  
  char teclaDigitada = keypad.getKey();
  
  digitalWrite(12, HIGH);
  
  if (teclaDigitada) {
    if (teclaDigitada != '#') {
      tone(buzzer,262);    
      delay(50);
      noTone(buzzer); 
      
      lcd.clear();
      lcd.print("Tecla invalida!");
      delay(500);
      lcd.clear();
      displayInicial();
    }
    
    if (teclaDigitada == '#') {
      controle = 1;
      lcd.clear();
      lcd.print("INSIRA SUA SENHA");
      lcd.setCursor(0, 1);
      
      tone(buzzer,NOTE_A5);
      delay(100);
      noTone(buzzer);
      
      while (controle == 1) {
        inserirSenha();
      }
    }
  }  
}

void inserirSenha() {
  
  char teclaDigitada = keypad.getKey();
  
  if (teclaDigitada) {
    if (contador == 0) {
      lcd.clear();
      lcd.print("INSIRA SUA SENHA");
      lcd.setCursor(0, 1);
    }
    
    if(teclaDigitada == '#'){
      lcd.clear();
      lcd.print("Tecla invalida!");
      delay(500);
      lcd.clear();
      lcd.print("INSIRA SUA SENHA");
      lcd.setCursor(0, 1);
      
      for (int i = 0; i < contador; i++) {
        lcd.print("*");
      }
    } 
    else if(teclaDigitada == 'D') {
      apagarTecla();
      tone(buzzer,262);    
      delay(50);
      noTone(buzzer); 
    }
    else {
      senhaDigitada[contador] = teclaDigitada;  
      contador++; 
      lcd.print("*");
      tone(buzzer,494);    
      delay(50);
      noTone(buzzer); 
    }
    
  }
  
  if(contador == tamanhoSenha - 1){
    if(!strcmp(senhaDigitada, senha)){
      int controlePorta = 1;
      
      lcd.clear();
      lcd.print(" ACESSO LIBERADO");
      lcd.setCursor(0, 1);
      lcd.print(" *SENHA VALIDA*");
      
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
      servo.write(90);
      somSenhaCorreta();
      delay(5000);
      lcd.clear();
      lcd.print("Tranque a porta!");
      lcd.setCursor(0, 1);
      lcd.print("***************");
      
      while(controlePorta = 1) {
        char teclaDigitada = keypad.getKey();
        
        if (calcularDistancia() <= 10) {
          if(calcularDistancia() <= 10 && teclaDigitada == '#'){
            digitalWrite(13, LOW);
            limparDados();
            lcd.clear();
            displayInicial();
            somSenhaIncorreta();
            controlePorta = 0;
            controle = 0;
            servo.write(0);
            break;
          } 
        }
      }
    }
    else {
      lcd.clear();
      lcd.print(" ACESSO NEGADO");
      lcd.setCursor(0, 1);
      lcd.print("*SENHA INVALIDA*");
      
      somSenhaIncorreta();
      digitalWrite(12, LOW);
      delay(500);
      digitalWrite(12, HIGH);
      delay(500);
      digitalWrite(12, LOW);
      delay(500);
      digitalWrite(12, HIGH);
      delay(5000);
      
      digitalWrite(12, LOW);
      limparDados();
      lcd.clear();
      displayInicial();
      controle = 0;
    }
  }
}

void displayInicial()
{
  lcd.setCursor(0, 0);
  lcd.print("    TRANCADO");
  lcd.setCursor(0, 1);
  lcd.print("****************");
}

void limparDados(){
  while(contador !=0){
    senhaDigitada[contador--] = 0; 
  }
  return;
}

void apagarTecla() {
  if (contador > 0) {
    senhaDigitada[contador] = 0;
    contador--;
    lcd.clear();
    lcd.print("INSIRA SUA SENHA");
    lcd.setCursor(0, 1);
    
    for (int i = 0; i < contador; i++) {
      lcd.print("*");
    }
  }
  return;
}

void somSenhaCorreta() {
  tone(buzzer,NOTE_A5);
  delay(100);
  tone(buzzer,NOTE_B5);
  delay(100);
  tone(buzzer,NOTE_C5);
  delay(100);
  tone(buzzer,NOTE_B5);
  delay(100);
  tone(buzzer,NOTE_C5);
  delay(100);
  tone(buzzer,NOTE_D5);
  delay(100);
  tone(buzzer,NOTE_C5);
  delay(100);
  tone(buzzer,NOTE_D5);
  delay(100);
  tone(buzzer,NOTE_E5);
  delay(100);
  tone(buzzer,NOTE_D5);
  delay(100);
  tone(buzzer,NOTE_E5);
  delay(100);
  tone(buzzer,NOTE_E5);
  delay(100);
  noTone(buzzer);
}

void somSenhaIncorreta() {
  tone(buzzer,NOTE_G4);
  delay(250);
  tone(buzzer,NOTE_C4);
  delay(500);
  noTone(buzzer);
}

long calcularDistancia(){
  digitalWrite(trigPin, LOW); //não envia som
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH); //envia som 
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW); //não envia o som e espera o retorno do som enviado
  duracao = pulseIn(echoPin,HIGH); //Captura a duração em tempo do retorno do som.
  HR_dist = duracao/56; //Calcula a distância
  //Serial.println(HR_dist); //Exibe na Serial a distância
  
  return HR_dist;
}
