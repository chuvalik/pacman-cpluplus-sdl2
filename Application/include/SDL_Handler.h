#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Global.h"
#include <stdio.h>
#include <string>

class SDL_Handler
{
public:
    SDL_Surface *screen;
    SDL_Surface *image;

    SDL_Event event;

    //
    bool init();

    //
    void cleanUp();

    // ctor
    SDL_Handler();

    //dtor
    ~SDL_Handler();

    // ���������� ������
    bool Update();

    // �������� ����������� �� ������� ����������� (screen)
    void applySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip);

    // ����� ������ � ������ ������ � ����������� �� ��������� ����
    void ChangeVideoMode(unsigned char videoMode);

    SDL_Surface *loadImage(std::string filename);

};

#endif
