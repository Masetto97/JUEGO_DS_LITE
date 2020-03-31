/*
-------------------------------------------------
	Includes
-------------------------------------------------
*/

// Includes C
#include <stdio.h>

// Includes propietarios NDS
#include <nds.h>

// Includes librerias propias
#include <nf_lib.h>





/*
-------------------------------------------------
	Main() - Bloque general del programa
-------------------------------------------------
*/


int main(int argc, char **argv){
	// Pantalla de espera inicializando NitroFS
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);
	consoleDemoInit();
	iprintf("\n NitroFS init. Please wait.\n\n");
	iprintf(" Iniciando NitroFS,\n por favor, espere.\n\n");
	swiWaitForVBlank();

	// Define el ROOT e inicializa el sistema de archivos
	NF_SetRootFolder("NITROFS");	// Define la carpeta ROOT para usar NITROFS

	// Inicializa el motor 2D
	NF_Set2D(0, 0);				// Modo 2D_0 en ambas pantallas
	NF_Set2D(1, 0);

	// Inicializa los fondos tileados
	NF_InitTiledBgBuffers();	// Inicializa los buffers para almacenar fondos
	NF_InitTiledBgSys(0);		// Inicializa los fondos Tileados para la pantalla superior
	NF_InitTiledBgSys(1);		// Iniciliaza los fondos Tileados para la pantalla inferior

	// Inicializa los Sprites
	NF_InitSpriteBuffers();		// Inicializa los buffers para almacenar sprites y paletas
	NF_InitSpriteSys(0);		// Inicializa los sprites para la pantalla superior
	NF_InitSpriteSys(1);		// Inicializa los sprites para la pantalla inferior

	// Inicializa el motor de texto
	NF_InitTextSys(0);			// Inicializa el texto para la pantalla superior

	// Inicializa los buffers de mapas de colisiones
	NF_InitCmapBuffers();

	// Carga los archivos de fondo desde la FAT / NitroFS a la RAM
	NF_LoadTiledBg("bg/este", "este", 256, 256);		// Carga el fondo para la capa 3, pantalla inferior
	NF_LoadTiledBg("bg/colmap", "boxes", 256, 1024);	// Carga el fondo para la capa 2, pantalla inferior

	// Carga los archivos de sprites desde la FAT / NitroFS a la RAM
	NF_LoadSpriteGfx("sprite/fantasmamorado", 0,16,16);
	NF_LoadSpritePal("sprite/fantasmamorado", 0);

	//Tipos de fantasmas disponibles:
	//fantasma
	//fantasmamorado
	//fantasmaverde
	//fantasmaazul

	// Carga la fuente por defecto para el texto
	NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);

	// Carga el mapa de colisiones
	NF_LoadColisionMap("maps/cmap", 0, 256, 1032);

	// Crea los fondos de la pantalla superior
	NF_CreateTiledBg(0, 3, "este");
	// Crea los fondos de la pantalla inferior
	NF_CreateTiledBg(1, 2, "boxes");

	// Crea una capa de texto
	NF_CreateTextLayer(0, 2, 0,"normal");

	// Transfiere a la VRAM los sprites necesarios
	NF_VramSpriteGfx(1, 0, 0, true);	// Puntero
	NF_VramSpritePal(1, 0, 0);

	// Crea el Sprite del puntero en la pantalla inferior
	NF_CreateSprite(1, 0, 0, 0, 124, 92);	// Crea el puntero en la pantalla inferior
	NF_SpriteLayer(1, 0, 2);				// Y la capa sobre la que se dibujara

        //Variables para contar
	u8 b = 0;
        u8 n = 0;
	// Variable para la lectura del keypad
	u16 keys = 0;

	// Variables para el control de movimiento
	s16 x = 128;
	s16 y = 1010;
	s16 spr_x = 0;
	s16 spr_y = 0;
	s16 bg_x = 0;
	s16 bg_y = 0;


        // Control de movimiento
        //variables para saber si se mueve a la izq,der o cae
	bool down = false;
	bool left = false;
	bool right = false;
        bool blanco=false;
	// Buffer de texto
	char mytext[32];



	// Bucle (repite para siempre)
	while(1) {

                //Si se preciona arriba y no esta cayendo se resta la y(sube) y se suma b que es la cantidad que va a subir,
		//luego si esta arriba presionado puede moverse a la izq o a la der, si se mueve a un lado no puede moverse al otro
		//lo mismo con la n se suma para saber el movimiento izq o der

		// Lee el teclado
		scanKeys();
		keys = keysHeld();
		if (keys & KEY_UP ){ if(down==false){b++;y--;
                if (keys & KEY_LEFT) if(right==false){left=true;x--;n++;}
		if (keys & KEY_RIGHT)if(left==false){right=true;x++;n++;}
                  }}

                if(n>50){n=0;right=false;left=false;}
                if(b>70){down=true;b=0;}
                if(down){y++;}

	
		// Limites del movimiento
		if (x < 0) x = 0;
		if (x > 255) x = 255;
		if (y < 0) y = 0;
		if (y > 1023) y = 1023;

		// Posicion del fondo
		bg_x = (x - 255);
		if (bg_x < 0) bg_x = 0;
		if (bg_x > 256) bg_x = 256;
		bg_y = (y - 96);
		if (bg_y < 0) bg_y = 0;
		if (bg_y > 1023) bg_y = 1023;

		// Posicion del Sprite
		spr_x = x -bg_x;
		spr_y = y-bg_y;
		NF_MoveSprite(1, 0, spr_x, spr_y);

		// Imprime la posicion del cursor
		sprintf(mytext,"x:%d  y:%d ", x, y);
		NF_WriteText(0, 2, 1, 1, mytext);

		// Imprime el n� de tile
		switch (NF_GetTile(0, x, y)) {
			case 0:
                                blanco=true;
				sprintf(mytext,"Tile: Vacio");
				break;
			case 1:
                                if(down&&blanco){
                                down=false;left=false;right=false;}
				sprintf(mytext,"Tile: Amarillo");				
				break;
			case 2:
				if(down&&blanco){
                                down=false;left=false;right=false;
                           	}
				else{blanco=false;
				}
				sprintf(mytext,"Tile: Rojo");
				break;
			case 3:
                           	if(y<3){
				return 0;}
				sprintf(mytext,"HAS GANADO!!!");
				break;
		}
		NF_WriteText(0, 2, 1, 3, mytext);

		NF_UpdateTextLayers();		// Actualiza las capas de texto
		NF_SpriteOamSet(1);		// Actualiza el Array del OAM

		swiWaitForVBlank();		// Espera al sincronismo vertical

		oamUpdate(&oamSub);		// Actualiza a VRAM el OAM Secundario

		NF_ScrollBg(1, 2, bg_x, bg_y);	// Actualiza el scroll
	}
	return 0;	
}

