package com.example.dh2;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioTrack;
import android.util.Log;
import java.util.ArrayDeque;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/** One Android output owner for the retained shared VXN state segments. */
final class GameplayVxnMusic {
    private static final String TAG="DH2VxnMusic";
    private static final int RATE=32000, CHANNELS=VxnImaAdpcmDecoder.CHANNELS;
    private static final int BLOCK_BYTES=VxnImaAdpcmDecoder.BLOCK_BYTES;
    private static final int BLOCK_FRAMES=VxnImaAdpcmDecoder.BLOCK_FRAMES;
    private static final int FADE_FRAMES=RATE*2, MIX_FRAMES=1024;
    private static final int DYING_SLOT=0, OLD_SLOT=1, CURRENT_SLOT=2;
    private final Context context;
    private volatile boolean running, enabled;
    private volatile long generation;
    private volatile float volume=1f;
    private volatile AudioTrack track;
    private final ArrayDeque<SourceMusicResources.Resource> cueQueue=new ArrayDeque<>();
    private Thread worker;

    GameplayVxnMusic(Context context){this.context=context.getApplicationContext();}

    synchronized void selectState(int soundUid,String next,
                                  SourceMusicResources.Resource resource){
        validateState(next);
        SourceMusicResources.Resource resolved=SourceMusicResources.resolve(soundUid,next);
        if(resolved==null||!resolved.assetPath.equals(resource.assetPath)||
                resolved.payloadBytes!=resource.payloadBytes||
                resolved.sampleFrames!=resource.sampleFrames)
            throw new IllegalArgumentException("Unsupported source VXN UID/state resource");
        if(!running){
            running=true;final long token=++generation;
            worker=new Thread(()->mixAndStream(resource,token),"dh2-music-mixer");
            worker.setDaemon(true);worker.start();
            return;
        }
        cueQueue.addLast(resource);notifyAll();
    }
    synchronized void setPlaybackEnabled(boolean value){
        enabled=value;AudioTrack active=track;
        if(active!=null){
            try{
                if(value&&active.getPlayState()!=AudioTrack.PLAYSTATE_PLAYING)active.play();
                else if(!value&&active.getPlayState()==AudioTrack.PLAYSTATE_PLAYING)active.pause();
            }catch(IllegalStateException e){Log.e(TAG,"AudioTrack focus/lifecycle transition failed",e);}
        }
        notifyAll();
    }
    synchronized void setVolume(float value){
        volume=Math.max(0f,Math.min(1f,value));AudioTrack active=track;
        if(active!=null)active.setVolume(volume);
    }
    synchronized boolean isRunning(){return running&&worker!=null;}
    synchronized void stop(){
        running=false;enabled=false;++generation;AudioTrack active=track;track=null;
        if(active!=null){try{active.pause();active.flush();}catch(IllegalStateException ignored){}active.release();}
        if(worker!=null){worker.interrupt();worker=null;}
        cueQueue.clear();notifyAll();
    }

    private void mixAndStream(SourceMusicResources.Resource initial,long token){
        AudioTrack local=null;
        boolean trackTransferred=false;
        Voice[] voices=new Voice[VxnSegmentMixMath.SLOT_COUNT];
        int[] gains=new int[voices.length],steps=new int[voices.length];
        int[] fadeRemaining=new int[voices.length],fadeTargets=new int[voices.length];
        short[][] source=new short[voices.length][];
        short[][] spans=new short[voices.length][MIX_FRAMES*CHANNELS];
        short[] mixed=new short[MIX_FRAMES*CHANNELS];
        try{
            int min=AudioTrack.getMinBufferSize(RATE,AudioFormat.CHANNEL_OUT_STEREO,
                    AudioFormat.ENCODING_PCM_16BIT);
            if(min<=0)throw new IOException("AudioTrack has no 32 kHz stereo PCM16 output");
            local=new AudioTrack.Builder()
                .setAudioAttributes(new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME)
                    .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).build())
                .setAudioFormat(new AudioFormat.Builder().setEncoding(AudioFormat.ENCODING_PCM_16BIT)
                    .setSampleRate(RATE).setChannelMask(AudioFormat.CHANNEL_OUT_STEREO).build())
                .setBufferSizeInBytes(Math.max(min*2,32768))
                .setTransferMode(AudioTrack.MODE_STREAM).build();
            local.setVolume(volume);
            synchronized(this){if(!isActive(token))return;track=local;trackTransferred=true;if(enabled)local.play();}
            voices[CURRENT_SLOT]=Voice.open(context,initial);
            gains[CURRENT_SLOT]=VxnSegmentMixMath.Q30_ONE;
            Log.i(TAG,"Source music mixer active | UID="+initial.soundUid+" | state="+initial.state+
                    " | slots="+voices.length+" | fade_frames="+FADE_FRAMES);
            SourceMusicResources.Resource selected=initial;
            while(isActive(token)){
                awaitEnabled(token);if(!isActive(token))break;
                SourceMusicResources.Resource cue=takeCue(token);
                if(cue!=null&&!cue.sameSegment(selected)){
                    applyCue(cue,voices,gains,steps,fadeRemaining,fadeTargets);
                    selected=cue;
                }
                int activeFrames=0;
                for(int slot=0;slot<voices.length;slot++){
                    if(voices[slot]!=null){
                        voices[slot].readFrames(spans[slot],MIX_FRAMES);
                        source[slot]=spans[slot];activeFrames=MIX_FRAMES;
                    }else source[slot]=null;
                }
                if(activeFrames==0)throw new IOException("VXN mixer has no active segment slots");
                VxnSegmentMixMath.mix(source,activeFrames,gains,steps,fadeRemaining,fadeTargets,mixed);
                for(int slot=0;slot<voices.length;slot++){
                    if(voices[slot]!=null&&fadeRemaining[slot]==0&&fadeTargets[slot]==0&&gains[slot]==0){
                        voices[slot].close();voices[slot]=null;
                    }
                }
                int sent=0,total=activeFrames*CHANNELS;
                while(sent<total&&isActive(token)){
                    int written=local.write(mixed,sent,total-sent,AudioTrack.WRITE_BLOCKING);
                    if(written<=0)throw new IOException("AudioTrack.write failed: "+written);
                    sent+=written;
                }
            }
        }catch(Exception e){if(isActive(token))Log.e(TAG,"Source VXN mixer failed",e);}
        finally{
            for(Voice voice:voices)if(voice!=null)voice.close();
            boolean release=false;
            synchronized(this){
                if(track==local){track=null;release=true;}
                else if(!trackTransferred&&local!=null)release=true;
                if(worker==Thread.currentThread())worker=null;
                if(generation==token){running=false;enabled=false;cueQueue.clear();}
            }
            if(release)local.release();
        }
    }

    private SourceMusicResources.Resource takeCue(long token){
        synchronized(this){if(!isActive(token))return null;return cueQueue.pollFirst();}
    }
    private void applyCue(SourceMusicResources.Resource selection,Voice[] voices,int[] gains,int[] steps,
                          int[] fadeRemaining,int[] fadeTargets)throws IOException{
        // Crypt Grpe's PlaylistElement fields 1 and 2 are both zero. Therefore
        // GetNextDyingSegmentLifeState selects 2 and ApplyTransitionRule does
        // not swap old/current: UpdateSegmentsStates rotates dying <- old <- current.
        if(voices[DYING_SLOT]!=null)voices[DYING_SLOT].close();
        voices[DYING_SLOT]=null;gains[DYING_SLOT]=0;steps[DYING_SLOT]=0;
        fadeRemaining[DYING_SLOT]=0;fadeTargets[DYING_SLOT]=0;
        moveSlot(voices,OLD_SLOT,DYING_SLOT,gains,steps,fadeRemaining,fadeTargets);
        moveSlot(voices,CURRENT_SLOT,OLD_SLOT,gains,steps,fadeRemaining,fadeTargets);
        voices[CURRENT_SLOT]=Voice.open(context,selection);
        gains[CURRENT_SLOT]=0;fadeTargets[CURRENT_SLOT]=VxnSegmentMixMath.Q30_ONE;
        fadeRemaining[CURRENT_SLOT]=FADE_FRAMES;
        steps[CURRENT_SLOT]=VxnSegmentMixMath.slope(0,VxnSegmentMixMath.Q30_ONE,FADE_FRAMES);
        startFadeOut(DYING_SLOT,voices,gains,steps,fadeRemaining,fadeTargets);
        startFadeOut(OLD_SLOT,voices,gains,steps,fadeRemaining,fadeTargets);
        Log.i(TAG,"Source music transition | UID="+selection.soundUid+" | target="+selection.state+" | frames="+FADE_FRAMES+
                " | active_slots="+voices.length+" | fifo=one-cue-per-decode");
    }
    private static void moveSlot(Voice[] voices,int from,int to,int[] gains,int[] steps,
                                 int[] remaining,int[] targets){
        voices[to]=voices[from];voices[from]=null;
        gains[to]=gains[from];steps[to]=steps[from];remaining[to]=remaining[from];targets[to]=targets[from];
        gains[from]=steps[from]=remaining[from]=targets[from]=0;
    }
    private static void startFadeOut(int slot,Voice[] voices,int[] gains,int[] steps,
                                     int[] remaining,int[] targets){
        if(voices[slot]==null)return;
        targets[slot]=0;remaining[slot]=FADE_FRAMES;
        steps[slot]=VxnSegmentMixMath.slope(gains[slot],0,FADE_FRAMES);
    }
    private synchronized void awaitEnabled(long token)throws InterruptedException{
        while(isActive(token)&&!enabled)wait(100);
    }
    private synchronized boolean isActive(long token){return running&&generation==token;}
    private static void validateState(String state){
        if(!("ambient".equals(state)||"combat".equals(state)))
            throw new IllegalArgumentException("Unsupported source VXN state: "+state);
    }
    private static void readFully(FileInputStream input,byte[] block)throws IOException{
        int offset=0;while(offset<block.length){int read=input.read(block,offset,block.length-offset);
            if(read<0)throw new IOException("Truncated Crypt IMA payload");offset+=read;}
    }
    private static void validateHeader(FileInputStream input,long base,long length,
                                       SourceMusicResources.Resource resource)throws IOException{
        if(length!=60L+resource.payloadBytes)
            throw new IOException("Unexpected source IMA-WAV asset size: "+length);
        input.getChannel().position(base);byte[] header=new byte[60];
        int offset=0;while(offset<header.length){int n=input.read(header,offset,header.length-offset);
            if(n<0)throw new IOException("Truncated source IMA-WAV header");offset+=n;}
        ByteBuffer h=ByteBuffer.wrap(header).order(ByteOrder.LITTLE_ENDIAN);
        if(h.getInt(0)!=0x46464952||h.getInt(8)!=0x45564157||h.getInt(12)!=0x20746d66||
                h.getInt(16)!=20||Short.toUnsignedInt(h.getShort(20))!=17||
                Short.toUnsignedInt(h.getShort(22))!=CHANNELS||h.getInt(24)!=RATE||
                Short.toUnsignedInt(h.getShort(32))!=BLOCK_BYTES||
                Short.toUnsignedInt(h.getShort(34))!=4||
                Short.toUnsignedInt(h.getShort(36))!=2||
                Short.toUnsignedInt(h.getShort(38))!=BLOCK_FRAMES||h.getInt(40)!=0x74636166||
                h.getInt(44)!=4||h.getInt(48)!=resource.sampleFrames||h.getInt(52)!=0x61746164||
                h.getInt(56)!=resource.payloadBytes)
            throw new IOException("Source IMA-WAV framing differs from its retained music resource");
    }

    private static final class Voice {
        final String state;
        final int sampleFrames;
        final AssetFileDescriptor descriptor;
        final FileInputStream input;
        final byte[] encoded=new byte[BLOCK_BYTES];
        final short[] decoded=new short[BLOCK_FRAMES*CHANNELS];
        int decodedFrames,decodedAt,framesRemaining;
        private Voice(String state,int sampleFrames,AssetFileDescriptor descriptor,FileInputStream input){
            this.state=state;this.sampleFrames=sampleFrames;this.framesRemaining=sampleFrames;
            this.descriptor=descriptor;this.input=input;
        }
        static Voice open(Context context,SourceMusicResources.Resource resource)throws IOException{
            validateState(resource.state);
            AssetFileDescriptor fd=context.getAssets().openFd(resource.assetPath);
            FileInputStream input=new FileInputStream(fd.getFileDescriptor());
            try{validateHeader(input,fd.getStartOffset(),fd.getLength(),resource);
                input.getChannel().position(fd.getStartOffset()+60);
                return new Voice(resource.state,resource.sampleFrames,fd,input);
            }catch(IOException e){try{input.close();}finally{fd.close();}throw e;}
        }
        void readFrames(short[] output,int wanted)throws IOException{
            for(int frame=0;frame<wanted;frame++){
                if(decodedAt>=decodedFrames)loadBlock();
                output[frame*2]=decoded[decodedAt*2];output[frame*2+1]=decoded[decodedAt*2+1];
                decodedAt++;
            }
        }
        private void loadBlock()throws IOException{
            if(framesRemaining<=0){framesRemaining=sampleFrames;input.getChannel().position(descriptor.getStartOffset()+60);}
            readFully(input,encoded);
            decodedFrames=VxnImaAdpcmDecoder.decodeBlock(encoded,decoded,Math.min(framesRemaining,BLOCK_FRAMES));
            if(decodedFrames<=0)throw new IOException("VXN state segment ended without decoded frames");
            decodedAt=0;framesRemaining-=decodedFrames;
        }
        void close(){try{input.close();}catch(IOException ignored){}try{descriptor.close();}catch(IOException ignored){}}
    }
}
