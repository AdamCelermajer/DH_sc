// Attempt every authoritative sized function start, including loader omissions.
// @category Recovery
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.ArmDisassembleCommand;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.math.BigInteger;
import java.nio.file.*;
import java.util.*;

public class EnsureNamedFunctions extends GhidraScript {
    public void run() throws Exception {
        String[] args=getScriptArgs();
        TreeMap<Long,long[]> ranges=new TreeMap<>();
        for(String line:Files.readAllLines(Paths.get(args[0]))) {
            if(line.isBlank()) continue;
            String[] fields=line.split("\\t");
            long raw=Long.parseUnsignedLong(fields[0],16), size=Long.parseLong(fields[1]);
            if(size>0) ranges.merge(raw&~1L,new long[]{raw,size},(oldRow,newRow)->oldRow[1]>=newRow[1]?oldRow:newRow);
        }
        int attempted=0,created=0;
        for(var record:ranges.entrySet()) {
            long raw=record.getValue()[0],size=record.getValue()[1];
            Address start=currentProgram.getImageBase().add(raw&~1L),end=start.add(size-1);
            if(getFunctionAt(start)!=null) continue;
            attempted++;
            try {
                Function containing=getFunctionContaining(start);
                if(containing!=null) {
                    long offset=containing.getEntryPoint().subtract(currentProgram.getImageBase());
                    long[] original=ranges.get(offset);
                    if(original==null) currentProgram.getFunctionManager().removeFunction(containing.getEntryPoint());
                    else containing.setBody(new AddressSet(containing.getEntryPoint(),containing.getEntryPoint().add(original[1]-1)));
                }
                currentProgram.getListing().clearCodeUnits(start,end,false);
                currentProgram.getProgramContext().setValue(currentProgram.getRegister("TMode"),start,end,BigInteger.valueOf(raw&1L));
                new ArmDisassembleCommand(start,new AddressSet(start,end),(raw&1L)!=0).applyTo(currentProgram,monitor);
                Function f=getFunctionAt(start);
                if(f==null) f=createFunction(start,null);
                if(f==null) throw new IllegalStateException("createFunction returned null");
                f.setBody(new AddressSet(start,end)); created++;
            } catch(Exception e) { println("NAMED_FUNCTION_WARNING "+start+" "+e); }
        }
        println("NAMED_FUNCTIONS_ENSURED attempted="+attempted+" created="+created);
    }
}
