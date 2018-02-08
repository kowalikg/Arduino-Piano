#include <Adafruit_NeoPixel.h>
#include <Tone.h>
/*
   Constants
*/
const int TONES_AMOUNT = 8;
const int SONGS_AMOUNT = 3;
const int buzzerRightPin = A4;
const int modeSwitchPin = 13;
const int switchSongPin = 12;
const int octaveSetterPin = 11;
const int notes[] = { 3, 4, 5, 6, 7, 8, 9, 10 };
const int diodePin = 2;
const int buzzerLeftPin = A5;
/*
   Color
*/
class SimpleColor {
  private:
    int red, green, blue;
  public:
    SimpleColor(const int, const int, const int);
    const int getRed();
    const int getGreen();
    const int getBlue();
};
SimpleColor::SimpleColor(const int red, const int green, const int blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
}
const int SimpleColor::getRed() {
  return red;
}
const int SimpleColor::getGreen() {
  return green;
}
const int SimpleColor::getBlue() {
  return blue;
}

struct Colors {
  const SimpleColor TURNED_LOW[TONES_AMOUNT] = {
    SimpleColor(0xff, 0x0, 0x0), SimpleColor(0xff, 0x33, 0x7f), SimpleColor(0xff, 0x66, 0x0), SimpleColor(0xff, 0xff, 0x0), SimpleColor(0x0, 0xff, 0x0),
    SimpleColor(0x0, 0xff, 0xff),  SimpleColor(0x0, 0x0, 0xff), SimpleColor(0x33, 0x0, 0x33)
  };
  const SimpleColor TURNED_HIGH = SimpleColor(0xff, 0xff, 0xff);
  const SimpleColor TURNED_OFF = SimpleColor(0x0, 0x0, 0x0);
};

/*
   Speaker
*/
class Speaker {
  private:
    Tone buzzerLeft, buzzerRight;
  public:
    void setSpeaker(int, int);
    void play(int);
    void stop();
};

void Speaker::setSpeaker(const int pinLeft, const int pinRight) {
  buzzerLeft.begin(pinLeft);
  buzzerRight.begin(pinRight);
}
void Speaker::play(int tone) {
  buzzerLeft.play(tone);
  buzzerRight.play(tone);

  delay(100);
}
void Speaker::stop() {
  buzzerLeft.stop();
  buzzerRight.stop();
}

/*
   ToneKeyboard
*/

struct LedToPinMap{
  int led;
  int pin;
};
class ToneKeyboard {
  private:
  LedToPinMap ledPinMap[TONES_AMOUNT];
  const int TONES_LOW[TONES_AMOUNT] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6 };
  const int TONES_HIGH[TONES_AMOUNT] = { NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6, NOTE_C7 };
  
  public:
  void setKeyboard(int[]);
  const int getTone(int, bool);
  const int getButton(int);
  const int getLed(int);
};
const int ToneKeyboard::getTone(int button, bool isHigh){
  const int * OCTAVE = isHigh ? TONES_HIGH : TONES_LOW;
  return OCTAVE[getLed(button)];
}
const int ToneKeyboard::getLed(int button){
  for (int i = 0; i < TONES_AMOUNT; i++){
    if (ledPinMap[i].pin == button)
      return ledPinMap[i].led;
  }
}
const int ToneKeyboard::getButton(int led){
  for (int i = 0; i < TONES_AMOUNT; i++){
    if (ledPinMap[i].led == led)
      return ledPinMap[i].pin;
  }
}
void ToneKeyboard::setKeyboard(int buttons[]){
  for (int i = 0; i < TONES_AMOUNT; i++){
    this->ledPinMap[i].led = i;
    this->ledPinMap[i].pin = buttons[i];
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

struct Grip{
  bool high;
  char tab;
  long duration;
};

class Tutor {
  private:
  Grip** songs;
  const int songsLength[SONGS_AMOUNT] = {8, 20, 78};

  Grip* currentSong;
  int currentSongNumber = 0;
  int currentSongLength;
  int currentNote;
  
  long lastTime;
  public:
  void setTutor();
  void nextSong();
  void teach();
  bool getNote(bool*);
  bool isHigh();
};
void Tutor::setTutor(){
  songs = new Grip*[SONGS_AMOUNT];
  songs[0] = new Grip[songsLength[0]] {
   {false, 128, 500}, {false, 32, 500}, {false, 64, 500}, {false, 16, 500}, {false, 32, 500}, {false, 8, 500}, {false, 1, 500}, {false, 0, 200}, 
  };
  songs[1] = new Grip[songsLength[1]] {
    {true, 128, 500},  {true, 64, 500}, {true, 32, 500}, {true, 128, 500}, {true, 16, 700}, {true, 32, 500}, {true, 16, 500}, {true, 32, 500}, {true, 64, 500}, {false, 2, 500}, {true, 128, 500}, {true, 64, 500},
    {true, 32, 500}, {true, 0, 100}, {true, 32, 500}, {true, 64, 500}, {true, 32, 500}, {true, 64, 500}, {true, 128, 500}, {true, 0, 200}
  };
  songs[2] = new Grip[songsLength[2]] {
    {false, 8, 1000},  {false, 4, 800},  {false, 8, 800},  {false, 32, 800},  {false, 0, 200}, 
    {false, 8, 1000},  {false, 4, 800},  {false, 8, 800},  {false, 32, 800},  {false, 0, 200}, 
    {true, 64, 2000}, {false, 0, 100}, {true, 64, 1000}, {false, 2, 2000}, {false, 0, 100}, {false, 2, 1000}, {false, 1, 2000}, {false, 0, 100}, {false, 1, 1000}, {false, 8, 1000}, {false, 0, 200},
    {false, 4, 2000}, {false, 0, 100}, {false, 4, 1000}, {false, 1, 1000}, {false, 2, 1000}, {false, 4, 1000}, {false, 8, 1000}, {false, 4, 1000}, {false, 8, 1000}, {false, 32, 2000}, {false, 0, 100}, {false, 32, 400}, {false, 0, 100},
    {false, 4, 2000}, {false, 0, 100}, {false, 4, 1000}, {false, 1, 1000}, {false, 2, 1000}, {false, 4, 1000}, {false, 8, 1000}, {false, 4, 1000}, {false, 8, 1000}, {false, 32, 2000}, {false, 0, 100}, {false, 32, 400}, {false, 0, 100},
    {true, 64, 2000}, {false, 0, 100}, {true, 64, 1000}, {true, 16, 1000}, {true, 64, 1000}, {false, 2, 1000}, {false, 1, 2000}, {true, 32, 2000}, {false, 0, 200},
    {false, 1, 1000},  {false, 8, 1000}, {false, 32, 1000}, {false, 8, 1000}, {false, 16, 1000}, {false, 64, 1000}, {false, 128, 1400}, {false, 0, 200},
    {false, 32, 800}, {false, 8, 800}, {false, 1, 1000}, {false, 0, 200},
  };
}

void Tutor::teach() {
  lastTime = millis();
  currentNote = 0;
  currentSong = songs[currentSongNumber];
  currentSongLength = songsLength[currentSongNumber];
}

void Tutor::nextSong(){
  currentSongNumber = (currentSongNumber + 1) % SONGS_AMOUNT;
}
bool Tutor::getNote(bool* notes) {
  long currentTime = millis();
  if(currentTime - lastTime > currentSong[currentNote].duration){
    currentNote++;
    lastTime = currentTime;
  }
  if(currentNote >= currentSongLength)
    return false;
  
  char tab = currentSong[currentNote].tab;
  char multiplier = 1;
  for (int i = TONES_AMOUNT - 1; i >= 0; i--){
    notes[i] = tab & multiplier;
    multiplier *= 2;
  }
  return true;
}
bool Tutor::isHigh(){
  return currentSong[currentNote].high;
}

Speaker speaker;
ToneKeyboard keyboard;
Tutor tutor;
Colors colors;

class Strip {
  private:
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(TONES_AMOUNT, diodePin, NEO_GRB + NEO_KHZ800);
  public:
  void setStrip();
  void turnOnLed(int, bool);
  void turnOffLed(int);
};

void Strip::setStrip(){
  strip.begin();
}
void Strip::turnOnLed(int led, bool high){
  SimpleColor color = high ? colors.TURNED_HIGH : colors.TURNED_LOW[led];
  strip.setPixelColor(led, strip.Color(color.getRed(), color.getGreen(), color.getBlue())); 
  strip.show();
}
void Strip::turnOffLed(int led){
  strip.setPixelColor(led, strip.Color(colors.TURNED_OFF.getRed(), colors.TURNED_OFF.getGreen(), colors.TURNED_OFF.getBlue()));
  strip.show();
}
Strip strip;

void checkButton(const int button, bool octaveSetterPressed, bool learning, bool tutorIsHigh){
  if (learning)
    strip.turnOnLed(keyboard.getLed(button), tutorIsHigh);
  if (!digitalRead(button)){
      if (!learning){
        strip.turnOnLed(keyboard.getLed(button), octaveSetterPressed);
        speaker.play(keyboard.getTone(button, octaveSetterPressed));
      }
      else{
        if (octaveSetterPressed == tutorIsHigh)
          speaker.play(keyboard.getTone(button, tutorIsHigh));
      }
  }
  else {
    if (!learning)
       strip.turnOffLed(keyboard.getLed(button));
    speaker.stop();
  }
}

bool switchSong(){
  int reading = digitalRead(switchSongPin);
  if (!reading){
    tutor.nextSong();
    return true;
  }
  return false;
}
void pianoMode(bool learning, bool octaveSetterPressed, bool notes[], bool tutorIsHigh){
  for (int i = 0; i < TONES_AMOUNT; i++){
    if (notes[i])
      checkButton(keyboard.getButton(i), octaveSetterPressed, learning, tutorIsHigh);
    else
      strip.turnOffLed(i);
  }
}
void setup() {
  Serial.begin(9600);
  strip.setStrip();
  tutor.setTutor();
  speaker.setSpeaker(buzzerLeftPin, buzzerRightPin);
  keyboard.setKeyboard(notes);
  pinMode(modeSwitchPin, INPUT_PULLUP);
  pinMode(switchSongPin, INPUT_PULLUP);
  pinMode(octaveSetterPin, INPUT_PULLUP);
}
const bool notes_piano[TONES_AMOUNT] = {true, true, true, true, true, true, true, true};
bool learn = false;

void loop() {
  bool notes[TONES_AMOUNT];
  if(digitalRead(modeSwitchPin)){
    learn = false;
    pianoMode(learn, !digitalRead(octaveSetterPin), notes_piano, false);
  }
  else {
    if(switchSong() || !learn){
      learn = true;
      tutor.teach();
    }
    learn = tutor.getNote(notes);
    if (learn) 
      pianoMode(learn, !digitalRead(octaveSetterPin), notes, tutor.isHigh());
  } 
}
