// Keep recovery deterministic and avoid costly speculative parameter rewriting.
// @category Recovery
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.lang.Register;
import ghidra.app.cmd.disassemble.ArmDisassembleCommand;
import java.math.BigInteger;
import java.nio.file.*;
import java.util.Map;
public class ConfigureRecovery extends GhidraScript {
    public void run() throws Exception {
        Map<String,String> options=getCurrentAnalysisOptionsAndValues(currentProgram);
        for(String key:new String[]{"Decompiler Parameter ID", "DWARF", "DWARF External Debug Files", "ARM Aggressive Instruction Finder"})
            if(options.containsKey(key)) setAnalysisOption(currentProgram,key,"false");
        // ELF st_value bit 0 is authoritative for ARM/Thumb function mode.
        // The generic loader did not preserve this for every tiny Thumb JNI entry.
        String[] args=getScriptArgs();
        if(args.length>0) {
            Register thumb=currentProgram.getRegister("TMode");
            for(String line:Files.readAllLines(Paths.get(args[0]))) {
                if(line.isBlank() || line.startsWith("#")) continue;
                String[] fields=line.split("\\t");
                long raw=Long.parseUnsignedLong(fields[0],16), size=Long.parseLong(fields[1]);
                if(size<=0) continue;
                Address start=currentProgram.getImageBase().add(raw & ~1L), end=start.add(size-1);
                currentProgram.getListing().clearCodeUnits(start,end,false);
                currentProgram.getProgramContext().setValue(thumb,start,end,BigInteger.valueOf(raw & 1L));
                if((raw & 1L)!=0) new ArmDisassembleCommand(start,new AddressSet(start,end),true).applyTo(currentProgram,monitor);
            }
        }
    }
}
