��򵥵Ļ���FFmpeg����Ƶ������
Simplest FFmpeg Video Encoder

������ Lei Xiaohua
leixiaohua1020@126.com
�й���ý��ѧ/���ֵ��Ӽ���
Communication University of China / Digital TV Technology
http://blog.csdn.net/leixiaohua1020
 
 
������ʵ����YUV�������ݱ���Ϊ��Ƶ������H.265��H264��MPEG2��VP8�ȵȣ���
����򵥵�FFmpeg��Ƶ���뷽��Ľ̡̳�
������������������Ŀ��
simplest_ffmpeg_video_encoder����򵥵Ļ���FFmpeg����Ƶ��������ʹ��libavcodec��libavformat���벢�ҷ�װ��Ƶ��
simplest_ffmpeg_video_encoder_pure����򵥵Ļ���FFmpeg����Ƶ������-�����档��ʹ��libavcodec������Ƶ����ʹ��libavformat��


This software encode YUV420P raw data to video bitstream (Such as H.264, H.265, VP8, MPEG2 etc).
It's the simplest video encoding software based on FFmpeg. 
It contains following project:
simplest_ffmpeg_video_encoder: Simplest video encoder. It uses libavcodec to encode video and uses libavformat to add container format to the video bitstream.
simplest_ffmpeg_video_encoder_pure: Pure video encoder. It only uses libavcodec to encode video (without libavformat).