#include "GameWindow.h"
#include "global.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <stdlib.h>

float c_big_start = 0;
float c_now = 0;
float c_move = 0;

float hahaha = 0;

int  start = 0;
float Attack::volume = 0.1;
void set_attack_volume(float volume)
{
    Attack::volume = volume;
}

bool compare(Tower *t1, Tower *t2)
{
    return (t1->getY() <= t2->getY());
}

void
GameWindow::game_init()
{
    char buffer[50];

    icon = al_load_bitmap("./icon.png");
    background = al_load_bitmap("battle.png");
    background_select_1=al_load_bitmap("./Munu.png"); //select character
    background_munu_0=al_load_bitmap("./start02.png"); //before start
    background_about_6=al_load_bitmap("./about.png");
    background_1win_4=al_load_bitmap("1win.png"); //before start
    background_2win_5=al_load_bitmap("2win.png");
    han = al_load_bitmap("explain han.png");
    chen = al_load_bitmap("explain chen.png");
    gao = al_load_bitmap("explain gao.png");
    lee = al_load_bitmap("explain lee.png");

    for(int i = 0; i < Num_TowerType; i++)
    {
        sprintf(buffer, "./Tower/%s.png", TowerClass[i]);
        tower[i] = al_load_bitmap(buffer);
    }

    al_set_display_icon(display, icon);
    al_reserve_samples(3);

    sample = al_load_sample("growl.wav");
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());

    sample = al_load_sample("bgm02.wav");
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());

    sample = al_load_sample("bomb.mp3");
    bomb = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(bomb, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(bomb, al_get_default_mixer());

    sample = al_load_sample("bomb2.wav");
    bomb2 = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(bomb2, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(bomb2, al_get_default_mixer());

    sample = al_load_sample("game_end.wav");
    game_end = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(game_end, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(game_end, al_get_default_mixer());

    level = new LEVEL(1);
    menu = new Menu();
    player1 = new Tower(1);
    player2 = new Tower(2);
    bullet = new Monster();
}

bool
GameWindow::mouse_hover(int startx, int starty, int width, int height)
{
    if(mouse_x >= startx && mouse_x <= startx + width)
        if(mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}


bool
GameWindow::isOnRoad()
{
    int startx, starty;
    int widthOfTower;

    widthOfTower = TowerWidth[selectedTower];

    for(int i=0; i < NumOfGrid; i++)
    {
        startx = (i % 15) * 40;
        starty = (i / 15) * 40;

        if(level->isRoad(i)) {
            if((mouse_x + (widthOfTower/2) < startx) || (mouse_x - (widthOfTower/2) > startx + grid_width))
                continue;
            else if((mouse_y + (widthOfTower/2) < starty) || (mouse_y > starty + grid_height))
                continue;
            else
                return true;
        }
    }
    return false;
}

Tower*
GameWindow::create_tower(int type)
{
    Tower *t = NULL;

    if(isOnRoad())
        return t;

    switch(type)
    {
    case ARCANE:
        t = new Arcane(mouse_x, mouse_y);
        break;
    case ARCHER:
        t = new Archer(mouse_x, mouse_y);
        break;
    case CANON:
        t = new Canon(mouse_x, mouse_y);
        break;
    case POISON:
        t = new Poison(mouse_x, mouse_y);
        break;
    case STORM:
        t = new Storm(mouse_x, mouse_y);
        break;
    default:
        break;
    }

    menu->Change_Coin(menu->getTowerCoin(type));

    return t;
}

Monster*
GameWindow::create_monster()
{
    Monster *m = NULL;

    if(level->MonsterNum[WOLF])
    {
        level->MonsterNum[WOLF]--;
        m = new Wolf(level->ReturnPath());
    }
    else if(level->MonsterNum[WOLFKNIGHT])
    {
        level->MonsterNum[WOLFKNIGHT]--;
        m = new WolfKnight(level->ReturnPath());
    }
    else if(level->MonsterNum[DEMONNIJIA])
    {
        level->MonsterNum[DEMONNIJIA]--;
        m = new DemonNijia(level->ReturnPath());
    }
    else if(level->MonsterNum[CAVEMAN])
    {
        level->MonsterNum[CAVEMAN]--;
        m = new CaveMan(level->ReturnPath());
    }
    else
    {
        al_stop_timer(monster_pro);
    }

    return m;
}

void
GameWindow::game_play()
{
    int msg;
    srand(time(NULL));
    msg = -1;
    game_reset();  // reset game and begin
    game_begin();

    while(msg != GAME_EXIT)
    {
        msg = game_run();
    }

    show_err_msg(msg);
}

void
GameWindow::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}

GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    timer = al_create_timer(1.0 / FPS);
    monster_pro = al_create_timer(1.0 / FPS);

    if(timer == NULL || monster_pro == NULL)
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",12,0); // load small font
    Medium_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",24,0); //load medium font
    Large_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",36,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(monster_pro));

    game_init();
}

void
GameWindow::game_begin()
{
    printf(">>> Start Level[%d]\n", level->getLevel());

    al_play_sample_instance(startSound);
    while(al_get_sample_instance_playing(startSound));

    draw_running_map();

    al_start_timer(timer);
    start = 1;
    al_start_timer(monster_pro);
}

int
GameWindow::game_run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event();
    }
    return error;
}

int
GameWindow::game_update()
{

 if(SCENE==1){

    unsigned int i, j;
    std::list<Tower*>::iterator it;

    bullet->Load_Move();

    if(HARD||EASY){
    if(c_big_start == 0){
        c_big_start = al_get_time();
        int a = (int)rand()%4;
        if(a == 1){
        if(menu->Enough(2,100))menu->wall(2);
        }if(a == 2){
        if(menu->Enough(2,300))menu->alcohol(2);
        }if(a == 3){
        if(menu->Enough(2,200))menu->blade(2);
        }
    }else{
        c_now = al_get_time();
        if(c_now - c_big_start > 5) c_big_start = 0;
    }
    if(c_move == 0){
        c_move = al_get_time();
        int a = (int)rand()%5;
        if(a)player1->update(2,a);
        else{
            if(player1->TriggerAttack(2)){
                            if(al_get_sample_instance_playing(bomb2))
                            al_stop_sample_instance(bomb2);
                            al_play_sample_instance(bomb2);
                        }
        }
    }else{
        c_now = al_get_time();
        if(EASY){
            if(c_now - c_move > 2)c_move = 0;
        }else {
            if(c_now - c_move > 0.5)c_move = 0;
        }
      }
    }

    int Winner = menu->winner();
    if(Winner == 1) SCENE = 3;
    if(Winner == 2) SCENE = 4;

    return GAME_CONTINUE;
    }
}

void
GameWindow::game_reset()
{
    // reset game and begin
    for(auto&& child : towerSet) {
        delete child;
    }
    towerSet.clear();
    monsterSet.clear();


    selectedTower = -1;
    lastClicked = -1;
    Coin_Inc_Count = 0;
    Monster_Pro_Count = 0;
    mute = false;
    redraw = false;
    menu->Reset();
    c_big_start = 0;
    c_now = 0;
    c_move = 0;

    hahaha= 0;

    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(startSound);
    al_stop_sample_instance(bomb);
    al_stop_sample_instance(bomb2);

    // stop timer
    al_stop_timer(timer);
    al_stop_timer(monster_pro);
}

void
GameWindow::game_destroy()
{
    game_reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_timer(timer);
    al_destroy_timer(monster_pro);

    for(int i=0;i<5; i++)
        al_destroy_bitmap(tower[i]);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);

    al_destroy_sample(sample);
    al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(backgroundSound);

    delete level;
    delete menu;
}

int
GameWindow::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    // offset for pause window
    int offsetX = field_width/2 - 200;
    int offsetY = field_height/2 - 200;

    al_wait_for_event(event_queue, &event);
    redraw = false;
    bool done = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        done = true;
        if(event.timer.source == timer) {
            redraw = true;
            if(hahaha){
                if(Coin_Inc_Count == 0)
                menu->Change_Coin(Coin_Time_Gain);
                Coin_Inc_Count = (Coin_Inc_Count + 1) % CoinSpeed;
            }
        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        done = true;
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
    done = true;
        if(SCENE == 2){

            switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_1:
                player1->KeyIn(1);
                break;
            case ALLEGRO_KEY_2:
               player1->KeyIn(2);
                break;
            case ALLEGRO_KEY_3:
               player1->KeyIn(3);
                break;
            case ALLEGRO_KEY_4:
                player1->KeyIn(4);
                break;
            case ALLEGRO_KEY_5:
                player1->KeyIn(5);
                break;
            case ALLEGRO_KEY_6:
                player1->KeyIn(6);
                break;
            case ALLEGRO_KEY_7:
                player1->KeyIn(7);
                break;
            case ALLEGRO_KEY_8:
                player1->KeyIn(8);
                break;
            case ALLEGRO_KEY_ENTER:
                SCENE = 1;
            break;
        }
        }
        else if(SCENE == 1){
        if(EASY || HARD){
            switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_PAD_1:
                if(menu->Enough(1,100)){
                    menu->wall(1);
                }
                break;
            case ALLEGRO_KEY_PAD_3:
                if(menu->Enough(1,300)){
                    menu->alcohol(1);
                }
                break;
            case ALLEGRO_KEY_PAD_2:
                if(menu->Enough(1,200)){
                    menu->blade(1);
                }
                break;
            case ALLEGRO_KEY_UP:
                         player1->update(1,1);
                        break;
                    case ALLEGRO_KEY_DOWN:
                         player1->update(1,2);
                        break;
                    case ALLEGRO_KEY_RIGHT:
                         player1->update(1,3);
                        break;
                    case ALLEGRO_KEY_LEFT:
                         player1->update(1,4);
                        break;
                    case ALLEGRO_KEY_SPACE:
                        if(player1->TriggerAttack(1)){
                            if(al_get_sample_instance_playing(bomb))
                            al_stop_sample_instance(bomb);
                            al_play_sample_instance(bomb);
                        }
                        break;
                    case ALLEGRO_KEY_P:
                    /*TODO: handle pause event here*/
                    if(al_get_timer_started(timer))
                    al_stop_timer(timer);
                    else
                    al_start_timer(timer);
                    break;
                    }
            }else{
            switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_PAD_1:
                if(menu->Enough(2,100)){
                    menu->wall(2);
                }
                break;
            case ALLEGRO_KEY_PAD_3:
                if(menu->Enough(2,300)){
                    menu->alcohol(2);
                }
                break;
            case ALLEGRO_KEY_PAD_2:
                if(menu->Enough(2,200)){
                    menu->blade(2);
                }
                break;
            case ALLEGRO_KEY_Z:
                if(menu->Enough(1,100)){
                    menu->wall(1);
                }
                break;

            case ALLEGRO_KEY_C:
                if(menu->Enough(1,300)){
                    menu->alcohol(1);
                }
                break;
            case ALLEGRO_KEY_X:
                if(menu->Enough(1,200)){
                    menu->blade(1);
                }
                break;
            case ALLEGRO_KEY_UP:
                         player1->update(2,1);
                        break;
                    case ALLEGRO_KEY_DOWN:
                         player1->update(2,2);
                        break;
                    case ALLEGRO_KEY_RIGHT:
                         player1->update(2,3);
                        break;
                    case ALLEGRO_KEY_LEFT:
                         player1->update(2,4);
                        break;
                    case ALLEGRO_KEY_W:
                         player1->update(1,1);
                        break;
                    case ALLEGRO_KEY_S:
                         player1->update(1,2);
                        break;
                    case ALLEGRO_KEY_D:
                        player1->update(1,3);
                        break;
                    case ALLEGRO_KEY_A:
                         player1->update(1,4);
                        break;
                    case ALLEGRO_KEY_SPACE:
                        if(player1->TriggerAttack(1)){
                            if(al_get_sample_instance_playing(bomb))
                            al_stop_sample_instance(bomb);
                            al_play_sample_instance(bomb);
                        }
                        break;
                    case ALLEGRO_KEY_ENTER:
                        if(player1->TriggerAttack(2)){
                            if(al_get_sample_instance_playing(bomb2))
                            al_stop_sample_instance(bomb2);
                            al_play_sample_instance(bomb2);
                        }
                        break;
                    case ALLEGRO_KEY_P:
                    /*TODO: handle pause event here*/
                    if(al_get_timer_started(timer))
                    al_stop_timer(timer);
                    else
                    al_start_timer(timer);
                    break;
                    }
            }}
             switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_M:
                mute = !mute;
                if(mute)
                    al_stop_sample_instance(backgroundSound);
                else
                    al_play_sample_instance(backgroundSound);
                break;
             }
} else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
    done = true;
        if(SCENE==0){
            if(mouse_hover(600,80,150,150)){
                al_stop_sample_instance(startSound);
                SCENE = 5;

            }
        }
        else if(SCENE==5){
            if(mouse_hover(400,100,700,250)){
                SCENE = 6;
            }
            if(mouse_hover(400,300,700,450)) SCENE=2;
        }else if(SCENE==6){
            if(mouse_hover(400,100,700,250)){
            EASY = true;
            SCENE = 2;
            }if(mouse_hover(400,300,700,450)){
            HARD = true;
            SCENE = 2;
            }
        }
       else if(SCENE==1){
        if(event.mouse.button == 1) {
            if(selectedTower != -1 && mouse_hover(0, 0, field_width, field_height)) {
                Tower *t = create_tower(selectedTower);

                if(t == NULL)
                    printf("Wrong place\n");
                else {
                    towerSet.push_back(t);
                    towerSet.sort(compare);
                }
            } else if(selectedTower == -1){
                std::list<Tower*>::iterator it = towerSet.begin();
                if(lastClicked != -1)
                {
                    std::advance(it, lastClicked);
                    (*it)->ToggleClicked();
                }
                for(i=0, it = towerSet.begin(); it != towerSet.end(); it++, i++)
                {
                    Circle *circle = (*it)->getCircle();
                    int t_width = (*it)->getWidth();

                    if(mouse_hover(circle->x - t_width/2, circle->y, t_width, t_width/2))
                    {
                        (*it)->ToggleClicked();
                        lastClicked = i;
                        break;
                    } else {
                        lastClicked = -1;
                    }
                }

            }
            // check if user wants to create some kind of tower
            // if so, show tower image attached to cursor
            selectedTower =  menu->MouseIn(mouse_x, mouse_y);

        }
        }else if(SCENE == 2){
            if(mouse_hover(960,80,1070,130)) SCENE=1;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        done = true;
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        menu->MouseIn(mouse_x, mouse_y);

    }

    if(redraw) {
        // update each object in game
        instruction = game_update();
        // Re-draw map
        draw_running_map();
        redraw = false;
    }
    return instruction;
}

void
GameWindow::draw_running_map()
{
    if(SCENE!=0 && SCENE!=3 && SCENE!= 4){
        al_play_sample_instance(backgroundSound);
    }
    if(SCENE==0){ //登陸畫面
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background_munu_0, 0, 0, 0);
   if(mouse_hover(800,80,150,150)){
    al_draw_bitmap(background_about_6, 0, 0, 0);
   }

   al_flip_display();
    }
    else if(SCENE==1){ //遊戲畫面
    //draw background
    unsigned int i, j;
    hahaha = al_get_time();

    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background, 0, 0, 0);

    if(selectedTower != -1) //red circle
      Tower::SelectedTower(mouse_x, mouse_y, selectedTower);

    al_draw_filled_rectangle(1000, 0, window_width, window_height, al_map_rgb(100, 100, 100));
    al_draw_filled_rectangle(0, 0, 150, window_height, al_map_rgb(100, 100, 100));

    menu->Draw();
    player1->Draw();
    bullet->Draw();

    al_flip_display();
    }
    else if(SCENE==2){ //選角畫面
            al_stop_sample_instance(bomb2);
            al_stop_sample_instance(bomb);
            al_clear_to_color(al_map_rgb(100, 100, 100));
            al_draw_bitmap(background_select_1, 0, 0, 0);
            if(mouse_hover(130,390,137,62)) {
            al_draw_line(136,392,266,392,al_map_rgb(150,10,10),15);
            al_draw_line(136,392,136,451,al_map_rgb(150,10,10),15);
            al_draw_line(266,451,266,392,al_map_rgb(150,10,10),15);
            al_draw_line(266,451,136,451,al_map_rgb(150,10,10),15);
                if(mouse_hover(141,397,120,49)){
                    al_draw_bitmap(han, 0, 0, 0);
                }
            }
              if(mouse_hover(358,388,137,62)) {
            al_draw_line(363,390,500,390,al_map_rgb(150,10,10),15);
            al_draw_line(363,390,363,452,al_map_rgb(150,10,10),15);
            al_draw_line(500,452,500,390,al_map_rgb(150,10,10),15);
            al_draw_line(500,452,363,452,al_map_rgb(150,10,10),15);
                    if(mouse_hover(368,395,127,52)){
                    al_draw_bitmap(chen, 0, 0, 0);}
            }
            if(mouse_hover(588,384,137,62)) {
            al_draw_line(593,389,724,389,al_map_rgb(150,10,10),15);
            al_draw_line(593,389,593,440,al_map_rgb(150,10,10),15);
            al_draw_line(724,440,724,389,al_map_rgb(150,10,10),15);
            al_draw_line(724,440,593,440,al_map_rgb(150,10,10),15);
                if(mouse_hover(598,394,121,41)){
                al_draw_bitmap(gao, 0, 0, 0);
                }
            }

            if(mouse_hover(835,383,137,62)) {
            al_draw_line(840,388,961,388,al_map_rgb(150,10,10),15);
            al_draw_line(840,388,840,446,al_map_rgb(150,10,10),15);
            al_draw_line(961,446,961,388,al_map_rgb(150,10,10),15);
            al_draw_line(961,446,840,446,al_map_rgb(150,10,10),15);
                if(mouse_hover(845,393,111,48)){
                al_draw_bitmap(lee, 0, 0, 0);
                }
            }

                if(mouse_hover(965,82,114,53)) {
            al_draw_line(965,82,1079,82,al_map_rgb(150,1,190),11);
            al_draw_line(965,82,965,135,al_map_rgb(150,1,190),11);
            al_draw_line(1079,135,1079,82,al_map_rgb(150,1,190),11);
            al_draw_line(1079,135,965,135,al_map_rgb(150,1,190),11);
            }

            al_flip_display();
    }
    else if(SCENE==3){ //player1 win
        al_draw_bitmap(background_1win_4,0, 0, 0);
        al_stop_sample_instance(backgroundSound);
        al_play_sample_instance(game_end);
        al_flip_display();
    }
    else if(SCENE==4){ //player2 win
        al_draw_bitmap(background_2win_5, 0, 0, 0);
        al_stop_sample_instance(backgroundSound);
        al_play_sample_instance(game_end);
        al_flip_display();
    }else if(SCENE == 5){//1P2P
        char buffer[50];
        al_clear_to_color(al_map_rgb(100, 100, 100));
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_filled_rectangle(400,100,700,250, al_map_rgb(100, 100, 100));
        sprintf(buffer, "%d     P",1);
        al_draw_text(Large_font, al_map_rgb(255, 255, 255), 490, 150 , 0, buffer);
        al_draw_filled_rectangle(400,300,700,450,al_map_rgb(100,100,100));
        sprintf(buffer, "%d     P",2 );
        al_draw_text(Large_font, al_map_rgb(255, 255, 255), 490, 350, 0, buffer);
        al_flip_display();
    }else if(SCENE == 6){//1P2P
        char buffer[50];
        al_clear_to_color(al_map_rgb(100, 100, 100));
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_filled_rectangle(400,100,700,250, al_map_rgb(100, 100, 100));
        sprintf(buffer, "  EASY  ",1);
        al_draw_text(Large_font, al_map_rgb(255, 255, 255), 490, 150 , 0, buffer);
        al_draw_filled_rectangle(400,300,700,450,al_map_rgb(100,100,100));
        sprintf(buffer, "  HARD  ",2 );
        al_draw_text(Large_font, al_map_rgb(255, 255, 255), 490, 350, 0, buffer);
        al_flip_display();
        }
}
