#include <RGBmatrixPanel.h>
#include <gamma.h>
#include <Adafruit_GFX.h>

// defines the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// defines the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_1_PIN_NUMBER = 11;
const int BUTTON_2_PIN_NUMBER = 10;
const int BUTTON_3_PIN_NUMBER = 12;
const int BUTTON_4_PIN_NUMBER = 13;

const byte MAX_SNAKE_SIZE = 40;

void game_over();

enum Direction{UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};
struct WallPositions{
  byte x;
  byte y;
};

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// Space Invader Code has not been included yet
class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(7, 0, 0);
const Color ORANGE(7, 4, 0);
const Color YELLOW(7, 7, 0);
const Color GREEN(0, 7, 0);
const Color BLUE(0, 0, 7);
const Color PURPLE(7, 0, 7);
const Color WHITE(7, 7, 7);
const Color LIME(4, 7, 0);
const Color AQUA(0, 7, 7);

class Wall{
  public:
    //constructor
    Wall(){}
    //getters
    WallPositions get_wallLocations(byte i){
      return wallLocations[i];
    }
    int get_wallSize() const{
      return wallSize;
    }
    bool get_isHorizontal() const{
      return isHorizontal;
    }
    //setters
    void set_x(byte x_in){
      wallLocations[0].x = x_in;
    }
    void set_y(byte y_in){
      wallLocations[0].y = y_in;
    }    
    void set_wallSize(byte size_in){
      wallSize = size_in;
    }
    void set_isHorizontal(bool isHorizontal_in){
      isHorizontal = isHorizontal_in;
    }
    void set_wall(){
      if(isHorizontal){
        for(byte i = 1; i < wallSize; i++){
          wallLocations[i].x = wallLocations[0].x + i;
          wallLocations[i].y = wallLocations[0].y;
        }
      }
      else{
        for(byte i = 1; i < wallSize; i++){
          wallLocations[i].x = wallLocations[0].x;
          wallLocations[i].y = wallLocations[0].y + i;
        }
      }
    }
        
    //draw wall on screen
    void draw(){
      for(byte i = 0; i < wallSize; i++){
        matrix.drawPixel(wallLocations[i].x, wallLocations[i].y, BLUE.to_333());
      }
    }
    //erase board from screen
    void erase(){
      for(byte i = 0; i < wallSize; i++){
        matrix.drawPixel(wallLocations[i].x, wallLocations[i].y, BLACK.to_333());
      }
    }
  private:
    byte wallSize;
    WallPositions wallLocations[6];
    bool isHorizontal;
};

class Apple{
  public:
    //constructor
    Apple(){
      reset();
    }
    //getters
    int get_x(){
      return x;
    }
    int get_y(){
      return y;
    }
    //setters
    void set_x(byte x_in){
      x = x_in;
    }
    void set_y(byte y_in){
      y = y_in;
    }
    //draw apple on board
    void draw(){
      matrix.drawPixel(x, y, RED.to_333());
    }
    void reset(byte x_in = -1, byte y_in = -1){
      x = x_in;
      y = y_in;
    }

  private:
    //location
    byte x;
    byte y;
};

class Quadrant{
  public:
    //constructor
    Quadrant(){
      reset();
    }
    Wall get_wall(int i){
      return wall[i];
    }
    void draw(){
      for(byte i = 0; i < 16; i++){
            wall[i].draw();
      }
    }
    //randomly places 0, 1, or 2 walls for each quadrant of the board
    void randomize(){
      // Quad 1
      switch(random(6)){
        case 0:
          wall[2].set_x(2);
          wall[2].set_y(2);
          wall[2].set_wallSize(6);
          wall[2].set_isHorizontal(true);
          wall[2].set_wall();  
        case 1:
          wall[1].set_x(7);
          wall[1].set_y(1);
          wall[1].set_wallSize(6);
          wall[1].set_isHorizontal(false);
          wall[1].set_wall();
          break;
        case 2:
          wall[0].set_x(4);
          wall[0].set_y(0);
          wall[0].set_wallSize(3);
          wall[0].set_isHorizontal(false);
          wall[0].set_wall();
        case 3:
          wall[2].set_x(2);
          wall[2].set_y(2);
          wall[2].set_wallSize(3);
          wall[2].set_isHorizontal(true);
          wall[2].set_wall();
          wall[1].set_x(7);
          wall[1].set_y(1);
          wall[1].set_wallSize(6);
          wall[1].set_isHorizontal(false);
          wall[1].set_wall();         
          break;
        case 4:
          wall[3].set_x(9);
          wall[3].set_y(4);
          wall[3].set_wallSize(5);
          wall[3].set_isHorizontal(true);
          wall[3].set_wall();
          break;
      }
      // Quad 2
      switch(random(3)){
        case 0:
          wall[4].set_x(18);
          wall[4].set_y(2);
          wall[4].set_wallSize(3);
          wall[4].set_isHorizontal(true);
          wall[4].set_wall();
          wall[5].set_x(26);
          wall[5].set_y(2);
          wall[5].set_wallSize(6);
          wall[5].set_isHorizontal(true);
          wall[5].set_wall();
          wall[6].set_x(18);
          wall[6].set_y(6);
          wall[6].set_wallSize(3);
          wall[6].set_isHorizontal(true);
          wall[6].set_wall();
          wall[7].set_x(26);
          wall[7].set_y(6);
          wall[7].set_wallSize(4);
          wall[7].set_isHorizontal(true);
          wall[7].set_wall();
          break;
        case 1:
          wall[4].set_x(18);
          wall[4].set_y(2);
          wall[4].set_wallSize(5);
          wall[4].set_isHorizontal(false);
          wall[4].set_wall();
          wall[5].set_x(26);
          wall[5].set_y(2);
          wall[5].set_wallSize(4);
          wall[5].set_isHorizontal(false);
          wall[5].set_wall();
          wall[6].set_x(18);
          wall[6].set_y(6);
          wall[6].set_wallSize(2);
          wall[6].set_isHorizontal(false);
          wall[6].set_wall();
          wall[7].set_x(26);
          wall[7].set_y(6);
          wall[7].set_wallSize(2);
          wall[7].set_isHorizontal(false);
          wall[7].set_wall();
          break;
        case 2:
          wall[4].set_x(18);
          wall[4].set_y(2);
          wall[4].set_wallSize(3);
          wall[4].set_isHorizontal(false);
          wall[4].set_wall();
          wall[5].set_x(26);
          wall[5].set_y(2);
          wall[5].set_wallSize(4);
          wall[5].set_isHorizontal(false);
          wall[5].set_wall();
          wall[6].set_x(18);
          wall[6].set_y(6);
          wall[6].set_wallSize(3);
          wall[6].set_isHorizontal(true);
          wall[6].set_wall();
          wall[7].set_x(26);
          wall[7].set_y(6);
          wall[7].set_wallSize(4);
          wall[7].set_isHorizontal(true);
          wall[7].set_wall();
          break;
        case 3:
          wall[4].set_x(18);
          wall[4].set_y(2);
          wall[4].set_wallSize(4);
          wall[4].set_isHorizontal(true);
          wall[4].set_wall();
          wall[5].set_x(26);
          wall[5].set_y(2);
          wall[5].set_wallSize(3);
          wall[5].set_isHorizontal(true);
          wall[5].set_wall();
          wall[6].set_x(18);
          wall[6].set_y(6);
          wall[6].set_wallSize(5);
          wall[6].set_isHorizontal(false);
          wall[6].set_wall();
          wall[7].set_x(26);
          wall[7].set_y(6);
          wall[7].set_wallSize(3);
          wall[7].set_isHorizontal(false);
          wall[7].set_wall();
          break;  
      }
      // Quad 3
      switch(random(6)){
        case 0:
          wall[8].set_x(6);
          wall[8].set_y(10);
          wall[8].set_wallSize(4);
          wall[8].set_isHorizontal(true);
          wall[8].set_wall();
          wall[9].set_x(6);
          wall[9].set_y(13);
          wall[9].set_wallSize(3);
          wall[9].set_isHorizontal(true);
          wall[9].set_wall();
          wall[10].set_x(14);
          wall[10].set_y(10);
          wall[10].set_wallSize(3);
          wall[10].set_isHorizontal(true);
          wall[10].set_wall();
          wall[11].set_x(14);
          wall[11].set_y(10);
          wall[11].set_wallSize(5);
          wall[11].set_isHorizontal(true);
          wall[11].set_wall();
          break;
        case 1:
          wall[8].set_x(6);
          wall[8].set_y(8);
          wall[8].set_wallSize(3);
          wall[8].set_isHorizontal(false);
          wall[8].set_wall();
          wall[9].set_x(6);
          wall[9].set_y(13);
          wall[9].set_wallSize(4);
          wall[9].set_isHorizontal(true);
          wall[9].set_wall();
          wall[10].set_x(14);
          wall[10].set_y(10);
          wall[10].set_wallSize(6);
          wall[10].set_isHorizontal(true);
          wall[10].set_wall();
          wall[11].set_x(14);
          wall[11].set_y(10);
          wall[11].set_wallSize(3);
          wall[11].set_isHorizontal(false);
          wall[11].set_wall();
          break;
        case 2:
          wall[8].set_x(6);
          wall[8].set_y(10);
          wall[8].set_wallSize(3);
          wall[8].set_isHorizontal(true);
          wall[8].set_wall();
          wall[9].set_x(6);
          wall[9].set_y(13);
          wall[9].set_wallSize(5);
          wall[9].set_isHorizontal(true);
          wall[9].set_wall();
          wall[10].set_x(14);
          wall[10].set_y(10);
          wall[10].set_wallSize(3);
          wall[10].set_isHorizontal(true);
          wall[10].set_wall();
          wall[11].set_x(14);
          wall[11].set_y(10);
          wall[11].set_wallSize(3);
          wall[11].set_isHorizontal(true);
          wall[11].set_wall();
          break;
        case 3:
          wall[8].set_x(6);
          wall[8].set_y(10);
          wall[8].set_wallSize(3);
          wall[8].set_isHorizontal(true);
          wall[8].set_wall();
          wall[9].set_x(6);
          wall[9].set_y(13);
          wall[9].set_wallSize(3);
          wall[9].set_isHorizontal(true);
          wall[9].set_wall();
          wall[10].set_x(14);
          wall[10].set_y(10);
          wall[10].set_wallSize(3);
          wall[10].set_isHorizontal(false);
          wall[10].set_wall();
          wall[11].set_x(14);
          wall[11].set_y(10);
          wall[11].set_wallSize(3);
          wall[11].set_isHorizontal(false);
          wall[11].set_wall();
          break;
      }
      
      // Quad 4
      switch(random(6)){
        case 0:
          wall[12].set_x(18);
          wall[12].set_y(14);
          wall[12].set_wallSize(3);
          wall[12].set_isHorizontal(true);
          wall[12].set_wall();
          wall[13].set_x(26);
          wall[13].set_y(14);
          wall[13].set_wallSize(5);
          wall[13].set_isHorizontal(true);
          wall[13].set_wall();
          wall[14].set_x(18);
          wall[14].set_y(10);
          wall[14].set_wallSize(3);
          wall[14].set_isHorizontal(true);
          wall[14].set_wall();
          wall[15].set_x(26);
          wall[15].set_y(10);
          wall[15].set_wallSize(3);
          wall[15].set_isHorizontal(true);
          wall[15].set_wall();
          break;
        case 1:
          wall[12].set_x(18);
          wall[12].set_y(14);
          wall[12].set_wallSize(3);
          wall[12].set_isHorizontal(false);
          wall[12].set_wall();
          wall[13].set_x(26);
          wall[13].set_y(14);
          wall[13].set_wallSize(3);
          wall[13].set_isHorizontal(false);
          wall[13].set_wall();
          wall[14].set_x(18);
          wall[14].set_y(10);
          wall[14].set_wallSize(5);
          wall[14].set_isHorizontal(false);
          wall[14].set_wall();
          wall[15].set_x(26);
          wall[15].set_y(10);
          wall[15].set_wallSize(3);
          wall[15].set_isHorizontal(false);
          wall[15].set_wall();
          break;
        case 2:
          wall[12].set_x(18);
          wall[12].set_y(14);
          wall[12].set_wallSize(3);
          wall[12].set_isHorizontal(false);
          wall[12].set_wall();
          wall[13].set_x(26);
          wall[13].set_y(14);
          wall[13].set_wallSize(3);
          wall[13].set_isHorizontal(false);
          wall[13].set_wall();
          wall[14].set_x(18);
          wall[14].set_y(10);
          wall[14].set_wallSize(3);
          wall[14].set_isHorizontal(true);
          wall[14].set_wall();
          wall[15].set_x(26);
          wall[15].set_y(10);
          wall[15].set_wallSize(3);
          wall[15].set_isHorizontal(true);
          wall[15].set_wall();
          break;
        case 3:
          wall[12].set_x(18);
          wall[12].set_y(14);
          wall[12].set_wallSize(4);
          wall[12].set_isHorizontal(true);
          wall[12].set_wall();
          wall[13].set_x(26);
          wall[13].set_y(14);
          wall[13].set_wallSize(4);
          wall[13].set_isHorizontal(true);
          wall[13].set_wall();
          wall[14].set_x(18);
          wall[14].set_y(10);
          wall[14].set_wallSize(4);
          wall[14].set_isHorizontal(false);
          wall[14].set_wall();
          wall[15].set_x(26);
          wall[15].set_y(10);
          wall[15].set_wallSize(4);
          wall[15].set_isHorizontal(false);
          wall[15].set_wall();
          break;
      }
    }
    void reset(){
      for(int i = 0; i < 15; i++){
        wall[i].set_x(-1);
        wall[i].set_y(-1);
        wall[i].set_wallSize(0);
        wall[i].set_isHorizontal(false);
      }
    }

    Wall wall[16];
};

class Snake{
  public:
    //Constructor
    Snake() {
      reset();
    }
    //getters
    int get_snakeSize()const{
      return snakeSize;
    }
    int get_xPositions(byte i){
      return positions[i].x;
    }
    int get_yPositions(byte i){
      return positions[i].y;
    }
    bool get_positionExists(byte i){
      return positions[i].locationExists;
    }
    //snake head touches apple, gets bigger by one
    void eat(){
      snakeSize++;
    }  
    void setColor(Color snakeColor){
      color = snakeColor;
    }
    //change direction
    void turn(bool right, bool left){
      if(right){
        int temp = facing;
        temp++;
        if(temp > (int)LEFT){
          temp = (int)UP;
        }
        facing = (Direction)temp;
      }
      else if(left){
        int temp = facing;
        temp--;
        if(temp < (int)UP){
          temp = (int)LEFT;
        }
        facing = (Direction)temp;
      }
    }
    void set_height(byte snakeHeight){
      height = snakeHeight;
    }
    //move snake in direction head is facing one unit
    void moveSnake(){
      for(byte i = snakeSize - 1; i > 0; i--){
        positions[i] = positions[i - 1];
      }
      positions[snakeSize].x = -1;
      positions[snakeSize].y = -1;
      positions[snakeSize].locationExists = false;
      switch(facing){
        case UP:
          positions[0].y--;
          if (positions[0].y < 0){
            positions[0].y = 15;
          }
          break;
        case LEFT:
          positions[0].x--;
          if(positions[0].x < 0){
            positions[0].x = 31;
          }
          break;
        case RIGHT:
          positions[0].x++;
          if(positions[0].x > 31){
            positions[0].x = 0;
          }
          break;
        case DOWN:
          positions[0].y++;
          if(positions[0].y > 15){
            positions[0].y = 0;
          }
          break;
      }
    }

    //draws snake on board
    void draw(){
      for(byte i = 1; i < snakeSize; i++){
        matrix.drawPixel(positions[0].x, positions[0].y, ORANGE.to_333());
        if(positions[i].locationExists){
          matrix.drawPixel(positions[i].x, positions[i].y, color.to_333());
        }
      }
    }
    //resets snake's data
    void reset(){
      snakeSize = 5;
      facing = RIGHT;
      for(byte i = 0; i < snakeSize; i++){
        positions[i].x = 5 - i;
        positions[i].y = height;
        positions[i].locationExists = true;  
      }
      for(byte i = snakeSize; i < MAX_SNAKE_SIZE; i++){
        positions[i].x = -1;
        positions[i].y = -1;
        positions[i].locationExists = false;
      }  
    }
    //erases snake from board
    void erase(){
      for(int i = 0; i < snakeSize; i++){
        if(positions[i].locationExists){
          matrix.drawPixel(positions[i].x, positions[i].y, BLACK.to_333());
        }
      }
    }
  private:
    //size of snake
    int snakeSize;
    //initialize snake values
    Color color;
    Direction facing;
    byte height;
    struct Location{
      int x = -1;
      int y = -1;
      bool locationExists = false;
    };
    Location positions[MAX_SNAKE_SIZE]; 
};

















class SnakeGame{
  public:
    //constructor
    SnakeGame(){
      reset();
    }
    void reset() {
      score = 0;
      time = 0;
      done = false;
      appleExists = false;
    }
    //reset the game
    void resetGame(){
      byte snakeHeight = 12;
      //byte snake2Height = 14;
      snake.set_height(snakeHeight);
      //snake2.set_height(snake2Height);
      snake.reset();
      snake.setColor(GREEN);
      //snake2.reset();
      //snake2.setColor(YELLOW);
      apple.reset();
      placeApple();
      quad.reset();
      quad.randomize();
    }
    void checkButtons(bool button1, bool button2, bool button3, bool button4) {
      if (button1) {
        snake.turn(true, false);
      }
      else if (button2) {
        snake.turn(false, true);
      }
      
    }
    void placeApple(){
      apple.set_x(random(0, 32));
      apple.set_y(random(0, 16));
      for(byte j = 0; j < 16; j++){
        for(byte k = 0; k < quad.get_wall(j).get_wallSize(); k++){
          if(quad.get_wall(j).get_wallLocations(k).x == apple.get_x()
          && quad.get_wall(j).get_wallLocations(k).y == apple.get_y()){
            placeApple();
          }
          // Check for snake placement
          for (byte h = 0; h < snake.get_snakeSize(); h++) {
            if (apple.get_x() == snake.get_xPositions(h) 
            && apple.get_y() == snake.get_yPositions(h)) {
              placeApple();
            }
          }
          
        }
      }
      appleExists = true;
    }
    int timeDecreaser = 400;
    void appleCollision(int &time) {
      if (snake.get_xPositions(0) == apple.get_x() && snake.get_yPositions(0) == apple.get_y()) {
        snake.eat();
        time /= 1.2;
        placeApple();
      }
      
    }
    void wallCollision(int & time){   
      bool snake1Hit = false;
      //bool snake2Hit = false;
      if(done){
        for(int j = 0; j < 16; j++){
          for(int k = 0; k < quad.get_wall(j).get_wallSize(); k++){
            if(snake.get_xPositions(0) == quad.get_wall(j).get_wallLocations(k).x
            && snake.get_yPositions(0) == quad.get_wall(j).get_wallLocations(k).y){
              snake1Hit = true;
            }
            
          }
        } 
        if(snake1Hit){
          game_over();
          done = false;
          time = 400;
        }
        
      }
    }
    
    
       
    void setup(){
      Serial.begin(9600);
      pinMode(BUTTON_1_PIN_NUMBER, INPUT);
      pinMode(BUTTON_2_PIN_NUMBER, INPUT);
      pinMode(BUTTON_3_PIN_NUMBER, INPUT);
      pinMode(BUTTON_4_PIN_NUMBER, INPUT);
      matrix.begin();
    }
   
    //runs the game
    void loop(bool button1, bool button2, bool button3, bool button4){
      if(millis() % timeDecreaser < timeDecreaser / 2){
        if(button1){
          button1_pressed = true;
        }
        if(button2){
          button2_pressed = true;
        }
        if(button3){
          button3_pressed = true;
        }
        if(button4){
          button4_pressed = true;
        }
      }
      else{
        button1_pressed = false;
        button2_pressed = false;
        button3_pressed = false;
        button4_pressed = false;
      }
      if(millis() % timeDecreaser == 0){
        time++;
        matrix.fillScreen(BLACK.to_333());
        if(!done){
          resetGame();
          done = true;
        }
        if(done){
          checkButtons(button1_pressed, button2_pressed, button3_pressed, button4_pressed);          
          snake.erase();
          snake2.erase();
          snake.moveSnake();
          snake2.moveSnake();
          snake.draw();
          //snake2.draw();
          quad.draw();
          if(!appleExists){
            placeApple();
          }
          apple.draw();
          wallCollision(timeDecreaser);
          appleCollision(timeDecreaser);
          //snakeCollision(timeDecreaser);
          //snakeOnSnake();
        }
      }
    }
  private:
    int score;
    int time;
    Snake snake;
    Snake snake2;
    Apple apple;
    Quadrant quad;
    bool done;
    bool appleExists;
    bool button1_pressed;
    bool button2_pressed;
    bool button3_pressed;
    bool button4_pressed;
};



































SnakeGame game;

void setup() {
  randomSeed(analogRead(POTENTIOMETER_PIN_NUMBER));
  game.setup();
}

void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_1_pressed = (digitalRead(BUTTON_1_PIN_NUMBER));
  bool button_2_pressed = (digitalRead(BUTTON_2_PIN_NUMBER));
  bool button_3_pressed = (digitalRead(BUTTON_3_PIN_NUMBER));
  bool button_4_pressed = (digitalRead(BUTTON_4_PIN_NUMBER));
  game.loop(button_1_pressed, button_2_pressed, button_3_pressed, button_4_pressed);
}



// displays "game over"
void game_over() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.setCursor(0,0);
  matrix.print("GAME");
  matrix.setCursor(0,8);
  matrix.print("OVER");
  matrix.setCursor(0,0);
  delay(2000);
}
















// displays that player 1 wins
void player1_wins() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(GREEN.to_333());
  matrix.setCursor(0,0);
  matrix.print("GREEN");
  matrix.setCursor(0,8);
  matrix.print("WINS!");
  matrix.setTextColor(WHITE.to_333());
  delay(2000);
}

//displays that player 2 wins
void player2_wins() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(YELLOW.to_333());
  matrix.setCursor(0,0);
  matrix.print("MAIZE");
  matrix.setCursor(0,8);
  matrix.print("WINS!");
  matrix.setTextColor(WHITE.to_333());
  delay(2000);
}