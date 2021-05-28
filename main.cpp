#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
}

using namespace std; 

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile)
{
	int ret;
	if (frame)
		printf("Send frame %ld\n", frame->pts);
	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0)
	{
		fprintf(stderr, "Error sending a frame for encoding\n");
		exit(1);
	}
	while (ret >= 0)
	{
		ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0)
		{
			fprintf(stderr, "Error during encoding\n");
			exit(1);
		}
		printf("Write packet %ld(size=%5d)\n", pkt->pts, pkt->size);
		fflush(stdout);
		fwrite(pkt->data, 1, pkt->size, outfile);
		av_packet_unref(pkt);
	}
}
int EncodeYUVToH264(const char* out_fileName, int in_w, int in_h, int framenum)
{
	// 打开输入YUV序列
	FILE *in_file = NULL;
	int ret = 0;

	// 打开输出视频文件
	FILE *out_file = NULL;
	out_file = fopen(out_fileName, "wb");

	// 初始化AVFormatContext结构体,根据文件名获取到合适的封装格式
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_fileName);
	const AVOutputFormat *fmt = pFormatCtx->oformat;

	// 初始化视频码流
	AVStream *video_st = avformat_new_stream(pFormatCtx, 0);
	if (video_st == NULL)
	{
		printf("failed allocating output stream\n");
		return -1;
	}
	video_st->time_base.num = 1;
	video_st->time_base.den = 25;

	// 编码器Context设置参数
	AVCodecContext *pCodecCtx = avcodec_alloc_context3(NULL);
	avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[0]->codecpar);
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUV422P;
	pCodecCtx->width = in_w;
	pCodecCtx->height = in_h;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;
	pCodecCtx->bit_rate = 400000;
	pCodecCtx->gop_size = 12;
	if (pCodecCtx->codec_id == AV_CODEC_ID_H264)
	{
		pCodecCtx->qmin = 10;
		pCodecCtx->qmax = 51;
		pCodecCtx->qcompress = 0.6;
	}
	if (pCodecCtx->codec_id == AV_CODEC_ID_MPEG2VIDEO)
		pCodecCtx->max_b_frames = 2;
	if (pCodecCtx->codec_id == AV_CODEC_ID_MPEG1VIDEO)
		pCodecCtx->mb_decision = 2;

	// 寻找编码器 
	const AVCodec *pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
	if (!pCodec)
	{
		printf("no right encoder!\n");
		return -1;
	}

	// 打开编码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("open encoder fail!\n");
		return -1;
	}

	// 输出格式信息
	av_dump_format(pFormatCtx, 0, out_fileName, 1);

	// 初始化帧
	AVFrame *pictureFrame = av_frame_alloc();
	pictureFrame->width = pCodecCtx->width;
	pictureFrame->height = pCodecCtx->height;
	pictureFrame->format = pCodecCtx->pix_fmt;

	// ffmpeg4.0
	int size = av_image_get_buffer_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, 1);
	uint8_t *picture_buf = (uint8_t *)av_malloc(size);
	printf("======size=%d\n", size);
	av_image_fill_arrays(pictureFrame->data, pictureFrame->linesize, picture_buf, AV_PIX_FMT_YUV422P, pCodecCtx->width, pCodecCtx->height, 1);
	printf("=====av_image_fill_arrays=========\n");
	// 写头文件
	pFormatCtx->streams[0]->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
	pFormatCtx->streams[0]->codecpar->width = in_w;
    pFormatCtx->streams[0]->codecpar->height = in_h;
	ret = avformat_write_header(pFormatCtx, NULL);
	printf("=====avformat_write_header=======ret==%d\n", ret);
	// 创建已编码帧
	AVPacket *pkt = av_packet_alloc();
	if (!pkt)
	{
		return -1;
	}
	printf("=====av_packet_alloc=======OK====\n");
	// 编码器Context大小
	int y_size = pCodecCtx->width * pCodecCtx->height;
	printf("=====pCodecCtx=======OK====\n");
	//循环每一帧
	for (int i = 1; i < 100; i++)
	{
		string fileName = "camera/frame_0_" + to_string(i) + ".raw";
		in_file = fopen(fileName.c_str(), "rb");
		printf("fileName=%s\n", fileName.c_str());
		if (!in_file)
		{
			printf("can not open file!\n");
			return -1;
		}
		// 读入YUV
		// if (fread(picture_buf, 1, y_size * 3 / 2, in_file) <= 0)
		// {
		// 	printf("read file fail!\n");
			
		// 	return -1;
		// }
		//读入YUV
		if (fread(picture_buf, 1, y_size * 2, in_file) <= 0)
		{
			printf("read file fail!\n");
			
			return -1;
		}
		else if (feof(in_file))
		{
			break;
		}
		// yuv420P
		// pictureFrame->data[0] = picture_buf; // Y
		// pictureFrame->data[1] = picture_buf + y_size; // U
		// pictureFrame->data[2] = picture_buf + y_size * 5 / 4;// V

		//UYVY422
		int j = 0;
		int k = 0;
	    for (int m = 0; m < y_size * 2; m = m + 4) {
			pictureFrame->data[0][j] = picture_buf[m + 1];
			pictureFrame->data[0][j + 1] = picture_buf[m + 3];
			pictureFrame->data[1][k] = picture_buf[m];
			pictureFrame->data[2][k] = picture_buf[m + 2];
			j += 2;
			k++;
		}
		pictureFrame->pts = i;
		// encode the image
		encode(pCodecCtx, pictureFrame, pkt, out_file);
		fclose(in_file);
	}
	// flush the encoder 
	encode(pCodecCtx, NULL, pkt, out_file);
	av_write_trailer(pFormatCtx);
	// 释放内存
	av_frame_free(&pictureFrame);
	av_packet_free(&pkt);
	avcodec_free_context(&pCodecCtx);
	av_free(picture_buf);
	avformat_free_context(pFormatCtx);
	fclose(out_file);
	return 0;
}

int main(int argc, char *argv[])
{
	// 输入YUV宽度和高度、帧数
	// int in_w = 640, in_h = 480;
	int in_w = 720, in_h = 480;
	int framenum = 300;
	// 输出H264文件
	const char* out_fileName = "./test_out.h264";
	if (EncodeYUVToH264(out_fileName, in_w, in_h, framenum) < 0)
		printf("encode failed!\n");
	printf("encode finished!\n");
	return getchar();
}
