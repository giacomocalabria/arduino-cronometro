/*           Cronometro a fotocellule
 *               Output seriale
 *                 17/10/2017
 *             Giacomo Calabria
 */
//--------------------------------------------------------------------------------------------------------------------
//                                                 Variabili e costanti
//--------------------------------------------------------------------------------------------------------------------
#define startbtn 8      //pin start
#define strsbtn 7       //pin reset e stop cronometro
int lapCR = LOW;        //variabile che indica lo stato delle fotocellule (aperte o chiuse)
int stato = 0;          //stato del cronometro
int passaggi = 0;       //numero di passaggi
long start = 0;         //tempo di partenza
long tempo_lap  = 0;    //tempo al giro
int Dt1,Dt;             //variabili per calcolare il tempo di oscuramento della fotocellula
#define lux 1000        //il valore deve essere cambiato in base alle condizioni ambientali 
                        //per la calibrazione decommentare Serial.println(val); vedere il valore a fotocellula aperta 
                        //e chiusa e poi variare il valore al piu basso possibile, cosigliati numerosi test 
//--------------------------------------------------------------------------------------------------------------------
//                                                 Impostazioni di base
//--------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);     //inizializza la comunicazione seriale nel canale 19200
  pinMode(startbtn, INPUT);  // Setta il pin digitale 8 per stop e reset in ingresso
  pinMode(strsbtn, INPUT);   // Setta il pin digitale 7 per partenza in ingresso
  pinMode(13,OUTPUT);        // Setta il pin digitale 13 per visualizzazione stop in uscita
}
//--------------------------------------------------------------------------------------------------------------------  
//                                 Funzione per l'avvio del cronometro
//--------------------------------------------------------------------------------------------------------------------
void partenza(){
 if (digitalRead(startbtn) == HIGH && stato == 0){ //Accende cronometro
  start = millis();   //assegna il valore temporale attuale alla variabile start
  stato = 1;    //assegnazione dello stato 1 corrispondente all'attesa dello stop/lap/reset        
  tone(11, 550, 200);   //produce un suono in corrispondenza della partenza
  Serial.println("cronometro partito");    //output seriale per comunicare la partenza del cronometro
 } 
}
//--------------------------------------------------------------------------------------------------------------------  
//                                        Funzione per il tempo al giro 
//--------------------------------------------------------------------------------------------------------------------
void lap(){
  while (stato == 1) {
  if (digitalRead(startbtn) == HIGH && stato == 1){ 
   tempo_lap = millis() - start;
   Dt1= millis();
   passaggi++;
   output(); // visutalizza output
   fotocellule();   //esecuzione funzione fotocellule
   while (lapCR == HIGH){ fotocellule();}//esecuzione funzione fotocellule
   Dt= millis()-Dt1; 
   if (Dt<3)delay(3-Dt);//ritardo necessario affinchè le fotocellule non mandino un numero eccessivo di dati
 }
 fermare();
 }}
//--------------------------------------------------------------------------------------------------------------------  
//                                 Funzione per la lettura del valore delle fotocellule
//--------------------------------------------------------------------------------------------------------------------
void fotocellule(){
   int val = analogRead(A0);  // lettura valore analogico della fotocellula
   //Serial.println(val);     // visualizzazione valore fotocellula per la calibrazione in campo
   if (val < lux) {lapCR = HIGH; digitalWrite(13, HIGH);}
   else {lapCR = LOW; digitalWrite(13, LOW);}                 
}
//--------------------------------------------------------------------------------------------------------------------  
//                                 Funzione per la comunicazione dell'output
//--------------------------------------------------------------------------------------------------------------------
void output(){
   Serial.print("tempo ");            //comunicazione tempo
   Serial.print(passaggi);
   Serial.print(": ");
   Serial.print(tempo_lap/1000);
   Serial.print(".");
   Serial.print(tempo_lap%100);
   Serial.println("s");
}
//--------------------------------------------------------------------------------------------------------------------  
//                    Funzione che ferma la lettura delle fotocellule e che attiva il reset del cronometro
//--------------------------------------------------------------------------------------------------------------------
 void fermare(){
 if (digitalRead(strsbtn) == HIGH && stato == 1) stato = 0;   //stop e reset del programma
 }
//--------------------------------------------------------------------------------------------------------------------  
//                                    Funzione per il reset del cronometro
//--------------------------------------------------------------------------------------------------------------------
void reset(){
 stato = 0;
 passaggi = 0;
 tempo_lap  = 0;
 start = 0;
}
//--------------------------------------------------------------------------------------------------------------------  
//                                             Ciclo del programma principale
//--------------------------------------------------------------------------------------------------------------------
void loop() {
 partenza();
 lap(); 
 reset();
} 
