#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "readwave.h"
#include "wavReadResult.h"
#include "DTW.h"
#include "hmmModel/templateList.h"
#include "hmmModel/hmmModel.h"
#include "hmmModel/hmmTool.h"
//#include "HMM.h"
//#include "HMM3.h"
#define READFROMFILE (0)
const int WRITE_DATA_TO_FILE=1;
double background=0;
double level=0; 
vector <bool> isFrameSpeech;
vector <short> backgroundFrame;

double CalcDB(SAMPLE* frame){
	double sum=0;
	for(unsigned int i=0;i<FRAMES_PER_BUFFER;i++)
	{
		sum+=frame[i]*frame[i];
	}
	return 10*log(sum);
}
double CalcDB(SAMPLE* frame,int size){
	double sum=0;
	for(unsigned int i=0;i<size;i++)
	{
		sum+=frame[i]*frame[i];
	}
	return 10*log(sum);
}
bool classifyFrame2(SAMPLE* audioFrame)
{
	double now=CalcDB(audioFrame);
	bool isSpeech=false;
	double forgetfactor=1;
	level=((level*forgetfactor)+now)/(forgetfactor+1);	
	if(now<background&&now!=0){	
		background=now;
	}else
	{
		background += (now-background)*0.01;		
	}
	if(level<background)
		level=background;	
	if(level-background>50)
	{			
		isSpeech=true;
		cout<<now<<" "<<level<<" "<<background<<" "<<isSpeech<<endl;
	}
	return isSpeech;
}

int frameCounting=0;
/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
						  unsigned long framesPerBuffer,
						  const PaStreamCallbackTimeInfo* timeInfo,
						  PaStreamCallbackFlags statusFlags,
						  void *userData )
{
	paTestData *data = (paTestData*)userData;
	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
	SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	int finished;

	(void) outputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	finished = paContinue;
	if( inputBuffer == NULL )
	{
		for(unsigned int i=0; i<framesPerBuffer; i++ )
		{
			*wptr++ = SAMPLE_SILENCE;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
		}
	}
	else
	{	
		frameCounting++;
		SAMPLE* frame=new SAMPLE[framesPerBuffer];
		for(unsigned int i=0;i<framesPerBuffer;i++)
		{
			frame[i]=*rptr;
			*wptr++ = *rptr++; 
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		bool isFrameSpeechBool=false;
		if(frameCounting<10){
		}else if(frameCounting==10){
			SAMPLE* temp=new SAMPLE[framesPerBuffer*10];
			for(int i=0;i<framesPerBuffer*10;i++)
			{
				temp[i]=data->recordedSamples[i];
			}
			background=CalcDB(temp,FRAMES_PER_BUFFER*10);
			delete[] temp;
		}else{
			isFrameSpeechBool=classifyFrame2(frame);
		}

		if(!isFrameSpeechBool)
		{
			data->silenceFrames+=framesPerBuffer;
		}else{
			data->silenceFrames=0;
		}
		for(unsigned int i=0;i<framesPerBuffer;i++)
		{
			isFrameSpeech.push_back(isFrameSpeechBool);
			//energyOfFrame.push_back(CalcDB(frame));
			backgroundFrame.push_back(background);
		}		
		delete []frame;
		if(data->silenceFrames/SAMPLE_RATE>1)
		{
			finished=paComplete;
		}
	}
	data->frameIndex += framesPerBuffer;
	return finished;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int playCallback( const void *inputBuffer, void *outputBuffer,
						unsigned long framesPerBuffer,
						const PaStreamCallbackTimeInfo* timeInfo,
						PaStreamCallbackFlags statusFlags,
						void *userData )
{	
	paTestData *data = (paTestData*)userData;
	SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
	unsigned int i;
	int finished;
	unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;
	(void) inputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	if( framesLeft < framesPerBuffer )
	{
		/* final buffer... */
		for( i=0; i<framesLeft; i++ )
		{
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		for( ; i<framesPerBuffer; i++ )
		{
			*wptr++ = 0;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
		}
		data->frameIndex += framesLeft;
		finished = paComplete;
	}
	else
	{
		for( i=0; i<framesPerBuffer; i++ )
		{
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		data->frameIndex += framesPerBuffer;
		finished = paContinue;
	}
	return finished;
}
/*******************************************************************/
int main(void);
void readWavFunctionTest()
{
	int num=0;
	int samplerate=0;
	short* data2=ReadWave(string("G:/中大/研一上学期/语音识别/test1/test1/template/five/1/smooth.wav").c_str(),&num,&samplerate);
	for(int i=0;i<num;++i)
		printf("%d ",data2[i]);
	wavReadResult resulttest=wavReadResult(data2,num,"test","1");
	free(data2);
	system("pause");
}
int main(void)
{	
	//readWavFunctionTest();
	//printf("\n");
	/*HMMtest();
	system("pause");
	return 0;
	DTW test;*/

	//const int templateIndex=0;
	//test.testSingleTemplateDTWforCost(templateIndex,0,9,1,9);
	//system("pause");
	//test.testTimeSynchronousDTWforCost(templateIndex,0,9,1,9);
	//test.testMultiTemplateDTW(0,9,5,12);
	//test.testCostTimeSynchronousDTWCorrectRatePrunning(25,0,9,0,9);
	//test.testMultiTemplateTimeSynchronousDTW(0,9,10,14);//word one to five,instances 7 to 13
	
	//HMM2 hmmModel(test,5);
	/*HMM hmmModel(test,5);
	hmmModel.SegmentalK_means(25);
	hmmModel.initProbabilities();
	hmmModel.getBestCostDTWCorrectRateFromSingleTemplate(0,9,0,14);*/
	/*system("pause");
	return 0;*/
	string folder="./template/pro7/8642097531_1_";
	string folderOrigin=folder;
	string times="";
	if(-1==CheckDir(folder.c_str()))
	{
		printf("create dir error!");
		system("pause");
	}
	folder+=times;
	CheckDir(folder.c_str());
	string rawName=folder+"origin.raw";
	string originFileName=folder+"origin.wav";
	string smoothFileName=folder+"smooth.wav";

	PaStreamParameters  inputParameters,outputParameters;
	PaStream*           stream;
	PaError             err = paNoError;
	paTestData          data;
	int                 i;
	int                 totalFrames;
	int                 numSamples=0;
	int                 numBytes;
	int index=0;
	SAMPLE *recordedSamples2 =nullptr;

	printf("patest_record.c\n"); fflush(stdout);

	data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
	data.frameIndex = 0;
	numSamples = totalFrames * NUM_CHANNELS;
	numBytes = numSamples * sizeof(SAMPLE);

	data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */

	data.silenceFrames=0;
	if( data.recordedSamples == NULL )
	{
		printf("Could not allocate record array.\n");
		return err;
	}
	for( i=0; i<numSamples; i++ ) 
		data.recordedSamples[i] = 0;
	if(!READFROMFILE)
	{
		err = Pa_Initialize();
		if( err != paNoError )  return err;

		inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
		if (inputParameters.device == paNoDevice) {
			fprintf(stderr,"Error: No default input device.\n");
			return err;
		}
		inputParameters.channelCount = 1;                    /* stereo input */
		inputParameters.sampleFormat = PA_SAMPLE_TYPE;
		inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;
		/* Record some audio. -------------------------------------------- */
		err = Pa_OpenStream(
			&stream,
			&inputParameters,
			NULL,                  /* &outputParameters, */
			SAMPLE_RATE,
			FRAMES_PER_BUFFER,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			recordCallback,
			&data );
		if( err != paNoError ) return err;
		cout<<"press enter to record"<<endl;

		getchar();
		err = Pa_StartStream( stream );
		if( err != paNoError )  return err;	
		printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

		while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
		{
			Pa_Sleep(1000);
			printf("index = %d\n", data.frameIndex ); fflush(stdout);
		}
		if( err < 0 )  return err;
		err = Pa_CloseStream( stream );
		if( err != paNoError )  return err;

		data.maxFrameIndex=totalFrames=numSamples=data.frameIndex;	
		cout<<data.maxFrameIndex<<endl;


		int left=0;
		int right=0;
		int GapSize=SAMPLE_RATE/1000*100;	
		int boundarySize=SAMPLE_RATE/1000*150;	
		int n=0;
		recordedSamples2 = (SAMPLE *) malloc( numSamples*sizeof(SAMPLE)* NUM_CHANNELS );

		for(i=numSamples;isFrameSpeech[i-1]==false;i--)
			n++;  
		for(i=numSamples-n;i<=numSamples-n+boundarySize;i++)
			isFrameSpeech[i]=true;

		while(right<numSamples)
		{
			if(right>0&&!isFrameSpeech[right-1]&&isFrameSpeech[right])//no speaking before,speaking after
				//means between left and right is not speaking
			{
				if(right-left>boundarySize)
				{
					left=right-boundarySize;				
				}
				if(right+1>=numSamples)
				{
					right+=boundarySize*3;
				}
				while(right!=left)
				{
					recordedSamples2[++index]=data.recordedSamples[left];
					left++;
				}
				right++;
			}else if(right>0&&isFrameSpeech[right-1]&&!isFrameSpeech[right])//speaking to not speaking
			{//means between left and right is speaking
				if(right-left<boundarySize)
				{
					left=right;
					right++;
				}else
				{				
					while(right!=left)
					{
						recordedSamples2[++index]=data.recordedSamples[left];
						left++;
					}
					right++;
					while(right-left<boundarySize&&right<numSamples)
					{
						recordedSamples2[++index]=data.recordedSamples[right];
						right++;
					}
					left=right;
					right++;
				}
			}else if(isFrameSpeech[right])
			{
				recordedSamples2[++index]=data.recordedSamples[right];
				left++;
				right++;
			}else if(!isFrameSpeech[right])
			{
				right++;
			}
		}
	}
	else if(READFROMFILE==1)
	{
		int sr=0;
		//short *re2=ReadWavFile("recorded2.wav",&index,&sr);
		system("pause");
	}	
	//wavReadResult results=wavReadResult(recordedSamples2,index,folderOrigin,times);
	if (WRITE_DATA_TO_FILE)
	{
#pragma warning( disable : 4996)		
		/*ofstream fout;
		fout.open("isspeech.txt");
		for(unsigned int i=0;i<isFrameSpeech.size();i++)
		{				
			if(isFrameSpeech[i]==true)
			{
				fout<<1<<endl;
			}else{
				fout<<0<<endl;
			}
		}	
		fout<<flush;fout.close();*/
	}	
#if WRITE_TO_FILE
	{
		if(!READFROMFILE){
			FILE  *fid;		
#pragma warning( disable : 4996)

			//fid = fopen(rawName.c_str(), "wb");
			//WriteWave(originFileName.c_str(),data.recordedSamples,numSamples, SAMPLE_RATE);
			WriteWave(smoothFileName.c_str(),recordedSamples2,index, SAMPLE_RATE);
			/*if( fid == NULL )
			{
				printf("Could not open file.");
			}
			else
			{
				for(int i=0;i<index;i++)
				{
					fprintf(fid,"%d ",recordedSamples2[i]);
				}
				fclose( fid );
				printf("Wrote data to %s %s %s\n",rawName.c_str(),
					originFileName.c_str(),smoothFileName.c_str());
			}*/
		}
	}
#endif

	/* Playback recorded data.  -------------------------------------------- */
	data.frameIndex = 0;	
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default output device.\n");
		return err;
	}
	outputParameters.channelCount = 1;                     /* stereo output */
	outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	printf("\n=== Now playing back. ===\n"); fflush(stdout);
	err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		playCallback,
		&data );
	if( err != paNoError )  return err;
	if( stream )
	{
		err = Pa_StartStream( stream );
		if( err != paNoError )  return err;
		printf("Waiting for playback to finish.\n"); fflush(stdout);

		while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
		if( err < 0 )  return err;

		err = Pa_CloseStream( stream );
		if( err != paNoError )  return err;

		printf((char*)"Done.\n"); fflush(stdout);
	}
	Pa_Terminate();
	if( data.recordedSamples )       /* Sure it is NULL or valid. */
		free( data.recordedSamples );
	if( recordedSamples2 )       /* Sure it is NULL or valid. */
		free( recordedSamples2 );
	if( err != paNoError )
	{
		fprintf( stderr, "An error occured while using the portaudio stream\n" );
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		err = 1;          /* Always return 0 or 1, but no other return codes. */
	}
	//system("pause");
	return err;
}

