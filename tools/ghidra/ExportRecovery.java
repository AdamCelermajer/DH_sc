// Export machine-derived pseudocode, with explicit per-function success/failure records.
// @category Recovery
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.util.task.TaskMonitor;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class ExportRecovery extends GhidraScript {
    private static String json(String value) {
        if (value == null) return "null";
        StringBuilder b = new StringBuilder("\"");
        for (char c : value.toCharArray()) {
            switch(c) {
                case '\\': b.append("\\\\"); break;
                case '"': b.append("\\\""); break;
                case '\n': b.append("\\n"); break;
                case '\r': b.append("\\r"); break;
                case '\t': b.append("\\t"); break;
                default: if (c < 32) b.append(String.format("\\u%04x", (int)c)); else b.append(c);
            }
        }
        return b.append('"').toString();
    }
    private static class Row {
        Function function; String code; String error; boolean success;
        Row(Function f) { function = f; }
    }
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if(args.length < 1) throw new IllegalArgumentException("ExportRecovery.java OUTPUT_DIR [WORKERS] [TIMEOUT_SECONDS]");
        Path out = Paths.get(args[0]).resolve(currentProgram.getName());
        Files.createDirectories(out);
        try(var oldFiles=Files.list(out)) {
            for(Path file:oldFiles.filter(p->p.getFileName().toString().matches("functions-[0-9]+\\.pseudo\\.c") || p.getFileName().toString().equals("function-index.jsonl") || p.getFileName().toString().equals("summary.json")).toList()) Files.delete(file);
        }
        int workers = args.length > 1 ? Integer.parseInt(args[1]) : 6;
        int timeout = args.length > 2 ? Integer.parseInt(args[2]) : 30;
        List<Function> functions = new ArrayList<>();
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        while(it.hasNext()) {
            Function f=it.next();
            var block=currentProgram.getMemory().getBlock(f.getEntryPoint());
            if(!f.isExternal() && block!=null && block.isExecute()) functions.add(f);
        }
        println("RECOVERY_EXPORT_BEGIN " + currentProgram.getName() + " functions=" + functions.size());
        AtomicInteger done=new AtomicInteger(), good=new AtomicInteger();
        List<DecompInterface> interfaces = Collections.synchronizedList(new ArrayList<>());
        ThreadLocal<DecompInterface> local = ThreadLocal.withInitial(() -> {
            DecompInterface d = new DecompInterface();
            DecompileOptions opts = new DecompileOptions();
            opts.grabFromProgram(currentProgram);
            d.setOptions(opts); d.toggleSyntaxTree(false); d.toggleCCode(true);
            d.setSimplificationStyle("decompile");
            if(!d.openProgram(currentProgram)) throw new IllegalStateException(d.getLastMessage());
            interfaces.add(d); return d;
        });
        ExecutorService pool=Executors.newFixedThreadPool(workers);
        List<Future<Row>> pending=new ArrayList<>();
        for(Function f : functions) pending.add(pool.submit(() -> {
            Row row=new Row(f);
            try {
                DecompileResults result=local.get().decompileFunction(f,timeout,TaskMonitor.DUMMY);
                row.success=result.decompileCompleted() && result.getDecompiledFunction()!=null;
                row.error=result.getErrorMessage();
                if(row.success) { row.code=result.getDecompiledFunction().getC(); good.incrementAndGet(); }
            } catch(Exception e) { row.error=e.toString(); }
            int n=done.incrementAndGet();
            if(n % 1000 == 0) println("RECOVERY_PROGRESS " + currentProgram.getName() + " " + n + "/" + functions.size() + " success=" + good.get());
            return row;
        }));
        pool.shutdown();
        long bytes=0; int chunk=-1, attempted=0, success=0; BufferedWriter code=null;
        String shard="";
        try(BufferedWriter index=Files.newBufferedWriter(out.resolve("function-index.jsonl"),StandardCharsets.UTF_8)) {
            for(Future<Row> future:pending) {
                Row r=future.get(); attempted++;
                if(code==null || bytes > 1024*1024) {
                    if(code!=null) code.close();
                    shard=String.format("functions-%03d.pseudo.c",++chunk);
                    code=Files.newBufferedWriter(out.resolve(shard),StandardCharsets.UTF_8);
                    String header="/* AUTOMATIC RECOVERY: Ghidra " + getGhidraVersion() + "; " + currentProgram.getName() + ".\n * This is unvalidated pseudocode, not buildable original C/C++.\n * See function-index.jsonl and original symbol/assembly inventories.\n */\n";
                    code.write(header); bytes=header.getBytes(StandardCharsets.UTF_8).length;
                }
                String label="/* address="+r.function.getEntryPoint()+" symbol="+r.function.getName(true).replace("*/","* /")+" */\n";
                String body=r.success ? r.code : "/* DECOMPILATION_FAILED: "+String.valueOf(r.error).replace("*/","* /")+" */\n";
                String text=label+body+"\n";
                long start=bytes; code.write(text); bytes+=text.getBytes(StandardCharsets.UTF_8).length;
                if(r.success) success++;
                index.write("{\"address\":"+json(r.function.getEntryPoint().toString())+",\"elf_address\":"+json(Long.toHexString(r.function.getEntryPoint().subtract(currentProgram.getImageBase())))+",\"name\":"+json(r.function.getName(true))+",\"signature\":"+json(r.function.getPrototypeString(false,false))+",\"body_bytes\":"+r.function.getBody().getNumAddresses()+",\"is_thunk\":"+r.function.isThunk()+",\"has_warning\":"+(r.code!=null && r.code.contains("WARNING:"))+",\"success\":"+r.success+",\"error\":"+json(r.error)+",\"file\":"+json(shard)+",\"byte_offset\":"+start+",\"byte_length\":"+(bytes-start)+"}\n");
            }
        } finally { if(code!=null) code.close(); for(DecompInterface d:interfaces) d.dispose(); pool.shutdownNow(); }
        String summary="{\"library\":"+json(currentProgram.getName())+",\"ghidra_version\":"+json(getGhidraVersion())+",\"language\":"+json(currentProgram.getLanguageID().toString())+",\"image_base\":"+json(currentProgram.getImageBase().toString())+",\"attempted\":"+attempted+",\"success\":"+success+",\"failed\":"+(attempted-success)+",\"shards\":"+(chunk+1)+",\"compilable\":false,\"gameplay_validated\":false}\n";
        Files.writeString(out.resolve("summary.json"),summary,StandardCharsets.UTF_8);
        println("RECOVERY_EXPORT_COMPLETE " + summary);
    }
}
