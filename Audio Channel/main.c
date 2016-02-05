#include "wavegen.h"
#include <assert.h>
#include <portaudio.h>

#define SAMPLING_RATE 44100

float freq1=10;
float freq2=14;
float freq3=18;
GTABLE *table = NULL;
OSCILT *osc1 = NULL;
OSCILT *osc2 = NULL;
OSCILT *osc3 = NULL;

const PaDeviceInfo *info;

PaStream* stream;

int paInit();

void dft(float *in, float* outReal, float* outImag, int N);
void idft(float *in, float* outReal, float* outImag, int N);

int main()
{
    GTABLE *table = NULL;

    table = new_sine(1024);
    osc1 = new_oscilt(SAMPLING_RATE, table, 0.f);
    osc2 = new_oscilt(SAMPLING_RATE, table, 0.f);
    osc3 = new_oscilt(SAMPLING_RATE, table, 0.f);

    FILE *output = NULL;
    FILE *outPlucked = NULL;
    FILE *spec = NULL;
    output=fopen("out.txt","w");
    outPlucked=fopen("outPluck.txt","w");
    spec=fopen("spec.txt","w");

    float *signal=NULL;
    float *pluckedSignal=NULL;
    float *outReal=NULL;
    float *outImag=NULL;

    int waveSize=(SAMPLING_RATE/5);
    float stepSize=waveSize/1024.;

    signal = (float*)malloc(waveSize * sizeof(float));
    pluckedSignal=(float*)malloc(1024 * sizeof(float));
    outReal=(float*)malloc(1024 * sizeof(float));
    outImag=(float*)malloc(1024 * sizeof(float));

	pa_init();

	fgetc(stdin);

    encode(1, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(0, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(2, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(0, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(3, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(0, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(4, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(0, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(5, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(0, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(6, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(0, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(7, signal);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);
    Pa_WriteStream(stream, signal, waveSize);

    encode(0, signal);
    Pa_WriteStream(stream, signal, waveSize);





//    for(;i<waveSize;i++){
//        signal[i]=interpolatedTabTick(osc2,freq2)+interpolatedTabTick(osc3,freq3);
//    }
//
//    for(i=0;j<waveSize,i<1024;j+=stepSize,i++){
//        pluckedSignal[i]=signal[(int)j];
//    }
//    printf("index: %d\n",i);
//    printf("i: %d\n",i);
//    printf("step: %f\n",stepSize);
//    printf("wavesize: %d\n",waveSize);
//
//    dft(pluckedSignal, outReal, outImag,1024);
//
//    for(i=0;i<waveSize;i++){
//        fprintf(output,"%.9f\n",signal[i]);
//    }
//
//    for(i=0;i<1024;i++){
//        fprintf(outPlucked,"%.9f\n",pluckedSignal[i]);
//        fprintf(spec,"%.9f\n",sqrt(outReal[i]*outReal[i] + outImag[i]*outImag[i]));
//    }
//

    fclose(output);
    fclose(outPlucked);
    fclose(spec);
    printf("Hello world!\n");
    return 0;
}


int pa_init(){
    /** PortAudio setup */
    PaStreamParameters outputParameters;
    const PaHostApiInfo *hostApiInfo;
    printf("Initializing portaudio. Please wait...\n");
    Pa_Initialize();
    int id, error;
    id=Pa_GetDefaultOutputDevice();
    info = Pa_GetDeviceInfo(id); //get info about chosen device
    hostApiInfo = Pa_GetHostApiInfo(info->hostApi);
    printf("Default sample rate: %f\n",info->defaultSampleRate);
    printf("Max Output channels: %d\n",info->maxOutputChannels);
    printf("Struct version: %d\n",info->structVersion);
    printf("Type: %d\n",hostApiInfo->type);

    PaDeviceIndex ind  = Pa_GetDeviceCount(  );
    printf("Device id: %d\n",id);

    printf("Opening AUDIO Output device [%s] %s\n", hostApiInfo->name, info->name);
    outputParameters.device=id;
    outputParameters.channelCount=1;
    outputParameters.sampleFormat=paFloat32;
    outputParameters.suggestedLatency=info->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL; //no specific info
    stream=calloc(sizeof(long),1);

    error=Pa_OpenStream(//Opening the PaStream object
                  &stream,//the actual portaudio stream object
                  NULL, //input params
                  &outputParameters, //output parameters
                  info->defaultSampleRate,
                  0, //(Interleaved) frame block length
                  paClipOff,//disabling clipping of out of range amplitude values
                  NULL,//callback function address, i. e. a pointer to function
                  NULL); //data for callback
	if(error)
        return -1;
    error=Pa_StartStream(stream);
	if(error)
        return -1;

    Pa_StartStream(stream);
    return 0;
}



void dft(float *in, float* outReal, float* outImag, int N){
    int i;
    for(i=0;i<N;i++){
        outReal[i] = outImag[i] = .0f;
        int n;
        for(n=0;n<N;n++){
            outReal[i] += in[n] *cosf(i*n*M_PI*2/N);
            outImag[i] -= in[n]*sinf(i*n*M_PI*2/N);
        }
        outReal[i]/=N;
        outImag[i]/=N;
    }
}

void idft(float *in, float* outReal, float* outImag, int N){
    int i;
    for(i=0;i<N;i++){
        in[i]= .0f;
        int n;
        for(n=0;n<N;n++){
            in[i]+= outReal[n] * cosf(i*n*M_PI*2/N) - outImag[n]*sinf(i*n*M_PI*2/N);
        }
    }
}


void encode(char digit, float *signal){
    assert(digit>=0);
    assert(signal!=0);
    int i;
    int waveSize=(SAMPLING_RATE/(int)freq1);

    switch(digit){
    case 1:
        for(i=0;i<waveSize;i++)
            signal[i]=interpolatedTabTick(osc1,freq1);
        break;
    case 2:
        for(i=0;i<waveSize;i++)
            signal[i]=interpolatedTabTick(osc1,freq1)/2+interpolatedTabTick(osc2,freq2)/2;
        break;
    case 3:
        for(i=0;i<waveSize;i++)
            signal[i]=interpolatedTabTick(osc1,freq1)/2+interpolatedTabTick(osc3,freq3)/2;
        break;
    case 4:
        for(i=0;i<waveSize;i++)
            signal[i]=interpolatedTabTick(osc1,freq1)/3+interpolatedTabTick(osc2,freq2)/3+interpolatedTabTick(osc3,freq3)/3;
        break;
    case 5:
        for(i=0;i<waveSize;i++)
            signal[i]=interpolatedTabTick(osc2,freq2);
        break;
    case 6:
        for(i=0;i<waveSize;i++)
            signal[i]=interpolatedTabTick(osc2,freq2)/2+interpolatedTabTick(osc3,freq3)/2;
        break;
    case 7:
        for(i=0;i<waveSize;i++)
            signal[i]=interpolatedTabTick(osc3,freq3);
        break;
    default:
        for(i=0;i<waveSize;i++)
            signal[i]=.0;
        break;
    break;
    }
}
