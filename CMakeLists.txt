cmake_minimum_required (VERSION 3.19)

project(RISCV_PIPELINE CXX)

add_executable(riscv_pipe
	src/InstructionFetchUnit.cpp
	src/InstructionDecodeUnit.cpp
	src/RegisterFile.cpp
	src/ExecutionUnit.cpp
	Utilities/src/Report.cpp
	Utilities/src/reporting.cpp)

include_directories (
	/Users/pragnajitdattaroy/Documents/systemc-2.3.4/include
	./include
	./Utilities/include)


