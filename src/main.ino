#include <Arduino.h>
#include "spectrogram.h"
#include "embedia.h"
#include "embedia_model.h"

// Probar grabar audios aproximadamente 5 veces. Predecir 3 veces y compararlos con lo que dice Python.
/* Declaración de constantes */
#define SENSOR_PIN 34
#define BOTON_PIN 5
#define OUTPUT_PIN 15

/* Prototipos de funciones */
void grabar_audio(void);
void imprimir_audio(void);
void imprimir_espectrograma(void);

float * audio;
float * espectrograma;

data3d_t input = { INPUT_CHANNELS, INPUT_WIDTH, INPUT_HEIGHT,  NULL};

// data_t input = { SPEECH_COMMANDS_MODEL_CHANNELS, SPEECH_COMMANDS_MODEL_WIDTH, SPEECH_COMMANDS_MODEL_HEIGHT, espectrograma };
data1d_t resultados; 

char myMessage;

//const char * predicciones[] = {"Yes", "No", "Up", "Down", "Right", "Left"};
const char * predicciones[] = {"uno", "dos", "mate" };


void setup() {
  Serial.begin(115200);
  model_init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(BOTON_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);
  delay(1000);

}

void loop() {
  //Serial.println(TS_US);
  audio = (float*)swap_alloc(sizeof(float)*FRAME_LENGTH); //buffer dinámico de la app
  espectrograma = (float*)swap_alloc(sizeof(float)*SPEC_SIZE);

  grabar_audio(); //hasta no presionar el botón el programa se bloquea acá
  //imprimir_audio();
  //imprimir_espectrograma();
  

  unsigned long tiempo = micros();
  
  create_spectrogram(audio, espectrograma);
  input.data = espectrograma;


  uint8_t prediccion = model_predict_class(input, &resultados);

  tiempo = micros() - tiempo;
  
  Serial.print("Dijiste: ");
  Serial.println(predicciones[prediccion]);

  //Serial.print("Confianza en %: ");
  //Serial.println(resultados.data[prediccion]);

  //Serial.print("Tiempo de prediccion en microsegundos: ");
  //Serial.println(tiempo);

 //switch de los comandos. Como en CyM
  
  Serial.println();
  
  if(!strcmp(predicciones[prediccion], "mate")) {
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(1000);
    digitalWrite(OUTPUT_PIN, LOW);
  }

  delay(2000);
}

void grabar_audio(){
  float prom=0;
  float valor;
  int i=0;
  int listo;
  int boton = 0;
  int max = 0;

  Serial.println("Presione el boton y pronuncie su orden");
  while(boton == 0){ //al presionar detecto 0
    boton = digitalRead(BOTON_PIN);
    delay(20); //Delay necesario por efecto rebote
  }

  while(boton != 0){ // al realizar el release recién se comienza a grabar
    boton = digitalRead(BOTON_PIN);
    delay(20);
  }

  Serial.println("Iniciando grabación...");
  delay(2000); // 2 segundo xD
  Serial.println("Grabando:");

  // Grabación de voz
  listo = 0;
  digitalWrite(LED_BUILTIN, HIGH);

  //El tiempo de grabación me lo da este for de puntos
  for(i=0;i<FRAME_LENGTH;i++){
    audio[i] = analogRead(SENSOR_PIN);
    prom += audio[i];
    delayMicroseconds(TS_US); //Tiempo de muestreo x(nTs) = x[n]
  }
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Fin de la grabación");
  prom /= FRAME_LENGTH;

  // Resto componente de continua
  for(i=0;i<FRAME_LENGTH;i++){
    audio[i] = (audio[i] - prom);
    if(max < audio[i]) {
      max = audio[i];
    }
  }

  for(i=0;i<FRAME_LENGTH;i++){
    audio[i] /= max;
  }

  max = 0;
  
  
}

// Imprimir el audio y ejecutarlo en Python. Compararlo con los audios originales entreandos con la IA.
void imprimir_audio(){
  Serial.print("audio_vec = np.array([");
  Serial.print(audio[0]);
  for(int i=1;i<FRAME_LENGTH;i++){
    Serial.print(", ");
    Serial.print(audio[i]);
  }
  Serial.print("])");
  Serial.println();
}

void imprimir_espectrograma(){
  Serial.println("espectrograma_vec = np.array([");
  Serial.print(espectrograma[0]);
  for(int i=1;i<SPEC_SIZE;i++){
    Serial.print(", ");
    Serial.print(espectrograma[i]);
  }
  Serial.println("])");
  Serial.println("espectrograma_vec = np.reshape(espectrograma_vec, (bloques, n_mels))");
  Serial.println();
}
