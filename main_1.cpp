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

int array_uyvy422(const char* in_yuv_folder, const char* out_yuv_file, int w, int h, int frame_num) {
	
	FILE *outPut = fopen(out_yuv_file, "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 2);
 
	for(int i = 1; i <= frame_num; i++) {
		string folder = in_yuv_folder;
        string fileName = folder + "frame_0_" + to_string(i) + ".raw";
		//printf("\n=========fileName==%s=======\n", fileName.c_str());
        FILE *inPut = fopen(fileName.c_str(), "rb+");
		fread(pic, 1, w*h*2, inPut);
		fwrite(pic, 1, w*h*2, outPut);
        fclose(inPut);
	}
 
	free(pic);
	fclose(outPut);
	return 0;
}

int main(int argc, char *argv[])
{
	char *in_yuv_folder = NULL;
    char *out_yuv_file = NULL;

	if (argc < 5) {
    	fprintf(stderr, "Usage: %s <input_folder out_yuv_file width height frame_num>, argc:%d\n",
                argv[0], argc);
		return 0;
	}
	//ffplay frame_0_10.raw -pix_fmt uyvy422 -s 720*480
	//./test ./after_frame/ out.yuv 720 480 100
	in_yuv_folder = argv[1];
	out_yuv_file = argv[2];
	int in_w = atoi(argv[3]);
	int in_h = atoi(argv[4]);
	int frame_num = atoi(argv[5]);
	//printf("\n======in_yuv_folder=%s,out_yuv_file=%s,in_w=%d,in_h=%d,frame_num=%d====\n", in_yuv_folder,
	//out_yuv_file, in_w, in_h, frame_num);
	printf("\n======array_uyvy422==start====\n");
	array_uyvy422(in_yuv_folder, out_yuv_file, in_w, in_h, frame_num);
	printf("\n======array_uyvy422==total frame=%d===\n", frame_num);
	printf("\n======array_uyvy422==end====\n");
	return 0;
}
