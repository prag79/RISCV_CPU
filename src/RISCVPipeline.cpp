#include "systemc.h"
#include "alu.h"
#include "registerfile.h"
#include "memory.h"
#include "instruction.h"
#include "decode.h"
#include "execute.h"
#include "fetch.h"

SC_MODULE(RISCVPipeline) {
    // Pipeline stages
    Fetch fetch;
    Decode decode;
    Execute execute;
    Memory memory;
    ALU alu;

    // Register file and memory
    RegisterFile regfile;
    MemoryUnit memunit;

    // Signals
    sc_signal<sc_uint<32>> pc, pc_next;
    sc_signal<sc_uint<32>> instr;
    sc_signal<sc_uint<5>> rs1, rs2, rd;
    sc_signal<sc_uint<12>> imm;
    sc_signal<sc_uint<32>> alu_result, mem_data;
    sc_signal<sc_uint<4>> opcode;
    sc_signal<bool> mem_read, mem_write, branch, zero;

    SC_CTOR(RISCVPipeline)
        : fetch("fetch"), decode("decode"), execute("execute"), memory("memory"), alu("alu"),
        regfile("regfile"), memunit("memunit") {
        // Connect pipeline stages
        fetch.pc(pc);
        fetch.instr(instr);
        fetch.pc_next(pc_next);
        decode.instr(instr);
        decode.opcode(opcode);
        decode.rs1(rs1);
        decode.rs2(rs2);
        decode.rd(rd);
        decode.imm(imm);
        execute.rs1(regfile.rs1);
        execute.rs2(regfile.rs2);
        execute.imm(imm);
        execute.opcode(opcode);
        execute.alu_result(alu_result);
        execute.branch(branch);
        execute.zero(zero);
        alu.rs1(regfile.rs1);
        alu.rs2(regfile.rs2);
        alu.imm(imm);
        alu.opcode(opcode);
        alu.result(alu_result);
        memory.mem_read(mem_read);
        memory.mem_write(mem_write);
        memory.alu_result(alu_result);
        memory.mem_data(mem_data);
        memory.branch(branch);
        memory.zero(zero);
        memory.memunit(memunit);
        memunit.mem_data(mem_data);
        memunit.mem_read(mem_read);
        memunit.mem_write(mem_write);
        memunit.addr(alu_result);
        regfile.rs1(rs1);
        regfile.rs2(rs2);
        regfile.rd(rd);
        regfile.reg_write(memory.reg_write);
        regfile.alu_result(alu_result);

        // Initialize signals
        pc = 0;
        pc_next = 0;
        instr = 0;
        rs1 = 0;
        rs2 = 0;
        rd = 0;
        imm = 0;
        alu_result = 0;
        mem_data = 0;
        opcode = 0;
        mem_read = false;
        mem_write = false;
        branch = false;
        zero = false;
    }
};

int sc_main(int argc, char* argv[]) {
    RISCVPipeline riscv_pipeline("riscv_pipeline");
    sc_start();
    return 0;
}