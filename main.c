#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

#define S_WIDTH 900
#define S_HEIGHT 720


typedef struct Scolor{
	int r;
	int g;
	int b;
}SColor;

typedef struct Scoord{
	int r;
	int g;
	int b;
}SCoord[S_WIDTH][S_HEIGHT];

SColor randColor();
int fillScreen(SDL_Renderer * aRenderer, SCoord  coordinates);
int averageGrid(SDL_Renderer * aRenderer, SCoord coordinates, SCoord backup);
	
int main(int argc, char **argv){
	
	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n",
			SDL_GetError());
		return -1;
	}

	{
		/* Création de la fenêtre */
		SDL_Window *pWindow = NULL;
		pWindow =
		    SDL_CreateWindow("Ma première application SDL2",
				     SDL_WINDOWPOS_UNDEFINED,
				     SDL_WINDOWPOS_UNDEFINED, S_WIDTH, S_HEIGHT,
				     SDL_WINDOW_SHOWN);

		if (pWindow) {
			SDL_Renderer * renderer = SDL_CreateRenderer( pWindow, -1, 0);
			SDL_Texture * texture = SDL_CreateTexture(renderer,
					        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, S_WIDTH, S_HEIGHT);	//Create texture for pixel drawing
			SCoord coordinates;	
			SCoord * backup = malloc(sizeof(SCoord));

			srand(time(NULL));
			
			randColor();				
			
			fillScreen(renderer, coordinates);			
				
			SDL_Delay(3000);
			
			int k = 0;
			
			for( k; k< 420; k++){
				averageGrid(renderer, coordinates, *backup);
				//SDL_Delay(1000);
			}
			IMG_SavePNG(SDL_GetWindowSurface(pWindow), "out1.png");
			
			SDL_Delay(10000);

			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(pWindow);
		} else {
			fprintf(stderr,
				"Erreur de création de la fenêtre: %s\n",
				SDL_GetError());
		}
	}

	SDL_Quit();

	return 0;
}

int fillScreen(SDL_Renderer * aRenderer, SCoord  coordinates){
	int x;
        int y;
	SColor toPrint;
	for(x = 0; x<S_WIDTH; x++){
		for(y = 0; y < S_HEIGHT; y++){
			toPrint = randColor();

			coordinates[x][y].r = toPrint.r;	
			coordinates[x][y].g = toPrint.g;
			coordinates[x][y].b = toPrint.b;

			SDL_SetRenderDrawColor(aRenderer, coordinates[x][y].r, coordinates[x][y].g, coordinates[x][y].b, 255);	
			SDL_RenderDrawPoint( aRenderer, x, y);
		}	
	}	
	SDL_RenderPresent(aRenderer);

}

SColor randColor( ){
	SColor color;
	color.r = rand()%255;
	color.g = rand()%255;
	color.b = rand()%255;
	
	return color;
}


int averageGrid(SDL_Renderer * aRenderer, SCoord coordinates, SCoord backup){
	printf("ENTERS AVGRID()\n");
	
	int x;
	int y;
	for(x=0; x <S_WIDTH; x ++){
		for(y = 0; y < S_HEIGHT; y++){
			backup[x][y].r = coordinates[x][y].r;
			backup[x][y].g = coordinates[x][y].g;
			backup[x][y].b = coordinates[x][y].b;
		}
	}
	for(x=0; x <S_WIDTH; x ++){
		for(y = 0; y < S_HEIGHT; y++){
			coordinates[x][y].r  = (backup[(x + S_WIDTH - 1)%S_WIDTH][y].r 
						+ backup[(x + S_WIDTH + 1)%S_WIDTH][y].r
						+ backup[x][y].r
						+ backup[x][(y + S_HEIGHT - 1)%S_HEIGHT].r
						+ backup[x][(y + S_HEIGHT + 1)%S_HEIGHT].r)
						/5;
			coordinates[x][y].g  = (backup[(x + S_WIDTH - 1)%S_WIDTH][y].g 
						+ backup[(x + S_WIDTH + 1)%S_WIDTH][y].g
						+ backup[x][y].g
						+ backup[x][(y + S_HEIGHT - 1)%S_HEIGHT].g
						+ backup[x][(y + S_HEIGHT + 1)%S_HEIGHT].g)
						/5;
			coordinates[x][y].b  = (backup[(x + S_WIDTH - 1)%S_WIDTH][y].b 
						+ backup[(x + S_WIDTH + 1)%S_WIDTH][y].b
						+ backup[x][y].b
						+ backup[x][(y + S_HEIGHT - 1)%S_HEIGHT].b
						+ backup[x][(y + S_HEIGHT + 1)%S_HEIGHT].b)
						/5;
			
			SDL_SetRenderDrawColor(aRenderer, coordinates[x][y].r, coordinates[x][y].g, coordinates[x][y].b, 255);	
			SDL_RenderDrawPoint( aRenderer, x, y);
		}
	}	
	SDL_RenderPresent(aRenderer);


}






