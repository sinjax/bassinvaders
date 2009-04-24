/*
 *  soundSource.h
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 12/04/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SOUNDSOURCE
#define SOUNDSOURCE
# include <iostream>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
typedef struct PacketQueue {
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	int duration;
} PacketQueue;
typedef struct AudioSpec{
	int freq;		/* DSP frequency -- samples per second */
	// Signed 16 bit int, as given back by avcodec_decode_audio
	// uint16_t format;		/* Audio data format */
	uint8_t  channels;	/* Number of channels: 1 mono, 2 stereo */
	uint8_t  silence;		/* Audio buffer silence value (calculated) */
	uint16_t samples;		/* Audio buffer size in samples (power of 2) */
	uint16_t padding;		/* Necessary for some compile environments */
	uint32_t size;		/* Audio buffer size in bytes (calculated) */
	void  *userdata;
} AudioSpec;

typedef struct SoundSample{
	uint8_t * sample;
	int len;
} SoundSample;

class SoundSource;

class SoundSourceIterator
{
public:
	SoundSourceIterator(SoundSource *,int);
	SoundSourceIterator(SoundSource *,int,int,bool,int);
	~SoundSourceIterator();
	bool hasNext();
	SoundSample * next();


	// Variables
	int start;
	int end;
	int read;
	int toRead;
	SoundSource * source;
	int getSound(uint8_t *stream, int len, int start, int end);
private:
	SoundSample * soundSample;
	int nextPacket(AVPacket *pkt, int block);
	AVPacketList *currentPacketList;

};
class SoundSource  {
	public:
		SoundSource(char * filename);
		~SoundSource();

		SoundSourceIterator * iter(int);
		/*
		 * Pointer to a stream containing sound
		 */
		SoundSourceIterator * iterTimes(
			int beginSeconds,
			int endSeconds,
			int sampleLen
		);
		/*
		 * Get sound between certain byte range
		 */
		SoundSourceIterator * iterBytes(
			 int ,
			 int ,
			 int
		);

	AVFormatContext * formatCtx;
	AVCodecContext  *codecCtx;
	AVCodec *codec;
	int audioStream;
	AudioSpec spec;
	PacketQueue packetQueue;

	private:
		void init();
		int addPacket(AVPacket *pkt);
		int audio_decode_frame(
			uint8_t *audio_buf,
			int buf_size
		);

		// Variables
		static int isInit;
	AVPacket flush_pkt;
};

#endif
