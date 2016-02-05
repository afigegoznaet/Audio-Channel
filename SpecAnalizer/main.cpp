#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <assert.h>
#include <math.h>
#include <fftw3.h>
#include "Fluid/specChart.h"
#include <unistd.h>
#include <FL/fl_ask.H>
#include <float.h>

#define FRAME_BLOCK_LEN 44100
SF_INFO sf_props;
//char* filename=(char*)"C:/temp/Workspace/motown-pulse.wav";
char* filename=(char*)"d:/laptop.wav";

int readFile( SNDFILE* fin, double* left, double* right);

void setupChart(Fl_Chart* chart, boolean logScale){
    int i;
    chart->type(FL_FILL_CHART);
	chart->maxsize(50);

	if(logScale)
		chart->bounds(7.7, log10l(21) * 1.12);
	else
		chart->bounds(.0, 31.);

	for(i=0;i<50;i++){
		chart->add(0,0,0);
	}

//	chart->input = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FRAME_BLOCK_LEN);
//	chart->output = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FRAME_BLOCK_LEN);
//    chart->plan = fftw_plan_dft_1d(FRAME_BLOCK_LEN, chart->input, chart->output, FFTW_FORWARD, FFTW_ALLOW_PRUNING);

}

void setupWindow(double* window){
    int i;
    double increment=0.;
    printf("%f\n",increment);
    for (i=0;i<FRAME_BLOCK_LEN/2;i++){

        window[FRAME_BLOCK_LEN-i-1]+=increment;
        window[i]+=increment;
        increment+=2*(1./(double)FRAME_BLOCK_LEN);
    }

    FILE* file;
    file = fopen("out.txt","w");
    for (i=0;i<FRAME_BLOCK_LEN;i++)
        fprintf(file, "%f\n",window[i]);
    fclose(file);
}

int process(void *gui){

    Analyzer* anal = (Analyzer*)gui;
    setupChart(anal->chart, false);

    double *left, *right;

    fftw_complex *leftInput, *leftOutput;
    fftw_complex *rightInput, *rightOutput;
    fftw_plan leftPlan, rightPlan;

    leftInput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FRAME_BLOCK_LEN);
    rightInput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FRAME_BLOCK_LEN);
    leftOutput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FRAME_BLOCK_LEN);
    rightOutput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FRAME_BLOCK_LEN);

    leftPlan = fftw_plan_dft_1d(FRAME_BLOCK_LEN, leftInput, leftOutput, FFTW_FORWARD, FFTW_ALLOW_PRUNING);
    rightPlan = fftw_plan_dft_1d(FRAME_BLOCK_LEN, rightInput, rightOutput, FFTW_FORWARD, FFTW_ALLOW_PRUNING);

    SNDFILE* fin;
    fin=sf_open(filename, SFM_READ, &sf_props);

    if(fin==NULL){
        printf("Could not open file %s\n",filename);
        fl_message("Could not open file %s\n",filename);
        return 0;
    }
    left=(double*)malloc(sf_props.frames * sizeof(double));
    right=(double*)malloc(sf_props.frames * sizeof(double));

    readFile(fin,left, right);

    sf_close(fin);
    fin=NULL;

    double window[FRAME_BLOCK_LEN]={0.};
    setupWindow(window);

    int increment=FRAME_BLOCK_LEN/20;
    int i,j;
    double maxSpec=0., maxWave=0.;
    char* c=(char*)malloc(2);
    for(i=0;i<(sf_props.frames-FRAME_BLOCK_LEN); i+=increment){

        for(j=0;j<FRAME_BLOCK_LEN;j++){
            leftInput[j][0]=left[j+i]*window[j];
            leftInput[j][1]=0.;
            if(leftInput[j][0]>maxWave)
                maxWave=leftInput[j][0];
        }
        printf("Max Wave: %f\n", maxWave);
        fftw_execute(leftPlan);
        printf("%d",i/increment);
        for(j=1;j<51;j++){
            double magn=sqrtf(leftOutput[j][0]*leftOutput[j][0] + leftOutput[j][1]*leftOutput[j][1]);
            sprintf(c,"%d ",j);
            //printf("%s ",c);
            anal->chart->replace(j,magn,c,FL_GREEN);
            if(magn>maxSpec)
                maxSpec=magn;
        }
        fl_message("Processed %f secs of %f",(float)i/FRAME_BLOCK_LEN,(float)sf_props.frames/FRAME_BLOCK_LEN);
//        usleep(100000000);
//        anal->chart->redraw();
    }
    fl_message("Max Spec: %.4f",maxSpec);
    return 0;
}

int readFile( SNDFILE* fin, double* left, double* right){
    sf_count_t readFrames, currentFrame=0;

    double* audio=NULL;
    audio=(double*) malloc(FRAME_BLOCK_LEN*2*sizeof(double));
    assert(audio!=0);



    printf("Channels: %d\n", sf_props.channels);
    printf("Format: %x\n", sf_props.format);
    printf("Frames: %d\n", (int)sf_props.frames);
    printf("Samplerate: %d\n", sf_props.samplerate);
    printf("Sections: %d\n", sf_props.sections);
    printf("Seekable: %d\n", sf_props.seekable);

    while(readFrames>0 && currentFrame<sf_props.frames ){
        readFrames=sf_readf_double(fin,audio,FRAME_BLOCK_LEN);
        printf("Frames Read: %d\n",(int)readFrames);

        if(sf_props.channels==2){
			int i;
			for(i=0;i<readFrames;i++){
				left[i+currentFrame]=audio[2*i];
				right[i+currentFrame]=audio[2*i+1];
			}
        }

        currentFrame+=readFrames;
    }
    free(audio);
    return 1;
}

int main(int argc, char** argv){
    if(argc>1)
        filename=argv[1];

    Analyzer *gui = new Analyzer();
    gui->show();
    return Fl::run();
}
