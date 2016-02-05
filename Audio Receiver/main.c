#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <assert.h>
#include <math.h>
#include <fftw3.h>

#define FRAME_BLOCK_LEN 44100
SF_INFO sf_props;

char* filename="d:/clean.wav";

int readFile( SNDFILE* fin, double* left, double* right);


int main()
{

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
        return 0;
    }
    left=(double*)malloc(sf_props.frames * sizeof(double));
    right=(double*)malloc(sf_props.frames * sizeof(double));

    readFile(fin,left, right);

    sf_close(fin);
    fin=NULL;

    int increment=FRAME_BLOCK_LEN/10;
    int i,j;
    for(i=0;i<(sf_props.frames-FRAME_BLOCK_LEN); i+=increment){

        for(j=0;j<FRAME_BLOCK_LEN;j++){
            leftInput[j][0]=left[j+i];
            leftInput[j][1]=0.;
        }
        fftw_execute(leftPlan);
        printf("%d",i/increment);
        for(j=5;j<40;j++){
            float magn=sqrtf(leftOutput[j][0]*leftOutput[j][0] + leftOutput[j][1]*leftOutput[j][1]);
            printf(",%f",magn);
        }
//        j=10;
//        float magn=sqrtf(leftOutput[j][0]*leftOutput[j][0] + leftOutput[j][1]*leftOutput[j][1]);
//        printf("%.15f",magn);
        printf("\r");
    }

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

