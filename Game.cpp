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
    int spritemouse = C2D_CarregaSpriteSet("gfx/mouse.png", 0, 0);
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
    mapa[20][33]=JOGO_PAREDE;
    mapa[22][33]=JOGO_PAREDE;
    mapa[20][35]=JOGO_PAREDE;

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
        for(int i=0;i<MAX_IMAS;i++)
                if(imas[i].tipo!=JOGO_MORTO)
                    atualizaIma(mapa, &imas[i], &jogador);

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
        if(controle == game_keyplusmouse)
            C2D_DesenhaSprite(spritemouse, 0, mouse->x-TAM_MOUSE/2, mouse->y-TAM_MOUSE/2);
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
                    imas[contaImas].x=32*j+(32-TAM_IMA)/2;
                    imas[contaImas].y=32*i+(32-TAM_IMA)/2;
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
    static int counter =0;
    // as variáveis para guardar as coordenadas do jogador
    int x=jogador->x, y=jogador->y;
    // Escolhe se o controle é pelo gamepad ou pelo teclado
    if(controle==Game::game_gamepad)
    {
        if(gamepads[0].eixos[C2D_GLEIXOX]!=0 || gamepads[0].eixos[C2D_GLEIXOY]!=0)
        {
            // Calcula o ângulo do deslocamento
            float angulo = calculaAngulo(gamepads[0].eixos[C2D_GLEIXOX], gamepads[0].eixos[C2D_GLEIXOY]);
            // Calcula a nova posição
            x = jogador->x+cos(angulo*PI/180)*VELOCIDADE_JOGADOR;
            y = jogador->y+sin(angulo*PI/180)*VELOCIDADE_JOGADOR;

        }
        // Calcula o angulo em que desenha o elemento
        if(gamepads[0].eixos[C2D_GREIXOX]!=0 || gamepads[0].eixos[C2D_GREIXOY]!=0)
            jogador->angulo=calculaAngulo(gamepads[0].eixos[C2D_GREIXOX], gamepads[0].eixos[C2D_GREIXOY]);
    }
    else
    {
        if(teclado[C2D_TESQUERDA].pressionando && !teclado[C2D_TCIMA].pressionando && !teclado[C2D_TBAIXO].pressionando)
            x=jogador->x-VELOCIDADE_JOGADOR;
        else if(teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TCIMA].pressionando && !teclado[C2D_TBAIXO].pressionando)
            x=jogador->x+VELOCIDADE_JOGADOR;
        else if(teclado[C2D_TCIMA].pressionando && !teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TESQUERDA].pressionando)
            y=jogador->y-VELOCIDADE_JOGADOR;
        else if(teclado[C2D_TBAIXO].pressionando && !teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TESQUERDA].pressionando)
            y=jogador->y+VELOCIDADE_JOGADOR;
        else if(teclado[C2D_TCIMA].pressionando && teclado[C2D_TDIREITA].pressionando)
        {
            x=jogador->x+VELOCIDADE_JOGADOR*0.707106781;
            y=jogador->y-VELOCIDADE_JOGADOR*0.707106781;
        }
        else if(teclado[C2D_TCIMA].pressionando && teclado[C2D_TESQUERDA].pressionando)
        {
            x=jogador->x-VELOCIDADE_JOGADOR*0.707106781;
            y=jogador->y-VELOCIDADE_JOGADOR*0.707106781;
        }
        else if(teclado[C2D_TBAIXO].pressionando && teclado[C2D_TESQUERDA].pressionando)
        {
            x=jogador->x-VELOCIDADE_JOGADOR*0.707106781;
            y=jogador->y+VELOCIDADE_JOGADOR*0.707106781;
        }
        else if(teclado[C2D_TBAIXO].pressionando && teclado[C2D_TESQUERDA].pressionando)
        {
            x=jogador->x-VELOCIDADE_JOGADOR*0.707106781;
            y=jogador->y+VELOCIDADE_JOGADOR*0.707106781;
        }
        else if(teclado[C2D_TBAIXO].pressionando && teclado[C2D_TDIREITA].pressionando)
        {
            x=jogador->x+VELOCIDADE_JOGADOR*0.707106781;
            y=jogador->y+VELOCIDADE_JOGADOR*0.707106781;
        }
        jogador->angulo=calculaAngulo(mouse->x-jogador->x+(TAM_JOGADOR/2), mouse->y-jogador->y+(TAM_JOGADOR/2));
    }
    // Verifica se o jogador está entrando dentro de um bloco
    int xesq = (int)x /32;
    int xdir = ((int)x +32)/32;
    int ycima = (int)y/32;
    int ybaixo = ((int)y+ 32)/32;
    int xmeio = ((int)x +16)/32;
    int ymeio = ((int)y + 16)/32;
    // Está batendo em cima?
    if(mapa[ycima][xmeio] != JOGO_CHAO)
        y=((ycima+1)*32);
    else if(mapa[ybaixo][xmeio] != JOGO_CHAO)
        y=((ycima)*32);
    // Está batendo à esquerda
    if(mapa[ymeio][xesq] != JOGO_CHAO)
        x=(xesq+1)*32 ;
    else if(mapa[ymeio][xdir] != JOGO_CHAO)
        x=(xesq)*32;
    // Atualiza a posição do jogador
    jogador->x=x;
    jogador->y=y;

}

void Game::atualizaIma(int mapa[33][59], Ima *ima, Jogador *jogador)
{
    //  se o jogador está perto para ganhar velocidade e direção
    int xcentroj = jogador->x + (TAM_JOGADOR/2);
    int ycentroj = jogador->y + (TAM_JOGADOR/2);
    int xcentroi = ima->x + TAM_IMA/2;
    int ycentroi = ima->y + TAM_IMA/2;
    // Calcula o quadrado da distância
    float distancia = sqrt((xcentroj-xcentroi)*(xcentroj-xcentroi)+ (ycentroj-ycentroi)*(ycentroj-ycentroi));
    // Se o quadrado da distância for menor que o quadrado da distância mínima
    if(distancia < DISTANCIA_JOGADOR)
    {
        // Calcula o angulo entre o jogador e o íma

        float anguloJogadorIma = calculaAngulo(xcentroi-xcentroj, ycentroi-ycentroj);
        float anguloImaJogador = calculaAngulo(xcentroj-xcentroi, ycentroj-ycentroi);
        // Calcula a diferença dos ângulos do jo
        int diferenca=abs(anguloJogadorIma-(int)jogador->angulo);
        // Caso seja maior que 270, normaliza o quadrante (casos especiais)
        if(diferenca>270)
            diferenca=360-diferenca;
        // Se for menor que 90, está apontando para o pólo positivo, se for maior que 90, aponta o põlo negativo
        // Aqui, repele o ímã
        if((diferenca < 45 && ima->tipo==JOGO_POSITIVO) || (diferenca>135 && ima->tipo==JOGO_NEGATIVO))
        {
            if(diferenca>90)
                diferenca = 180-diferenca;
            ima->angulo = anguloJogadorIma;
            ima->velocidade=VELOCIDADE_REPULSAO_IMA*(45.0-diferenca)/45.0;
            printf("Deu velocidade de repulsao %f\n", ima->velocidade);
        }
        // Aqui faz o contrário, afasta os ímãs
        else if((diferenca < 45 && ima->tipo==JOGO_NEGATIVO) || (diferenca>135 && ima->tipo==JOGO_POSITIVO))
        {
            // normaliza em zero
            if(diferenca>90)
                diferenca = 180-diferenca;
            ima->angulo = anguloImaJogador;
            ima->velocidade=(VELOCIDADE_ATRACAO_IMA)*(45.0-diferenca)/45.0;
            if(distancia < TAM_JOGADOR/2 + TAM_IMA +5)
                ima->velocidade=0;
            printf("Deu velocidade de atracao %f\n", ima->velocidade);
        }

    }
    // Futura posição do ímã na tela
    float x=ima->x+ima->velocidade*cos(ima->angulo*PI/180);
    float y=ima->y+ima->velocidade*sin(ima->angulo*PI/180);

    // Verifica se o jogador está entrando dentro de um bloco
    int xesq = (int)x/32;
    int xdir = ((int)x+TAM_IMA)/32;
    int ycima = (int)y/32;
    int ybaixo = ((int)y + TAM_IMA)/32;
    int xmeio = ((int)x + TAM_IMA/2)/32;
    int ymeio = ((int)y + TAM_IMA/2)/32;

    bool bateu=false;
    // Está batendo em cima?
    if(mapa[ycima][xmeio] != JOGO_CHAO)
    {
        y=((ycima+1)*32);
        ima->angulo=360-ima->angulo;
    }
    else if(mapa[ybaixo][xmeio] != JOGO_CHAO)
    {
        y=((ycima)*32+32-TAM_IMA);
        ima->angulo=360-ima->angulo;
    }
    // Está batendo à esquerda
    if(mapa[ymeio][xesq] != JOGO_CHAO)
    {
        x=(xesq+1)*32;
        ima->angulo=180-ima->angulo;
    }
    else if(mapa[ymeio][xdir] != JOGO_CHAO)
    {
        x=((xesq)*32+32-TAM_IMA);
        ima->angulo=180-ima->angulo;
    }
    if(ima->angulo<0)
        ima->angulo+=360;
    else
        ima->angulo= (int)ima->angulo%360;
    // Simula o atrito
    if(ima->velocidade>0)
    {
        ima->velocidade-=0.1;
        if(ima->velocidade<0)
            ima->velocidade=0;
    }

    // Atualiza a posição do ímã
    ima->x = x;
    ima->y=y;
}

float Game::calculaAngulo(const float dx, const float dy)
{
    float angulo = -1;
    if(dx == 0)
        angulo=90;
    else{
        float tg = (float)abs(dy) / abs(dx);
        angulo=atan(tg)*180/PI;
    }
    // According to the quadrant we are, we should add 90, 180 or 270 degrees
    if(dx<0 && dy >= 0)
        angulo=180-angulo;
    else if(dx<=0 && dy < 0)
        angulo += 180;
    else if(dx>0 && dy <0)
        angulo = 360 - angulo;
    return angulo;
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


