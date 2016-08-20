#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();
void player_wins();

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

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    
    int get_strength() const {
      return strength;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
      y = y + 1;
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if(get_strength() > 0) {
     

        switch(get_strength())
        {
          case 1:
            draw_with_rgb(RED,BLUE);
            break;
          case 2:
            draw_with_rgb(ORANGE,BLUE);
            break;
          case 3:
            draw_with_rgb(YELLOW,BLUE);
            break;
          case 4:
            draw_with_rgb(GREEN,BLUE);
            break;
          case 5:
            draw_with_rgb(BLUE,BLUE);
            break;
          case 6:
            draw_with_rgb(PURPLE,BLUE);
            break;
          case 7:
            draw_with_rgb(WHITE,BLUE);
            break;
        }
      }
    }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK,BLACK);
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      initialize(x,y,get_strength() - 1);
      if(get_strength() == 0) {
        erase();
      } else {
        draw();
      }
    }

  private:
    int x;
    int y;
    int strength;
    
    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      // Black Parts
      matrix.drawPixel(get_x(), get_y(), BLACK.to_333());
      matrix.drawPixel(get_x() + 3, get_y(),BLACK.to_333());
      matrix.drawPixel(get_x() + 1, get_y() + 3, BLACK.to_333());
      matrix.drawPixel(get_x() + 1, get_y() + 3, BLACK.to_333());
      
    
      // Body Parts
      matrix.drawPixel(get_x() + 1, get_y(), body_color.to_333());
      matrix.drawPixel(get_x() + 2, get_y(), body_color.to_333());
      matrix.drawPixel(get_x(), get_y() + 1, body_color.to_333());
      matrix.drawPixel(get_x() + 3, get_y() + 1, body_color.to_333());
      matrix.drawPixel(get_x(), get_y() + 2, body_color.to_333());
      matrix.drawPixel(get_x() + 1, get_y() + 2, body_color.to_333());
      matrix.drawPixel(get_x() + 2, get_y() + 2, body_color.to_333());
      matrix.drawPixel(get_x() + 3, get_y() + 2, body_color.to_333());
      matrix.drawPixel(get_x(), get_y() + 3, body_color.to_333());
      matrix.drawPixel(get_x() + 3, get_y() + 3, body_color.to_333());
      
      // eye parts
      matrix.drawPixel(get_x() + 1, get_y() + 1, eye_color.to_333());
      matrix.drawPixel(get_x() + 2, get_y() + 1, eye_color.to_333());
       
    }
};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 15;
      fired = false;
    }
    
    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 15;
      fired = false;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired;
    }
    
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
      fired = true;
      x = x_arg;
      y = y_arg;
    }
    
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      if(y > -2) {
        y = y - 1;
        
      }
      if(y < 0) {
        reset();
        fired = false;
      }
    }
    
    // resets private data members to initial values
    void hit() {
      reset();
    }
    
    // draws the Cannonball, if it is fired
    void draw() {
      if(has_been_fired()) {
        matrix.drawPixel(x,y,ORANGE.to_333());
        matrix.drawPixel(x,y + 1, ORANGE.to_333());
      }
    }
    
    // draws black where the Cannonball used to be
    void erase() {
        matrix.drawPixel(x,y,BLACK.to_333());
        matrix.drawPixel(x,y + 1, BLACK.to_333());
    }


    

  private:
    int x;
    int y;
    bool fired;
};
class Player {
  public:
    Player() {
      x = 0;
      y = 15;
      lives = 3;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives;
      
    }
    
    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }
    
    // Modifies: lives
    void die() {
      lives--;
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(AQUA);
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    
    // draws the player
    void draw_with_rgb(Color color) {
      matrix.drawPixel(x,y,color.to_333());
      matrix.drawPixel(x + 1,y,color.to_333());
      matrix.drawPixel(x - 1,y,color.to_333());
      matrix.drawPixel(x,y - 1,color.to_333());
    }
};


class Game {
  public:
    Game() {
      level = 0;
      time = 0;
    }

    void set_time(int value) {
      time = value;
    }

    int get_time() {
      return time;
    }
    
    // returns enemy at specified index, if in bounds
    Invader get_enemy(int index) {
      if(index >= 0 && index <= 15) return enemies[index]; 
    }
    
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    // see http://arduino.cc/en/Reference/Setup
    void setup() {
        
        reset_level();  
    }
    
    
    // displays and runs Space Invaders!
    // see spec for details of game
    // Modifies: global variable matrix
    void loop(int potentiometer_value, bool button_pressed) {
      
      // draw invaders based on level, if the board is cleared
        if(level_cleared() == true) {
          reset_level();
       
          switch(level) {
            case 1:
              for(int i = 0; i < 8; i++) {
                enemies[i].initialize(0 + 4 * i, 0, 1);
               enemies[i].draw();
              }
              break;
            case 2:
              for(int i = 0; i < 16; i++) {
                int temp_strength;
                if(i % 2 == 0) temp_strength = 1;
                else temp_strength = 2;
                if(i < 8) enemies[i].initialize(0 + 4 * i, 0, temp_strength); 
                else enemies[i].initialize(0 + 4 * (i - 8), 4, temp_strength);
                enemies[i].draw();
              }
              break;
            case 3:
              for(int i = 0; i < 16; i++) {
                int temp_strength;
                temp_strength = (i % 5 + 1);
                if(i < 8) enemies[i].initialize(0 + 4 * i, 0, temp_strength);
                  else enemies[i].initialize(0 + 4 * (i - 8), 4, temp_strength);
                enemies[i].draw(); 
              }
              break;
            case 4:
              for(int i = 0; i < 16; i++) {
                int temp_strength;
                if(i < 8) {
                  if(i % 2 == 0) temp_strength = 5;
                    else temp_strength = 4;
                } else {
                  if(i % 2 == 0) temp_strength =2;
                    else temp_strength = 3;
                }
                if(i < 8) enemies[i].initialize(0 + 4 * i, 0, temp_strength);
                  else enemies[i].initialize(0 + 4 * (i - 8), 4, temp_strength);
                enemies[i].draw();  
              }
              break;
            default:
              for(int i = 0; i < 16; i++) {
                int temp_strength;
                temp_strength = (int)(rand() % 7 + 1);
                if(i < 8) enemies[i].initialize(0 + 4 * i, 0, temp_strength);
                  else enemies[i].initialize(0 + 4 * (i - 8), 4, temp_strength);
                enemies[i].draw();  
              }
              break;
          }
        } else {
          
          //the board is no cleared, we have to still use current level
          
          if( millis() % 1000 == 0) {
             /* bool touched_button;
              if(level == 1) {
                for(int i = 0; i < 8; i++) {
                   if(enemies[i].get_y() == 12) enemies[i].erase();
                    else {
                      enemies[i].erase();
                      enemies[i].move();
                      enemies[i].draw();
                    }
                }
              } else {
                 int num_row2_killed = 0;
                */
                 
                  
                  bool rowkilled = true;
                for(int k = 8; k < 16; k++) {
                 if(enemies[k].get_strength() != 0){
                    
                    rowkilled = false;
                 }             
                }
                
                if(rowkilled == true) {
                  for(int t = 0; t < 8; t++) {
                    enemies[t].erase();
                        enemies[t].move();
                        enemies[t].draw();
                  }
                }
                  else if (rowkilled == false){
                    for(int k = 8; k < 16; k++) {
                      enemies[k].erase();
                        enemies[k].move();
                        enemies[k].draw();
                    }
                    
                  
                }
              
          
          set_time(millis());
      }

       //draw player
        int x_coordinates;
        x_coordinates = potentiometer_value / 32;
        player.erase();
        player.set_x(x_coordinates);
        player.draw();
 
        
         if(button_pressed == true && ball.has_been_fired() == false) {
          ball.fire(x_coordinates, player.get_y() - 3);
          ball.draw();
          delay(100);
         
          
         }
        if(button_pressed == false && ball.has_been_fired() == true) {
          ball.erase();
          ball.move();
          ball.draw();
          delay(100);
        }    
        

    for (int i = 0; i < NUM_ENEMIES; i++){
         if ((enemies[i].get_y()+1 == ball.get_y()) && (enemies[i].get_x() == ball.get_x())){
          if (enemies[i].get_strength() != 0){
            enemies[i].hit();
             ball.reset();
             
          }
         }
        if ((enemies[i].get_y()+1 == ball.get_y()) && (enemies[i].get_x()+1 == ball.get_x())){
          if (enemies[i].get_strength() != 0){
            enemies[i].hit();
             ball.reset();
             
          }
          
        }
        if ((enemies[i].get_y()+1 == ball.get_y()) && (enemies[i].get_x()+2 == ball.get_x())){
          if (enemies[i].get_strength() != 0){
            enemies[i].hit();
             ball.reset();
             
          }
        }
        if ((enemies[i].get_y()+1 == ball.get_y()) && (enemies[i].get_x()+3 == ball.get_x())){
          if (enemies[i].get_strength() != 0){
            enemies[i].hit();
             ball.reset();
             
          }
      
        }
        int lives = 3;
        if (lives == 0){        
            game_over();
        }
        if ((enemies[i].get_y()+1 == player.get_y()-1) && (enemies[i].get_x() == player.get_x())){
          player.die();  
          print_lives(player.getlives());
          reset_newlevel();
        }         
        if ((enemies[i].get_y()+1 == player.get_y()-1) && (enemies[i].get_x()+1 == player.get_x())){
          player.die();
          print_lives(player.getlives());
          reset_newlevel();
        }               
        if ((enemies[i].get_y()+1 == player.get_y()-1) && (enemies[i].get_x()+2 == player.get_x())){
          player.die();
          print_lives(player.getlives());
          reset_newlevel();
        }
        if ((enemies[i].get_y()+1 == player.get_y()-1) && (enemies[i].get_x()+3 == player.get_x())){
          player.die();
          print_lives(player.getlives());
          reset_newlevel();
        }
        /*if (enemies[i].get_y()+1 == 15){
          lives--;
          level = level -1;
          reset_level();
        }
        */
        }
        
    }           
 }      
    

  private:
    int level;
    int time;
    long ball_time;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
     
      for(int i = 0; i < 16; i++) {
        if(enemies[i].get_strength() != 0) return false;
      }
      return true;
    }

    // set up a level
    void reset_level() {
      level = level + 1;
      print_level(level);
      
      
  
    }
    void reset_newlevel() {
      level = level;
      print_level(level);
      
      
  
    }
};

// a global variable that represents the game Space Invaders
Game game;

// see http://arduino.cc/en/Reference/Setup
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
}

// see http://arduino.cc/en/Reference/Loop
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  game.loop(potentiometer_value, button_pressed);
}

Player player;
// displays Level
void print_level(int level) {
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.setCursor(0,0);
  matrix.print("Level  ");
  matrix.print(level);
  delay(3000);
  matrix.fillScreen(BLACK.to_333());
  delay(1000);
  
}

// displays number of lives
void print_lives(int lives) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.setCursor(0,0);
  matrix.print("Lives: ");
  matrix.print(lives);
  delay(2000);
}

// displays "game over"
void game_over() {
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print("Suck 8=D~");
  delay(2000);
}

// displays "you win!"
void player_wins() {
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print("You Win!");
  delay(2000);
}
