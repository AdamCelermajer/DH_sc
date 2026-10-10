package com.example.dh2;

public final class VxnSegmentMixMathHostTest {
    private static void require(boolean value,String message){
        if(!value)throw new AssertionError(message);
    }
    public static void main(String[] args){
        int one=VxnSegmentMixMath.Q30_ONE;
        require(VxnSegmentMixMath.slope(0,one,64000)==16777,"source 2 second Q30 fade slope");
        require(VxnSegmentMixMath.slope(one,0,64000)==-16777,"source 2 second fade-out slope");

        short[] constant={1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
        short[][] tracks={constant,null,null};
        int[] gain={0,0,0},step={VxnSegmentMixMath.slope(0,one,4),0,0};
        int[] remaining={4,0,0},target={one,0,0};short[] output=new short[constant.length];
        VxnSegmentMixMath.mix(tracks,5,gain,step,remaining,target,output);
        int[] fadeIn={0,250,500,750,1000};
        for(int i=0;i<fadeIn.length;i++)require(output[2*i]==fadeIn[i],"fade-in sample "+i);
        require(gain[0]==one&&remaining[0]==0,"fade-in reaches exact unity endpoint");

        gain[0]=one;step[0]=VxnSegmentMixMath.slope(one,0,4);remaining[0]=4;target[0]=0;
        VxnSegmentMixMath.mix(tracks,5,gain,step,remaining,target,output);
        int[] fadeOut={1000,750,500,250,0};
        for(int i=0;i<fadeOut.length;i++)require(output[2*i]==fadeOut[i],"fade-out sample "+i);

        short[] hot={20000, -20000};short[][] three={hot,hot,hot};
        int[] unity={one,one,one},zeroStep={0,0,0},zeroRemaining={0,0,0},zeroTargets={0,0,0};
        short[] clipped=new short[2];
        VxnSegmentMixMath.mix(three,1,unity,zeroStep,zeroRemaining,zeroTargets,clipped);
        require(clipped[0]==32767&&clipped[1]==-32768,"three-slot sum saturates after mixing");
        System.out.println("VoxN segment mix math: 14 reference assertions passed");
    }
}
