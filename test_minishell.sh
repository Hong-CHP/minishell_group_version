#!/bin/bash

# Test script for minishell
# Make sure to compile minishell first: make

echo "=== MINISHELL TESTING SCRIPT ==="
echo "Make sure you have compiled minishell first!"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Create test directory
mkdir -p test_files
cd test_files

# Create some test files
echo "Hello World" > test.txt
echo "Line 1" > file1.txt
echo "Line 2" >> file1.txt
echo "This is a test file" > file2.txt

cd ..

echo -e "${YELLOW}Testing basic commands...${NC}"
echo ""

# Test 1: Basic commands
echo "Test 1: Basic echo command"
echo "echo hello world" | ./minishell
echo ""

# Test 2: pwd command
echo "Test 2: pwd command"
echo "pwd" | ./minishell
echo ""

# Test 3: ls command
echo "Test 3: ls command"
echo "ls" | ./minishell
echo ""

# Test 4: Built-in commands
echo "Test 4: Built-in commands"
echo -e "echo 'Testing echo builtin'\npwd\nenv | head -5" | ./minishell
echo ""

# Test 5: Command with arguments
echo "Test 5: Command with arguments"
echo "echo -n 'No newline here'" | ./minishell
echo ""
echo ""

# Test 6: Simple pipe
echo "Test 6: Simple pipe"
echo "echo 'hello world' | cat" | ./minishell
echo ""

# Test 7: Multiple pipes
echo "Test 7: Multiple pipes"
echo "echo 'hello world' | cat | cat" | ./minishell
echo ""

# Test 8: File operations
echo "Test 8: File operations"
echo "cat test_files/test.txt" | ./minishell
echo ""

# Test 9: Error handling
echo "Test 9: Error handling (command not found)"
echo "nonexistentcommand" | ./minishell
echo ""

# Test 10: cd command
echo "Test 10: cd command"
echo -e "pwd\ncd test_files\npwd\ncd ..\npwd" | ./minishell
echo ""

# Test 11: Complex pipe with grep
echo "Test 11: Complex pipe with grep"
echo "ls | grep minishell" | ./minishell
echo ""

# Test 12: wc command
echo "Test 12: wc command"
echo "echo 'hello world' | wc -w" | ./minishell
echo ""

# Test 13: Multiple commands
echo "Test 13: Multiple simple commands"
echo -e "echo 'First command'\necho 'Second command'\necho 'Third command'" | ./minishell
echo ""

# Test 14: PATH resolution
echo "Test 14: PATH resolution"
echo "which ls" | ./minishell
echo ""

# Test 15: Environment variables
echo "Test 15: Environment variables"
echo "env | grep PATH" | ./minishell
echo ""

# Test 16: Complex pipe
echo "Test 16: Complex pipe"
echo "echo 'hello' | cat -e" | ./minishell
echo ""

# Test 17: Complex pipe with variable
echo "Test 16: Complex pipe with variable"
echo "cat /etc/passwd | grep $USER | cut -d: -f1" | ./minishell
echo ""

# Test 18: Quotes
echo "Test 18: Quotes"
echo "echo \"this is a 'test'\"" | ./minishell
echo ""

# Test 18: Quotes
echo "Test 18: Quotes"
echo " echo "\'mix\'"" | ./minishell
echo ""

# Test 19: Quotes
echo "Test 19: Quotes"
echo "echo 'this is a \"test\"'" | ./minishell
echo ""

# Test 20: slashs
echo "Test 20: slashs"
echo "echo hello\ world" | ./minishell
echo ""

# Test 20: slashs
echo "Test 20: slashs"
echo "echo 'hello\ world'" | ./minishell
echo ""

# Test 21: Mix quotes and slashes
echo "Test 21: Mix quotes and slashes"
echo "echo \"Mix 'quotes' and "double quotes"\"" | ./minishell
echo ""

# Test 22: Export environment variables
echo "Test 22: Export environment variables"
echo "export TEST_VAR=minishell\necho $TEST_VAR\nunset TEST_VAR\necho $TEST_VAR" | ./minishell
echo ""

# Test 23: Subshell-1
echo "Test 23: Subshell"
echo "(echo \"Inside subshell\" && ls) | grep minishell" | ./minishell
echo ""

# Test 24: Subshell
echo "Test 24: Subshell-2"
echo "(cd /tmp && pwd) && pwd" | ./minishell
echo ""

# Test 25: Error test-1
echo "Test 25: Error test-1"
echo "cat nonexistentfile" | ./minishell
echo ""

# Test 26: Error test-2
echo "Test 26: Error test-2"
echo "ls /root 2> error.txt\necho error.txt" | ./minishell
echo ""

echo -e "${GREEN}=== INTERACTIVE TESTING ===${NC}"
echo "Now you can test interactively. Try these commands:"
echo "1. Basic commands: ls, pwd, echo hello"
echo "2. Pipes: ls | grep mini"
echo "3. Built-ins: cd, pwd, env"
echo "4. Complex: cat test_files/test.txt | wc -l"
echo "5. Type 'exit' to quit"
echo ""

# Cleanup
# rm -rf test_files

echo -e "${YELLOW}Starting interactive minishell...${NC}"
./minishell