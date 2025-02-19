// Copyright (C) 2020 David Helkowski
// Anti-Corruption License

#import <ReplayKit/ReplayKit.h>
#include "nng/nng.h"
#include "nng/protocol/pipeline0/push.h"
#include "nng/protocol/pipeline0/pull.h"
#include "nng/protocol/reqrep0/rep.h"
#include "nng/protocol/reqrep0/req.h"
#include "ujsonin/ujsonin.h"

typedef struct fp_msg_base_s {
    int type;
} fp_msg_base;

typedef struct fp_msg_text_s {
    int type; // 1
    char *text;
} fp_msg_text;

typedef struct fp_msg_buffer_s {
    int type; // 2
    CMSampleBufferRef sampleBuffer;
    long frameNum;
} fp_msg_buffer;

typedef struct fp_msg_port_s {
    int type; // 3
    int port;
    char *ip;
} fp_msg_port;

void mynano__send_jpeg( nng_socket push, unsigned char *data, unsigned long dataLen, int ow, int oh, int dw, int dh );
void mynano__send_text( nng_socket push, const char *text );

@interface SampleHandler : RPBroadcastSampleHandler

@property long frameNum;
@property nng_socket pushF;
@property int pushOk;
@property int started;
@property id framePasserInst;
@property id controlThreadInst;
@property fp_msg_buffer msgBuffer;

@property int outputPort;
@property int inputPort;
@property char *outputIp;
@property int controlPort;

@end

@interface FramePasser : NSObject
-(FramePasser *)init:(int)inputPort outputIp:(char*)outputIp outputPort:(int)outputPort;
-(void)dealloc;
-(void)startSending;
-(void)stopSending;
-(void)oneFrame;
-(void)entry:(id)param;
-(void)handle_buffer:(fp_msg_buffer *)msg wContext:(CIContext *)context;

@property CIContext *context;
@property int outputPort;
@property int inputPort;
@property int w;
@property int h;
@property char *outputIp;
@property nng_socket push;
@property bool sending;
@property uint16_t forceFrame1;
@property uint16_t *forceFrame2;
@property uint32_t *crc;
@property uint32_t *crc2;
@property size_t destW;
@property size_t destH;
@property float scale;
@property CIFilter *ciFilter;
@end

@interface ControlThread : NSObject
-(ControlThread *)init:(int)controlPort framePasser:(id)framePasser;
-(void)dealloc;
-(void)entry:(id)param;
@property int controlPort;
@property FramePasser *framePasser;
@property nng_socket rep;
@end
