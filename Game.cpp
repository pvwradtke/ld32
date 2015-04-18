/*
 * File:   Game.cpp
 * Author: paulo
 *
 * Created on 26 de Abril de 2013, 23:05
 */

#include "Game.hpp"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

Game::Game() {
    initialized=false;
}

Game::~Game() {
}

bool Game::init(bool fullscreen){
    if(C2D_Inicia(1920, 1080, C2D_JANELA, "Ludum Dare 32", true))
    {
        fonteSistema = C2D_CarregaFonte("gfx/TerminusBold.ttf", "sistema", 24);
        fonteURL = C2D_CarregaFonte("gfx/TerminusBold.ttf", "URL", 60);
        fonteTitulo = C2D_CarregaFonte("gfx/DanceParty.ttf", "titulo", 100);
        teclado = C2D_PegaTeclas();
        gamepads = C2D_PegaGamepads();
        mouse = C2D_PegaMouse();
        initialized=true;
    }
    else
        return false;
    if(!loadhighscore())
        highScore = 150;
    return true;
}

bool Game::run(){
    if(!initialized)
        return false;
    int currentstate = Game::splash;
    int exit=false;
    std::string levelname="./resources/tutorial.dat";
    while(!exit){
        switch(currentstate)
        {
            case Game::splash:
                if(!splashscreen())
                    return false;
                currentstate = Game::mainmenu;
                break;
            case Game::mainmenu:
                currentstate = mainmenuscreen();
                break;
            case Game::credits:
                creditsscreen();
                currentstate = Game::mainmenuscreen();
                break;
            case Game::game:{
                srand(time(NULL));
                int previousHighScore = highScore;
                if(gamescreen())
                    currentstate = creditsscreen();
               else
                    currentstate = Game::mainmenu;
               if(highScore > previousHighScore)
                    savehighscore();
                break;
            }
            case Game::tutorial:
                currentstate = Game::mainmenu;
                break;
            case Game::quit:
                exit=true;
            default:
                currentstate = Game::splash;
                break;

        }
        if(teclado[C2D_TENCERRA].pressionou)
            exit=true;
    }
    return true;
}

bool Game::splashscreen(){
    int logo = C2D_CarregaSpriteSet("./gfx/chienloco.png", 0,0);
    if(logo == 0)
        return false;
    int timer=0;
    bool end = false;
    C2D_TrocaCorLimpezaTela(0,0,0);
    char url[] = "http://chienloco.com/wp";
    // Pega a largura do texto da URL
    int ltexto, atexto;
    C2D_DimensoesTexto(fonteSistema, url, &ltexto, &atexto);
    while(!end && !teclado[C2D_TENCERRA].pressionou){
        C2D_LimpaTela();
        int alpha;
        if(timer<120)
            alpha = (timer*255)/120;
        if(alpha>255)
            alpha=255;
        C2D_AlteraAlphaDoSprite(logo, alpha);
        C2D_DesenhaSprite(logo, 0, 480, 414);
        if(timer>=255)
            C2D_DesenhaTexto(fonteSistema, 1440-ltexto, 600, url, C2D_TEXTO_ESQUERDA, 255, 255, 255, 255);
        timer++;
        if(timer>=530)
            end=true;
        C2D_Sincroniza(C2D_FPS_PADRAO);
        // se pressionou qualquer tecla, pula fora
        for(int i=0;i<C2D_MAX_TECLAS;i++)
            if(teclado[i].pressionou)
                end=true;
    }
    C2D_RemoveSpriteSet(logo);
    return true;
}

int Game::mainmenuscreen(){
    int timer=0;
    bool end = false;

    bool tutorialonly = false;
    int score = 0;

    int choice;
    C2D_TrocaCorLimpezaTela(0,0,0);
    while(!end){
        C2D_LimpaTela();
        C2D_DesenhaTexto(fonteTitulo, 960, 350, "Ludum Dare 32", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 600, "Press Gamepad button A button to play with Joystick (best!)", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 640, "Press Space to play with Keyboard+Mouse (then go out and buy a gamepad)", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 800, "Press ESC to quit", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_DesenhaTexto(fonteSistema, 960, 900, "(c) 2013 Paulo V W Radtke - follow me at @pvwradtke or http://chienloco.com/wp/", C2D_TEXTO_CENTRALIZADO, 255, 255, 255, 255);
        C2D_Sincroniza(C2D_FPS_PADRAO);
        if(teclado[C2D_TESC].pressionou)
        {
            choice = Game::quit;
            end=true;
        }
        else if(teclado[C2D_TC].pressionou)
        {
            end = true;
            choice = Game::credits;
        }
    }
    return choice;
}

int Game::gamescreen(){
    // Load resources
    return 0;
}



bool Game::creditsscreen(){
    return true;
}

bool Game::loadhighscore(){
    FILE *hs = fopen("./resources/hs.dat", "r");
    if(hs==0)
        return false;
    fread(&highScore, sizeof(int), 1, hs);
    fclose(hs);
    return true;
}

bool Game::savehighscore(){
    FILE *hs = fopen("./resources/hs.dat", "w");
    if(hs==0)
        return false;
    fwrite(&highScore, sizeof(int), 1, hs);
    fclose(hs);
    return true;
}


