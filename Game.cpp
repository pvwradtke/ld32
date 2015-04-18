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

int tg_fp[90]={0, 35, 71, 107, 143, 179, 215, 251, 287, 324, 361, 398, 435, 472, 510, 548, 587, 626, 665, 705, 745, 786, 827, 869, 911, 954, 998, 1043, 1088, 1135, 1182, 1230, 1279, 1329, 1381, 1434, 1487, 1543, 1600, 1658, 1718, 1780, 1844, 1909, 1977, 2047, 2120, 2196, 2274, 2355, 2440, 2529, 2621, 2717, 2818, 2924, 3036, 3153, 3277, 3408, 3547, 3694, 3851, 4019, 4199, 4391, 4599, 4824, 5068, 5335, 5626, 5947, 6303, 6698, 7142, 7643, 8214, 8870, 9635, 10536, 11614, 12930, 14572, 16679, 19485, 23408, 29287, 39078, 58647, 117329};
int sin_fp[360]={0, 35, 71, 107, 142, 178, 214, 249, 285, 320, 355, 390, 425, 460, 495, 530, 564, 598, 632, 666, 700, 733, 767, 800, 832, 865, 897, 929, 961, 992, 1023, 1054, 1085, 1115, 1145, 1174, 1203, 1232, 1260, 1288, 1316, 1343, 1370, 1396, 1422, 1448, 1473, 1497, 1521, 1545, 1568, 1591, 1613, 1635, 1656, 1677, 1697, 1717, 1736, 1755, 1773, 1791, 1808, 1824, 1840, 1856, 1870, 1885, 1898, 1911, 1924, 1936, 1947, 1958, 1968, 1978, 1987, 1995, 2003, 2010, 2016, 2022, 2028, 2032, 2036, 2040, 2043, 2045, 2046, 2047, 2048, 2047, 2046, 2045, 2043, 2040, 2036, 2032, 2028, 2022, 2016, 2010, 2003, 1995, 1987, 1978, 1968, 1958, 1947, 1936, 1924, 1911, 1898, 1885, 1870, 1856, 1840, 1824, 1808, 1791, 1773, 1755, 1736, 1717, 1697, 1677, 1656, 1635, 1613, 1591, 1568, 1545, 1521, 1497, 1473, 1448, 1422, 1396, 1370, 1343, 1316, 1288, 1260, 1232, 1203, 1174, 1145, 1115, 1085, 1054, 1024, 992, 961, 929, 897, 865, 832, 800, 767, 733, 700, 666, 632, 598, 564, 530, 495, 460, 425, 390, 355, 320, 285, 249, 214, 178, 142, 107, 71, 35, 0, -35, -71, -107, -142, -178, -214, -249, -285, -320, -355, -390, -425, -460, -495, -530, -564, -598, -632, -666, -700, -733, -767, -800, -832, -865, -897, -929, -961, -992, -1023, -1054, -1085, -1115, -1145, -1174, -1203, -1232, -1260, -1288, -1316, -1343, -1370, -1396, -1422, -1448, -1473, -1497, -1521, -1545, -1568, -1591, -1613, -1635, -1656, -1677, -1697, -1717, -1736, -1755, -1773, -1791, -1808, -1824, -1840, -1856, -1870, -1885, -1898, -1911, -1924, -1936, -1947, -1958, -1968, -1978, -1987, -1995, -2003, -2010, -2016, -2022, -2028, -2032, -2036, -2040, -2043, -2045, -2046, -2047, -2048, -2047, -2046, -2045, -2043, -2040, -2036, -2032, -2028, -2022, -2016, -2010, -2003, -1995, -1987, -1978, -1968, -1958, -1947, -1936, -1924, -1911, -1898, -1885, -1870, -1856, -1840, -1824, -1808, -1791, -1773, -1755, -1736, -1717, -1697, -1677, -1656, -1635, -1613, -1591, -1568, -1545, -1521, -1497, -1473, -1448, -1422, -1396, -1370, -1343, -1316, -1288, -1260, -1232, -1203, -1174, -1145, -1115, -1085, -1054, -1024, -992, -961, -929, -897, -865, -832, -800, -767, -733, -700, -666, -632, -598, -564, -530, -495, -460, -425, -390, -355, -320, -285, -249, -214, -178, -142, -107, -71, -35};
int cos_fp[360]={2048, 2047, 2046, 2045, 2043, 2040, 2036, 2032, 2028, 2022, 2016, 2010, 2003, 1995, 1987, 1978, 1968, 1958, 1947, 1936, 1924, 1911, 1898, 1885, 1870, 1856, 1840, 1824, 1808, 1791, 1773, 1755, 1736, 1717, 1697, 1677, 1656, 1635, 1613, 1591, 1568, 1545, 1521, 1497, 1473, 1448, 1422, 1396, 1370, 1343, 1316, 1288, 1260, 1232, 1203, 1174, 1145, 1115, 1085, 1054, 1024, 992, 961, 929, 897, 865, 832, 800, 767, 733, 700, 666, 632, 598, 564, 530, 495, 460, 425, 390, 355, 320, 285, 249, 214, 178, 142, 107, 71, 35, 0, -35, -71, -107, -142, -178, -214, -249, -285, -320, -355, -390, -425, -460, -495, -530, -564, -598, -632, -666, -700, -733, -767, -800, -832, -865, -897, -929, -961, -992, -1023, -1054, -1085, -1115, -1145, -1174, -1203, -1232, -1260, -1288, -1316, -1343, -1370, -1396, -1422, -1448, -1473, -1497, -1521, -1545, -1568, -1591, -1613, -1635, -1656, -1677, -1697, -1717, -1736, -1755, -1773, -1791, -1808, -1824, -1840, -1856, -1870, -1885, -1898, -1911, -1924, -1936, -1947, -1958, -1968, -1978, -1987, -1995, -2003, -2010, -2016, -2022, -2028, -2032, -2036, -2040, -2043, -2045, -2046, -2047, -2048, -2047, -2046, -2045, -2043, -2040, -2036, -2032, -2028, -2022, -2016, -2010, -2003, -1995, -1987, -1978, -1968, -1958, -1947, -1936, -1924, -1911, -1898, -1885, -1870, -1856, -1840, -1824, -1808, -1791, -1773, -1755, -1736, -1717, -1697, -1677, -1656, -1635, -1613, -1591, -1568, -1545, -1521, -1497, -1473, -1448, -1422, -1396, -1370, -1343, -1316, -1288, -1260, -1232, -1203, -1174, -1145, -1115, -1085, -1054, -1024, -992, -961, -929, -897, -865, -832, -800, -767, -733, -700, -666, -632, -598, -564, -530, -495, -460, -425, -390, -355, -320, -285, -249, -214, -178, -142, -107, -71, -35, 0, 35, 71, 107, 142, 178, 214, 249, 285, 320, 355, 390, 425, 460, 495, 530, 564, 598, 632, 666, 700, 733, 767, 800, 832, 865, 897, 929, 961, 992, 1023, 1054, 1085, 1115, 1145, 1174, 1203, 1232, 1260, 1288, 1316, 1343, 1370, 1396, 1422, 1448, 1473, 1497, 1521, 1545, 1568, 1591, 1613, 1635, 1656, 1677, 1697, 1717, 1736, 1755, 1773, 1791, 1808, 1824, 1840, 1856, 1870, 1885, 1898, 1911, 1924, 1936, 1947, 1958, 1968, 1978, 1987, 1995, 2003, 2010, 2016, 2022, 2028, 2032, 2036, 2040, 2043, 2045, 2046, 2047};

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
        C2D_DesenhaSpriteEspecial(spriteJogador, 0, DESLX+(jogador.x >> FP_SHIFT), DESLY+(jogador.y >> FP_SHIFT), C2D_FLIP_NENHUM, 1.0, 1.0, jogador.angulo);
        for(int i=0;i<MAX_IMAS;i++)
            if(imas[i].tipo!=JOGO_MORTO)
                C2D_DesenhaSprite(spriteIma, imas[i].tipo-JOGO_NEGATIVO, DESLX+(imas[i].x >> FP_SHIFT), DESLY+(imas[i].y >> FP_SHIFT));
        if(controle == game_keyplusmouse)
            C2D_DesenhaSprite(spritemouse, 0, mouse->x-5, mouse->y-5);
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
                jogador->x=(32*j) << FP_SHIFT;
                jogador->y=(32*i) << FP_SHIFT;
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
                    imas[contaImas].x=(32*j+6)  << FP_SHIFT;
                    imas[contaImas].y=(32*i+6)  << FP_SHIFT;
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
    // as variáveis para guardar as coordenadas do jogador
    double x=jogador->x, y=jogador->y;
    // Escolhe se o controle é pelo gamepad ou pelo teclado
    if(controle==Game::game_gamepad)
    {
        if(gamepads[0].eixos[C2D_GLEIXOX]!=0 && gamepads[0].eixos[C2D_GLEIXOY]!=0)
        {
            // Calcula o ângulo do deslocamento
            double angulo = calculaAngulo(gamepads[0].eixos[C2D_GLEIXOX], gamepads[0].eixos[C2D_GLEIXOY]);
            printf("Eixo x: %03d - Eixo y: %03d - Angulo: %f\n", gamepads[0].eixos[C2D_GLEIXOX], gamepads[0].eixos[C2D_GLEIXOY], angulo);
            // Calcula a nova posição
            x = jogador->x+cos_fp[jogador->angulo]*VELOCIDADE_JOGADOR;
            y = jogador->y+sin_fp[jogador->angulo]*VELOCIDADE_JOGADOR;

        }
        // Calcula o angulo em que desenha o elemento
        jogador->angulo=calculaAngulo(gamepads[0].eixos[C2D_GREIXOX], gamepads[0].eixos[C2D_GREIXOY]);
    }
    else
    {
        if(teclado[C2D_TESQUERDA].pressionando && !teclado[C2D_TCIMA].pressionando && !teclado[C2D_TBAIXO].pressionando)
            x=jogador->x-(VELOCIDADE_JOGADOR<<FP_SHIFT);
        else if(teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TCIMA].pressionando && !teclado[C2D_TBAIXO].pressionando)
            x=jogador->x+(VELOCIDADE_JOGADOR<<FP_SHIFT);
        else if(teclado[C2D_TCIMA].pressionando && !teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TESQUERDA].pressionando)
            y=jogador->y-(VELOCIDADE_JOGADOR<<FP_SHIFT);
        else if(teclado[C2D_TBAIXO].pressionando && !teclado[C2D_TDIREITA].pressionando && !teclado[C2D_TESQUERDA].pressionando)
            y=jogador->y+(VELOCIDADE_JOGADOR<<FP_SHIFT);
        else if(teclado[C2D_TCIMA].pressionando && teclado[C2D_TDIREITA].pressionando)
        {
            x=jogador->x+VELOCIDADE_JOGADOR*cos_fp[45];
            y=jogador->y-VELOCIDADE_JOGADOR*sin_fp[45];
        }
        else if(teclado[C2D_TCIMA].pressionando && teclado[C2D_TESQUERDA].pressionando)
        {
            x=jogador->x-VELOCIDADE_JOGADOR*cos_fp[45];
            y=jogador->y-VELOCIDADE_JOGADOR*sin_fp[45];
        }
        else if(teclado[C2D_TBAIXO].pressionando && teclado[C2D_TESQUERDA].pressionando)
        {
            x=jogador->x-VELOCIDADE_JOGADOR*cos_fp[45];
            y=jogador->y+VELOCIDADE_JOGADOR*sin_fp[45];
        }
        else if(teclado[C2D_TBAIXO].pressionando && teclado[C2D_TESQUERDA].pressionando)
        {
            x=jogador->x-VELOCIDADE_JOGADOR*cos_fp[45];
            y=jogador->y+VELOCIDADE_JOGADOR*sin_fp[45];
        }
        else if(teclado[C2D_TBAIXO].pressionando && teclado[C2D_TDIREITA].pressionando)
        {
            x=jogador->x+VELOCIDADE_JOGADOR*cos_fp[45];
            y=jogador->y+VELOCIDADE_JOGADOR*sin_fp[45];
        }
        jogador->angulo=calculaAngulo((mouse->x << FP_SHIFT)-(jogador->x+(16 << FP_SHIFT)), (mouse->y << FP_SHIFT)-(jogador->y+(16 << FP_SHIFT)));
        printf("Angulo do jogador: %f\n", jogador->angulo);
    }

    // Atualiza a posição do jogador
    jogador->x=x;
    jogador->y=y;
}

int Game::calculaAngulo(const int dx, const int dy)
{
    int angle = -1;
    if(dx == 0)
        angle=90;
    else{
        // We shift by FP_SHIFT to have meaningful integer values
        int tg = (abs(dy) << FP_SHIFT) / abs(dx);
        for(int i=0;i<89;i++){
            if(tg_fp[i]<=tg && tg <=tg_fp[i+1]){
                int difi = tg - tg_fp[i];
                int difip1 = tg_fp[i+1] - tg;
                if(difi < difip1)
                    angle=i;
                else
                    angle=i+1;
                break;
            }
        }
        if(-1 == angle)
            angle=89;
    }
    // According to the quadrant we are, we should add 90, 180 or 270 degrees
    if(dx<0 && dy >= 0)
        angle=180-angle;
    else if(dx<=0 && dy < 0)
        angle += 180;
    else if(dx>0 && dy <0)
        angle = 360 - angle;
    return angle;
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


