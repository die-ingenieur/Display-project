#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// defines the screens perameters
#define Screen_width 128
#define Screen_height 64
//defines buttons layout
#define Button_pin  1
#define Button_pin2 2
#define Button_pin3 3
#define Button_pin4 4

#define OLED_ADDR 0x3C
//this sets up display information
Adafruit_SSD1306 display(Screen_width , Screen_height,&Wire,-1);
//============
//snake game
//============
#define snake_size 4
#define max_size 60
int Ssnake_length=3;
int SheadX = 64;
int SheadY= 32;
int Horizontal = 4;
int vertical=0;
int Sdirection[4][2] {
    {0,-4},
    {4,0},
    {0,4},
    {-4,0}
};
int StailX[max_size];
int Staily[max_size];
int Sfoodx,Sfoody;
int Ssnake_score =0;
bool SgameOver =false;
bool button_pin_state, button_pin2_state;
bool button_pin3_state, button_pin4_state;
bool Intro_for_snake = true;
int game_option= 0;
bool intro_not_activated= true;
bool game_selected =false;
void game_select(){
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);
        display.setCursor(1, 1);
        display.println(F("Select gamemode"));
        display.setCursor(1, 20);
        display.println(F("Left=flap,right= snake"));
        if ( button_pin4_state==LOW) {
            game_option=1;
            game_selected=true;
        }
        else if( button_pin2_state==LOW){
            game_option = 2;
            game_selected=true;
            }
        display.display();
}

void SresetGame(){
    SheadX=64;
    SheadY=32;
    Ssnake_length=3;
    SgameOver=false;
    Sstartdelay();
}
void SnakeIntro(){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(10,5);    //where the writing starts
    display.println("Snake ");
    display.setTextSize(1);
    display.setCursor(8,40);
    display.println("Press up key to");
    display.setCursor(40,50);
    display.println("start");

    display.display();
}
void Sstartdelay(){
    for(int i =1; i<=3; i++){   //gives a quick delay otherwise i notice you lose 50% on start as its so fast
        display.clearDisplay();
        Sdrawgame();
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(2);
        display.setCursor(50,30);    //where the writing starts
        display.print(i);     //prints 1-3 which acts a countdown which is pretty cool;
        display.display();
        delay(500);
    }
}
void moveSnake(){
    for (int i=Ssnake_length-1; i>0;i--){ //
        StailX[i] = StailX[i-1]; //moves next snake segment to one in front
        Staily[i]=Staily[i-1];
}
    StailX[0]=SheadX;   //moves the item behind the head to the postion the head occupied
    Staily[0]=SheadY;      
    SheadX+=Horizontal;
    SheadY+=vertical;
}
void Sdrawgame(){    // draws game
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2,2);
    display.print("score: ");
    display.print(Ssnake_score);
    display.display();

    display.fillRect(Sfoodx,Sfoody,4,4, SSD1306_WHITE);

    display.fillRect(SheadX,SheadY,snake_size,snake_size, SSD1306_WHITE);

    //body
    for (int i=0;i<Ssnake_length; i++){
        display.fillRect(StailX[i],Staily[i], snake_size,snake_size, SSD1306_WHITE);   //all segments 
    }
    display.display();
}
void check_food_collision(){
    if ((abs(SheadX-Sfoodx) < snake_size)&&(abs(SheadY-Sfoody)<snake_size)){  //food collsion if the x corodines of food and snake are beelfow 4
        Ssnake_score+=1;   
        if (Ssnake_length<max_size){
            Ssnake_length+=1;
        }
        createfruit();
    }
}
void check_all_collions(){
    if (SheadX<0||SheadX>Screen_width-1||SheadY<0|| SheadY>Screen_height-1){
        SgameOver=true;
    }
    for (int i=0; i<Ssnake_length;i++){
        if (SheadX==StailX[i] &&SheadY==Staily[i]){
            SgameOver=true;
        }
    }
}
void createfruit(){
    Sfoodx=(random(0,Screen_width-2));
    Sfoody=(random(1,63));
}
void snake_button_movement(){
    if ( button_pin_state==LOW){Horizontal = Sdirection[0][0];vertical = Sdirection[0][1];}   //direction of motion
    else if( button_pin2_state==LOW){Horizontal = Sdirection[1][0]; vertical = Sdirection[1][1];}
    else if( button_pin3_state==LOW){Horizontal = Sdirection[2][0]; vertical = Sdirection[2][1];}
    else if( button_pin4_state==LOW){Horizontal = Sdirection[3][0]; vertical = Sdirection[3][1];}

}
void Snake_gameOver(){  //game over
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(2);
        display.setCursor(20, 8);
        display.println(F("Game Over"));
        display.setTextSize(1);
        display.setCursor(18, 36);
        display.print(F("Score: "));
        display.println(Ssnake_score);
        display.setCursor(2, 52);
        display.println(F("Press up to restart"));
        display.display();
        delay(25);
}
void snakegame(){   //main snake loop
    button_pin_state=digitalRead(Button_pin);

    if (Intro_for_snake) {
        SnakeIntro();
        if (button_pin_state==LOW) {
            randomSeed(micros());
            Intro_for_snake=false; 
            SresetGame();
}
        return;
    }
    if (SgameOver){
        Snake_gameOver();
        if (button_pin_state==LOW) SresetGame();
            delay(500);
        return;
    }
    snake_button_movement();
    moveSnake();
    check_all_collions();
    check_food_collision();
    Sdrawgame();
    delay(150);
}

//================
//Flappy game game
//==================

const unsigned char PROGMEM birdBitmap [] = {
    0xc1, 0xba, 0x78, 0x4e, 0x42, 0x66, 0xbd, 0xc3   //the bird's 8x8 bitmap
    };
//now we define the varibale for the bird
int bird_x = 30;    //initial start position of bird on the x- axis
int bird_y = 30;    //initial start position of bird on the y- axis
int bird_size=8;    //bc the bitmap is 8x8
// deifne dimentions for the pipe
int pipe_x = Screen_width;   //pipe starts from the right of the screen
int pipe_width =12;          // how wide the pipe is
int gap_y =32;     
int gapsize =30;    
int velocity=0;    // velocity controls the movment of the bird veritcally due to gravity
//gamemodes
enum GameState {READY,PLAYING,GAME_OVER};
GameState gameState = READY;
// define movement 
int score =0;
int gravity  = 1;     // as you go down the screen, values increase so gravity is pos
int tap_power = -5;   // tap_power brings the bird up on the screen
bool pipeScored=false;  //tracks is a pipe is scored
bool lastButton=HIGH;  // keeps track of the last button push,(HIGH= released), so we can
                       //compare it to a new button press= allows the user
                       //more control as everytime he can't just leave the button pushed down(see void loop)
void resetGame(){    //resets game , eg when the user lost or just started
    bird_y=30;
    velocity=0;
    pipe_x=Screen_width;
    gap_y=random(15,50);

    score=0;
    pipeScored= false;
    gameState = PLAYING;  //Sets the gameState back to PLAYING( it would normally be at READY or GAME_OVER)
    startdelay();
}
void startdelay(){
    for(int i =1; i<=3; i++){   //gives a quick delay otherwise i notice you lose 50% on start as its so fast
        display.clearDisplay();
        drawMainGame();
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(2);
        display.setCursor(50,30);    //where the writing starts
        display.print(i);     //prints 1-3 which acts a countdown which is pretty cool;
        display.display();
        delay(500); 
    }
}
void Intro(){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(10,5);    //where the writing starts
    display.println("Flappy ");
    display.setCursor(10,22);
    display.println("bird");
    display.setTextSize(1);
    display.setCursor(8,40);
    display.println("Press any key to");
    display.setCursor(40,50);
    display.println("start");

    display.display();
    intro_not_activated=false;
}
void drawMainGame(){
    display.clearDisplay();
    //How to draw a rect: fillRect(x, y, width, height,color)
    display.fillRect(pipe_x,0,pipe_width,gap_y - gapsize/2,SSD1306_WHITE); //0 bc it draws from top, 
    // We can use gap_y as a  point, taking away the gap size = the rectangle has a height exactly missing
    // the size of the gap
    display.fillRect(pipe_x,gap_y +gapsize/2,pipe_width,Screen_height-(gap_y +gapsize / 2),SSD1306_WHITE); 
    //We can instead to gap_size/2 to take away half from the top, half from the bottom rect still creating one full gap
    display.drawBitmap(bird_x, bird_y, birdBitmap, bird_size,bird_size, SSD1306_WHITE); //bird
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2,2);
    display.print("score: ");
    display.print(score);
    display.display();
}
void updates(bool jump_now){  //getting the bool jump_now from justPressed, so we 
    if (jump_now) velocity = tap_power;  //setting velocity to tap_power means the bird goes up(as velcoity=neg)
    velocity+=gravity; //this increases the velocity = the bird will lose upward momemtum & start to go down
    if (velocity== 4) velocity=4;
    bird_y+=velocity;   //birds position on y axis
    pipe_x-=2;   //moves the pipe continuously 2 pixels left on x-co-ordination
    if (pipe_x+pipe_width<0){  //if rect moves outside screen,= new tangle
        pipe_x=Screen_width;   
        gap_y=random(15,50);  //this will randomly moves the gap to a dif position
        pipeScored = false;   //this resets the ailtiy to score a point
    }
    if (!pipeScored && pipe_x+pipe_width<bird_x){   //if bird passed the pipe 
        //we also check pipe-scored to avoid giving multiple points, pipeScored turns back to false from the code 
        //above
        score+=1;
        pipeScored=true; //happy happy happy
    }
    //collision detection
    const int gapTop = gap_y - gapsize /2;    
    const int gapBottom = gap_y + gapsize /2;
    const bool hitPipeX = bird_x + bird_size > pipe_x && bird_x < pipe_x + pipe_width;
    const bool outsideGap = bird_y < gapTop || bird_y + bird_size > gapBottom;
    if (bird_y< 0 || bird_y +bird_size > Screen_height|| ( hitPipeX && outsideGap )) {
      gameState = GAME_OVER;
    }

}
void flappygame(){
    bool button = digitalRead(Button_pin);
    bool justPressed = (lastButton==HIGH && button == LOW);  // bool value if lastbutton=high and button=low 
                        // a change in code
    lastButton=button;
    if (gameState==READY){
        if (justPressed) resetGame();  //intro
        Intro();
        delay(75);
        return;
    }
    if (gameState==GAME_OVER){    //game over message
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(2);
        display.setCursor(20, 8);
        display.println(F("Game Over"));
        display.setTextSize(1);
        display.setCursor(18, 36);
        display.print(F("Score: "));
        display.println(score);
        display.setCursor(2, 52);
        display.println(F("Press to restart"));
        display.display();
        if (justPressed) resetGame();
        delay(25);
        return;
    }
    updates(justPressed);
    drawMainGame();
    delay(25);
    
}


//==========
//main code
//==========
void setup() {
    pinMode(Button_pin, INPUT_PULLUP);
    pinMode(Button_pin2, INPUT_PULLUP);
    pinMode(Button_pin3, INPUT_PULLUP);
    pinMode(Button_pin4, INPUT_PULLUP);

    Wire.begin(21,18); //SDA and \\scl
    if (! display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){ //prevention if no OLED
        Serial.println(F("no SSD1306 oled found"));   
        while (true) { delay(100); }
    }
    display.clearDisplay();

    
}

void loop() {
    button_pin2_state=digitalRead(Button_pin2);
    button_pin3_state=digitalRead(Button_pin3);
    button_pin4_state=digitalRead(Button_pin4);
    if (!game_selected) game_select();   //selects which game to play based on user descion
    if (game_option==1){
        snakegame();
    }else if (game_option==2){
        if ( intro_not_activated) Intro();
        flappygame();
    }else{
        return;
    }
}








