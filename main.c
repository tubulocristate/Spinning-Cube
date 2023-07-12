#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct Point2D {
	float x;
	float y;
} Point2D;

typedef struct Point3D {
	float x;
	float y;
	float z;
} Point3D;


Point3D pyramid_points[] = {
	{0, 1, 0},
	{1, -1, -1},
	{1, -1, 1},
	{-1, -1, 1},
	{-1, -1, -1}
};

int pyramid_lines[][2] = {
	{0,1},
	{0,2},
	{0,3},
	{0,4},
	{1,2},
	{2,3},
	{3,4},
	{4,1},
};

Point3D cube[] = {
	{0,0,0}, // 0
	{1,0,0}, // 1
	{1,1,0}, // 2
	{0,1,0}, // 3
	{0,0,1}, // 4
	{1,0,1}, // 5
	{1,1,1}, // 6
	{0,1,1}  // 7
};
int cube_lines[][2] = {
	{0, 1},
	{0, 3},
	{0, 4},
	{1, 2},
	{1, 5},
	{2, 3},
	{2, 6},
	{3, 7},
	{4, 5},
	{4, 7},
	{5, 6},
	{6, 7},
};

int check_for_quit(int quit);
void bresenham_line(SDL_Renderer *renderer, Point2D point1, Point2D point2); 
void draw_point(SDL_Renderer *renderer, int x, int y);
void swap(int* a , int*b);
float absolute(float x );
int cast_to_int(float x);
int roundNumber(float x);
void Xiaolin_Line(SDL_Renderer *renderer, Point2D point1 , Point2D point2);
Point3D rotate_around_x(Point3D point, float angle);
Point3D rotate_around_y(Point3D point, float angle);
Point2D weak_perspective_projection(Point3D point, float distance);
void draw_figure(SDL_Renderer *renderer, Point3D *points, int lines[][2], int number_of_lines, float scalar);
Point2D scale(Point2D point, float constant);


int main(void)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
	

	int quit = 0;
	float angle = 0.005;
	while (quit != 1){
		quit = check_for_quit(quit);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (int i = 0; i < 8; i++){
			cube[i] = rotate_around_y(cube[i], angle);
			cube[i] = rotate_around_x(cube[i], angle);
		}
		draw_figure(renderer, cube, cube_lines, 12, 100);

		SDL_RenderPresent(renderer);		
		SDL_GL_SwapWindow(window);
		SDL_Delay(16);
	}
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}
int check_for_quit(int quit)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0){
		if (e.type == SDL_QUIT){
			quit = 1;
			}
	}
	return quit;
}

void draw_point(SDL_Renderer *renderer, int x, int y)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

void swap(int* a , int*b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

float absolute(float x )
{
    if (x < 0) return -x;
    else return x;
}

int cast_to_int(float x)
{
    return (int)x;
}

int roundNumber(float x)
{
    return cast_to_int(x + 0.5) ;
}

void Xiaolin_Line(SDL_Renderer *renderer, Point2D point1, Point2D point2)
{
    Point2D center = {WIDTH/2, HEIGHT/2};
    int y0 = point1.y;
    int y1 = point2.y;
    int x0 = point1.x;
    int x1 = point2.x;
    int steep = absolute(y1 - y0) > absolute(x1 - x0) ;
  
    if (steep)
    {
        swap(&x0 , &y0);
        swap(&x1 , &y1);
    }
    if (x0 > x1)
    {
        swap(&x0 ,&x1);
        swap(&y0 ,&y1);
    }
  
    float dx = x1-x0;
    float dy = y1-y0;
    float gradient = dy/dx;
    if (dx == 0.0)
        gradient = 1;
  
    int xpxl1 = x0;
    int xpxl2 = x1;
    float intersectY = y0;
  
    if (steep)
    {
        int x;
        for (x = xpxl1 ; x <=xpxl2 ; x++)
        {
            draw_point(renderer, center.x+cast_to_int(intersectY), center.y-x);
            draw_point(renderer, center.x+cast_to_int(intersectY)-1, center.y-x);
            intersectY += gradient;
        }
    }
    else
    {
        int x;
        for (x = xpxl1 ; x <=xpxl2 ; x++)
        {
            draw_point(renderer, center.x+x, center.y-cast_to_int(intersectY));
            draw_point(renderer, center.x+x, center.y-cast_to_int(intersectY)-1);
            intersectY += gradient;
        }
    }
  
}

Point3D rotate_around_x(Point3D point, float angle)
{
	float new_x = point.x;
	float new_y = cos(angle)*point.y - sin(angle)*point.z;
	float new_z = sin(angle)*point.y + cos(angle)*point.z;
	Point3D rotated_point = {new_x, new_y, new_z};
	return rotated_point;
}
Point3D rotate_around_y(Point3D point, float angle)
{
	float new_x = cos(angle)*point.x - sin(angle)*point.z;
	float new_y = point.y;
	float new_z = sin(angle)*point.x + cos(angle)*point.z;
	Point3D rotated_point = {new_x, new_y, new_z};
	return rotated_point;
}
Point2D weak_perspective_projection(Point3D point, float distance)
{
	float x = point.x * distance / (point.z + distance);
	float y = point.y * distance / (point.z + distance);
	Point2D projected_point = {x, y};
	return projected_point;
}
void draw_figure(SDL_Renderer *renderer, Point3D *points, int lines[][2], int number_of_lines, float scalar)
{
	for (int i = 0; i < number_of_lines; i++){
		int idx1 = lines[i][0];
		int idx2 = lines[i][1];
		Point2D point1 = scale(weak_perspective_projection(points[idx1], 2), scalar);
		Point2D point2 = scale(weak_perspective_projection(points[idx2], 2), scalar);
		Xiaolin_Line(renderer, point1, point2);
	}
}
Point2D scale(Point2D point, float constant)
{
	float new_x = point.x * constant;
	float new_y = point.y * constant;
	Point2D scaled_point = {new_x, new_y};
	return scaled_point;
}
