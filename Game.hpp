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

class Game {
public:
    enum States{splash, mainmenu, help, game_keyplusmouse, game_gamepad, tutorial, credits, gameover, quit};
    enum marcas{JOGO_CHAO=0, JOGO_PAREDE, JOGO_JOGADOR, JOGO_INIMIGO, JOGO_POSITIVO, JOGO_NEGATIVO};
    enum EventDescriptorType {eventtext, eventenemy, eventjump, eventend, eventdisablescore, eventmusic, eventcomment};
    Game();
    virtual ~Game();

    bool init(bool fullscreen);
    bool run();
    bool splashscreen();
    int mainmenuscreen();
    int gamescreen(int controle);
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

