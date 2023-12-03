#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "personagem.h";//Struct Personagem
#include "elementos.h";//Struct Elementos
/// <summary>
/// Usar este aqui
/// </summary>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int POS_X_INICIAL = SCREEN_WIDTH / 2;
const int POS_Y_INICIAL = SCREEN_HEIGHT / 2;


#define SIZE_MAX_ELEMENTOS      11

ALLEGRO_FONT* font = NULL;

////////////////////////////////////////////
//INDICE
////////////////////////////////////////////

#define INDICE_ELEM_CLORO       0
#define INDICE_ELEM_SODIO       1
#define INDICE_ELEM_OXIGENIO    2
#define INDICE_ELEM_HIDROGENIO  3
#define INDICE_ELEM_CARBONO     4
#define INDICE_ELEM_NITORGENIO  5
#define INDICE_ELEM_ENXOFRE     6 
#define INDICE_ELEM_SAL         7
#define INDICE_ELEM_METANO      8
#define INDICE_ELEM_AGUA        9
#define INDICE_ELEM_AMONIA      10


////////////////////////////////////////////
//ID
////////////////////////////////////////////

#define ID_ELEM_CLORO       1
#define ID_ELEM_SODIO       2
#define ID_ELEM_OXIGENIO    3
#define ID_ELEM_HIDROGENIO  4
#define ID_ELEM_CARBONO     5
#define ID_ELEM_NITORGENIO  6
#define ID_ELEM_ENXOFRE     7 
#define ID_ELEM_SAL         8
#define ID_ELEM_METANO      9
#define ID_ELEM_AGUA        10
#define ID_ELEM_AMONIA      11

//**************************************
// Variaveis
//**************************************
struct personagem nerdola;//Struct personagem

elementos arrayElementos[SIZE_MAX_ELEMENTOS];


//Usado na camera
float camera_x = 0;//atualiza camera x
float camera_y = 0;//atualiza camera y
float zoom_map = 4;//Dá zoom

//X para função de capturar usar
float pos_character_x = 0;
float pos_character_x_mais_larg = 0;
float pos_character_y = 0;
float pos_character_y_mais_alt = 0;
//Y para função de capturar usar
float pos_element_x = 0;
float pos_element_x_mais_larg = 0;
float pos_element_y = 0;
float pos_element_y_mais_alt = 0;

//Usado para o personagem e os elementos
int tamanho_map_x = 1000;//Tamanho do mapa x
int tamanho_map_y = 600;//Tamanho do mapa y
int tam_map_max_x = 2250;//Tamanho maximo na horizontal
int tam_map_max_y = 2037;//Tamanho maximo na vertical

int tamanho_nerd_x = 58;//Tamanho x do protagonista
int tamanho_nerd_y = 63;//Tamanho y do protagonista

int num_rand_x = 0;//Numero aleatorio x
int num_rand_y = 0;//Numero aleatorio y

char infoAdicional[10];// Letras
char textoCompleto[20];// Letras finais

int contador_pontos_todal = 0;//Contador de pontos 

ALLEGRO_EVENT_QUEUE* event_queue;

/*************************
* MENU
*************************/

ALLEGRO_FONT* font_menu;
ALLEGRO_TIMER* timer_menu;
ALLEGRO_TIMER* blink_timer_menu;
ALLEGRO_TIMER* temporizador;

ALLEGRO_BITMAP* bg_menu;
ALLEGRO_BITMAP* tabela_menu;
ALLEGRO_BITMAP* titulo_menu; // Adicione o caminho da sua imagem aqui
float escala_menu = 0.9;
float bg_escala_width = 1.25;
float bg_escala_height = 1.13;


float frame_menu = 0.f;
int pos_x_menu = 0, pos_y_menu = 0;
int current_frame_y_menu = 161;

bool text_visible_menu = false;
bool tabela_visible_menu = false;

bool running = true;
bool redraw_menu = false;

int countdown = 10;
int estado = 0;

//**************************************
// Funções
//**************************************
void num_rand() {
    srand(time(NULL));
    for (int i = 0; i < 7; i++) {
        arrayElementos[i].ativo = true;
    }
    for (int indice = 0; indice < SIZE_MAX_ELEMENTOS; indice++) {
        if (arrayElementos[indice].ativo == true) {
            arrayElementos[indice].pos_x = rand() % (tam_map_max_x - arrayElementos[indice].largura);
            arrayElementos[indice].pos_y = rand() % (tam_map_max_y - arrayElementos[indice].altura);
        }
    }
}

void atualizar_menu() {
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);


    if (event.type == ALLEGRO_EVENT_TIMER) {
        if (event.timer.source == timer_menu) {
            redraw_menu = true;
        }
        else if (event.timer.source == blink_timer_menu) {
            text_visible_menu = !text_visible_menu;
        }
    }
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            estado = 1;
            contador_pontos_todal = 0;
        }
    }
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_TAB) {
            tabela_visible_menu = !tabela_visible_menu;
        }
    }



    if (redraw_menu && al_is_event_queue_empty(event_queue)) {
        redraw_menu = false;

        frame_menu += 0.3f;
        if (frame_menu > 3) {
            frame_menu -= 3;
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));

        al_draw_scaled_bitmap(bg_menu, 0, 0, al_get_bitmap_width(bg_menu), al_get_bitmap_height(bg_menu),
            0, 0, al_get_bitmap_width(bg_menu) * bg_escala_width, al_get_bitmap_height(bg_menu) * bg_escala_height, 0);

        al_draw_scaled_bitmap(titulo_menu, 0, 0, al_get_bitmap_width(titulo_menu), al_get_bitmap_height(titulo_menu),
            80, 30, al_get_bitmap_width(titulo_menu) * escala_menu, al_get_bitmap_height(titulo_menu) * escala_menu, 0);

        al_draw_textf(font_menu, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %d", contador_pontos_todal);
        
        if (tabela_visible_menu) {
            al_draw_scaled_bitmap(tabela_menu, 0, 0, al_get_bitmap_width(tabela_menu), al_get_bitmap_height(tabela_menu),
                200, 70, al_get_bitmap_width(tabela_menu) * 1.4, al_get_bitmap_height(tabela_menu) * 1, 0);
        }
        else if (text_visible_menu) {
            al_draw_text(font_menu, al_map_rgb(255, 255, 255), 10, 540, ALLEGRO_ALIGN_LEFT, "Use a tecla BACKSPACE para iniciar");
            al_draw_text(font_menu, al_map_rgb(255, 255, 255), 10, 560, ALLEGRO_ALIGN_LEFT, "Use a tecla TAB para ver a tabela de elementos");
        }
    }
}

void iniciar_personagem() {
    //Nerdola e seus tamanhos
    nerdola.frame = 0.f;
    nerdola.largura = tamanho_nerd_x;
    nerdola.altura = tamanho_nerd_y;
    nerdola.pos_x = tamanho_map_x - tamanho_nerd_x;
    nerdola.pos_y = tamanho_map_y - tamanho_nerd_y;
    nerdola.current_frame_y = 0;
    nerdola.time = 0;
}

void atualizar_camera() {
    //Atualiza a camera comforme o personagem anda pelo mapa
    camera_x = nerdola.pos_x - tamanho_map_x / 2;
    camera_y = nerdola.pos_y - tamanho_map_y / 2;
}

void iniciar_elementos() {

    //Inicia Elemento cloro
    arrayElementos[INDICE_ELEM_CLORO].id = INDICE_ELEM_CLORO;
    memcpy((void*)arrayElementos[INDICE_ELEM_CLORO].nome, (void*)"Cloro", sizeof("Cloro"));
    arrayElementos[INDICE_ELEM_CLORO].bitmap = al_load_bitmap("./cloro.png");
    arrayElementos[INDICE_ELEM_CLORO].largura = 50;
    arrayElementos[INDICE_ELEM_CLORO].altura = 50;
    arrayElementos[INDICE_ELEM_CLORO].pos_x = 0;
    arrayElementos[INDICE_ELEM_CLORO].pos_y = 0;
    arrayElementos[INDICE_ELEM_CLORO].contador = 0;
    arrayElementos[INDICE_ELEM_CLORO].ativo = true;
    arrayElementos[INDICE_ELEM_CLORO].capturado = false;
    //Cloro    Check

    //Inicia Elemento Sodio
    arrayElementos[INDICE_ELEM_SODIO].id = INDICE_ELEM_SODIO;
    memcpy((void*)arrayElementos[INDICE_ELEM_SODIO].nome, (void*)"Sodio", sizeof("Sodio"));
    arrayElementos[INDICE_ELEM_SODIO].bitmap = al_load_bitmap("./sodio.png");
    arrayElementos[INDICE_ELEM_SODIO].largura = 50;
    arrayElementos[INDICE_ELEM_SODIO].altura = 50;
    arrayElementos[INDICE_ELEM_SODIO].pos_x = 0;
    arrayElementos[INDICE_ELEM_SODIO].pos_y = 0;
    arrayElementos[INDICE_ELEM_SODIO].contador = 0;
    arrayElementos[INDICE_ELEM_SODIO].ativo = true;
    arrayElementos[INDICE_ELEM_SODIO].capturado = false;
    //sodio    Check


        //Inicia Elemento Oxigênio
    arrayElementos[INDICE_ELEM_OXIGENIO].id = INDICE_ELEM_OXIGENIO;
    memcpy((void*)arrayElementos[INDICE_ELEM_OXIGENIO].nome, (void*)"Oxigenio", sizeof("Oxigenio"));
    arrayElementos[INDICE_ELEM_OXIGENIO].bitmap = al_load_bitmap("./oxigenio.png");
    arrayElementos[INDICE_ELEM_OXIGENIO].largura = 50;
    arrayElementos[INDICE_ELEM_OXIGENIO].altura = 50;
    arrayElementos[INDICE_ELEM_OXIGENIO].pos_x = 0;
    arrayElementos[INDICE_ELEM_OXIGENIO].pos_y = 0;
    arrayElementos[INDICE_ELEM_OXIGENIO].contador = 0;
    arrayElementos[INDICE_ELEM_OXIGENIO].ativo = true;
    arrayElementos[INDICE_ELEM_OXIGENIO].capturado = false;

    //Oxigênio  Check


        //Inicia Elemento HIDROGENIO
    arrayElementos[INDICE_ELEM_HIDROGENIO].id = INDICE_ELEM_HIDROGENIO;
    memcpy((void*)arrayElementos[INDICE_ELEM_HIDROGENIO].nome, (void*)"Hidrogenio", sizeof("Hidrogenio"));
    arrayElementos[INDICE_ELEM_HIDROGENIO].bitmap = al_load_bitmap("./hidrogenio.png");
    arrayElementos[INDICE_ELEM_HIDROGENIO].largura = 50;
    arrayElementos[INDICE_ELEM_HIDROGENIO].altura = 50;
    arrayElementos[INDICE_ELEM_HIDROGENIO].pos_x = 0;
    arrayElementos[INDICE_ELEM_HIDROGENIO].pos_y = 0;
    arrayElementos[INDICE_ELEM_HIDROGENIO].contador = 0;
    arrayElementos[INDICE_ELEM_HIDROGENIO].ativo = true;
    arrayElementos[INDICE_ELEM_HIDROGENIO].capturado = false;
    //HIDROGENIO    Check


        //Inicia Elemento CARBONO
    arrayElementos[INDICE_ELEM_CARBONO].id = INDICE_ELEM_CARBONO;
    memcpy((void*)arrayElementos[INDICE_ELEM_CARBONO].nome, (void*)"Carbono", sizeof("Carbono"));
    arrayElementos[INDICE_ELEM_CARBONO].bitmap = al_load_bitmap("./carbono.png");
    arrayElementos[INDICE_ELEM_CARBONO].largura = 50;
    arrayElementos[INDICE_ELEM_CARBONO].altura = 50;
    arrayElementos[INDICE_ELEM_CARBONO].pos_x = 0;
    arrayElementos[INDICE_ELEM_CARBONO].pos_y = 0;
    arrayElementos[INDICE_ELEM_CARBONO].contador = 0;
    arrayElementos[INDICE_ELEM_CARBONO].ativo = true;
    arrayElementos[INDICE_ELEM_CARBONO].capturado = false;
    //CARBONO       Check

        //Inicia Elemento NITORGENIO
    arrayElementos[INDICE_ELEM_NITORGENIO].id = INDICE_ELEM_NITORGENIO;
    memcpy((void*)arrayElementos[INDICE_ELEM_NITORGENIO].nome, (void*)"Nitrogenio", sizeof("Nitrogenio"));
    arrayElementos[INDICE_ELEM_NITORGENIO].bitmap = al_load_bitmap("./nitrogenio.png");
    arrayElementos[INDICE_ELEM_NITORGENIO].largura = 50;
    arrayElementos[INDICE_ELEM_NITORGENIO].altura = 50;
    arrayElementos[INDICE_ELEM_NITORGENIO].pos_x = 0;
    arrayElementos[INDICE_ELEM_NITORGENIO].pos_y = 0;
    arrayElementos[INDICE_ELEM_NITORGENIO].contador = 0;
    arrayElementos[INDICE_ELEM_NITORGENIO].ativo = true;
    arrayElementos[INDICE_ELEM_NITORGENIO].capturado = false;
    //NITORGENIO     Check

        //Inicia Elemento ENXOFRE
    arrayElementos[INDICE_ELEM_ENXOFRE].id = INDICE_ELEM_ENXOFRE;
    memcpy((void*)arrayElementos[INDICE_ELEM_ENXOFRE].nome, (void*)"Enxofre", sizeof("Enxofre"));
    arrayElementos[INDICE_ELEM_ENXOFRE].bitmap = al_load_bitmap("./enxofre.png");
    arrayElementos[INDICE_ELEM_ENXOFRE].largura = 50;
    arrayElementos[INDICE_ELEM_ENXOFRE].altura = 50;
    arrayElementos[INDICE_ELEM_ENXOFRE].pos_x = 0;
    arrayElementos[INDICE_ELEM_ENXOFRE].pos_y = 0;
    arrayElementos[INDICE_ELEM_ENXOFRE].contador = 0;
    arrayElementos[INDICE_ELEM_ENXOFRE].ativo = true;
    arrayElementos[INDICE_ELEM_ENXOFRE].capturado = false;
    //ENXOFRE       

        //Inicia Elemento SAL
    arrayElementos[INDICE_ELEM_SAL].id = INDICE_ELEM_SAL;
    memcpy((void*)arrayElementos[INDICE_ELEM_SAL].nome, (void*)"Sal", sizeof("Sal"));
    arrayElementos[INDICE_ELEM_SAL].bitmap = al_load_bitmap("./cloro.png");
    arrayElementos[INDICE_ELEM_SAL].largura = 0;
    arrayElementos[INDICE_ELEM_SAL].altura = 0;
    arrayElementos[INDICE_ELEM_SAL].pos_x = 0;
    arrayElementos[INDICE_ELEM_SAL].pos_y = 0;
    arrayElementos[INDICE_ELEM_SAL].contador = 0;
    arrayElementos[INDICE_ELEM_SAL].ativo = false;
    arrayElementos[INDICE_ELEM_SAL].capturado = false;
    //SAL      Check


       //Inicia Elemento METANO
    arrayElementos[INDICE_ELEM_METANO].id = INDICE_ELEM_METANO;
    memcpy((void*)arrayElementos[INDICE_ELEM_METANO].nome, (void*)"Metano", sizeof("Metano"));
    arrayElementos[INDICE_ELEM_METANO].bitmap = al_load_bitmap("./cloro.png");
    arrayElementos[INDICE_ELEM_METANO].largura = 0;
    arrayElementos[INDICE_ELEM_METANO].altura = 0;
    arrayElementos[INDICE_ELEM_METANO].pos_x = 0;
    arrayElementos[INDICE_ELEM_METANO].pos_y = 0;
    arrayElementos[INDICE_ELEM_METANO].contador = 0;
    arrayElementos[INDICE_ELEM_METANO].ativo = false;
    arrayElementos[INDICE_ELEM_METANO].capturado = false;
    //METANO        Check


        //Inicia Elemento AGUA
    arrayElementos[INDICE_ELEM_AGUA].id = INDICE_ELEM_AGUA;
    memcpy((void*)arrayElementos[INDICE_ELEM_AGUA].nome, (void*)"Agua", sizeof("Agua"));
    arrayElementos[INDICE_ELEM_AGUA].bitmap = al_load_bitmap("./cloro.png");
    arrayElementos[INDICE_ELEM_AGUA].largura = 0;
    arrayElementos[INDICE_ELEM_AGUA].altura = 0;
    arrayElementos[INDICE_ELEM_AGUA].pos_x = 0;
    arrayElementos[INDICE_ELEM_AGUA].pos_y = 0;
    arrayElementos[INDICE_ELEM_AGUA].contador = 0;
    arrayElementos[INDICE_ELEM_AGUA].ativo = false;
    arrayElementos[INDICE_ELEM_AGUA].capturado = false;
    //AGUA      Check

        //Inicia Elemento AMONIA
    arrayElementos[INDICE_ELEM_AMONIA].id = INDICE_ELEM_AMONIA;
    memcpy((void*)arrayElementos[INDICE_ELEM_AMONIA].nome, (void*)"Amonia", sizeof("Amonia"));
    arrayElementos[INDICE_ELEM_AMONIA].bitmap = al_load_bitmap("./cloro.png");
    arrayElementos[INDICE_ELEM_AMONIA].largura = 0;
    arrayElementos[INDICE_ELEM_AMONIA].altura = 0;
    arrayElementos[INDICE_ELEM_AMONIA].pos_x = 0;
    arrayElementos[INDICE_ELEM_AMONIA].pos_y = 0;
    arrayElementos[INDICE_ELEM_AMONIA].contador = 0;
    arrayElementos[INDICE_ELEM_AMONIA].ativo = false;
    arrayElementos[INDICE_ELEM_AMONIA].capturado = false;
    //AMONIA
}

void capturação() {
    pos_character_x = nerdola.pos_x;
    pos_character_x_mais_larg = nerdola.pos_x + tamanho_nerd_x;
    pos_character_y = nerdola.pos_y;
    pos_character_y_mais_alt = nerdola.pos_y + tamanho_nerd_y;




    //Varre o array para ver como está indo o bicho
    for (int indice = 0; indice < SIZE_MAX_ELEMENTOS; indice++) {
        // Ve o elemento esta ativo e ve o elemento NAO foi capturado
        if (arrayElementos[indice].ativo == true && arrayElementos[indice].capturado == false) {

            // Calcula Quadrado do Elemento
            pos_element_x = arrayElementos[indice].pos_x;
            pos_element_x_mais_larg = arrayElementos[indice].pos_x + arrayElementos[indice].largura;
            pos_element_y = arrayElementos[indice].pos_y;
            pos_element_y_mais_alt = arrayElementos[indice].pos_y + arrayElementos[indice].altura;

            // Se algum ponto do quadrado do personagem estiver dentro do quadrado do elemento  
            if (
                (((pos_character_x >= pos_element_x) && (pos_character_x <= pos_element_x_mais_larg)) ||
                    ((pos_character_x_mais_larg >= pos_element_x) && (pos_character_x_mais_larg <= pos_element_x_mais_larg))
                    ) && (
                        ((pos_character_y >= pos_element_y) && (pos_character_y <= pos_element_y_mais_alt)) ||
                        ((pos_character_y_mais_alt >= pos_element_y) && (pos_character_y_mais_alt <= pos_element_y_mais_alt))
                        )
                )
            {
                arrayElementos[indice].pos_x = rand() % (tam_map_max_x - arrayElementos[indice].largura);
                arrayElementos[indice].pos_y = rand() % (tam_map_max_y - arrayElementos[indice].altura);
                arrayElementos[indice].contador += 1;
                contador_pontos_todal += 1;
            }
        }
    }


}

void craft() {

    //Sal
    if ((arrayElementos[INDICE_ELEM_SODIO].contador >= 1)
        && (arrayElementos[INDICE_ELEM_CLORO].contador >= 1)) {
        arrayElementos[INDICE_ELEM_SAL].contador += 1;
        arrayElementos[INDICE_ELEM_SODIO].contador -= 1;
        arrayElementos[INDICE_ELEM_CLORO].contador -= 1;
        contador_pontos_todal += 5;
    }


    //AMONIA
    if ((arrayElementos[INDICE_ELEM_HIDROGENIO].contador >= 3)
        && (arrayElementos[INDICE_ELEM_NITORGENIO].contador >= 1)) {
        arrayElementos[INDICE_ELEM_AMONIA].contador += 1;
        arrayElementos[INDICE_ELEM_HIDROGENIO].contador -= 3;
        arrayElementos[INDICE_ELEM_NITORGENIO].contador -= 1;
        contador_pontos_todal += 15;
    }


    //Metano
    if ((arrayElementos[INDICE_ELEM_CARBONO].contador >= 1)
        && (arrayElementos[INDICE_ELEM_HIDROGENIO].contador >= 4)) {
        arrayElementos[INDICE_ELEM_METANO].contador += 1;
        arrayElementos[INDICE_ELEM_CARBONO].contador -= 1;
        arrayElementos[INDICE_ELEM_HIDROGENIO].contador -= 4;
        contador_pontos_todal += 20;
    }


    ///Agua
    if ((arrayElementos[INDICE_ELEM_OXIGENIO].contador >= 1)
        && (arrayElementos[INDICE_ELEM_HIDROGENIO].contador >= 2)) {
        arrayElementos[INDICE_ELEM_AGUA].contador += 1;
        arrayElementos[INDICE_ELEM_OXIGENIO].contador -= 1;
        arrayElementos[INDICE_ELEM_HIDROGENIO].contador -= 2;
        contador_pontos_todal += 10;
    }


}

void desenhar_cena(ALLEGRO_BITMAP* bg, ALLEGRO_BITMAP* sprite, personagem nerdola, elementos arrayElem[]) {
    //Desenha o fundo do jogo, o mapa e o nerdola 

    //Da um fundo preto ao jogo
    al_clear_to_color(al_map_rgb(0, 0, 0));


    //desenha a cena
    al_draw_scaled_bitmap(bg, 0, 0, tam_map_max_x, tam_map_max_y, -camera_x, -camera_y, tam_map_max_x * zoom_map, tam_map_max_y * zoom_map, 0);


    //desenha o personagem
    al_draw_bitmap_region(sprite, nerdola.largura * (int)nerdola.frame, nerdola.current_frame_y, nerdola.largura, nerdola.altura, (nerdola.pos_x - camera_x), (nerdola.pos_y - camera_y), 0);
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            estado = 0;
            contador_pontos_todal = 0;
            countdown = 10;
        }
    }

    //desenha elementos
    for (int indice = 0; indice < SIZE_MAX_ELEMENTOS; indice++) {
        if (arrayElementos[indice].ativo == true) {
            al_draw_bitmap(arrayElementos[INDICE_ELEM_CLORO].bitmap, arrayElementos[indice].pos_x - camera_x, arrayElementos[indice].pos_y - camera_y, 0);
        }

        float pos_text_y = 10;
        for (int indice = 0; indice < SIZE_MAX_ELEMENTOS; indice++) {
            // Se o elemento esta capturado
            snprintf(infoAdicional, sizeof(infoAdicional), " %d", arrayElementos[indice].contador);
            snprintf(textoCompleto, sizeof(textoCompleto), "%s%s", arrayElementos[indice].nome, infoAdicional);
            al_draw_textf(font, al_map_rgb(255, 0, 0), 10, pos_text_y + 10, 0, textoCompleto);
            pos_text_y = pos_text_y + 20;
        }
    }
}

void destruir_elementos() {
    for (int indice = 0; indice < SIZE_MAX_ELEMENTOS; indice++) {
        al_destroy_bitmap(arrayElementos[indice].bitmap);
    }
}



//**************************************
// Principal
//**************************************
int main() {

    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(tamanho_map_x, tamanho_map_y);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Reino da Alquimia");

    font_menu = al_load_font("./lustra-text-black.ttf", 15, 0);
    timer_menu = al_create_timer(1.0 / 30.0);
    temporizador = al_create_timer(1.0); // 1 segundo
    blink_timer_menu = al_create_timer(1.0);
    bg_menu = al_load_bitmap("./fundo.png");
    tabela_menu = al_load_bitmap("./tabela.jpg");
    titulo_menu = al_load_bitmap("./letras.png");

    font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 20.0);

    ALLEGRO_BITMAP* sprite = al_load_bitmap("./nerdola.png");
    ALLEGRO_BITMAP* bg = al_load_bitmap("./cenasss.png");


    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer_menu));
    al_register_event_source(event_queue, al_get_timer_event_source(blink_timer_menu));
    al_register_event_source(event_queue, al_get_timer_event_source(temporizador));
    al_start_timer(timer);

    al_start_timer(timer_menu);
    al_start_timer(blink_timer_menu);
    al_start_timer(temporizador);

    //Zera o contador para zera, para outro player jogar
    contador_pontos_todal = 0;

    //Inicia Personagem

    iniciar_personagem();

    //Inicia Elementos

    iniciar_elementos();


    //Numeros Aleatorios a todos
    num_rand();



    //Loop
    while (true) {
        if (estado == 0) {
            atualizar_menu();

        }
        else {

            //Atualiza camera
            atualizar_camera();

            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);

            if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == temporizador) {
                if (countdown > 0) {
                    countdown--;
                }
                else {
                    //running = false;
                    estado = 0;    
                    countdown = 10;
                }
            }


            //Se clicar no C irá fazer a função craft
            if (event.keyboard.keycode == ALLEGRO_KEY_C) {
                craft();
            }


            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }

            else if ((event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_D) && (nerdola.pos_x + tamanho_nerd_x < tam_map_max_x)) {
                //Right
                nerdola.current_frame_y = nerdola.altura;
                nerdola.pos_x += 10;
                nerdola.frame += 0.3f;
                if (nerdola.frame > 3) {
                    nerdola.frame -= 3;
                }
                nerdola.time = 0;
            }
            else if ((event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_A) && (nerdola.pos_x - 1 > 2)) {
                //Left
                nerdola.current_frame_y = nerdola.altura * 3;
                nerdola.pos_x -= 10;
                nerdola.frame += 0.3f;
                if (nerdola.frame > 3) {
                    nerdola.frame -= 3;
                }
                nerdola.time = 0;
            }
            else if ((event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) && (nerdola.pos_y + tamanho_nerd_y < tam_map_max_y)) {
                //Down
                nerdola.current_frame_y = 0;
                nerdola.pos_y += 10;
                nerdola.frame += 0.3f;
                if (nerdola.frame > 3) {
                    nerdola.frame -= 3;
                }
                nerdola.time = 0;
            }
            else if ((event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W) && (nerdola.pos_y - 1 > 2)) {
                //Up
                nerdola.current_frame_y = nerdola.altura * 2;
                nerdola.pos_y -= 10;
                nerdola.frame += 0.3f;
                if (nerdola.frame > 3) {
                    nerdola.frame -= 3;
                }
                nerdola.time = 0;
            }
            else {
                nerdola.time++;
            }


            if (nerdola.time > 12) {
                nerdola.current_frame_y = nerdola.altura * 4;
                nerdola.frame += 0.1f;
                if (nerdola.frame > 2) {
                    nerdola.frame -= 2;
                }
            };


            //desenhar_cena(bg, sprite, nerdola);
            desenhar_cena(bg, sprite, nerdola, arrayElementos);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Tempo restante: %d segundos", countdown);

            //Verificar a captura
            capturação();
        }
        al_flip_display();
    }

    // Destroi elementos
    destruir_elementos();

    al_destroy_bitmap(sprite);
    al_destroy_bitmap(tabela_menu);
    al_destroy_bitmap(titulo_menu);
    al_destroy_bitmap(bg);
    al_destroy_bitmap(bg_menu);
    al_destroy_font(font);
    al_destroy_font(font_menu);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_timer(temporizador);

    return 0;
}