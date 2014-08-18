#include "Wumbo-Header.h"
#include <stdio.h>
#if PLATFORM_PC
#if PLATFORM_WINDOWS
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AVFoundation/AVAudioPlayer.h>
#endif
#include "stb_vorbis.h"
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include "FjordsSound.h"

const char *FJORDSRESOURCE(const char* _string)
{
	return _string;
}

//
//	Yeah so. I'm not commenting this.
//	This replicates the function calls
//	The GM version of FJORDS made to 
//	supersound.dll. Um. Maybe I should comment it.
//
//				Later though.
//



class ALBuffer;
std::map<std::string, ALBuffer*> mapo;

class ALBuffer
{
private:
	ALuint buffer;
	short *data;
	stb_vorbis *vorb;
public:
	ALBuffer()
	{
		buffer = 0;
		data = NULL;
		vorb = NULL;
	}
	~ALBuffer()
	{
		alDeleteBuffers(1, &buffer);
		stb_vorbis_close(vorb);
		delete data;
	}
	bool load(std::string fname)
	{
		//printf("PATH TO FILE IS %s\n",fname.c_str());
		if (mapo.find(std::string(fname)) != mapo.end())
		{
			buffer = 0;
			return true;
		}
		else
			mapo.insert(std::pair<std::string,ALBuffer*>(std::string(fname),this));
		
		int err = 0;
		//alPrintError(99);
		vorb = stb_vorbis_open_filename((char*)fname.c_str(), &err, NULL);
		if (err > 1)
		{
			printf("failed to open OGG: %s\n",fname.c_str());
			return false;
		}
		//printf("stb_vorbis error is %i\n",err);
		//alPrintError(8);
		stb_vorbis_info info = stb_vorbis_get_info(vorb);
		unsigned int number_of_samples = stb_vorbis_stream_length_in_samples(vorb) * info.channels;
		unsigned int number_of_shorts = number_of_samples * sizeof(ALshort);
		data = new short[number_of_samples];
		stb_vorbis_get_samples_short_interleaved(vorb, info.channels, data, number_of_shorts);
		//memset(data,0,number_of_shorts*sizeof(short));
		
		alGenBuffers((ALuint)1, &buffer);
		//alPrintError(7);
		alBufferData(buffer, (info.channels == 1)?AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, number_of_shorts, info.sample_rate);
		//alPrintError(42);
		return true;
	}
	ALuint getBuffer()
	{
		return buffer;
	}
};

std::string GetFilenameWithoutExtension(std::string str)
{
	int i = 0;
	for(i=str.length()-1;i>=0;i--)
		if (str.at(i) == '/')
			break;
	std::string l = str.substr(i);
	return l.substr(1,l.length()-5);
}

class FjordsSound
{
private:
	ALuint source;
	static int staticsourcecount;
public:
	std::string name;
#if PLATFORM_IOS
	AVAudioPlayer *audio;
#endif
	bool isMusic;
	FjordsSound()
	{
		source = 0;
		isMusic = false;
	}
#if PLATFORM_IOS
	bool loadMusic(const char *fnaame)
	{
		//return load(fname);
		char *fname = (char*)FJORDSRESOURCE(fnaame);
		int len = strlen(fname);
		fname[len-1] = 'a';
		fname[len-2] = '4';
		fname[len-3] = 'm';
		NSError *err = [[NSError alloc] init];
		NSLog(@"ATTEMPTING TO OPEN %s",fname);
		audio = [[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:fname]] error:&err];
		NSLog(@"Error: %@",[err description]);NSLog([[NSURL fileURLWithPath:[NSString stringWithUTF8String:fname]] absoluteString]);
		//[audio prepareToPlay]
		staticsourcecount++;
		printf("source = %i / %i\n",rand(),staticsourcecount);
		return true;
	}
#endif
	bool load(const char *fname)
	{
		fname = FJORDSRESOURCE(fname);
		ALuint buffer;
		bool retval = true;
		if (mapo.find(std::string(fname)) != mapo.end())
		{
			buffer = (mapo.find(std::string(fname)))->second->getBuffer();
			printf("OLD BUFFER: %s\n",fname);
		}
		else
		{
			ALBuffer *buffoon = new ALBuffer();
			retval = buffoon->load((std::string(fname)).c_str());
			if (!retval)
				printf("An error occurred while trying to load %s\n",fname);
			buffer = buffoon->getBuffer();
			mapo.insert(std::pair<std::string,ALBuffer*>(std::string(fname),buffoon));
			//printf("NEW BUFFER: %s\n",fname);
		}
		staticsourcecount++;
		alGenSources((ALuint)1, &source);
		printf("source = %i / %i\n",source,staticsourcecount);
		alSourcef(source, AL_PITCH, 1.f);
		alSourcef(source, AL_GAIN, 1.f);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		alSourcei(source, AL_LOOPING, AL_FALSE);
		alSourcei(source, AL_BUFFER, buffer);
		//alPrintError(9);
		name = GetFilenameWithoutExtension(std::string(fname));
		printf("\t\t\tMY NAME IS %s\n",name.c_str());
		return retval;
	}
	ALuint getSource()
	{
		return source;
	}
};
int FjordsSound::staticsourcecount;

ALCdevice *fjords_device;
ALCcontext *fjords_context;
double fjords_scalefactor;

double ss_init()
{
	fjords_scalefactor = 1;
#if 1//!PLATFORM_IOS
	fjords_device = alcOpenDevice(NULL);
	fjords_context = alcCreateContext(fjords_device, NULL);
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	
	ALfloat listenerPos[] = { 0.f, 0.f, 1.f};
	ALfloat listenerVel[] = { 0.f, 0.f, 0.f};
	
	alListenerfv(AL_POSITION, listenerPos);
	//alPrintError(11);
	// check for errors
	alListenerfv(AL_VELOCITY, listenerVel);
	//alPrintError(12);
	// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
	//alPrintError(13);
	// check for errors
	alcMakeContextCurrent(fjords_context);
	//alPrintError(14);
#endif
	
	return 1;
}

std::vector<FjordsSound> sounds;

std::string ss_getname(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return "";
	return sounds.at(num).name;
}

double ss_getnextsoundid()
{
	return sounds.size();
}

double ss_unload()
{
	sounds.clear();
	while(sounds.size() > 0)
	{
		alSourceStop(sounds.at(0).getSource());
		sounds.erase(sounds.begin());
	}
	
	while(mapo.size() > 0)
	{
		delete mapo.begin()->second;
		mapo.erase(mapo.begin());
	}
	
	alcMakeContextCurrent(NULL);
	alcDestroyContext(fjords_context);
	alcCloseDevice(fjords_device);
	return 1;
}



double fjords_setsoundscalefactor(double factor)
{
	if (factor > 0)
		fjords_scalefactor = factor;
	else
		fjords_scalefactor = 1;
	return 1;
}


double ss_setvol(double soundid, double volume);

double ss_loadsound(std::string fname)//char *fname)
{
	FjordsSound snd;
	if (!snd.load((char*)fname.c_str()))
		return -1;
	sounds.push_back(snd);
	ss_setvol(sounds.size()-1,10000*fjords_scalefactor);
	return sounds.size()-1;
}
double ss_loadmusic(std::string fname)
{
#if PLATFORM_IOS
	FjordsSound snd;
	snd.loadMusic((char*)fname.c_str());
	snd.isMusic = true;
	sounds.push_back(snd);
	return sounds.size()-1;
#else
	FjordsSound snd;
	if (!snd.load((char*)fname.c_str()))
		return -1;
	snd.isMusic = true;
	sounds.push_back(snd);
	ss_setvol(sounds.size()-1,10000);
	return sounds.size()-1;
#endif
}


double ss_playsound(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		[sounds.at(num).audio play];
		return 1;
	}
#endif
	alSourcei(sounds.at(num).getSource(), AL_LOOPING, AL_FALSE);
	alSourcePlay(sounds.at(num).getSource());
	
	ALenum errcode;
    if ((errcode = alGetError()) != AL_NO_ERROR)
		printf("OpenAL Error: %i\n", errcode);
	return 1;
}

double ss_isplaying(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		return [sounds.at(num).audio isPlaying];
	}
#endif
	ALint plays = 0;
	alGetSourcei(sounds.at(num).getSource(), AL_SOURCE_STATE, &plays);
	//printf("sound playing? ---> %i\n",plays == AL_PLAYING);
	if (plays == AL_PLAYING)
		return 1;
	else
		return 0;
}

double ss_stopall()
{
	unsigned int n = sounds.size();
	for(unsigned int i=0;i<n;i++)
		ss_stopsound(i);
	return 0;
}

double ss_loopsound(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		sounds.at(num).audio.numberOfLoops = -1;
		[sounds.at(num).audio play];
		return 1;
	}
#endif
	alSourcei(sounds.at(num).getSource(), AL_LOOPING, AL_TRUE);
	alSourcePlay(sounds.at(num).getSource());
	return 1;
}

double ss_islooping(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		return (sounds.at(num).audio.numberOfLoops < 0);
	}
#endif
	ALint plays = 0;
	alGetSourcei(sounds.at(num).getSource(), AL_SOURCE_STATE, &plays);
	if (plays == AL_PLAYING)
	{
		ALint frootloops = 0;
		alGetSourcei(sounds.at(num).getSource(), AL_LOOPING, &frootloops);
		if (frootloops == AL_TRUE)
			return 1;
		else
			return 0;
	}
	return 0;
}

double ss_stopsound(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		[sounds.at(num).audio stop];
		return 1;
	}
#endif
	alSourceStop(sounds.at(num).getSource());
	return 1;
}

double ss_setvol(double soundid, double volume)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
	if (volume < 0)
		volume = 0;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		sounds.at(num).audio.volume = volume/10000;
		return 1;
	}
#endif
	if (sounds.at(num).isMusic)
		alSourcef(sounds.at(num).getSource(), AL_GAIN, volume/10000);
	else
		alSourcef(sounds.at(num).getSource(), AL_GAIN, (fjords_scalefactor*volume)/10000);
	return 1;
}

double ss_getvol(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		return sounds.at(num).audio.volume;
	}
#endif
	ALfloat volztodawallz = 0;
	alGetSourcef(sounds.at(num).getSource(), AL_GAIN, &volztodawallz);
	if (sounds.at(num).isMusic)
		return volztodawallz*10000;
	else
		return (volztodawallz/fjords_scalefactor)*10000;
}

double ss_setfreq(double soundid, double alvinandthechipmunks)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
	if (alvinandthechipmunks < 0)
		alvinandthechipmunks = 0;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		NSLog(@"NOPE NOPE setfreq");
		return 1;
	}
#endif
	//printf("set freq to %i\n",(int)alvinandthechipmunks);
	alSourcef(sounds.at(num).getSource(), AL_PITCH, alvinandthechipmunks/44100.0);
	return 1;
}

double ss_setfreqAL(double soundid, double alvinandthechipmunks)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
	if (alvinandthechipmunks < 0)
		alvinandthechipmunks = 0;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		NSLog(@"NOPE NOPE setfreq");
		return 1;
	}
#endif
	//printf("set freq to %i\n",(int)alvinandthechipmunks);
	alSourcef(sounds.at(num).getSource(), AL_PITCH, alvinandthechipmunks);
	return 1;
}


double ss_getfreq(double soundid)
{
	int num = (int) soundid;
	if (num < 0 || num >= sounds.size())
		return -1;
#if PLATFORM_IOS
	if (sounds.at(num).isMusic)
	{
		NSLog(@"NOPE NOPE getfreq");
		return 0;
	}
#endif
	ALfloat alvinandthechipmunks = 0;
	alGetSourcef(sounds.at(num).getSource(), AL_PITCH, &alvinandthechipmunks);
	return alvinandthechipmunks*44100;
}