#!/bin/bash

# Colors for terminal output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}Starting compilation and execution process...${NC}"

# Print current directory to help with debugging
echo -e "${BLUE}Current directory: $(pwd)${NC}"
echo -e "${BLUE}Directory contents:${NC}"
ls -la

# Ask the user for the correct directory names
echo -e "${BLUE}Please enter the name of the input creator directory (default: 1-createInput):${NC}"
read input_dir
input_dir=${input_dir:-"1-createInput"}

echo -e "${BLUE}Please enter the name of the solver directory (default: 2-solver):${NC}"
read solver_dir
solver_dir=${solver_dir:-"2-solver"}

echo -e "${BLUE}Please enter the name of the IO directory (default: 3-IO):${NC}"
read io_dir
io_dir=${io_dir:-"3-IO"}

# Create the IO directory structure if it doesn't exist
mkdir -p ${io_dir}/input
mkdir -p ${io_dir}/output

# Clean the directories by removing all files except .gitkeep
find ${io_dir}/input -type f -not -name ".gitkeep" -delete 2>/dev/null || true
find ${io_dir}/output -type f -not -name ".gitkeep" -delete 2>/dev/null || true

echo -e "${GREEN}Created and cleaned directory structure${NC}"

# Step 1: Compile and run the input creator
echo -e "${BLUE}Step 1: Compiling and running input creator in ${input_dir}...${NC}"
if [ -d "${input_dir}" ]; then
    cd ${input_dir}
    echo -e "${BLUE}Now in directory: $(pwd)${NC}"
    rm -f main
    if g++ -std=c++11 -Wall *.cpp -o main 2>/dev/null || clang++ -std=c++11 -Wall *.cpp -o main; then
        echo -e "${GREEN}Compilation successful!${NC}"
        ./main
        echo -e "${GREEN}Input files created in ${io_dir}/input folder${NC}"
        cd ..
    else
        echo -e "${RED}Compilation failed!${NC}"
        cd ..
        exit 1
    fi
else
    echo -e "${RED}Directory ${input_dir} not found!${NC}"
    echo -e "${BLUE}Available directories:${NC}"
    ls -la | grep "^d"
    exit 1
fi

# Step 2: Copy globalConstants.h and compile/run the solver
echo -e "${BLUE}Step 2: Compiling and running solver in ${solver_dir}...${NC}"
if [ -d "${solver_dir}" ]; then
    cd ${solver_dir}
    echo -e "${BLUE}Now in directory: $(pwd)${NC}"
    
    if [ -f "../${io_dir}/input/globalConstants.h" ]; then
        cp ../${io_dir}/input/globalConstants.h ./globalConstants.h
        echo -e "${GREEN}Copied globalConstants.h to solver directory${NC}"
    else
        echo -e "${RED}Warning: globalConstants.h not found in ${io_dir}/input!${NC}"
        find ../${io_dir}/input -type f -name "*.h" -exec ls -la {} \;
    fi

    rm -f main
    if g++ -std=c++11 -Wall *.cpp -o main 2>/dev/null || clang++ -std=c++11 -Wall *.cpp -o main; then
        echo -e "${GREEN}Compilation successful!${NC}"
        ./main
        echo -e "${GREEN}Solver executed successfully${NC}"
        cd ..
    else
        echo -e "${RED}Compilation failed!${NC}"
        cd ..
        exit 1
    fi
else
    echo -e "${RED}Directory ${solver_dir} not found!${NC}"
    echo -e "${BLUE}Available directories:${NC}"
    ls -la | grep "^d"
    exit 1
fi

echo -e "${GREEN}All steps completed successfully!${NC}"