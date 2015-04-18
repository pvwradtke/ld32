/*
 * File:   Game.hpp
 * Author: paulo
 *
 * Created on 26 de Abril de 2013, 23:05
 */

#ifndef GAME_HPP
#define	GAME_HPP

#include <memory>
#include <string>
#include <c2d3/chien2d.h>
#include <c2d3/chienaudio.h>

#define MAX_IMAS        64
#define MAX_INIMIGOS    64

#define DESLX       16
#define DESLY       12

#define PI          3.14159265

// Define the number of bits to shift left or right values
#define FP_SHIFT 11
// The fixed point factor (same as shifting FP_SHIFT bits to the left)
#define FP_FACTOR   2 << FP_SHIFT

#define VELOCIDADE_JOGADOR  5

// All values are multiplied by 256 for fixed point precision
typedef struct TagEventDescriptor
{
    int frame;
    int type;
    std::string text;
    int textduration;
    int width, height, y, speed;
    int target;
    int repeat;
}EventDescriptor;

typedef struct TagJogador
{
    int x;
    int y;
    int angulo;
}Jogador;

typedef struct TagIma
{
    int tipo;
    int  x;
    int  y;
    int  angulo;
    int  velocidade;
}Ima;

class Game {
public:
    enum States{splash, mainmenu, help, game_keyplusmouse, game_gamepad, tutorial, credits, gameover, quit};
    enum marcas{JOGO_CHAO=0, JOGO_PAREDE, JOGO_JOGADOR, JOGO_INIMIGO, JOGO_NEGATIVO, JOGO_POSITIVO, JOGO_MORTO};
    enum EventDescriptorType {eventtext, eventenemy, eventjump, eventend, eventdisablescore, eventmusic, eventcomment};
    Game();
    virtual ~Game();

    bool init(bool fullscreen);
    bool run();
    bool splashscreen();
    int mainmenuscreen();
    int gamescreen(int controle);
    void processaFase(int mapa[33][59], Jogador *jogador, Ima imas[]);
    void atualizaJogador(int mapa[33][59], Jogador *jogador, int controle);
    int  calculaAngulo(const int dx, const int dy);
    bool loadhighscore();
    bool savehighscore();
    bool creditsscreen();
private:
    bool    initialized;
    const C2D_Botao *teclado;
    const C2D_Gamepad *gamepads;
    const C2D_Mouse *mouse;
    int     fonteTitulo;
    int     fonteSistema;
    int     fonteURL;
    int     highScore;
};

#endif	/* GAME_HPP */

