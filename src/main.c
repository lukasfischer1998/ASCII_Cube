/*********************** GNU General Public License 3.0 ***********************\
|                                                                              |
|  Copyright (C) 2023 Lukas Fischer                                            |
|                                                                              |
|  This program is free software: you can redistribute it and/or modify        |
|  it under the terms of the GNU General Public License as published by        |
|  the Free Software Foundation, either version 3 of the License, or           |
|  (at your option) any later version.                                         |
|                                                                              |
|  This program is distributed in the hope that it will be useful,             |
|  but WITHOUT ANY WARRANTY; without even the implied warranty of              |
|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               |
|  GNU General Public License for more details.                                |
|                                                                              |
|  You should have received a copy of the GNU General Public License           |
|  along with this program.  If not, see <https://www.gnu.org/licenses/>.      |
|                                                                              |
\******************************************************************************/

#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

// Deklaration der globalen Variablen
float A, B, C; // Rotationswinkel

// Werte für die Berechnung des Würfels und der Darstellung
float wuerfelBreite = 20;
int breite = 160, hoehe = 44;
float tiefenPuffer[160 * 44];    // Tiefenpuffer
char bildschirmPuffer[160 * 44]; // BildschirmPuffer
int hintergrundASCIICode = '.';  // Hintergrund-ASCII-Code
int entfernungVonCam = 100;      // Abstand von der Kamera
float horizontalerOffset;        // Horizontaler Offset
float K1 = 40;                   // Konstante

float inkrementsGeschwindigkeit = 0.8; // Inkrementsgeschwindigkeit

float x, y, z; // 3D-Koordinaten
float ooz;     // Inverse der Z-Koordinate
int xp, yp;    // Bildschirmkoordinaten
int idx;       // Pufferindex

float berechneX(int i, int j, int k)
{
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float berechneY(int i, int j, int k)
{
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float berechneZ(int i, int j, int k)
{
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void berechneFuerOberflaeche(float wuerfelX, float wuerfelY, float wuerfelZ, int ch)
{
    x = berechneX(wuerfelX, wuerfelY, wuerfelZ);
    y = berechneY(wuerfelX, wuerfelY, wuerfelZ);
    z = berechneZ(wuerfelX, wuerfelY, wuerfelZ) + entfernungVonCam;

    ooz = 1 / z;

    xp = (int)(breite / 2 + horizontalerOffset + K1 * ooz * x * 2);
    yp = (int)(hoehe / 2 + K1 * ooz * y);

    idx = xp + yp * breite;
    if (idx >= 0 && idx < breite * hoehe)
    {
        if (ooz > tiefenPuffer[idx])
        {
            tiefenPuffer[idx] = ooz;
            bildschirmPuffer[idx] = ch;
        }
    }
}

int main()
{
    printf("\x1b[2J");
    while (!kbhit())
    {
        memset(bildschirmPuffer, hintergrundASCIICode, breite * hoehe);
        memset(tiefenPuffer, 0, breite * hoehe * 4);
        wuerfelBreite = 20;
        horizontalerOffset = -2 * wuerfelBreite;
        // erster Würfel
        for (float wuerfelX = -wuerfelBreite; wuerfelX < wuerfelBreite; wuerfelX += inkrementsGeschwindigkeit)
        {
            for (float wuerfelY = -wuerfelBreite; wuerfelY < wuerfelBreite; wuerfelY += inkrementsGeschwindigkeit)
            {
                berechneFuerOberflaeche(wuerfelX, wuerfelY, -wuerfelBreite, '@');
                berechneFuerOberflaeche(wuerfelBreite, wuerfelY, wuerfelX, '$');
                berechneFuerOberflaeche(-wuerfelBreite, wuerfelY, -wuerfelX, '~');
                berechneFuerOberflaeche(-wuerfelX, wuerfelY, wuerfelBreite, '#');
                berechneFuerOberflaeche(wuerfelX, -wuerfelBreite, -wuerfelY, ';');
                berechneFuerOberflaeche(wuerfelX, wuerfelBreite, wuerfelY, '+');
            }
        }
        wuerfelBreite = 10;
        horizontalerOffset = 1 * wuerfelBreite;
        // zweiter Würfel
        for (float wuerfelX = -wuerfelBreite; wuerfelX < wuerfelBreite; wuerfelX += inkrementsGeschwindigkeit)
        {
            for (float wuerfelY = -wuerfelBreite; wuerfelY < wuerfelBreite; wuerfelY += inkrementsGeschwindigkeit)
            {
                berechneFuerOberflaeche(wuerfelX, wuerfelY, -wuerfelBreite, '@');
                berechneFuerOberflaeche(wuerfelBreite, wuerfelY, wuerfelX, '$');
                berechneFuerOberflaeche(-wuerfelBreite, wuerfelY, -wuerfelX, '~');
                berechneFuerOberflaeche(-wuerfelX, wuerfelY, wuerfelBreite, '#');
                berechneFuerOberflaeche(wuerfelX, -wuerfelBreite, -wuerfelY, ';');
                berechneFuerOberflaeche(wuerfelX, wuerfelBreite, wuerfelY, '+');
            }
        }
        wuerfelBreite = 5;
        horizontalerOffset = 8 * wuerfelBreite;
        // dritter Würfel
        for (float wuerfelX = -wuerfelBreite; wuerfelX < wuerfelBreite; wuerfelX += inkrementsGeschwindigkeit)
        {
            for (float wuerfelY = -wuerfelBreite; wuerfelY < wuerfelBreite; wuerfelY += inkrementsGeschwindigkeit)
            {
                berechneFuerOberflaeche(wuerfelX, wuerfelY, -wuerfelBreite, '@');
                berechneFuerOberflaeche(wuerfelBreite, wuerfelY, wuerfelX, '$');
                berechneFuerOberflaeche(-wuerfelBreite, wuerfelY, -wuerfelX, '~');
                berechneFuerOberflaeche(-wuerfelX, wuerfelY, wuerfelBreite, '#');
                berechneFuerOberflaeche(wuerfelX, -wuerfelBreite, -wuerfelY, ';');
                berechneFuerOberflaeche(wuerfelX, wuerfelBreite, wuerfelY, '+');
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < breite * hoehe; k++)
        {
            if (k % breite != 0)
            {
                putchar(bildschirmPuffer[k]);
            }
            else
            {
                putchar(10);
            }
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
        usleep(8000 * 2);
    }
    printf("Key pressed. Exiting...\n");
    return 0;
}
