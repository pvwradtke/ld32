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
#include <math.h>

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
            case Game::game_gamepad:
            case Game::game_keyplusmouse:{
                srand(time(NULL));
                int previousHighScore = highScore;
                if(gamescreen(currentstate))
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
        // Idem para o gamepad
        for(int i=0;i<C2D_GMAX_BOTOES;i++)
            if(gamepads[0].botoes[i].pressionou)
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
        else if(teclado[C2D_TESPACO].pressionou)
        {
            end=true;
            choice = Game::game_keyplusmouse;
        }
        else if(gamepads[0].botoes[C2D_GBOTAO_A].pressionou)
        {
            end = true;
            choice = Game::game_gamepad;
        }
    }
    return choice;
}

int Game::gamescreen(int controle){
    // The current map
    int mapa[33][59];
    memset(mapa, JOGO_CHAO, 33*59*sizeof(int));
    // Load resources
    int cenario = C2D_CarregaSpriteSet("gfx/map.png", 32, 32);
    int spriteJogador = C2D_CarregaSpriteSet("gfx/jogador.png", 32, 32);
    int spriteIma = C2D_CarregaSpriteSet("gfx/imas.png", 20, 20);
    for(int x=0;x<59;x++)
    {
        mapa[0][x]=JOGO_PAREDE;
        mapa[32][x]=JOGO_PAREDE;
    }
    for(int y=0;y<33;y++)
    {
        mapa[y][0]=JOGO_PAREDE;
        mapa[y][58]=JOGO_PAREDE;
    }
    // Posiciona o jogador e alguns imas
    mapa[16][29]=JOGO_JOGADOR;
    mapa[10][49]=JOGO_POSITIVO;
    mapa[23][10]=JOGO_NEGATIVO;

    // Inicializa a fase com os dados do mapa
    Jogador jogador;
    Ima imas[MAX_IMAS];
    processaFase(mapa, &jogador, imas);

    C2D_TrocaCorLimpezaTela(0,0,0);
    bool fim=false;
    while(!fim)
    {
        // Lógica dos personagens
        atualizaJogador(mapa, &jogador, controle);

        if(teclado[C2D_TESC].pressionou || teclado[C2D_TENCERRA].pressionou)
            fim=true;

        C2D_LimpaTela();
        // Desenha o mapa
        for(int i=0;i<33;i++)
            for(int j=0;j<59;j++)
                if(mapa[i][j]==JOGO_CHAO)
                    C2D_DesenhaSprite(cenario, 0, DESLX+32*j, DESLY+32*i);
                else
                    C2D_DesenhaSprite(cenario, 1, DESLX+32*j, DESLY+32*i);
        // Desenha os personagens
        C2D_DesenhaSpriteEspecial(spriteJogador, 0, DESLX+(int)jogador.x, DESLY+(int)jogador.y, C2D_FLIP_NENHUM, 1.0, 1.0, jogador.angulo);
        for(int i=0;i<MAX_IMAS;i++)
            if(imas[i].tipo!=JOGO_MORTO)
                C2D_DesenhaSprite(spriteIma, imas[i].tipo-JOGO_NEGATIVO, DESLX+(int)imas[i].x, DESLY+(int)imas[i].y);
        C2D_Sincroniza(C2D_FPS_PADRAO);

    }
    return 0;
}

void Game::processaFase(int mapa[33][59], Jogador *jogador, Ima imas[])
{
    // Procura os elementos
    int contaImas=0;
    for(int i=0;i<33;i++)
        for(int j=0;j<59;j++)
        {
            switch(mapa[i][j])
            {
            case JOGO_JOGADOR:
                jogador->x=32*j;
                jogador->y=32*i;
                jogador->angulo=0;
                mapa[i][j]=JOGO_CHAO;
                break;
            case JOGO_NEGATIVO:
            case JOGO_POSITIVO:
                if(contaImas<MAX_IMAS)
                {
                    imas[contaImas].tipo=mapa[i][j];
                    imas[contaImas].angulo=0;
                    imas[contaImas].velocidade=0;
                    imas[contaImas].x=32*j+6;
                    imas[contaImas].y=32*i+6;
                    mapa[i][j]=JOGO_CHAO;
                    contaImas++;
                }
                break;
            }
        }
    // Reseta os imas restantes
    for(int i=contaImas;i<MAX_IMAS;i++)
        imas[i].tipo=JOGO_MORTO;
}

void Game::atualizaJogador(int mapa[33][59], Jogador *jogador, int controle)
{
    // Escolhe se o controle é pelo gamepad ou pelo teclado
    if(controle==Game::game_gamepad)
    {
        if(gamepads[0].eixos[C2D_GLEIXOX]!=0 && gamepads[0].eixos[C2D_GLEIXOY]!=0)
        {
            // Calcula o ângulo do deslocamento
            double angulo = calculaAngulo(gamepads[0].eixos[C2D_GLEIXOX], gamepads[0].eixos[C2D_GLEIXOY]);
            // Calcula a nova posição
            double x = jogador->x+cos(jogador->angulo*PI/180)*VELOCIDADE_JOGADOR;
            double y = jogador->y+sin(jogador->angulo*PI/180)*VELOCIDADE_JOGADOR;
            jogador->x = x;
            jogador->y = y;
        }
        // Calcula o angulo em que desenha o elemento
        jogador->angulo=calculaAngulo(gamepads[0].eixos[C2D_GREIXOX], gamepads[0].eixos[C2D_GREIXOY]);
    }
}

double Game::calculaAngulo(int x, int y)
{
    // Verifica se é um ângulo paralelo a um dos eixos
    if(x==0)
    {
        if(y==0)
            return 0;
        else if(y>0)
            return 90;
        else
            return 270;
    }
    if(y==0)
    {
        if(x==0)
            return 0;
        else if(x>0)
            return 0;
        else
            return 180;
    }
    double tangente = y/x;
    return atan(tangente)*180/PI;
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


