// Give the six canonical ARM EABI float imports their real returning signatures.
// @category Recovery
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.ArmDisassembleCommand;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.FloatDataType;
import ghidra.program.model.data.IntegerDataType;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.lang.Register;
import java.math.BigInteger;

public class FixArmEabiFloatHelpers extends GhidraScript {
    private static class Helper {
        final long elfOffset;
        final String name;
        final DataType result;
        Helper(long elfOffset, String name, DataType result) {
            this.elfOffset = elfOffset;
            this.name = name;
            this.result = result;
        }
    }

    private static final Helper[] HELPERS = {
        new Helper(0x30e2f8L, "__aeabi_fcmpgt", IntegerDataType.dataType),
        new Helper(0x30e3acL, "__aeabi_fsub", FloatDataType.dataType),
        new Helper(0x30e70cL, "__aeabi_fcmplt", IntegerDataType.dataType),
        new Helper(0x30eba4L, "__aeabi_fadd", FloatDataType.dataType),
        new Helper(0x30ec94L, "__aeabi_fdiv", FloatDataType.dataType),
        new Helper(0x30ed6cL, "__aeabi_fmul", FloatDataType.dataType)
    };

    private void applySignature(Function function, Helper helper) throws Exception {
        function.setCallingConvention(
            currentProgram.getCompilerSpec().getDefaultCallingConvention().getName());
        function.setCustomVariableStorage(false);
        function.setReturnType(helper.result, SourceType.USER_DEFINED);
        function.replaceParameters(
            FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,
            true,
            SourceType.USER_DEFINED,
            new ParameterImpl("left", FloatDataType.dataType, 0, currentProgram,
                SourceType.USER_DEFINED),
            new ParameterImpl("right", FloatDataType.dataType, 1, currentProgram,
                SourceType.USER_DEFINED));
        function.setNoReturn(false);
    }

    private Function ensureCanonicalThunk(Helper helper, Address entry) throws Exception {
        Function function = getFunctionAt(entry);
        if (function != null) return function;
        if (getFunctionContaining(entry) != null) {
            throw new IllegalStateException("Canonical import address is inside another function: " +
                entry);
        }
        if (!currentProgram.getMemory().contains(entry)) {
            throw new IllegalStateException("Canonical import address is unmapped: " + entry);
        }

        Address end = entry.add(11);
        Register thumb = currentProgram.getRegister("TMode");
        currentProgram.getListing().clearCodeUnits(entry, end, false);
        currentProgram.getProgramContext().setValue(thumb, entry, end, BigInteger.ZERO);
        new ArmDisassembleCommand(entry, new AddressSet(entry, end), false)
            .applyTo(currentProgram, monitor);
        if (currentProgram.getListing().getInstructionAt(entry) == null) {
            throw new IllegalStateException("Could not decode canonical ARM import thunk at " + entry);
        }
        function = createFunction(entry, helper.name);
        if (function == null) {
            throw new IllegalStateException("Could not create canonical import thunk " +
                helper.name + " at " + entry);
        }
        return function;
    }

    private void fixOne(Helper helper) throws Exception {
        Address canonicalPlt = currentProgram.getImageBase().add(helper.elfOffset);
        Function thunk = ensureCanonicalThunk(helper, canonicalPlt);
        if (!thunk.getName().contains(helper.name)) thunk.setName(helper.name,
            SourceType.USER_DEFINED);
        applySignature(thunk, helper);
        Function target = thunk.isThunk() ? thunk.getThunkedFunction(false) : null;
        if (target != null && target != thunk) applySignature(target, helper);
        println("AEABI_FLOAT_HELPER_FIXED " + helper.name + " elf=0x" +
            Long.toHexString(helper.elfOffset) + " canonical_plt=" + canonicalPlt +
            " thunk=" + thunk.getName(true) + " target=" +
            (target == null ? "<none>" : target.getName(true)));
    }

    @Override
    public void run() throws Exception {
        if (!currentProgram.getName().equals("libDungeonHunter2.so")) {
            println("AEABI_FLOAT_HELPER_SKIP program=" + currentProgram.getName());
            return;
        }
        for (Helper helper : HELPERS) fixOne(helper);
        println("AEABI_FLOAT_HELPERS_FIXED count=" + HELPERS.length);
    }
}
