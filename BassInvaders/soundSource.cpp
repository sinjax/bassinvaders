/*
 *  soundSource.cpp
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 12/04/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "soundSource.h"
int SoundSource::isInit = 0;

void SoundSource::init()
{
	if(!SoundSource::isInit)
	{
		SoundSource::isInit = 1;
		av_register_all();
	}
	// std::cout << "Here is this->packetQueue: "<< this->packetQueue << std::endl;
	// Initialise Packet Queue
	memset(&(this->packetQueue), 0, sizeof(PacketQueue));

}

SoundSource::SoundSource(char * filename){
	this->init();
	if(av_open_input_file(&(this->formatCtx),filename,NULL,0,NULL)!=0)
		throw -1;

	if(av_find_stream_info(this->formatCtx)<0)
		throw -1;
	this->audioStream = -1;
	for(int i = 0; i < this->formatCtx->nb_streams; i++)
	{
		if(this->formatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO)
		{
			this->audioStream = i;
			break;
		}
	}
	if(this->audioStream ==-1)
		throw -1;
	dump_format(formatCtx, 0, filename, 0);
	AVPacket packet;
	this->codecCtx=this->formatCtx->streams[this->audioStream]->codec;
	this->spec.freq = this->codecCtx->sample_rate;
	this->spec.channels = this->codecCtx->channels;
	this->spec.silence = 0;
	this->spec.samples = 1024;
	this->spec.userdata = this->codecCtx;

	this->codec=avcodec_find_decoder(this->codecCtx->codec_id);
	avcodec_open(this->codecCtx, this->codec);


	avcodec_flush_buffers(this->codecCtx);

	av_init_packet(&flush_pkt);
	flush_pkt.data = (uint8_t*)"FLUSH";
	this->addPacket(&flush_pkt);
	while(av_read_frame(this->formatCtx, &packet)>=0)
	{
		if(packet.stream_index==audioStream)
		{
			if(this->addPacket(&packet)<0){
				printf("Couldn't save packet!");
			}
		}
		else
		{
			av_free_packet(&packet);
		}
	}

}

int SoundSource::addPacket(AVPacket * pkt){
	AVPacketList *pkt1;
	if(pkt != &flush_pkt && av_dup_packet(pkt) < 0) {
		return -1;
	}
	pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	if (!pkt1)
		return -1;
	pkt1->pkt = *pkt;
	pkt1->next = NULL;

	if (!this->packetQueue.last_pkt)
		this->packetQueue.first_pkt = pkt1;
	else
		this->packetQueue.last_pkt->next = pkt1;
	this->packetQueue.last_pkt = pkt1;
	this->packetQueue.nb_packets++;
	this->packetQueue.size += pkt1->pkt.size;
	this->packetQueue.duration += pkt1->pkt.duration*this->spec.channels;
	return 0;
}


SoundSourceIterator * SoundSource::iter(int sampleLen = 1024)
{
	return new SoundSourceIterator(this,sampleLen);
}

SoundSourceIterator * SoundSource::iterBytes(int start,int stop,int sampleLen = 1024)
{
	return new SoundSourceIterator(this,start,stop,false,sampleLen);
}
SoundSourceIterator::SoundSourceIterator(SoundSource * source,int start,int stop,bool byTime, int sampleLen)
{
	if(!byTime){
		this->source = source;
		this->start = start < 0 ? 0 : start;
		this->end = stop < 0 || stop >= source->packetQueue.size ? source->packetQueue.size  : stop ;
		this->read = this->start;
		this->toRead = this->end;
	}
	this->currentPacketList = this->source->packetQueue.first_pkt;

	this->soundSample = new SoundSample();
	this->soundSample->sample = new uint8_t[sampleLen];
	this->soundSample->len = sampleLen;

}

SoundSourceIterator::SoundSourceIterator(SoundSource * source, int sampleLen)
{
	this->source = source;
	this->start = -1;
	this->end= -1;
	this->read = 0;
	this->toRead = source->packetQueue.duration;
	this->currentPacketList = this->source->packetQueue.first_pkt;

	this->soundSample = new SoundSample();
	this->soundSample->sample = new uint8_t[sampleLen];
	this->soundSample->len = sampleLen;
}

SoundSourceIterator::~SoundSourceIterator()
{
	delete this->soundSample->sample;
	delete this->soundSample;
}

int SoundSourceIterator::nextPacket(AVPacket *pkt, int block) {

	int ret;

	for(;;)
	{
		// If there is one in the queue
		AVPacketList *currentPacketList1 = this->currentPacketList;
		if (currentPacketList1)
		{
			// Set the first one to the next one
			this->currentPacketList = currentPacketList1->next;


			// Sort out the return, free the queue item
			*pkt = currentPacketList1->pkt;
			ret = 1;
			break;
		}
		else if (!block)
		{
			ret = 0;
			break;
		}
	}
	return ret;
}



int SoundSourceIterator::getSound(uint8_t *stream, int len, int start = -1, int end = -1)
{
	AVCodecContext *aCodecCtx = this->source->codecCtx;
	int len1, audio_size;
	static uint16_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
	static unsigned int audio_buf_size = 0;
	static unsigned int audio_buf_index = 0;
	static AVPacket pkt;
	static uint8_t *audio_pkt_data = NULL;
	static int audio_pkt_size = 0;
		while(len > 0) {
		if(audio_buf_index >= audio_buf_size) {
			/* We have already sent all our data; get more */

			// Old Audio Decode Frame
			// audio_size = audio_decode_frame(aCodecCtx, audio_buf,
			// sizeof(audio_buf)) ->
			// int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,
			// int buf_size) {;

			audio_size = -1;
			int len1=-100, data_size = 0, popRet;
			for(;;)
			{
				while(audio_pkt_size > 0)
				{
					data_size = sizeof(audio_buf);
					len1 = avcodec_decode_audio2(aCodecCtx,
												 (int16_t *)audio_buf,
												 &data_size,
												 audio_pkt_data,
												 audio_pkt_size);
					//printf("Got Length: %d\n",len1);
					if(len1 < 0) {
						/* if error, skip frame */
						audio_pkt_size = 0;
						break;
					}
					audio_pkt_data += len1;
					audio_pkt_size -= len1;
					if(data_size <= 0) {
						/* No data yet, get more frames */
						continue;
					}
					audio_size  = data_size;
					break;

				}
				if(audio_size < 0)
				{
					do{
						popRet = this->nextPacket(&pkt, 0) ;
					}while(start != -1 && (pkt.pos + pkt.size) <= start);


					if(popRet == 0 || (end != -1 && (pkt.pos + pkt.size) >= end))
					{
						audio_size  = 0;
						break;
					}
					else
					{
						//std::cout << "Got next Packet" << std::endl ;
					}
					audio_pkt_data = pkt.data;
					audio_pkt_size = pkt.size;
				}
				else
				{
					break;
				}
			}

			// Old Audio Decode Frame End
			//printf("Got audio size: %d\n",audio_size);
			if(audio_size == 0)
			{
				memset(audio_buf, 0, len);
				return 0;
			}
			if(audio_size < 0) {
				/* If error, output silence */
				audio_buf_size = 1024;
				memset(audio_buf, 0, audio_buf_size);
			} else {
				audio_buf_size = audio_size;
			}
			audio_buf_index = 0;
		}

		len1 = audio_buf_size - audio_buf_index;

		if(len1 == 0)
		{
			return 0;
		}

		if(len1 > len)
			len1 = len;
		memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
		len -= len1;
		stream += len1;
		audio_buf_index += len1;
	}
	return 0;
}

bool SoundSourceIterator::hasNext()
{
	return this->read < this->toRead;
}

SoundSample * SoundSourceIterator::next()
{
	this->getSound(this->soundSample->sample,this->soundSample->len,this->start,this->end);
	this->read+=this->soundSample->len;
	return this->soundSample;
}
/*
int main(int argc, char *argv[]){
	SoundSource * s = new SoundSource("/Users/dag/Documents/bassinvaders/jamesgames/08 Julie and Candy.mp3");
	SoundSourceIterator * iter;
	iter = s->iterBytes(44100*0*4,44100*2*4);
	while(iter->hasNext())
	{
		SoundSample * sample = iter->next();
		for(int i = 0; i < sample->len; i++)
		{
			std::cout << sample->sample[i];
		}
	}
	delete iter;
	iter = s->iter();
	while(iter->hasNext())
	{
		SoundSample * sample = iter->next();
		for(int i = 0; i < sample->len; i++)
		{
			std::cout << sample->sample[i];
		}
	}

}*/
