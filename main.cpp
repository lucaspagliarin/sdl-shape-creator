#define SDL_MAIN_HANDLED
#include<SDL2/SDL.h>
#include<stdio.h>
#include <unistd.h>

#include <list>

#include<Context.h>
#include<Color.h>
#include<Point.h>
#include<Painter.h>
#include<Line.h>
#include<Rectangle.h>
#include<Polygon.h>
#include<Circle.h>
#include<Bezier.h>
#include<Shape.h>

using namespace std;



// SDL stuff
SDL_Window* pWindow = nullptr;
SDL_Renderer* pRenderer = nullptr;
SDL_Surface * window_surface = nullptr;

int offset = 10;

list<unique_ptr<Shape>> shapes;

void preLoadShapes() {
    Color color = Color(255,0,0);

    Point p1 = Point(600,400);
    Point p2 = Point(10,10);

    shapes.push_back(make_unique<Line>(p1, p2, color, 0));

    Point p3 = Point(600,430);
    Point p4 = Point(10,40);

    shapes.push_back(make_unique<Line>(p3, p4, color, 1));

    Point rec_min = Point(600,430);
    Point rec_max = Point(10,40);

    shapes.push_back(make_unique<Rectangle>(p3, p4, color));

    Point circle_center = Point(420,420);
    int circle_radius = 250;


    shapes.push_back(make_unique<Circle>(circle_center, circle_radius, color));

    list<Point> polygon_points;

    polygon_points.push_back(Point(420,80));
    polygon_points.push_back(Point(540, 350));
    polygon_points.push_back(Point(300, 160));
    polygon_points.push_back(Point(520, 160));
    polygon_points.push_back(Point(340, 340));

    shapes.push_back(make_unique<Polygon>(polygon_points, Color(20,140,255))); 

    shapes.push_back(make_unique<Bezier>(Point(420,80), Point(540, 350), Point(300, 160), Point(340, 340), Color(20,140,140)));
}


void display()
{

    Painter p = Painter();

    // p.drawCircle(Point(150,150), 50, color);

    for (const auto& shape : shapes) {
        shape->draw(); // O operador -> acessa o comportamento polimórfico correto!
    }
}

void clear() {

    Painter p = Painter();
    SDL_Surface * window_surface = Context::getInstance()->getWindowSurface();
    for(int x = 0;x < window_surface->w; x++){
        for(int y = 0;y < window_surface->h; y++) {
            p.setPixel(x, y, 255, 255, 255);
        }
    }

}

// void clear() {
//     // Pega o renderer direto do seu Context
//     SDL_Renderer* renderer = Context::getInstance()->getRenderer(); 
    
//     // Define a cor de fundo (neste caso, Branco: 255, 255, 255, 255)
//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
//     // Limpa a tela inteira com a cor definida acima
//     SDL_RenderClear(renderer);
// }

// Driver code
int main(int argc, char* args[])
{

	SDL_Event event;

    preLoadShapes();

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		// if succeeded create our window
		pWindow = SDL_CreateWindow("SDL_Shape_Creator",
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
														1280, 960,
												SDL_WINDOW_RESIZABLE);

		// if the window creation succeeded create our renderer
		if (pWindow != 0) {
			window_surface = SDL_GetWindowSurface(pWindow);

            // Inicializa o contexto gráfico da aplicação
            Context::getInstance()->setWindowSurface(window_surface);
        }

	}
	else
		return 1; // sdl could not initialize

	while (1)
	{

        // Limpa a tela
        clear();

        // Realiza o desenho
        display();

        // Aguarda instantes
        usleep(100000);

		// Verifica se foi mandado fechar a janela
		while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    window_surface = SDL_GetWindowSurface(pWindow);
                    Context::getInstance()->setWindowSurface(window_surface);
                }
            }

        }

		// Atualiza a tela
		SDL_UpdateWindowSurface(pWindow);
	}

	// clean up SDL
	SDL_Quit();
	return 0;
}
