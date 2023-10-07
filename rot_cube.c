#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


float a,b,c;
float cubeWidth = 20;
int width = 160;
int height = 44;
float zBuffer[160*44];
char charBuffer[160*44];
int backgroundASCII = ' ';
float incrementSpeed = 0.6;
float x, y,z;
int distanceFromScreen = 100;
int xp, yp;
float ooz;
float K1 = 40;

int idx;


float calculate_x(int i, int j, int k) 
{
    return j * sin(a) * sin(b) * cos(c) - k * cos(a) * sin(b) * cos(c) + j * cos(a) * sin(c) + k * sin(a) * sin(c) + i * cos(b) * cos(c);
}

float calculate_y(int i, int j, int k) 
{
    return j * cos(a) * cos(c) + k * sin(a) * cos(c) - j * sin(a) * sin(b) * sin(c) + k * cos(a) * sin(b) * sin(c) - i * cos(b) * sin(c);
}

float calculate_z(int i, int j, int k)
{
    return k * cos(a) * cos(b) - j * sin(a) * cos(b) + i * sin(b);
}

void calcualteForSurface(float cubeX, float cubeY, float cubeZ, int ch) 
{
    x = calculate_x(cubeX, cubeY, cubeZ);
    y = calculate_y(cubeX, cubeY, cubeZ);
    z = calculate_z(cubeX, cubeY, cubeZ) + distanceFromScreen;
    ooz = 1 / z;
    xp = (int)(width / 2  + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);
    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) 
    {
        if (ooz > zBuffer[idx]) 
        {
            zBuffer[idx] = ooz;
            charBuffer[idx] = ch;
        }
    }
}

int main()
{
    printf("\x1b[2J");

    while (1)
    {
        memset(charBuffer,backgroundASCII,width*height);
        memset(zBuffer,0,width*height*4); 
        for(float cubeX = - cubeWidth; cubeX < cubeWidth; cubeX+= incrementSpeed)
        {
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                calcualteForSurface(cubeX,cubeY,-cubeWidth,'.');
                calcualteForSurface(cubeWidth,cubeY,cubeX,'$');
                calcualteForSurface(-cubeWidth,cubeY,-cubeX,'#');
                calcualteForSurface(-cubeX,cubeY,cubeWidth,'~');
                calcualteForSurface(cubeX,-cubeWidth,cubeY,';');
                calcualteForSurface(cubeX,cubeWidth,cubeY,'-');
            }
        }

        printf("\x1b[H");

        for(int k = 0; k < width * height; k++)
        {
            putchar(k % width ? charBuffer[k] : 10);
        }

        a += 0.005;
        b += 0.005;
        c += 0.001;
        
        usleep(1000);

    }
    
    return 0;

}