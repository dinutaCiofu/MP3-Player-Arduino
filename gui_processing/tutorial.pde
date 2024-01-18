import controlP5.*; // import ControlP5 library
import processing.serial.*;

Serial port;

ControlP5 cp5;
PFont font;
PImage playImage, playPressedImage, nextImage, nextPressedImage, prevImage, prevPressedImage, fundalImage;
String currentSong = ""; // Numele melodiei
String[] songList = {
  "Sicko Mode (kd DJ Tool)",
  "Clarity",
  "Fathless - Insomnia",
  "Rune RK - Calabria",
  "Gwen Stefani - Holaback Gir",
  "James Hype - Ride It VIP",
  "Borgeous & David Solano - Big Bang",
  "Excision & Space Laces - Throwin' Elbows",
  "Swedish House Mafia - Greyhound",
  "Adventure Club x Yuna - Lullabies",
  "SAINt JHN - Roses",
  "Sia & Safri Duo - Alive",
  "Foster The People - Pumped up Kicks",
  "Da Tweekaz - Moana",
  "Zh3r0 - Firecracker",
  "Cecilia Krull Vs. Do It Big - My Life Is Timed Out",
  "Refuzion - Dragostea Din Tei",
  "Vicetone & Tony Igy - Astronomia"
};
int index = 0;
Boolean isPlaying = true;
void setup(){
  size(700, 500); // window size
  
  
  // load the images for the buttons
  playImage = loadImage("play-normal.png");
  playPressedImage = loadImage("play-pressed.png");
  nextImage = loadImage("next-normal.png");
  nextPressedImage = loadImage("next.png");
  prevImage = loadImage("previous-normal.png");
  prevPressedImage = loadImage("previous.png");
  fundalImage = loadImage("friends.png");
  
  printArray(Serial.list()); // prints all available serial ports
  port = new Serial(this, "COM3", 115200);
  
  // add button
  cp5 = new ControlP5(this);
  font = createFont("calibri light bold", 35); // custom fonts for buttons and title
  
  cp5.addButton("fundal").setValue(128)
     .setPosition(220, 55)
     .setSize(90, 90)
     .setImage(fundalImage);
  
  cp5.addButton("previous").setValue(128)
     .setPosition(200, 350)
     .setSize(40, 40)
     .setImages(prevImage, prevPressedImage, prevPressedImage);
     
  cp5.addButton("play").setValue(128)
     .setPosition(300, 350)
     .setSize(40, 40)
     .setImages(playImage, playPressedImage, playPressedImage);
     
  cp5.addButton("next")
     .setValue(128)
     .setPosition(400, 350)
     .setSize(40, 40)
     .setImages(nextImage, nextPressedImage, nextPressedImage);

}

void draw(){ // same as loop in arduino
  background(0, 102, 115); // background color of window (r, g, b)
  
  // title of the window
  fill(0, 0, 0);            // text color (r,g,b)
  textSize(30);
  textFont(font);
  text("MP3 Player", 260, 50); // ("text", x coordinate, y coordinate)
  
  fill(0, 0, 0);          
  textSize(20);
  textFont(font);
  text(currentSong, 50, 330);
  port.stop();
}

void controlEvent(ControlEvent event) {
  if (event.isController()) {
    String buttonName = event.getController().getName();
    if (buttonName.equals("play")) {
      println("Play button pressed");
       port.write('p');
      if(isPlaying == true){
        isPlaying = false;
      }else{
        isPlaying = true;
      }
    updateCurrentSong(index);
    } else if (buttonName.equals("next")) {
      println("Next button pressed");
      if(index < 18){
        index++;
        isPlaying = true;
        updateCurrentSong(index);
      }else{
        index = 0;
        isPlaying = true;
        updateCurrentSong(index);
      }
      port.write('n');
    } else if (buttonName.equals("previous")) {
      println("Previous button pressed");
      if(index == 0){
        index = 17;
        isPlaying = true;
        updateCurrentSong(index);
      }else{
        index--;
        isPlaying = true;
        updateCurrentSong(index);
      }
      port.write('i');
    }
  }
}

void updateCurrentSong(int songIndex) {
  if (songIndex >= 0 && songIndex < songList.length) {
    println(index);
    currentSong = songList[songIndex];
  } else {
    currentSong = "N/A";
  }
}
