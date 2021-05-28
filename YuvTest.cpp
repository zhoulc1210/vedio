#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
/**
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
}*/

using namespace std; 
int simplest_uyvy422(int w, int h,int num);

int main(int argc, char *argv[]) {
	
	int in_w = 720, in_h = 480;
	//int framenum = 200;
    int framenum = 100;
	
	simplest_uyvy422(in_w, in_h, framenum);
	
	return 0;
}

int simplest_uyvy422(int w, int h,int num) {
	
	FILE *outPut=fopen("./test_out_deinterlace.yuv", "wb+");
	unsigned char *pic=(unsigned char *)malloc(w * h * 2);
 
	for(int i = 1; i <= num; i++) {
        //string fileName = "camera/frame_0_" + to_string(i) + ".raw";
        string fileName = "deinterlace/frame_0_" + to_string(i) + ".raw";
        FILE *inPut=fopen(fileName.c_str(), "rb+");
		fread(pic, 1, w*h*2, inPut);
		fwrite(pic, 1, w*h*2, outPut);
        fclose(inPut);
	}
 
	free(pic);
	fclose(outPut);
	return 0;
}