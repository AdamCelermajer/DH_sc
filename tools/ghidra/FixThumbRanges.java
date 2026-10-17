// Restore authoritative symbol sizes/mode after generic flow analysis.
// @category Recovery
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.ArmDisassembleCommand;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.math.BigInteger;
import java.nio.file.*;
import java.util.*;

public class FixThumbRanges extends GhidraScript {
    public void run() throws Exception {
        String[] args=getScriptArgs();
        if(args.length!=1) throw new IllegalArgumentException("FixThumbRanges.java MODES_TSV");
        int fixed=0;
        TreeMap<Long,long[]> ranges=new TreeMap<>();
        for(String line:Files.readAllLines(Paths.get(args[0]))) {
            if(line.isBlank() || line.startsWith("#")) continue;
            String[] fields=line.split("\\t");
            long raw=Long.parseUnsignedLong(fields[0],16), size=Long.parseLong(fields[1]);
            if(size>0) ranges.merge(raw&~1L,new long[]{raw,size},(oldRow,newRow)->oldRow[1]>=newRow[1]?oldRow:newRow);
        }
        for(Map.Entry<Long,long[]> record:ranges.entrySet()) {
            long raw=record.getValue()[0], size=record.getValue()[1];
            if((raw&1)==0) continue;
            Long next=ranges.higherKey(raw&~1L);
            if(next!=null) size=Math.min(size,next-(raw&~1L));
            Address start=currentProgram.getImageBase().add(raw&~1L), end=start.add(size-1);
            Function f=getFunctionAt(start);
            if(f==null) continue;
            String originalName=f.getName();
            List<Function> remove=new ArrayList<>();
            FunctionIterator others=currentProgram.getFunctionManager().getFunctions(new AddressSet(start,end),true);
            while(others.hasNext()) {
                Function other=others.next();
                long offset=other.getEntryPoint().subtract(currentProgram.getImageBase());
                if(!other.getEntryPoint().equals(start) && other.getName().startsWith("FUN_") && !ranges.containsKey(offset)) remove.add(other);
            }
            for(Function other:remove) currentProgram.getFunctionManager().removeFunction(other.getEntryPoint());
            currentProgram.getListing().clearCodeUnits(start,end,false);
            currentProgram.getProgramContext().setValue(currentProgram.getRegister("TMode"),start,end,BigInteger.ONE);
            new ArmDisassembleCommand(start,new AddressSet(start,end),true).applyTo(currentProgram,monitor);
            f=getFunctionAt(start);
            if(f==null) f=createFunction(start,originalName);
            if(f==null) { println("THUMB_FUNCTION_WARNING " + start); continue; }
            try { f.setBody(new AddressSet(start,end)); } catch(Exception e) { println("THUMB_BODY_WARNING " + start + " " + e); }
            fixed++;
        }
        println("THUMB_RANGES_FIXED " + fixed);
    }
}
