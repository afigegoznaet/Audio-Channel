#ifndef WAVEGEN_H_INCLUDED
#define WAVEGEN_H_INCLUDED

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct t_gtable
{
	float* table;
	unsigned long length;
} GTABLE;

typedef struct  t_oscil
{
	float srate;
	float twopiovrsr;
	float currentFreq;
	float currentPhase;
	float increment;
} OSCIL;

typedef struct t_tab_oscil
{
	OSCIL osc;
	const GTABLE* gtable;
	float dtablen;
	float sizeovrsr;
} OSCILT;

GTABLE* new_gtable(unsigned long length);
void gtable_free(GTABLE** gtable);

GTABLE* new_sine(unsigned long length);
OSCIL*  new_oscil(float srate,float phase);
OSCILT* new_oscilt(unsigned long srate, const GTABLE* table, float phase);

float sinetick  (OSCIL* p_osc, float freq);

float tabtick(OSCILT* p_osc, float freq);
float interpolatedTabTick(OSCILT* p_osc, float freq);

#endif // WAVEGEN_H_INCLUDED
