#pragma once
typedef struct{
	int id;
	unsigned char nome[50];
	ALLEGRO_BITMAP* bitmap;
	int pos_x;
	int pos_y;
	int largura;
	int altura;
	bool ativo;
	bool capturado;
	int contador;			
}elementos;