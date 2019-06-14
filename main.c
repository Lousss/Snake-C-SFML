#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SEG_SIZE 20
#define SNAKE_SIZE 3000

struct Segment {
    sfVector2f pos;
    sfRectangleShape* shape;
};
typedef struct Segment Segment;

struct Snake {
    int size;
    Segment a[SNAKE_SIZE];
};

typedef struct Snake Snake;

struct Button {
    sfRectangleShape* shape;
    sfText* text;
    sfVector2f size;
    sfVector2f pos;
    sfColor idle;
    sfColor active;
    sfColor h;
    
}; typedef struct Button Button;

Snake create(Snake Snake, sfRectangleShape* rec) {
    Snake.size = 0;
    for(int i = 0; i < 3; i++) {
                    Snake.a[i].pos.x = 100-Snake.size*20; Snake.a[i].pos.y = 120;
                    Snake.a[i].shape = sfRectangleShape_copy(rec);
                    if(i) sfRectangleShape_setFillColor(Snake.a[i].shape, sfBlue);
                    Snake.size++;
                    sfRectangleShape_setPosition(Snake.a[i].shape, Snake.a[i].pos);
    }
    return Snake;
}

Snake add_segment(Snake Snake,sfRectangleShape* shape) {
    Segment s;
    s.shape = sfRectangleShape_copy(shape);
    s.pos.x = Snake.a[Snake.size].pos.x;
    s.pos.y = Snake.a[Snake.size].pos.y;
    sfRectangleShape_setPosition(s.shape, s.pos);
    Snake.size++;
    Snake.a[Snake.size-1] = s;
    return Snake;
}

void Move(Snake Snake, sfVector2f move) {

    sfVector2f prevPos = sfRectangleShape_getPosition(Snake.a[0].shape);
    sfRectangleShape_move(Snake.a[0].shape , move);

    for(unsigned int i = 1; i < Snake.size; i++)
    {
        sfVector2f tmp =  sfRectangleShape_getPosition(Snake.a[i].shape);
        sfRectangleShape_setPosition(Snake.a[i].shape, prevPos);
        prevPos = tmp;
    }
}

void Keyboard(sfRenderWindow* window) {
     if(sfKeyboard_isKeyPressed(sfKeyEscape)) sfRenderWindow_close(window);
    
}
 // Time between frames
/*sfTime clock(sfClock* Clock) {
    return sfClock_restart(Clock);
}*/

Button create_button (int posx, int posy, sfText* text) {
    Button button;
    sfVector2f button_text_position;
    button.shape =   sfRectangleShape_create();
    button.size.x = 200; button.size.y = 50;
    sfRectangleShape_setSize(button.shape, button.size);
    button.pos.x = posx; button.pos.y = posy;
    sfRectangleShape_setPosition(button.shape, button.pos);
    button.idle = sfColor_fromRGBA(70,70,70,200);
    button.h = sfColor_fromRGBA(150,150,150, 255);
    button.active = sfColor_fromRGBA(20,20,20,200);
    sfRectangleShape_setFillColor(button.shape, button.idle);
    button.text = sfText_copy(text);
    sfVector2f bsp = sfRectangleShape_getPosition(button.shape);
    sfFloatRect tgb = sfText_getGlobalBounds(button.text);
    sfFloatRect sgb = sfRectangleShape_getGlobalBounds(button.shape);
    button_text_position.x = bsp.x + (sgb.width / 2.0) - (tgb.width /2.0);
    button_text_position.y = bsp.y + (sgb.height / 2.0) - (tgb.height / 2.0) - 3.0;
    sfText_setPosition(button.text, button_text_position);
    return button;
}

enum scenes {MENU = 0, GAME, HELP, RESTART};

void main(void) {
    srand(time(NULL));
    int c = 1;
    int scene = MENU;
    sfVideoMode mode = {1440, 900, 32};
   // sfClock* gametime = sfClock_create();
  //  float t; 
    sfClock* moveC;
    sfClock* moveS = sfClock_create();
    sfRenderWindow* window;  float t = 1;
    sfEvent event;
    sfTexture* tseg;
    sfSprite* seg;
    sfTexture* tseg2;
    sfSprite* seg2;
    sfVector2f pos; pos.x = 0; pos.y = 0;
    sfVector2i mousePos;
    
    int scoree = 0;
    
    // GUI
    sfRectangleShape* score =  sfRectangleShape_create();
    sfVector2f score_size; score_size.x = 1432; score_size.y = 92;
    sfVector2f score_pos; score_pos.x = 4; score_pos.y = 4;
    sfRectangleShape_setPosition(score, score_pos);
    sfRectangleShape_setSize(score, score_size);
    sfRectangleShape_setFillColor( score , sfBlack);
    sfRectangleShape_setOutlineColor(score, sfBlue);
    sfRectangleShape_setOutlineThickness(score,4);
    
    sfFont* Font = sfFont_createFromFile("Comfortaa-Regular.ttf");
    sfText* text = sfText_create();
    sfVector2f text_pos, text_score_pos, text_score_pos2; text_pos.x = 40; text_pos.y = 20;
    sfText_setFont(text, Font);
    int bb = 20;
    char b[10];
    sprintf(b, "%d", bb);
    sfText_setString(text, "Press esc to exit ");
    sfText_setPosition(text, text_pos);
    sfText_setCharacterSize(text, 50);
    
    text_score_pos.x = 950; text_score_pos.y = 20;
    sfText* text_score = sfText_create();
    sfText_setFont(text_score, Font);
    sfText_setString(text_score, "Score ");
    sfText_setCharacterSize(text_score, 50);
    sfText_setPosition(text_score, text_score_pos);
    
    text_score_pos2.x = 1186; text_score_pos2.y = text_score_pos.y;
    sfText* text_score2 = sfText_copy(text_score);
    sfText_setString(text_score2, "0");
    sfText_setPosition(text_score2, text_score_pos2);
    
    sfText* button_text = sfText_create();
    sfText_setFont(button_text, Font);
    sfText_setString(button_text, "New Game");
    sfText_setCharacterSize(button_text, 20);
    Button new_game = create_button(620,590, button_text);
    sfText_setString(button_text, "Restart");
    Button restrt = create_button(620,630 , button_text);
    sfText_setString(button_text, "Help");
    Button help = create_button(620, 670, button_text);
    sfText_setString(button_text, "Back");
    Button back = create_button(620, 710, button_text);
    sfText_setString(button_text, "Exit");
    Button exit1 = create_button(620, 750, button_text);
    Button exit2 = create_button(620, 710, button_text);
    sfText* result = sfText_create();
    sfText_setFont(result, Font);
    sfText_setCharacterSize(result, 70);
    sfText_setString(result, "You score is ");
    sfVector2f resultPos;
    resultPos.x = 495; resultPos.y = 300;
    sfText_setPosition(result, resultPos);
    sfText* result2 = sfText_copy(result);
    sfText_setCharacterSize(result2, 55);
    resultPos.x = 695;
    resultPos.y = 440;
    sfText_setPosition(result2, resultPos);

    tseg = sfTexture_createFromFile("snake_bgpng.png", NULL);
    tseg2 = sfTexture_createFromFile("help.png", NULL);

    seg = sfSprite_create();
    seg2 = sfSprite_create();

    sfSprite_setTexture(seg, tseg, 1);
    sfSprite_setTexture(seg2, tseg2, 1);

    sfSprite_setPosition(seg, pos);
    sfSprite_setPosition(seg, pos);


    
    
    sfRectangleShape* shape = sfRectangleShape_create();
    sfVector2f size; 
    size.x = 20; 
    size.y = 20; 
    sfRectangleShape_setFillColor(shape, sfGreen);
    sfRectangleShape_setSize(shape, size);
    window = sfRenderWindow_create(mode, "SFML", sfResize | sfClose, NULL);

    sfRectangleShape* shape2 = sfRectangleShape_copy(shape);
    sfRectangleShape_setFillColor(shape2, sfRed);
    sfVector2f apple_pos;
    apple_pos.x = 1000.0;
    apple_pos.y = 120.0;
    
    sfRectangleShape_setPosition(shape2, apple_pos);
    Snake snake = create(snake, shape);
    sfVector2f move;
    move.x = 20.f*t; 
    move.y = 0; 
    int direc = 1;
    
    while(sfRenderWindow_isOpen(window)) {
      //  sfRectangleShape_move(shape, move);
       Keyboard(window);  
       mousePos = sfMouse_getPosition(window);
       
      if(scene == MENU) {      
        sfFloatRect n = sfRectangleShape_getGlobalBounds(new_game.shape);  
        if(sfFloatRect_contains(&n,mousePos.x, mousePos.y)) { if(sfMouse_isButtonPressed(sfMouseLeft)) { sfRectangleShape_setFillColor(new_game.shape, new_game.active); scene = GAME; }  else  sfRectangleShape_setFillColor(new_game.shape, new_game.h); }
        else sfRectangleShape_setFillColor(new_game.shape, new_game.idle);
        
        sfFloatRect n2 = sfRectangleShape_getGlobalBounds(help.shape);  
        if(sfFloatRect_contains(&n2,mousePos.x, mousePos.y)) { if(sfMouse_isButtonPressed(sfMouseLeft)) { sfRectangleShape_setFillColor(help.shape, help.active); scene = HELP; }  else  sfRectangleShape_setFillColor(help.shape, help.h); }
        else sfRectangleShape_setFillColor(help.shape, help.idle);
        
        sfFloatRect n6 = sfRectangleShape_getGlobalBounds(exit1.shape);
         if(sfFloatRect_contains(&n6,mousePos.x, mousePos.y)) { if(sfMouse_isButtonPressed(sfMouseLeft)) { sfRectangleShape_setFillColor(exit1.shape, exit1.active);  
            sfRenderWindow_close(window); } 
         else   sfRectangleShape_setFillColor(exit1.shape, exit1.h); }
         else sfRectangleShape_setFillColor(exit1.shape, exit1.idle); 
        
      }
      else if (scene == GAME){
  
     // Time between frames
        //t = 1000 * sfTime_asSeconds(clock(gametime));
         //move.x = 20.f*t; move.y = 0;
        //printf("t = %lf", t);
        if(sfKeyboard_isKeyPressed(sfKeyA) && c == 1) { c = 0;  snake = add_segment(snake, shape); snake.size = 0; move.x = 20.f*t; move.y = 0; direc = 1; scoree = 0; snake = create(snake, shape);}
        if(sfKeyboard_isKeyPressed(sfKeyLeft) && direc != 1) { move.x = -20.f*t; move.y = 0; direc = 3;}
        if(sfKeyboard_isKeyPressed(sfKeyRight) && direc != 3) { move.x = 20.f*t; move.y = 0; direc =1;}
        if(sfKeyboard_isKeyPressed(sfKeyUp) && direc != 2)  {   move.x = 0; move.y = -20.f;  direc = 4; }
        if(sfKeyboard_isKeyPressed(sfKeyDown) && direc != 4){    move.x = 0; move.y = 20.f;  direc = 2;} 
        sfTime timeMove = sfClock_getElapsedTime(moveS);
        
        if(sfTime_asSeconds(timeMove) >= .09) {// For controlling snake speed
         Move(snake, move);   
         c = 1;
         sfFloatRect rect = sfRectangleShape_getGlobalBounds(snake.a[0].shape);
         sfVector2f apple_pos = sfRectangleShape_getPosition(shape2);
    if( sfFloatRect_contains(&rect,apple_pos.x , apple_pos.y)) { apple_pos.x = 20 * (rand() % 71); apple_pos.y = 100 + 20 * ( rand() % 40); printf("%f", apple_pos.x);   
           if ( c == 1) { c=0; snake = add_segment(snake, shape); scoree+=10; 
               
        }
           if (scoree >= 0 && scoree <=90) text_score_pos2.x = 1172;
           else if (scoree >= 100 && scoree <= 990) text_score_pos2.x = 1158;
           else if (scoree >= 1000 && scoree <= 9990) text_score_pos2.x = 1144;
           else text_score_pos2.x = 1130;
           sfText_setPosition(text_score2, text_score_pos2);
           char s[10]; 
           sprintf(s, "%d", scoree);
           sfText_setString(text_score2,s);
             for (int i = 0; i < snake.size; i++) {
                
                  sfVector2f seg_pos = sfRectangleShape_getPosition(snake.a[i].shape);
                  if(seg_pos.x == apple_pos.x && seg_pos.y == apple_pos.y) { apple_pos.x = 20 * (rand() % 71); apple_pos.y =100 + 20 * ( rand() % 40);  }
                  
            }
        sfRectangleShape_setPosition(shape2, apple_pos); }
        
       for (int i = 1; i < snake.size; i++) {
            sfVector2f direc_m;
            if(direc == 1) { direc_m.x = 20*1; direc_m.y =0; }
            if(direc == 2) { direc_m.x = 0; direc_m.y =20; }
            if(direc == 3) { direc_m.x = -20*1; direc_m.y =0; }
            if(direc == 4) { direc_m.x = 0; direc_m.y =-20; }
            
        
       sfVector2f headpos = sfRectangleShape_getPosition(snake.a[0].shape);
       sfVector2f seg_pos = sfRectangleShape_getPosition(snake.a[i].shape);
        
       if((seg_pos.x == headpos.x && seg_pos.y == headpos.y)) scene=RESTART;
       
       headpos.x += direc_m.x; 
       headpos.y += direc_m.y;
 
      if(headpos.x < -20 || headpos.x > 1440 || headpos.y > 900 || headpos.y < 80) scene = RESTART; 
    }
        sfClock_restart(moveS); }
          
    }
        
        else if (scene == RESTART ) {
         char s[10]; 
         sprintf(s, "%d", scoree);
         sfText_setString(result2,s);
           if(scoree > 9 && scoree <= 99) resultPos.x = 680;
           else if (scoree > 99 && scoree <= 999) resultPos.x = 665;
           else if (scoree > 999) resultPos.x = 655;
           else resultPos.x = 695;
         sfText_setPosition(result2, resultPos);
           
         sfFloatRect n4 = sfRectangleShape_getGlobalBounds(restrt.shape);  
         if(sfFloatRect_contains(&n4,mousePos.x, mousePos.y)) { if(sfMouse_isButtonPressed(sfMouseLeft)) { sfRectangleShape_setFillColor(restrt.shape, restrt.active); move.x = 20.f*t; move.y = 0; direc = 1;  snake = create(snake, shape); scoree = 0; sfText_setString(text_score2, "0"); apple_pos.x = 1000.0;
          apple_pos.y = 120.0; sfRectangleShape_setPosition(shape2, apple_pos); scene = GAME; }  
    
         else   sfRectangleShape_setFillColor(restrt.shape, restrt.h); }
         else sfRectangleShape_setFillColor(restrt.shape, restrt.idle);
         
         sfFloatRect n5 = sfRectangleShape_getGlobalBounds(exit2.shape);
         if(sfFloatRect_contains(&n5,mousePos.x, mousePos.y)) { if(sfMouse_isButtonPressed(sfMouseLeft)) { sfRectangleShape_setFillColor(exit2.shape, exit2.active);  
            sfRenderWindow_close(window); } 
         else   sfRectangleShape_setFillColor(exit2.shape, exit2.h); }
         else sfRectangleShape_setFillColor(exit2.shape, exit2.idle); }
         
        if (scene == HELP) {
         sfFloatRect n3 = sfRectangleShape_getGlobalBounds(back.shape);  
         if(sfFloatRect_contains(&n3,mousePos.x, mousePos.y)) { if(sfMouse_isButtonPressed(sfMouseLeft)) { sfRectangleShape_setFillColor(back.shape, back.active); scene = MENU; }  else  sfRectangleShape_setFillColor(back.shape, back.h); }
         else sfRectangleShape_setFillColor(back.shape, back.idle);
        }
        // Events
        while(sfRenderWindow_pollEvent(window, &event)) {
            
             if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
    
    /* Clear the screen */   //Render
        sfRenderWindow_clear(window, sfBlack);
         if(scene == MENU) { 
         sfRenderWindow_drawSprite(window, seg, NULL);  
         sfRenderWindow_drawRectangleShape(window, new_game.shape, NULL);
         sfRenderWindow_drawText(window,new_game.text, NULL); 
         sfRenderWindow_drawRectangleShape(window, help.shape, NULL);
         sfRenderWindow_drawText(window, help.text, NULL); 
         sfRenderWindow_drawRectangleShape(window, exit1.shape, NULL);
         sfRenderWindow_drawText(window,exit1.text, NULL); 
        }   
         else if(scene == GAME){
         for (int i = 0; i < snake.size; i++) 
         sfRenderWindow_drawRectangleShape(window, snake.a[i].shape, NULL);
         sfRenderWindow_drawRectangleShape(window, shape2, NULL);
         sfRenderWindow_drawRectangleShape(window, score, NULL);
         sfRenderWindow_drawText(window, text, NULL);
         sfRenderWindow_drawText(window, text_score, NULL);
         sfRenderWindow_drawText(window, text_score2, NULL); 
        }
         else if (scene == RESTART ) {
         sfRenderWindow_drawRectangleShape(window, restrt.shape, NULL);
         sfRenderWindow_drawText(window,restrt.text, NULL); 
         sfRenderWindow_drawRectangleShape(window, exit2.shape, NULL);
         sfRenderWindow_drawText(window,exit2.text, NULL); 
         sfRenderWindow_drawText(window,result, NULL); 
         sfRenderWindow_drawText(window,result2, NULL); 
        }
         else if (scene == HELP) {
         sfRenderWindow_drawSprite(window, seg2, NULL);  

         sfRenderWindow_drawRectangleShape(window, back.shape, NULL);
         sfRenderWindow_drawText(window, back.text, NULL); 

        }
        /* Update the window */
        sfRenderWindow_display(window);

    } // Delete all
    sfRenderWindow_destroy(window);
    sfTexture_destroy(tseg);
    sfSprite_destroy(seg);
    sfTexture_destroy(tseg2);
    sfSprite_destroy(seg2);
    
    for (int i = 0; i < snake.size; i++) sfRectangleShape_destroy(snake.a[i].shape);
    
    sfRectangleShape_destroy(shape);
    sfRectangleShape_destroy(shape2);
    sfRectangleShape_destroy(new_game.shape);
    sfRectangleShape_destroy(help.shape);
    sfRectangleShape_destroy(exit1.shape);
    sfRectangleShape_destroy(restrt.shape);
    sfRectangleShape_destroy(back.shape);
    sfRectangleShape_destroy(exit2.shape);
    sfRectangleShape_destroy(score);
    sfClock_destroy(moveS);

    sfText_destroy(new_game.text);
    sfText_destroy(help.text);
    sfText_destroy(exit1.text);
    sfText_destroy(back.text);
    sfText_destroy(exit2.text);
    sfText_destroy(restrt.text);
    sfText_destroy(text);
    sfText_destroy(text_score);
    sfText_destroy(text_score2);
    sfText_destroy(result);
    sfText_destroy(result2);
    
}
