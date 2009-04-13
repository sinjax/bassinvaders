/*
 A possible design:
 - Sound Engine
 -- Sound Source
		e.g: getFromTo() <-- time, byte range, empty == all
 -- Sound Analyser
		e.g: analyse() <-- given a time period to analyse something
 --- Sound Analysis
		e.g: The result of an analysis <-- given some time to analyse something
 -- Sound Sink
		e.g.: Register for sound, given it via callback
 
 Sound Engine holds all else
 Initialise a Source, sources can give sound data. Have a concept of
 "ready for sink"
 Analysers have constant random access
 Sinks are given sound from a point and get it sequentially
 
 The engine knows 
 - what Source data is available to sinks
 - what Analysis data is available for any given time period
 - can broadcast this data on an event model or can just hand it over!
 
 */
# include <stdio.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#define INBUF_SIZE 1024
#define ALIGN(addr,size) (uint8_t *) (((uint32_t) (((uint8_t *) (addr)) + (size) - 1)) & ~((size) - 1))
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

typedef struct PacketQueue {
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
} PacketQueue;

int quit = 0;
// Initialise the queue
void packet_queue_init(PacketQueue *q) 
{
	memset(q, 0, sizeof(PacketQueue));
}
PacketQueue audioq;

// Get a new AV packet, and put it in the queue of packets
int packet_queue_put(PacketQueue *q, AVPacket *pkt) {
	
	AVPacketList *pkt1;
	if(av_dup_packet(pkt) < 0) {
		return -1;
	}
	pkt1 = av_malloc(sizeof(AVPacketList));
	if (!pkt1)
		return -1;
	pkt1->pkt = *pkt;
	pkt1->next = NULL;
	
	if (!q->last_pkt)
		q->first_pkt = pkt1;
	else
		q->last_pkt->next = pkt1;
	q->last_pkt = pkt1;
	q->nb_packets++;
	q->size += pkt1->pkt.size;
	return 0;
}

int packet_queue_pop(PacketQueue *q, AVPacket *pkt, int block) {
	AVPacketList *pkt1;
	int ret;
		
	for(;;) {
		
		if(quit) {
			ret = -1;
			break;
		}
		
		pkt1 = q->first_pkt;
		// If there is one in the queue
		if (pkt1) {
			// Set the first one to the next one
			q->first_pkt = pkt1->next;
			
			if (!q->first_pkt)
				q->last_pkt = NULL;
			
			// Sort out the queue sizes
			q->nb_packets--;
			q->size -= pkt1->pkt.size;
			
			// Sort out the return, free the queue item
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		} else if (!block) {
			ret = 0;
			break;
		} 
	}
	return ret;
}

int packet_queue_peek(PacketQueue *q, AVPacket *pkt, int block) {
	AVPacketList *pkt1;
	int ret;
	
	for(;;) {
		
		if(quit) {
			ret = -1;
			break;
		}
		
		pkt1 = q->first_pkt;
		// If there is one in the queue
		if (pkt1) {
			*pkt = pkt1->pkt;
			ret = 1;
			break;
		} else if (!block) {
			ret = 0;
			break;
		} 
	}
	return ret;
}

int decode_interrupt_cb(void) {
	return quit;
}

int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,
                       int buf_size) {
	
	static AVPacket pkt;
	static uint8_t *audio_pkt_data = NULL;
	static int audio_pkt_size = 0;
	
	int len1=-100, data_size = 0, popRet;
	for(;;) {
		while(audio_pkt_size > 0) {
			data_size = buf_size;
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
			
			/* We have data, return it and come back for more later */
			return data_size;
		}
		
		if(pkt.data)
			av_free_packet(&pkt);
		
		if(quit) 
		{
			return -1;
		}
		popRet = packet_queue_pop(&audioq, &pkt, 0) ;
		if(popRet == 0) 
		{
			return 0;
		}
		else
		{
			//printf("Got next Packet\n");
		}
		audio_pkt_data = pkt.data;
		audio_pkt_size = pkt.size;
	}
}

void audio_callback(void *userdata, uint8_t *stream, int len) {
	AVCodecContext *aCodecCtx = (AVCodecContext *)userdata;
	int len1, audio_size;
	
	static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
	static unsigned int audio_buf_size = 0;
	static unsigned int audio_buf_index = 0;
	
	while(len > 0) {
		if(audio_buf_index >= audio_buf_size) {
			/* We have already sent all our data; get more */
			audio_size = audio_decode_frame(aCodecCtx, audio_buf,
											sizeof(audio_buf));
			//printf("Got audio size: %d\n",audio_size);
			if(audio_size == 0)
			{
				memset(audio_buf, 0, len);
				return;
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
			return;
		}
		
		if(len1 > len)
			len1 = len;
		memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
		len -= len1;
		stream += len1;
		audio_buf_index += len1;
	}
}

int main(int argc, char *argv[])
{
	AVFormatContext * pFormatCtx;
	AVCodecContext  *pCodecCtx;
	AVCodec *pCodec;
	AVPacket packet;
	int i, audioStream;
	AudioSpec spec;
	
	url_set_interrupt_cb(decode_interrupt_cb); 
	av_register_all();
	if(av_open_input_file(&pFormatCtx,argv[1],NULL,0,NULL)!=0)
		return -1;
	
	if(av_find_stream_info(pFormatCtx)<0)
		return -1;
	audioStream = -1;
	for(i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO)
		{
			audioStream = i;
			break;
		}
	}
	if(audioStream ==-1)
		return -1;
	
	pCodecCtx=pFormatCtx->streams[audioStream]->codec;
	spec.freq = pCodecCtx->sample_rate;
	spec.channels = pCodecCtx->channels;
	spec.silence = 0;
	spec.samples = 1024;
	spec.userdata = pCodecCtx;
	dump_format(pFormatCtx, 0, argv[1], 0);
	//printf("Sample Rate: %d\n""Channels: %d\n",spec.freq,spec.channels);
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	avcodec_open(pCodecCtx, pCodec);
	packet_queue_init(&audioq);
	while(av_read_frame(pFormatCtx, &packet)>=0) 
	{
		if(packet.stream_index==audioStream) 
		{
			if(packet_queue_put(&audioq, &packet)<0){
				printf("Couldn't save packet!");
			}
		} 
		else 
		{
			av_free_packet(&packet);
		}
	}
	//printf("Queue current number of packets: %d\n",audioq.nb_packets);
	//printf("Queue current size: %d\n",audioq.size);
	uint8_t bees[spec.samples];
	//int total = audioq.size*4 ;
	int minValue = 0;
	int maxValue = 0;
	//printf("Total: %d\n",total);
	while(audioq.nb_packets > 0)
	{
		int i = 0;
		int toRead = spec.samples;
		/*if(toRead  >= total)
		{
			toRead = total;
		}
		total -= toRead;*/
		audio_callback((void *)pCodecCtx,bees, toRead );
		//printf("Read: %d\n",toRead);
		for(i = 0; i < toRead ; i++)
		{
			//printf("%d,",bees[i]);
				putchar(bees[i]);
			if(bees[i] < minValue)
			{
				minValue = bees[i] ;
			}
			
			if(bees[i] > maxValue)
			{
				maxValue = bees[i];
			}
			//printf("%c",(bees[i] >> 0) & 0xff);
		}
		/*printf("Min value: %d\n",minValue);
		printf("Max value: %d\n",maxValue);
		printf("Total read: %d\n",total );
		printf("nbpackets: %d\n",audioq.nb_packets );*/
	}
	
	
	
	return 0;
}
