const int n_duino = 2;
const int n_address = 4;
const int n_ports = 12;
const int n_analog = 3;

char address[n_duino][n_address];
char digit_data[n_duino][n_ports];
int analog_data[n_duino][n_analog];


#define CONTROL 12
#define OK 11
#define STAY 10

void setup() {
  pinMode(OK, OUTPUT);
  pinMode(CONTROL, OUTPUT);
  pinMode(STAY, OUTPUT);
  Serial.begin(57600);
  setAddress();
  setDigitZero();
  setAnalogZero();
  
  digitalWrite(CONTROL, HIGH);
  digitalWrite(OK, HIGH);
  delay(3000);
  digitalWrite(CONTROL, LOW);
  digitalWrite(OK, LOW);
  
}

void loop() {
  IO();
  handler();
}


void IO(){
  for(int i = 0; i < n_duino; i++){
    char buff[n_address + n_ports];

    for(int j = 0; j < n_address; j++){
      buff[j] = address[i][j];
    }

    for(int j = n_address; j < n_ports + n_address; j++){
      buff[j] = digit_data[i][j-n_address];
    }
    int len = n_address + n_ports;
    Serial.flush();
    Serial.write(buff, len);
    int cnt = 0;
    while(Serial.available() < 1){
      if(cnt == 10)
        break;
      control(1);
      cnt++;
    }
    
    digitalWrite(OK, HIGH);
    delay(100);
    digitalWrite(OK, LOW);
    delay(100);
    
    String in = Serial.readString();
    Serial.flush();
    
    writeDataAnalog(i,in);
    digitalWrite(STAY, HIGH);
    delay(5000);
    digitalWrite(STAY, LOW);
  }
}


void handler(){
 int light_duino_in = 2;
 int light_duino_out = 1;
 int light_analog = 0;
 
 int light_analog_data = analog_data[light_duino_in-1][light_analog];

 if(light_analog_data < 300){
  digit_data[light_duino_out-1][10] = '1';
 }else{
  digit_data[light_duino_out-1][10] = '0';
 }
}


void writeDataAnalog(int i, String in){
  char in_mass[in.length()];
  for(int i = 0; i < in.length(); i++){
    in_mass[i] = in[i];
  }
  int st = 0;
  int cnt = 0;
  for(int j = 0; j < in.length(); j++){
    if(in_mass[j] == '.'){
      if(st != 0){
        String tmp;
        for(int s = st; s < j; s++){
          tmp += in_mass[s];
        }
        analog_data[i][cnt] = tmp.toInt();
        cnt++;
      }
      st = j+1;
    }
  }
  for(int j = in.length()-1; j >= 0; j--){
    if(in_mass[j] == '.'){
      String tmp;
      for(int s = j+1; s < in.length(); s++){
        tmp += in_mass[s];
      }
      analog_data[i][cnt] = tmp.toInt();
      cnt++;
    }
  }
}

void setAddress(){
  ////////////////////
  address[0][0] = '0';
  address[0][1] = '0';
  address[0][2] = '0';
  address[0][3] = '1';
  ////////////////////
  address[1][0] = '0';
  address[1][1] = '0';
  address[1][2] = '1';
  address[1][3] = '0';
  ////////////////////
}

void setDigitZero(){
  for(int i = 0; i < n_duino; i++){
    for(int j = 0; j < n_ports; j++){
      digit_data[i][j] = '0';
    }
  }
}


void setAnalogZero(){
  for(int i = 0; i < n_duino; i++){
    for(int j = 0; j < n_analog; j++){
      analog_data[i][j] = 0;
    }
  }
}

void control(int n){
  for(int i = 0; i < n; i++){
    digitalWrite(CONTROL, HIGH);
    delay(100);
    digitalWrite(CONTROL, LOW);
    delay(100);
  }
}
