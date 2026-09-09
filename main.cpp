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
#include "Utils.h"

using namespace std;



// SDL stuff
SDL_Window* pWindow = nullptr;
SDL_Renderer* pRenderer = nullptr;
SDL_Surface * window_surface = nullptr;

list<unique_ptr<Shape>> shapes;

Color windowColor = Color(255,255,255);

enum ToolType { 
    TOOL_LINE, 
    TOOL_RECTANGLE, 
    TOOL_CIRCLE, 
    TOOL_POLYGON, 
    TOOL_BEZIER, 
    TOOL_FILL, 
    TOOL_SELECT,
    TOOL_COLOR, 
};
const int TOOL_COUNT = 8;

ToolType previousTool = TOOL_SELECT;
ToolType currentTool = TOOL_SELECT;
Color currentColor = Color(0, 0, 0);

vector<Point> pendingPoints;

int mouseX = 0, mouseY = 0;


// Selector

Shape* selectedShape = nullptr;
bool draggingShape = false;
int dragLastX = 0, dragLastY = 0;

void deselectAll() {
    for (auto& s : shapes) s->setSelected(false);
    selectedShape = nullptr;
}

bool trySelectShapeAt(int x, int y) {
    Point clicked(x, y);
    for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
        if ((*it)->contains(clicked, 5)) {
            deselectAll();
            (*it)->setSelected(true);
            selectedShape = it->get();
            return true;
        }
    }
    deselectAll();
    return false;
}

void deleteSelectedShape() {
    if (!selectedShape) return;
    for (auto it = shapes.begin(); it != shapes.end(); ++it) {
        if (it->get() == selectedShape) {
            shapes.erase(it);
            break;
        }
    }
    selectedShape = nullptr;
    draggingShape = false;
}

// Configuraçoes da ToolBox 

const int TOOLBOX_HEIGHT = 50;
const int TOOLBOX_BOX_SIZE = 40;
const int TOOLBOX_GAP = 8;
const int TOOLBOX_MARGIN = 10;
const int TOOLBOX_PADDING = 8;

struct ToolBoxEntry {
    ToolType tool;
    string name;
    Color color;
    char shortcut;
};

ToolBoxEntry toolboxEntries[TOOL_COUNT] = {
    { TOOL_LINE,        "Line",     Color(120, 120, 120), 'L' },
    { TOOL_RECTANGLE,   "Rectangle",Color(60, 120, 220),  'R' },
    { TOOL_CIRCLE,      "Circle",   Color(60, 180, 90),   'C' },
    { TOOL_POLYGON,     "Polygon",  Color(230, 140, 40),  'P' },
    { TOOL_BEZIER,      "Bezier",   Color(160, 70, 200),  'B' },
    { TOOL_FILL,        "Fill",     Color(230, 210, 40),  'F' },
    { TOOL_SELECT,      "Select",   Color(40, 40, 40),    'S' },
    { TOOL_COLOR,       "Color Picker", currentColor ,'I' },
};

struct ColorPicker {
    string name;
    Color color;
    char shortcut;
};

const int COLOR_COUNT = 5;

ColorPicker colorPickerOptions[COLOR_COUNT] = {
    { "Black", Color(20, 20, 20), '1' },   
    { "Red", Color(220, 30, 30), '2' },        
    { "Green", Color(30, 140, 30), '3' },       
    { "Blue", Color(30, 60, 220), '4' },        
    { "White", Color(255, 255, 255), '5' },        
};

// devolve os limites (x1,y1,x2,y2) da i-ésima caixa da toolbox
void getToolBoxRect(int i, int &x1, int &y1, int &x2, int &y2) {
    x1 = TOOLBOX_MARGIN + i * (TOOLBOX_BOX_SIZE + TOOLBOX_GAP);
    y1 = TOOLBOX_MARGIN;
    x2 = x1 + TOOLBOX_BOX_SIZE;
    y2 = y1 + TOOLBOX_BOX_SIZE;
}

// devolve os limites (x1,y1,x2,y2) da i-ésima caixa da toolbox
void getToolBoxColorPickerRect(int &x1, int &y1, int &x2, int &y2) {
    SDL_Surface * window_surface = Context::getInstance()->getWindowSurface();
    x1 = window_surface->w - TOOLBOX_BOX_SIZE - TOOLBOX_MARGIN;
    y1 = TOOLBOX_MARGIN;
    x2 = x1 + TOOLBOX_BOX_SIZE;
    y2 = y1 + TOOLBOX_BOX_SIZE;
}

// devolve os limites (x1,y1,x2,y2) da i-ésima caixa da toolbox
void getToolBoxColorsRect(int i, int &x1, int &y1, int &x2, int &y2) {
    SDL_Surface * window_surface = Context::getInstance()->getWindowSurface();
    x1 = window_surface->w - (2 * TOOLBOX_BOX_SIZE) - TOOLBOX_MARGIN - TOOLBOX_GAP;
    y1 = TOOLBOX_MARGIN + i * (TOOLBOX_BOX_SIZE + TOOLBOX_GAP);
    x2 = x1 + TOOLBOX_BOX_SIZE;
    y2 = y1 + TOOLBOX_BOX_SIZE;
}

// -1 se o clique não caiu em nenhuma caixa
int hitTestToolBox(int x, int y) {
    if (y > TOOLBOX_HEIGHT) return -1;
    for (int i = 0; i < TOOL_COUNT; i++) {
        int x1, y1, x2, y2;
        if (toolboxEntries[i].tool == TOOL_COLOR) {
            getToolBoxColorPickerRect(x1,y1,x2,y2);
        } else {
            getToolBoxRect(i, x1, y1, x2, y2);
        }
        if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return i;
    }
    return -1;
}

int hitTestColorsToolBox(int x, int y) {
    for (int i = 0; i < TOOL_COUNT; i++) {
        int x1, y1, x2, y2;
        getToolBoxColorsRect(i, x1, y1, x2, y2);
        if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return i;
    }
    return -1;
}

void drawToolBox() {
    Painter p = Painter();
    for (int i = 0; i < TOOL_COUNT; i++) {
        int x1, y1, x2, y2;
        if (toolboxEntries[i].tool == TOOL_COLOR) {
            getToolBoxColorPickerRect(x1,y1,x2,y2);
            p.fillRect(x1, y1, x2, y2, currentColor);
            Context::getInstance()->drawChar(toolboxEntries[i].shortcut, x1 + TOOLBOX_PADDING, y1 + TOOLBOX_PADDING, 1);
        } else {
            getToolBoxRect(i, x1, y1, x2, y2);
            p.fillRect(x1, y1, x2, y2, toolboxEntries[i].color);
            Context::getInstance()->drawChar(toolboxEntries[i].shortcut, x1 + TOOLBOX_PADDING, y1 + TOOLBOX_PADDING, 1);
        }

        // contorno: vermelho e mais grosso se for a ferramenta ativa
        Color border = (toolboxEntries[i].tool == currentTool) ? Color(255, 0, 0) : Color(0, 0, 0);
        p.drawRectangle(x1, y1, x2, y2, border);
        if (toolboxEntries[i].tool == currentTool) {
            p.drawRectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, border);
        }
    }
}

void drawColors() {
    Painter p = Painter();
    for (int i = 0; i < COLOR_COUNT; i++) {
        int x1, y1, x2, y2;
        
        getToolBoxColorsRect(i, x1, y1, x2, y2);
        p.fillRect(x1, y1, x2, y2, colorPickerOptions[i].color);
        if(Color::compareColors(colorPickerOptions[i].color, Color(255,255,255))){
            Context::getInstance()->drawChar(colorPickerOptions[i].shortcut, x1 + TOOLBOX_PADDING, y1 + TOOLBOX_PADDING, 1, Color(20,20,20));
        } else {
            Context::getInstance()->drawChar(colorPickerOptions[i].shortcut, x1 + TOOLBOX_PADDING, y1 + TOOLBOX_PADDING, 1);
        }
        
        

        // contorno: vermelho e mais grosso se for a ferramenta ativa
        Color border = (Color::compareColors(colorPickerOptions[i].color, currentColor) ? Color(255, 0, 0) : Color(0, 0, 0));
        p.drawRectangle(x1, y1, x2, y2, border);
        if (Color::compareColors(colorPickerOptions[i].color, currentColor)) {
            p.drawRectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, border);
        }
    }
}

// ============================================================
// Cancela a forma que está sendo construída (ESC)
// ============================================================
void cancelPendingShape() {
    pendingPoints.clear();
}

bool showToolText = false;

void drawToolInfo(string name){
    const int TEXT_BOX_WIDTH = 210;
    const int TEXT_BOX_HEIGHT = 50;
    const int TEXT_BOX_PADDING = 20;

    Painter p = Painter();
    SDL_Surface * window_surface = Context::getInstance()->getWindowSurface();

    int x1 = window_surface->w/2 - TEXT_BOX_WIDTH/2;
    int x2 = window_surface->w/2 + TEXT_BOX_WIDTH/2;
    int y1 = window_surface->h/2 - TEXT_BOX_HEIGHT/2;
    int y2 = window_surface->h/2 + TEXT_BOX_HEIGHT/2;
    
    p.fillRect(x1, y1, x2, y2, Color(0,0,0, 150));
    Context::getInstance()->drawString(name, x1 + TEXT_BOX_PADDING, y1 + TEXT_BOX_PADDING, 1);
        
}

// ============================================================
// Seleciona ferramenta (por clique na toolbox ou por tecla)
// ============================================================
void selectTool(ToolType tool) {
    previousTool = currentTool;
    currentTool = tool;
    cancelPendingShape();
    draggingShape = false;

    showToolText = true;

    const char* names[] = { "Reta", "Retangulo", "Circulo", "Poligono", "Bezier", "Preenchimento", "Selecionar", "Cor" };
    drawToolInfo(names[tool]);
}

void selectColor(Color color) {
    currentColor = color;
}

// ============================================================
// Finaliza a forma corrente e adiciona na lista de shapes
// ============================================================
void finalizeShape() {
    switch (currentTool) {

        case TOOL_LINE:
            if (pendingPoints.size() == 2) {
                shapes.push_back(make_unique<Line>(pendingPoints[0], pendingPoints[1], currentColor, 0));
                pendingPoints.clear();
            }
            break;

        case TOOL_RECTANGLE:
            if (pendingPoints.size() == 2) {
                shapes.push_back(make_unique<Rectangle>(pendingPoints[0], pendingPoints[1], currentColor));
                pendingPoints.clear();
            }
            break;

        case TOOL_CIRCLE:
            if (pendingPoints.size() == 2) {
                int radius = (int) Utils::getInstance()->distance(pendingPoints[0], pendingPoints[1]);
                shapes.push_back(make_unique<Circle>(pendingPoints[0], radius, currentColor));
                pendingPoints.clear();
            }
            break;

        case TOOL_BEZIER:
            // ordem esperada pelo construtor: start, end, anchor1, anchor2
            if (pendingPoints.size() == 4) {
                shapes.push_back(make_unique<Bezier>(
                    pendingPoints[0], pendingPoints[1],
                    pendingPoints[2], pendingPoints[3],
                    currentColor));
                pendingPoints.clear();
            }
            break;

        case TOOL_POLYGON:
            // fechado explicitamente pelo botao direito do mouse
            if (pendingPoints.size() >= 3) {
                list<Point> poly(pendingPoints.begin(), pendingPoints.end());
                shapes.push_back(make_unique<Polygon>(poly, currentColor));
            }
            pendingPoints.clear();
            break;

        default:
            break;
    }
}


// ============================================================
// Trata clique do mouse fora da toolbox: acumula pontos e,
// quando a forma tiver pontos suficientes, finaliza.
// ============================================================
void handleCanvasClick(int x, int y, Uint8 button) {
    Point clicked(x, y);

    if (currentTool == TOOL_SELECT) {
        if (button != SDL_BUTTON_LEFT) return;
        if (trySelectShapeAt(x, y)) {
            draggingShape = true;
            dragLastX = x;
            dragLastY = y;
        }
        return;
    }

    if (currentTool == TOOL_FILL) {
        if (button != SDL_BUTTON_LEFT) return;
        if (trySelectShapeAt(x, y)) {
            selectedShape->setFill(currentColor);
        } else {
            windowColor = currentColor;
        }
        return;
    }

    if (currentTool == TOOL_POLYGON) {
        if (button == SDL_BUTTON_RIGHT) {
            finalizeShape(); // fecha o poligono com os pontos acumulados
        } else if (button == SDL_BUTTON_LEFT) {
            pendingPoints.push_back(clicked);
        }
        return;
    }

    // demais ferramentas: so o botao esquerdo acumula pontos
    if (button != SDL_BUTTON_LEFT) return;

    pendingPoints.push_back(clicked);

    int needed = 2;
    if (currentTool == TOOL_BEZIER) needed = 4;

    if ((int) pendingPoints.size() >= needed) {
        finalizeShape();
    }
}

void drawPreview() {
    if (pendingPoints.empty()) return;

    Painter p = Painter();
    Point mouse(mouseX, mouseY);

    // marca os pontos ja confirmados
    for (Point& pt : pendingPoints) {
        p.drawRectangle(pt.getX() - 2, pt.getY() - 2, pt.getX() + 2, pt.getY() + 2, Color(255, 0, 0));
    }

    switch (currentTool) {
        case TOOL_LINE:
            p.drawLine(pendingPoints.back(), mouse, Color(255, 0, 0), 0);
            break;
        case TOOL_RECTANGLE:
            p.drawRectangle(pendingPoints.back().getX(), pendingPoints.back().getY(), mouseX, mouseY, Color(255, 0, 0));
            break;
        case TOOL_BEZIER:
            if (pendingPoints.size() == 3) {
                p.drawBezier(pendingPoints[0], pendingPoints[1], pendingPoints[2], pendingPoints[1], Color(255, 0, 0));
            } else if (pendingPoints.size() == 2) { 
                p.drawBezier(pendingPoints[0], pendingPoints[1], pendingPoints[0], pendingPoints[1], Color(255, 0, 0));
            } else if (pendingPoints.size() == 1) {
                p.drawCircle(pendingPoints[0], 3, Color(255,0,0));
            }
            break;

        case TOOL_CIRCLE: {
            int radius = (int) Utils::distance(pendingPoints[0], mouse);
            p.drawCircle(pendingPoints[0], radius, Color(255, 0, 0));
            break;
        }

        case TOOL_POLYGON: {
            Point anterior = pendingPoints.front();
            for (size_t i = 1; i < pendingPoints.size(); i++) {
                p.drawLine(anterior, pendingPoints[i], Color(255, 0, 0), 0);
                anterior = pendingPoints[i];
            }
            p.drawLine(anterior, mouse, Color(255, 0, 0), 1);
            break;
        }

        default:
            break;
    }
}



void preLoadShapes() {
    Color color = Color(150,244,0);
    Color color2 = Color(180,130,0);

    Point p3 = Point(600,430);
    Point p4 = Point(10,40);

    // shapes.push_back(make_unique<Line>(p3, p4, color, 1));

    // Point rec_min = Point(600,430);
    // Point rec_max = Point(10,40);


    shapes.push_back(make_unique<Rectangle>(p3, p4, color));
    Shape& lastRect = *(shapes.back());
    lastRect.setFill(color);

    // Point p1 = Point(600,400);
    // Point p2 = Point(10,10);

    // shapes.push_back(make_unique<Line>(p1, p2, color2, 0));

    // Point circle_center = Point(420,420);
    // int circle_radius = 250;


    // shapes.push_back(make_unique<Circle>(circle_center, circle_radius, color));

    list<Point> polygon_points;

    polygon_points.push_back(Point(820,80));
    polygon_points.push_back(Point(1140, 350));
    polygon_points.push_back(Point(900, 160));
    polygon_points.push_back(Point(1120, 160));
    polygon_points.push_back(Point(1240, 340));

    shapes.push_back(make_unique<Polygon>(polygon_points, Color(20,140,255))); 

    Shape& lastPoly = *(shapes.back());
    lastPoly.setFill(Color(30,140,255));

    // shapes.push_back(make_unique<Bezier>(Point(420,80), Point(540, 350), Point(300, 160), Point(340, 340), Color(20,140,140)));
}


void display()
{
    Painter p = Painter();

    for (const auto& shape : shapes) {
        shape->draw();
    }

    drawPreview();
    drawToolBox();
    if (currentTool == TOOL_COLOR) {
        drawColors();
    } 

    if (showToolText) {

        drawToolInfo(toolboxEntries[currentTool].name);

    }
}

void clear() {

    Painter p = Painter();
    SDL_Surface * window_surface = Context::getInstance()->getWindowSurface();
    for(int x = 0;x < window_surface->w; x++){
        for(int y = 0;y < window_surface->h; y++) {
            p.setPixel(x, y, windowColor.getR(), windowColor.getG(), windowColor.getB());
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

    // preLoadShapes();

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

        bool fontLoaded = Context::getInstance()->loadBitmapFont("../assets/Minecraft_bitmap_font_SDL2.bmp", 16, 16, 16);
    
        if (!fontLoaded) {
            printf("Font not loaded: %s\n", SDL_GetError());
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
        usleep(16000);

        SDL_GetMouseState(&mouseX, &mouseY);

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

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;
                
                if(currentTool == TOOL_COLOR){
                    int colorIndex = hitTestColorsToolBox(x, y);
                    if (colorIndex >= 0) {
                        selectColor(colorPickerOptions[colorIndex].color);
                        showToolText = false;
                    } else {
                        selectTool(previousTool);
                        showToolText = true;
                    }

                } else {
                    int toolIndex = hitTestToolBox(x, y);
                    if (toolIndex >= 0) {
                        selectTool(toolboxEntries[toolIndex].tool);
                        showToolText = true;
                    } else {
                        handleCanvasClick(x, y, event.button.button);
                        showToolText = false;
                    }
                }
                
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                if (draggingShape && selectedShape != nullptr && (event.motion.state & SDL_BUTTON_LMASK))
                {
                    int dx = event.motion.x - dragLastX;
                    int dy = event.motion.y - dragLastY;
                    selectedShape->translate(dx, dy);
                    dragLastX = event.motion.x;
                    dragLastY = event.motion.y;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    draggingShape = false;
                }
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym) {
                    case SDLK_l: selectTool(TOOL_LINE); break;
                    case SDLK_r: selectTool(TOOL_RECTANGLE); break;
                    case SDLK_c: selectTool(TOOL_CIRCLE); break;
                    case SDLK_p: selectTool(TOOL_POLYGON); break;
                    case SDLK_b: selectTool(TOOL_BEZIER); break;
                    case SDLK_f: selectTool(TOOL_FILL); break;
                    case SDLK_i: selectTool(TOOL_COLOR); break;
                    case SDLK_s:
                        if (event.key.keysym.mod & KMOD_CTRL) {
                            Utils::saveFile();
                        } else {
                            selectTool(TOOL_SELECT);
                        }
                        break;
                    case SDLK_DELETE:
                    case SDLK_BACKSPACE:
                        deleteSelectedShape();
                        break;
                    case SDLK_ESCAPE: cancelPendingShape(); break;
                    // paleta de cores basica (Fase 3 - pode crescer depois)
                    case SDLK_1: selectColor(colorPickerOptions[0].color); break; // preto
                    case SDLK_2: selectColor(colorPickerOptions[1].color); break; // vermelho
                    case SDLK_3: selectColor(colorPickerOptions[2].color); break; // verde
                    case SDLK_4: selectColor(colorPickerOptions[3].color); break; // azul
                    case SDLK_5: selectColor(colorPickerOptions[4].color); break; // branco
                    default: break;
                }
            }
        }

        SDL_UpdateWindowSurface(pWindow);
	}

	// clean up SDL
	SDL_Quit();
	return 0;
}
