package com.example.dh2;

/** Source-backed per-block VoxN IMA ADPCM decoder; output is interleaved PCM16. */
final class VxnImaAdpcmDecoder {
    static final int BLOCK_BYTES=1024, CHANNELS=2, BLOCK_FRAMES=1017;
    private static final int[] STEP={7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,
        34,37,41,45,50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,
        230,253,279,307,337,371,408,449,494,544,598,658,724,796,876,963,1060,
        1166,1282,1411,1552,1707,1878,2066,2272,2499,2749,3024,3327,3660,4026,
        4428,4871,5358,5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,
        15289,16818,18500,20350,22385,24623,27086,29794,32767};
    private static final int[] INDEX={-1,-1,-1,-1,2,4,6,8,-1,-1,-1,-1,2,4,6,8};

    private VxnImaAdpcmDecoder(){}

    /** Decodes one source block, preserving the VoxN channel-group interleave. */
    static int decodeBlock(byte[] input,short[] output,int framesWanted){
        if(input==null||input.length!=BLOCK_BYTES||output==null||
                output.length<BLOCK_FRAMES*CHANNELS||framesWanted<0||framesWanted>BLOCK_FRAMES)
            throw new IllegalArgumentException("Invalid VoxN IMA block buffers or frame limit");
        int[] predictor=new int[CHANNELS],index=new int[CHANNELS];
        for(int c=0;c<CHANNELS;c++){
            int off=c*4;predictor[c]=(short)((input[off]&0xff)|((input[off+1]&0xff)<<8));
            index[c]=input[off+2]&0xff;
            if(index[c]>88)throw new IllegalArgumentException("Invalid IMA step index: "+index[c]);
        }
        int frames=0;
        if(framesWanted>0){output[0]=(short)predictor[0];output[1]=(short)predictor[1];frames=1;}
        final int[] groupSamples=new int[CHANNELS*8];
        for(int group=0;group<127&&frames<framesWanted;group++){
            for(int c=0;c<CHANNELS;c++){
                int off=8+group*8+c*4;
                for(int b=0;b<4;b++){
                    int packed=input[off+b]&0xff;
                    for(int half=0;half<2;half++){
                        int nibble=half==0?packed&15:packed>>>4;
                        int step=STEP[index[c]],delta=step>>3;
                        if((nibble&4)!=0)delta+=step;
                        if((nibble&2)!=0)delta+=step>>1;
                        if((nibble&1)!=0)delta+=step>>2;
                        predictor[c]+=(nibble&8)!=0?-delta:delta;
                        predictor[c]=Math.max(-32768,Math.min(32767,predictor[c]));
                        index[c]=Math.max(0,Math.min(88,index[c]+INDEX[nibble]));
                        groupSamples[c*8+b*2+half]=predictor[c];
                    }
                }
            }
            for(int i=0;i<8&&frames<framesWanted;i++,frames++){
                output[frames*2]=(short)groupSamples[i];
                output[frames*2+1]=(short)groupSamples[8+i];
            }
        }
        return frames;
    }
}
