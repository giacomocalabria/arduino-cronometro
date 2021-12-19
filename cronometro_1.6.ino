/*           Cronometro a fotocellule
 *               Output seriale
 *                 17/10/2017
 *             Giacomo Calabria
 */
//--------------------------------------------------------------------------------------------------------------------
//                                                 Variabili e costanti
//--------------------------------------------------------------------------------------------------------------------
#define stspbtn 7      //pin start stop
#define stbtn 8
#define lapbtn 9
bool lapCR = false;        //variabile che indica lo stato delle fotocellule (aperte o chiuse)
int stato = 0;          //stato del cronometro
int passaggi = 0;       //numero di passaggi
long start = 0;         //tempo di partenza
long tempo_lap  = 0;    //tempo al giro
uint16_t tempi[1000];
#define lux 500        //il valore deve essere cambiato in base alle condizioni ambientali 
                        //per la calibrazione decommentare Serial.println(val); vedere il valore a fotocellula aperta 
                        //e chiusa e poi variare il valore al piu basso possibile, cosigliati numerosi test 
//--------------------------------------------------------------------------------------------------------------------
//                                                 Impostazioni di base
//--------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);     //inizializza la comunicazione seriale nel canale 19200
  pinMode(stspbtn, INPUT);  // Setta il pin digitale 7 per start e stop / reset in ingresso
  pinMode(stbtn,OUTPUT); 
  pinMode(lapbtn,OUTPUT);
  pinMode(13,OUTPUT);        // Setta il pin digitale 13 per visualizzazione stop in uscita
  digitalWrite(stbtn, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(lapbtn, HIGH);
}
//--------------------------------------------------------------------------------------------------------------------  
//                                 Funzione per l'avvio del cronometro
//--------------------------------------------------------------------------------------------------------------------
void partenza(){
 digitalWrite(stbtn, HIGH); 
 if (digitalRead(stspbtn) == HIGH && stato == 0){ //Accende cronometro
  start = millis();   //assegna il valore temporale attuale alla variabile start
  digitalWrite(stbtn, LOW); 
  delay(250);
  digitalWrite(stbtn, HIGH);
  stato = 1;    //assegnazione dello stato 1 corrispondente all'attesa dello stop/lap/reset        
  Serial.println("Cronometro partito");    //output seriale per comunicare la partenza del cronometro
  delay(1000);
 } 
}
//--------------------------------------------------------------------------------------------------------------------  
//                                        Funzione per il tempo al giro 
//--------------------------------------------------------------------------------------------------------------------
void lap(){
 while (stato == 1) {
  fotocellule();
  digitalWrite(13, HIGH);
  if (lapCR){
   digitalWrite(lapbtn, LOW); 
   tempo_lap = millis() - start;
   digitalWrite(13, LOW);
   passaggi++; 
   output(); // visutalizza output
   while (lapCR) fotocellule();//esecuzione funzione fotocellule
   delay(500);
  }
 digitalWrite(13, HIGH);
 digitalWrite(lapbtn, HIGH); 
 fermare();
 }
}
//--------------------------------------------------------------------------------------------------------------------  
//                                 Funzione per la lettura del valore delle fotocellule
//--------------------------------------------------------------------------------------------------------------------
void fotocellule(){
   //int val = analogRead(A0);  // lettura valore analogico della fotocellula
   //Serial.println(analogRead(A0));     // visualizzazione valore fotocellula per la calibrazione in campo
   if (analogRead(A0) > lux) lapCR = true;
   else lapCR = false;                  
}
//--------------------------------------------------------------------------------------------------------------------  
//                                 Funzione per la comunicazione dell'output
//--------------------------------------------------------------------------------------------------------------------
void output(){
   if(tempo_lap<60000){
    Serial.print("SL ");            //comunicazione tempo
    Serial.print(passaggi);
    Serial.print("   ");
    Serial.print(tempo_lap/1000);
    Serial.print(".");
    Serial.println(tempo_lap/100%10);//per decimi
   }
   else{
    Serial.print("SL ");            //comunicazione tempo
    Serial.print(passaggi);
    Serial.print("   ");
    Serial.print(tempo_lap/1000/60);
    Serial.print(":");
    Serial.print((tempo_lap/1000)-((tempo_lap/1000/60)*60));
    Serial.print(",");
    Serial.println(tempo_lap/100%10);
   }
   tempi[passaggi]=tempo_lap;
   
}
//--------------------------------------------------------------------------------------------------------------------  
//                    Funzione che ferma la lettura delle fotocellule e che attiva il reset del cronometro
//--------------------------------------------------------------------------------------------------------------------
 void fermare(){
 if (digitalRead(stspbtn) == HIGH){
   tempo_lap = millis() - start; 
   stato = 0;   //stop e reset del programma
   if(tempo_lap<60000){
    Serial.print("SP ");            //comunicazione tempo
    Serial.print(passaggi);
    Serial.print("   ");
    Serial.print(tempo_lap/1000);
    Serial.print(".");
    Serial.println(tempo_lap/100%10);//per decimi
   }
   else{
    Serial.print("SP ");            //comunicazione tempo
    Serial.print(passaggi);
    Serial.print("   ");
    Serial.print(tempo_lap/1000/60);
    Serial.print(":");
    Serial.print((tempo_lap/1000)-((tempo_lap/1000/60)*60));
    Serial.print(",");
    Serial.println(tempo_lap/100%10);
   }
  reset();
  delay(10000);
 }}
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
} 
