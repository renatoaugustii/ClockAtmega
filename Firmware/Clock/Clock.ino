/*
   PROJETO RELÓGIO COM ATMEGA 328P
   
   Author: Eng. Renato Augusto
    
   OBJETIVO: O objetivo da construção desse projeto se deve ao desafio de construir um relógio desde o desenvolvimento da programação do microcontrolador  ATMEGA 328p
   e da placa onde será montado os componentes eletrônicos. A IDE do Arduino foi utilizada apenas para gravar o microcontrolador. O funcionamento do relógio é simples e 
   não possui qualquer função especial. (Não possui Despertador)
   Este projeto é livre e poderá ser melhorado e modificado conforme necessidade do usuário, desde que seja comentado quem foi o criador do arquivo, assim como o seu link de origem.

   Todo o esquemático, fotos e desenhos estão disponíveis na página do Github abaixo.
   
   Github: https://github.com/renatoaugustii/ClockAtmega

   Atribuição-NãoComercial -  Sujeito a punições legais

   **  CONTROLE DE VERSÕES **
   Versão: 1.0 - Por Renato Augusto
   Data: 26/09/2019 
   Belo Horizonte - MG / Brasil 
   
   (Se realizar alguma modificação no projeto identifique o número da versão acima)
   
   Qualquer dúvida ou sujestão entre em contato:
   renato.augusto.correa@outlook.com
   
*/

#include <DS1307.h> // BIBLIOTECA PADRÃO PARA USO DO CI DS1307 - CLOCK

//Modulo RTC DS1307 ligado as portas 27 e 28 do ATMEGA 328p
DS1307 rtc(A4, A5);

//Define Pins to use on Display
int const disp_1 = 5; // PINO 11 DO ATMEGA
int const disp_2 = 6; // PINO 12 DO ATMEGA
int const disp_3 = 7; // PINO 13 DO ATMEGA
int const disp_4 = 8; // PINO 14 DO ATMEGA

//Deine Pins to use on Decoder BCD 4511
int const A = 9; // PINO 15 DO ATMEGA
int const B = 10; // PINO 16 DO ATMEGA
int const C = 11; // PINO 17 DO ATMEGA
int const D = 12; // PINO 18 DO ATMEGA

int const DP = 13; // PINO 19 DO ATMEGA

int const btn_h = 4; // PINO 6 DO ATMEGA
int const btn_m = 3; // PINO 5 DO ATMEGA

int ajuste_hora = 12; // VARIÁVEL DE INCREMENTO NAS HORAS
int ajuste_minuto = 0; // VARIÁVEL DE INCREMENTO NOS MINUTOS

// VARIÁVEIS DE TRATATIVA DE TEMPO
long time_lag = 4;
long tempo_atual = 0;
long tempo_botao_h = 0;
long tempo_botao_m = 0;

// VARIÁVEL PARA SALVAR O ESTADO DE : NO DISPLAY
bool estado = false;

void setup() {
  
  acertar_relogio(ajuste_hora,ajuste_minuto); // CARREGA HORAS E MINUTOS NO DISPLAY
  
  Serial.begin(9600); // INICIALIZA COMUNICAÇÃO SERIAL

  //DEFINIÇÃO DE I/O'S
  pinMode(disp_1, OUTPUT);
  pinMode(disp_2, OUTPUT);
  pinMode(disp_3, OUTPUT);
  pinMode(disp_4, OUTPUT);

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);

  pinMode(DP, OUTPUT);

  pinMode(13, OUTPUT);

  pinMode(btn_h,INPUT); // POSSUI PULL_DOWN FÍSICO
  pinMode(btn_m,INPUT); // POSSUI PULL_DOWN FÍSICO

 tempo_atual = millis(); //  GRAVA O TEMPO DECORRIDO PARA SER USADO NAS LÓGICAS DE ATRASO SEM DELAY
}

void loop() {

 
  //Mostra as informações no Serial Monitor
  String hora = rtc.getTimeStr();
  String h = hora.substring(0, 2);
  String m = hora.substring(3, 5);
  String s = hora.substring(6, 8);

  int hours = h.toInt();
  int minutes = m.toInt();
  int seconds = s.toInt();

  if (digitalRead (btn_h) == true) {
          if ((millis()-tempo_botao_h)>500) // verifica se o tempo decorrido é maior que 500ms, caso seja será trocado de estado
          { 
                hours = hours+1;  //Define o horario
                if (hours>23){hours=0;}
                rtc.setTime(hours, minutes, seconds);
                tempo_botao_h = millis();
          }
  }
  
  if (digitalRead (btn_m) == true) {
      if ((millis()-tempo_botao_m)>500) // verifica se o tempo decorrido é maior que 500ms, caso seja será trocado de estado
          { 
            minutes = minutes+1;  //Define o horario
            if (minutes>59){minutes=0;}
            rtc.setTime(hours, minutes, seconds);
            tempo_botao_m = millis();
          }
  }

  //Serial.print(hours);
  //Serial.print(":");
  //Serial.println(minutes);

  show_hours(hours);
  show_minutes(minutes);

  
  if ((millis()-tempo_atual)>500) // verifica se o tempo decorrido é maior que 500ms, caso seja será trocado de estado
  { 
      estado = !estado;
      digitalWrite(DP,estado);
      Serial.println("LIGA : ");
      tempo_atual = millis();
    }

}

void acertar_relogio(int hh,int mm){
    //Aciona o relogio
  rtc.halt(false);
  rtc.setDOW(MONDAY);      //Define o dia da semana
  rtc.setTime(hh, mm, 0);     //Define o horario
  rtc.setDate(01, 01, 2019);   //Define o dia, mes e ano
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  
  }
void Show_Number (int num) 
{
  if (num == 0) {
    digitalWrite(A, LOW);  //0
    digitalWrite(B, LOW) ;
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 1) {
    digitalWrite(A, HIGH);  //1
    digitalWrite(B, LOW) ;
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 2) {
    digitalWrite(A, LOW);  //2
    digitalWrite(B, HIGH) ;
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 3) {
    digitalWrite(A, HIGH);  //3
    digitalWrite(B, HIGH) ;
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 4) {
    digitalWrite(A, LOW);  //4
    digitalWrite(B, LOW) ;
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 5) {
    digitalWrite(A, HIGH);  //5
    digitalWrite(B, LOW) ;
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 6) {
    digitalWrite(A, LOW);  //6
    digitalWrite(B, HIGH) ;
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 7) {
    digitalWrite(A, HIGH);  //7
    digitalWrite(B, HIGH) ;
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    return;
  }
  if (num == 8) {
    digitalWrite(A, LOW);  //8
    digitalWrite(B, LOW) ;
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    return;
  }
  if (num == 9) {
    digitalWrite(A, HIGH);  //9
    digitalWrite(B, LOW) ;
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    return;
  }
  return;
}

void Show_Display (int num) {

  if (num == 1) {
    digitalWrite(disp_1, HIGH);  //0
    digitalWrite(disp_2, LOW);
    digitalWrite(disp_3, LOW);
    digitalWrite(disp_4, LOW);
    return;
  }
  if (num == 2) {
    digitalWrite(disp_1, LOW);  //0
    digitalWrite(disp_2, HIGH);
    digitalWrite(disp_3, LOW);
    digitalWrite(disp_4, LOW);
    return;
  }
  if (num == 3) {
    digitalWrite(disp_1, LOW);  //0
    digitalWrite(disp_2, LOW);
    digitalWrite(disp_3, HIGH);
    digitalWrite(disp_4, LOW);
    return;
  }
  if (num == 4) {
    digitalWrite(disp_1, LOW);  //0
    digitalWrite(disp_2, LOW);
    digitalWrite(disp_3, LOW);
    digitalWrite(disp_4, HIGH);
    return;
  }

  delay(time_lag);

  if (num == 1) {
    digitalWrite(disp_1, LOW);  //0
    digitalWrite(disp_2, LOW);
    digitalWrite(disp_3, LOW);
    digitalWrite(disp_4, LOW);
    return;
  }
  if (num == 2) {
    digitalWrite(disp_1, LOW);  //0
    digitalWrite(disp_2, LOW);
    digitalWrite(disp_3, LOW);
    digitalWrite(disp_4, LOW);
    return;
  }
  if (num == 3) {
    digitalWrite(disp_1, LOW);  //0
    digitalWrite(disp_2, LOW);
    digitalWrite(disp_3, LOW);
    digitalWrite(disp_4, LOW);
    return;
  }
  if (num == 4) {
    digitalWrite(disp_1, LOW);  //0
    digitalWrite(disp_2, LOW);
    digitalWrite(disp_3, LOW);
    digitalWrite(disp_4, LOW);
    return;
  }
  return;
}

void show_hours(int hour) {

  if (hour < 10) {
    Show_Number(0);
    Show_Display(1);
  }
  if (hour >= 10 && hour < 20) {
    Show_Number(1);
    Show_Display(1);
  }
  if (hour >= 20) {
    Show_Number(2);
    Show_Display(1);
  }

  delay(time_lag);

  if (hour < 10) {
    Show_Number(hour);
    Show_Display(2);
  }
  if (hour >= 10 && hour < 20) {
    Show_Number(hour - 10);
    Show_Display(2);
  }
  if (hour >= 20) {
    Show_Number(hour - 20);
    Show_Display(2);
  }

  delay(time_lag);
  return;
}

void show_minutes(int minute) {
  if (minute < 10) {
    Show_Number(0);
    Show_Display(3);
  }
  if (minute >= 10 && minute < 20) {
    Show_Number(1);
    Show_Display(3);
  }
  if (minute >= 20 && minute < 30) {
    Show_Number(2);
    Show_Display(3);
  }
  if (minute >= 30 && minute < 40) {
    Show_Number(3);
    Show_Display(3);
  }
  if (minute >= 40 && minute < 50) {
    Show_Number(4);
    Show_Display(3);
  }
  if (minute >= 50 && minute < 60) {
    Show_Number(5);
    Show_Display(3);
  }

  delay(time_lag);

  if (minute < 10) {
    Show_Number(minute);
    Show_Display(4);
  }
  if (minute >= 10 && minute < 20) {
    Show_Number(minute - 10);
    Show_Display(4);
  }
  if (minute >= 20 && minute < 30) {
    Show_Number(minute - 20);
    Show_Display(4);
  }
  if (minute >= 30 && minute < 40) {
    Show_Number(minute - 30);
    Show_Display(4);
  }
  if (minute >= 40 && minute < 50) {
    Show_Number(minute - 40);
    Show_Display(4);
  }
  if (minute >= 50 && minute < 60) {
    Show_Number(minute - 50);
    Show_Display(4);
  }
  delay(time_lag);
  return;
}
