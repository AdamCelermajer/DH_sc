// Decompile only the known engine-math functions and report no-return cutoffs.
// @category Recovery
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.FloatDataType;
import ghidra.program.model.data.IntegerDataType;
import ghidra.program.model.listing.Function;
import ghidra.util.task.TaskMonitor;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class VerifyEngineMathDecomp extends GhidraScript {
    private static class Helper {
        long elfOffset;
        String name;
        DataType result;
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

    private static class Row {
        String elfAddress;
        String address;
        String symbol;
        String signature;
        String error;
        boolean completed;
        boolean noReturnWarning;
        int bodyChars;
    }

    private static class HelperRow {
        String elfAddress;
        String address;
        String name;
        String signature;
        String callingConvention;
        boolean noReturn;
        boolean matchesPrototype;
    }

    private static String json(String value) {
        if (value == null) return "null";
        StringBuilder out = new StringBuilder("\"");
        for (char c : value.toCharArray()) {
            switch (c) {
                case '\\': out.append("\\\\"); break;
                case '"': out.append("\\\""); break;
                case '\n': out.append("\\n"); break;
                case '\r': out.append("\\r"); break;
                case '\t': out.append("\\t"); break;
                default:
                    if (c < 32) out.append(String.format("\\u%04x", (int)c));
                    else out.append(c);
            }
        }
        return out.append('"').toString();
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException(
                "VerifyEngineMathDecomp.java TARGETS.tsv OUTPUT.json [TIMEOUT_SECONDS]");
        }
        int timeout = args.length > 2 ? Integer.parseInt(args[2]) : 60;
        List<Row> rows = new ArrayList<>();
        DecompInterface decompiler = new DecompInterface();
        DecompileOptions options = new DecompileOptions();
        options.grabFromProgram(currentProgram);
        decompiler.setOptions(options);
        decompiler.toggleSyntaxTree(false);
        decompiler.toggleCCode(true);
        decompiler.setSimplificationStyle("decompile");
        if (!decompiler.openProgram(currentProgram)) {
            throw new IllegalStateException(decompiler.getLastMessage());
        }

        try {
            for (String line : Files.readAllLines(Paths.get(args[0]))) {
                if (line.isBlank() || line.startsWith("#")) continue;
                String[] fields = line.split("\t", 2);
                long elfOffset = Long.parseUnsignedLong(fields[0], 16);
                Row row = new Row();
                row.elfAddress = fields[0];
                Address address = currentProgram.getImageBase().add(elfOffset);
                row.address = address.toString();
                Function function = getFunctionAt(address);
                if (function == null) {
                    row.symbol = fields.length > 1 ? fields[1] : "";
                    row.error = "No function at target address";
                    rows.add(row);
                    continue;
                }
                row.symbol = function.getName(true);
                row.signature = function.getPrototypeString(false, false);
                DecompileResults result = decompiler.decompileFunction(function, timeout,
                    TaskMonitor.DUMMY);
                row.completed = result.decompileCompleted() &&
                    result.getDecompiledFunction() != null;
                row.error = result.getErrorMessage();
                if (row.completed) {
                    String code = result.getDecompiledFunction().getC();
                    row.bodyChars = code.length();
                    row.noReturnWarning = code.contains("WARNING: Subroutine does not return");
                }
                rows.add(row);
                println("ENGINE_MATH_DECOMPILE " + row.elfAddress + " completed=" +
                    row.completed + " noreturn_warning=" + row.noReturnWarning +
                    " symbol=" + row.symbol);
            }
        }
        finally {
            decompiler.dispose();
        }

        int completed = 0;
        int warningCount = 0;
        boolean pass = !rows.isEmpty();
        for (Row row : rows) {
            if (row.completed) completed++;
            if (row.noReturnWarning) warningCount++;
            if (!row.completed || row.noReturnWarning) pass = false;
        }

        String defaultCallingConvention = currentProgram.getCompilerSpec()
            .getDefaultCallingConvention().getName();
        List<HelperRow> helperRows = new ArrayList<>();
        for (Helper helper : HELPERS) {
            Address address = currentProgram.getImageBase().add(helper.elfOffset);
            Function function = getFunctionAt(address);
            HelperRow row = new HelperRow();
            row.elfAddress = "0x" + Long.toHexString(helper.elfOffset);
            row.address = address.toString();
            row.name = function == null ? "" : function.getName(true);
            row.signature = function == null ? "" :
                function.getPrototypeString(false, false);
            row.callingConvention = function == null ? "" :
                function.getCallingConventionName();
            row.noReturn = function == null || function.hasNoReturn();
            row.matchesPrototype = function != null &&
                function.getName(true).contains(helper.name) &&
                function.getCallingConventionName().equals(defaultCallingConvention) &&
                function.getReturnType().isEquivalent(helper.result) &&
                function.getParameterCount() == 2 &&
                function.getParameter(0).getDataType().isEquivalent(FloatDataType.dataType) &&
                function.getParameter(1).getDataType().isEquivalent(FloatDataType.dataType);
            helperRows.add(row);
            if (row.noReturn || !row.matchesPrototype) pass = false;
            println("ENGINE_MATH_HELPER " + row.elfAddress + " " + row.name +
                " noreturn=" + row.noReturn + " proto=" + row.matchesPrototype +
                " cc=" + row.callingConvention);
        }

        StringBuilder report = new StringBuilder();
        report.append("{\n  \"validation\": ").append(json(pass ? "PASS" : "FAIL"));
        report.append(",\n  \"library\": ").append(json(currentProgram.getName()));
        report.append(",\n  \"ghidra_version\": ").append(json(getGhidraVersion()));
        report.append(",\n  \"image_base\": ").append(json(currentProgram.getImageBase().toString()));
        report.append(",\n  \"target_count\": ").append(rows.size());
        report.append(",\n  \"decompiled\": ").append(completed);
        report.append(",\n  \"no_return_warnings\": ").append(warningCount);
        report.append(",\n  \"helper_count\": ").append(helperRows.size());
        report.append(",\n  \"helpers\": [\n");
        for (int i = 0; i < helperRows.size(); i++) {
            HelperRow row = helperRows.get(i);
            report.append("    {\"elf_address\": ").append(json(row.elfAddress));
            report.append(", \"address\": ").append(json(row.address));
            report.append(", \"name\": ").append(json(row.name));
            report.append(", \"signature\": ").append(json(row.signature));
            report.append(", \"calling_convention\": ").append(json(row.callingConvention));
            report.append(", \"no_return\": ").append(row.noReturn);
            report.append(", \"matches_prototype\": ").append(row.matchesPrototype).append("}");
            if (i + 1 < helperRows.size()) report.append(',');
            report.append('\n');
        }
        report.append("  ],\n  \"functions\": [\n");
        for (int i = 0; i < rows.size(); i++) {
            Row row = rows.get(i);
            report.append("    {\"elf_address\": ").append(json(row.elfAddress));
            report.append(", \"address\": ").append(json(row.address));
            report.append(", \"symbol\": ").append(json(row.symbol));
            report.append(", \"signature\": ").append(json(row.signature));
            report.append(", \"completed\": ").append(row.completed);
            report.append(", \"no_return_warning\": ").append(row.noReturnWarning);
            report.append(", \"body_chars\": ").append(row.bodyChars);
            report.append(", \"error\": ").append(json(row.error)).append("}");
            if (i + 1 < rows.size()) report.append(',');
            report.append('\n');
        }
        report.append("  ]\n}\n");
        Path output = Paths.get(args[1]);
        if (output.getParent() != null) Files.createDirectories(output.getParent());
        Files.writeString(output, report.toString(), StandardCharsets.UTF_8);
        println("ENGINE_MATH_DECOMP_VALIDATION " + (pass ? "PASS" : "FAIL") +
            " decompiled=" + completed + "/" + rows.size() +
            " warnings=" + warningCount + " report=" + output);
        if (!pass) throw new IllegalStateException("Focused engine-math decompilation failed");
    }
}
