-- Root runs only against a real captured trace. No absent slice means zero.
-- First inspect exact per-thread slice names before aggregating EGL/driver work.
SELECT t.tid,t.name AS thread_name,s.name,count(*) AS samples,
       round(avg(s.dur)/1e6,3) AS mean_ms,round(max(s.dur)/1e6,3) AS max_ms
FROM slice s JOIN thread_track tt ON tt.id=s.track_id
JOIN thread t ON t.utid=tt.utid JOIN process p ON p.upid=t.upid
WHERE p.name='com.example.dh2' AND s.dur>=0
  AND (s.name LIKE '%DrawFrame%' OR s.name LIKE '%eglSwap%'
    OR s.name LIKE '%dequeueBuffer%' OR s.name LIKE '%queueBuffer%'
    OR s.name LIKE 'DH2:%')
GROUP BY t.tid,t.name,s.name ORDER BY max_ms DESC;

-- Running vs Runnable/preempted vs sleeping/blocked remain distinct evidence.
-- io_wait/blocked_function availability depends on actual recorded sources.
SELECT t.tid,t.name,st.state,st.io_wait,st.blocked_function,
       round(sum(st.dur)/1e6,3) AS total_ms,count(*) AS intervals
FROM thread_state st JOIN thread t ON t.utid=st.utid
JOIN process p ON p.upid=t.upid
WHERE p.name='com.example.dh2' AND st.dur>=0
GROUP BY t.tid,t.name,st.state,st.io_wait,st.blocked_function
ORDER BY total_ms DESC;

-- Check capture loss/unsupported sources before conclusions.
SELECT name,idx,value FROM stats
WHERE value>0 AND (name LIKE '%lost%' OR name LIKE '%error%'
  OR name LIKE '%overrun%' OR name LIKE '%discard%');

-- OPTIONAL Android12+ FrameTimeline; run only if the table/source is present.
-- Distinguish SurfaceView game layer from Activity/HWUI window layers in UI.
SELECT layer_name,present_type,jank_type,count(*) AS frames,
       round(avg(dur)/1e6,3) AS mean_duration_ms
FROM actual_frame_timeline_slice WHERE dur>=0
GROUP BY layer_name,present_type,jank_type ORDER BY frames DESC;
