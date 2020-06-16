/*
Utility for read from stdin and write to stdout 
if stdout close last N bytes write to file
by Sergey Shckuliov sergey.sckuliov@gmail.com 2020/06/16
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv) {

	int buffer_size;
	int page_size;

	char* buffer;
	char* cbuffer;

	int64_t readed_bytes;
	
	char* template_dumpname = "%s.dump";

	if(argc < 3) {
		fprintf(stderr, "Usage: %s <page size> <buffer length> [template dump file]\n", argv[0]);
		fprintf(stderr, "Read from console <page size> and write to console.\n");
		fprintf(stderr, "If problem with write or read then dumped last <buffer length> to file <datetime>.dump\n");
		fprintf(stderr, "[template dump file] - %%s.dump\n");
		fprintf(stderr, "Written be Sergey Shckuliov 2020-06-16 sergey.sckuliov@gmail.com\n");
		return 1;
	}
	if(argc > 3) {
		template_dumpname = argv[3];
	}
	
	page_size = atoi(argv[1]);
	buffer_size = atoi(argv[2]);
	if(page_size<=0 || buffer_size<=0 || page_size>buffer_size) {
		fprintf(stderr, "<page size> must be greater <buffer size>\n");
		return 1;
	}
	if(page_size<=0 || buffer_size<=0 || page_size>buffer_size) {
		fprintf(stderr, "<page size> must be greater <buffer size>\n");
		return 1;
	}
	if(buffer_size % page_size) {
		fprintf(stderr, "<buffer size> must be multiple <page size>\n");
		return 1;
	}

	size_t readed = 0, writed = 0;
	readed_bytes = 0;
	buffer = malloc(buffer_size);
	cbuffer = buffer;	
	while((readed = fread(cbuffer, 1, page_size, stdin)) == page_size) {
		readed_bytes+=readed;
		writed = fwrite(cbuffer, 1, page_size, stdout);
		if(writed != page_size) {
			break;			
		}
		cbuffer = &buffer[readed_bytes % buffer_size];
	}
	if(readed != page_size) {
		readed_bytes+=readed;
		fprintf(stderr,"Readed: %d != %d\n",readed,page_size);
	} else if(writed != page_size) {
		fprintf(stderr,"Writed: %d != %d\n",writed,page_size);
	}
	fprintf(stderr,"Total read: %lld\n",readed_bytes);

	int max_filename = strlen(template_dumpname)+32;	
	char* filename = malloc(strlen(template_dumpname)+32);
	time_t rawtime;
	struct tm *info;
	char buffer_time[16];

	time( &rawtime );
	info = localtime( &rawtime );
    strftime(buffer_time, 16, "%Y%m%d%H%M%S", info);
    snprintf(filename,max_filename,template_dumpname,buffer_time);
	FILE* dump = fopen(filename,"wb");
	if(dump) {
		if(readed_bytes>buffer_size) {
			int start = readed_bytes % buffer_size;
			fwrite(&buffer[start], 1, buffer_size-start,dump);
			if(start>0) {
				fwrite(buffer,1, start,dump);
			}
		} else {
			fwrite(buffer,1,readed_bytes,dump);
		}
		fclose(dump);
	} else {
		fprintf(stderr,"Error open dump file: %s\n",filename);
	}
	free(filename);
	free(buffer);
	return 0;
}
