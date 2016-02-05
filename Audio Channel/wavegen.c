#include "wavegen.h"
/* local typing-saving macro for tick functions */
#define OSC_WRAPPHASE  if(p_osc->currentPhase > 2*M_PI) p_osc->currentPhase -= 2*M_PI;	\
						if(p_osc->currentPhase < 0.0) p_osc->currentPhase += 2*M_PI

GTABLE* new_gtable(unsigned long length){
    unsigned long i;
    GTABLE* gtable = NULL;
    if(length == 0)
        return NULL;
    gtable = (GTABLE*) malloc(sizeof(GTABLE));

    gtable->table = (float*) malloc((length+1)*sizeof(float));
    if(gtable->table == NULL){
        free(gtable);
        return NULL;
    }
    for(i=0;i<=length;i++)
        gtable->table[i]=.0;
    gtable->length = length;
    return gtable;
}

void gtable_free(GTABLE** gtable){
    if(gtable && *gtable && (*gtable)->table){
        free((*gtable)->table);
        free((*gtable));
        *gtable=NULL;
    }
}



static void normalize_gtable(GTABLE* gtable){
    float amp,maxamp=.0;
    unsigned long i=0;
    unsigned long length = gtable->length;
    for(i=0;i<length;i++){
        amp = fabs(gtable->table[i]);
        if(maxamp<amp)
            maxamp=amp;
    }
    maxamp = 1. / maxamp;
    for(i=0;i<length;i++)
        gtable->table[i] *= maxamp;
    gtable->table[i]=gtable->table[0];
}

OSCILT*  new_oscilt(unsigned long srate,const GTABLE* gtable,float phase)
{
	OSCILT* p_osc;
	/* do we have a good GTABLE?*/
	if(gtable == NULL || gtable->table==NULL || gtable->length == 0)
		return NULL;
	p_osc = (OSCILT *) malloc(sizeof(OSCILT));
	if(p_osc==NULL)
		return NULL;
	/* init the osc..*/
	p_osc->osc.currentFreq = 0.0;
	p_osc->osc.currentPhase = gtable->length * phase;
	p_osc->osc.increment = 0.0;
	/* then the table-specific things */
	p_osc->gtable = gtable;
	p_osc->dtablen = (float) gtable->length;
	p_osc->sizeovrsr =  p_osc->dtablen  / (float) srate;
	return p_osc;
}

GTABLE* new_sine(unsigned long length){
    unsigned long i;
    float step;
    GTABLE* gtable = NULL;

    gtable = new_gtable(length);
    if(gtable==NULL)
        return NULL;


    step = 2 * M_PI / length;

    for(i=0;i<length;i++)
        gtable->table[i] = sin(step*i);

    gtable->table[i]=gtable->table[0];
    normalize_gtable(gtable);
    return gtable;
}

float tabtick(OSCILT* p_osc, float freq){
    int index = (int) p_osc->osc.currentPhase;
    float dtablen = p_osc->dtablen;
    float curphase = p_osc->osc.currentPhase;
    float* table = p_osc->gtable->table;
    if(p_osc->osc.currentFreq != freq){
        p_osc->osc.currentFreq = freq;
        p_osc->osc.increment = p_osc->sizeovrsr * p_osc->osc.currentFreq;
    }
    curphase += p_osc->osc.increment;
    while(curphase >= dtablen)
        curphase -= dtablen;
    while(curphase < .0)
        curphase += dtablen;
    p_osc->osc.currentPhase = curphase;
    return table[index];
}

float interpolatedTabTick(OSCILT* p_osc, float freq){
    int base_index = (int) p_osc->osc.currentPhase;
    unsigned long next_index = base_index+1;
    float frac,slope,val;
    float dtablen = p_osc->dtablen;
    float curphase = p_osc->osc.currentPhase;
    float* table = p_osc->gtable->table;
    if(p_osc->osc.currentFreq != freq){
        p_osc->osc.currentFreq = freq;
        p_osc->osc.increment = p_osc->sizeovrsr * p_osc->osc.currentFreq;
    }
    frac = curphase - base_index;
    val = table[base_index];
    slope = table[next_index] - val;
    val+=(frac*slope);

    curphase += p_osc->osc.increment;
    while(curphase >= dtablen)
        curphase -= dtablen;
    while(curphase < .0)
        curphase += dtablen;
    p_osc->osc.currentPhase = curphase;
    return val;
}

/*********** SINE **************/

float sinetick(OSCIL* p_osc, float freq)
{
	float val;

	val = sin(p_osc->currentPhase);
	if(p_osc->currentFreq != freq){
		p_osc->currentFreq = freq;
		p_osc->increment = p_osc->twopiovrsr * freq;
	}
	p_osc->currentPhase += p_osc->increment;
	OSC_WRAPPHASE;
	return val;
}

