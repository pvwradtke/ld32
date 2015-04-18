#include <stdio.h>
#include <c2d3/chien2d.h>
#include <c2d3/chienaudio.h>

int main(int argc, char **argv)
{
    if(C2D_Inicia(1920, 1080, C2D_JANELA, "Teste da lib", true))
    {
        unsigned int fonte = C2D_CarregaFonte("gfx/DanceParty.ttf", "Dance Party 32", 32);
        if(fonte==0)
            printf("Erro ao carregar a fonte\n");
        const C2D_Botao *teclado = C2D_PegaTeclas();
        const C2D_Gamepad *gamepads = C2D_PegaGamepads();
        const C2D_Mouse *mouse = C2D_PegaMouse();
        bool sai = false;
        while(!sai)
        {
            if(teclado[C2D_TENCERRA].pressionou)
                sai=true;
            if(teclado[C2D_TESC].pressionou)
                sai=true;
            if(C2D_GamepadEstaConectado(0))
                if(gamepads[0].botoes[C2D_GBOTAO_B].pressionou)
                    sai = true;
            C2D_TrocaCorLimpezaTela((Uint8)0, (Uint8)0, (Uint8)0);
            C2D_LimpaTela();
            C2D_DesenhaTexto(fonte, 50, 50, "Hello World! Falamos Português em UTF-8!", 0, 255, 255, 255, 255);
            C2D_Sincroniza(60);
        }
        C2D_Encerra();
    }
    else
        printf("Erro ao inicializar a Chien2D\n");
}
