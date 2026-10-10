package com.example.dh2;

public final class VxnImaAdpcmDecoderHostTest {
    private static void require(boolean value,String message){
        if(!value)throw new AssertionError(message);
    }
    public static void main(String[] args){
        byte[] block=new byte[VxnImaAdpcmDecoder.BLOCK_BYTES];
        short[] pcm=new short[VxnImaAdpcmDecoder.BLOCK_FRAMES*2];
        block[0]=100;block[2]=0;block[4]=(byte)0x9c;block[5]=(byte)0xff;block[6]=0;
        int frames=VxnImaAdpcmDecoder.decodeBlock(block,pcm,1017);
        require(frames==1017,"full source block frame count");
        for(int i=0;i<frames;i++)
            require(pcm[2*i]==100&&pcm[2*i+1]==-100,"zero-nibble stereo seed preservation at "+i);

        block[8]=0x71;
        frames=VxnImaAdpcmDecoder.decodeBlock(block,pcm,4);
        require(frames==4,"truncated final-block cap");
        require(pcm[0]==100&&pcm[1]==-100,"channel predictor seeds are interleaved first");
        require(pcm[2]==101&&pcm[3]==-100,"low nibble is decoded before high nibble");
        require(pcm[4]==112&&pcm[5]==-100,"source IMA step/index update is applied");
        require(pcm[6]==114&&pcm[7]==-100,"channel group layout is preserved");
        System.out.println("VoxN IMA block decoder: 6 cases passed");
    }
}
