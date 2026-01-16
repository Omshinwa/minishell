#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                   MINISHELL - MEMORY LEAK STRESS TESTS                       #
#                    42 School - Valgrind Full Coverage                        #
#                                                                              #
# **************************************************************************** #

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
NC='\033[0m'

MINISHELL="./minishell"
VALGRIND_OPTS="--leak-check=full --show-leak-kinds=all --track-fds=yes --error-exitcode=42"
SUPP_FILE="readline.supp"

TOTAL=0
PASSED=0
FAILED=0

print_header() {
    echo -e "\n${BOLD}${BLUE}═══════════════════════════════════════════${NC}"
    echo -e "${BOLD}${BLUE}  $1${NC}"
    echo -e "${BOLD}${BLUE}═══════════════════════════════════════════${NC}\n"
}

test_leak() {
    local test_name="$1"
    local commands="$2"
    
    TOTAL=$((TOTAL + 1))
    echo -e "${YELLOW}Test $TOTAL:${NC} $test_name"
    
    # Create suppression file if it doesn't exist
    if [ ! -f "$SUPP_FILE" ]; then
        cat > "$SUPP_FILE" << 'EOF'
{
   readline_leak
   Memcheck:Leak
   match-leak-kinds: reachable
   ...
   fun:readline
}
{
   add_history_leak
   Memcheck:Leak
   match-leak-kinds: reachable
   ...
   fun:add_history
}
EOF
    fi
    
    # Run test
    echo -e "$commands" | valgrind $VALGRIND_OPTS \
        --suppressions="$SUPP_FILE" \
        "$MINISHELL" > /dev/null 2>&1
    
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}  ✓ No leaks detected${NC}"
        PASSED=$((PASSED + 1))
        return 0
    elif [ $exit_code -eq 42 ]; then
        echo -e "${RED}  ✗ MEMORY LEAK DETECTED${NC}"
        
        # Run again with full output
        echo -e "${YELLOW}  Detailed output:${NC}"
        echo -e "$commands" | valgrind --leak-check=full \
            --show-leak-kinds=all \
            --suppressions="$SUPP_FILE" \
            "$MINISHELL" 2>&1 | grep -A 5 "LEAK SUMMARY"
        
        FAILED=$((FAILED + 1))
        return 1
    else
        echo -e "${RED}  ✗ ERROR: Exit code $exit_code${NC}"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

# ============================================================================ #
#                              BASIC LEAK TESTS                                #
# ============================================================================ #

test_basic_leaks() {
    print_header "BASIC LEAK TESTS"
    
    test_leak "Empty command" "
exit"
    
    test_leak "Simple echo" "echo hello
exit"
    
    test_leak "Simple ls" "ls
exit"
    
    test_leak "PWD command" "pwd
exit"
    
    test_leak "Multiple commands" "echo test1
echo test2
echo test3
exit"
}

# ============================================================================ #
#                            BUILTIN LEAK TESTS                                #
# ============================================================================ #

test_builtin_leaks() {
    print_header "BUILTIN LEAK TESTS"
    
    test_leak "Export and echo" "export VAR=value
echo \$VAR
exit"
    
    test_leak "Multiple exports" "export A=1
export B=2
export C=3
echo \$A \$B \$C
exit"
    
    test_leak "Unset variable" "export VAR=test
unset VAR
echo \$VAR
exit"
    
    test_leak "CD commands" "cd /tmp
pwd
cd ..
pwd
cd
pwd
exit"
    
    test_leak "Env listing" "env
exit"
    
    test_leak "Export listing" "export
exit"
}

# ============================================================================ #
#                              PIPE LEAK TESTS                                 #
# ============================================================================ #

test_pipe_leaks() {
    print_header "PIPE LEAK TESTS"
    
    test_leak "Simple pipe" "echo hello | cat
exit"
    
    test_leak "Multiple pipes" "echo test | cat | cat | cat
exit"
    
    test_leak "Pipe with grep" "echo -e 'line1\nline2\nline3' | grep line
exit"
    
    test_leak "Long pipe chain" "cat /etc/passwd | head -10 | tail -5 | wc -l
exit"
    
    test_leak "Pipe with builtin" "pwd | cat
exit"
}

# ============================================================================ #
#                          REDIRECTION LEAK TESTS                              #
# ============================================================================ #

test_redir_leaks() {
    print_header "REDIRECTION LEAK TESTS"
    
    test_leak "Output redirection" "echo test > /tmp/test_output.txt
cat /tmp/test_output.txt
rm /tmp/test_output.txt
exit"
    
    test_leak "Append redirection" "echo line1 >> /tmp/test_append.txt
echo line2 >> /tmp/test_append.txt
cat /tmp/test_append.txt
rm /tmp/test_append.txt
exit"
    
    test_leak "Input redirection" "echo content > /tmp/test_input.txt
cat < /tmp/test_input.txt
rm /tmp/test_input.txt
exit"
    
    test_leak "Multiple redirections" "echo test > /tmp/out1.txt > /tmp/out2.txt
rm /tmp/out1.txt /tmp/out2.txt
exit"
}

# ============================================================================ #
#                            HEREDOC LEAK TESTS                                #
# ============================================================================ #

test_heredoc_leaks() {
    print_header "HEREDOC LEAK TESTS"
    
    test_leak "Basic heredoc" "cat << EOF
line1
line2
EOF
exit"
    
    test_leak "Heredoc with expansion" "export VAR=test
cat << EOF
\$VAR
EOF
exit"
    
    test_leak "Heredoc quoted delimiter" "cat << 'EOF'
\$HOME
EOF
exit"
    
    test_leak "Heredoc to file" "cat << EOF > /tmp/heredoc_out.txt
test content
EOF
cat /tmp/heredoc_out.txt
rm /tmp/heredoc_out.txt
exit"
}

# ============================================================================ #
#                          WILDCARD LEAK TESTS                                 #
# ============================================================================ #

test_wildcard_leaks() {
    print_header "WILDCARD LEAK TESTS"
    
    # Create test files
    mkdir -p /tmp/wildcard_leak_test
    touch /tmp/wildcard_leak_test/file1.c
    touch /tmp/wildcard_leak_test/file2.c
    touch /tmp/wildcard_leak_test/test.txt
    
    test_leak "Wildcard expansion" "echo /tmp/wildcard_leak_test/*.c
exit"
    
    test_leak "Multiple wildcards" "echo /tmp/wildcard_leak_test/*.c /tmp/wildcard_leak_test/*.txt
exit"
    
    test_leak "Wildcard no match" "echo /tmp/wildcard_leak_test/*.zzz
exit"
    
    # Cleanup
    rm -rf /tmp/wildcard_leak_test
}

# ============================================================================ #
#                            STRESS LEAK TESTS                                 #
# ============================================================================ #

test_stress_leaks() {
    print_header "STRESS LEAK TESTS"
    
    # Generate a loop of 50 commands
    local loop_commands=""
    for i in {1..50}; do
        loop_commands+="echo iteration $i\n"
    done
    loop_commands+="exit"
    
    test_leak "50 iterations loop" "$loop_commands"
    
    # Export/unset loop
    local export_loop=""
    for i in {1..20}; do
        export_loop+="export VAR$i=value$i\n"
    done
    for i in {1..20}; do
        export_loop+="unset VAR$i\n"
    done
    export_loop+="exit"
    
    test_leak "Export/Unset loop (20x)" "$export_loop"
    
    # Pipe stress
    test_leak "Deep pipe chain (10 levels)" "echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat
exit"
}

# ============================================================================ #
#                         FILE DESCRIPTOR TESTS                                #
# ============================================================================ #

test_fd_leaks() {
    print_header "FILE DESCRIPTOR LEAK TESTS"
    
    echo -e "${YELLOW}Checking for leaked file descriptors...${NC}\n"
    
    # Test 1: Simple command
    TOTAL=$((TOTAL + 1))
    echo -e "echo test\nexit" | valgrind --track-fds=yes --error-exitcode=42 \
        --suppressions="$SUPP_FILE" "$MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Simple command - no FD leaks${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ Simple command - FD LEAK DETECTED${NC}"
        echo -e "echo test\nexit" | valgrind --track-fds=yes \
            --suppressions="$SUPP_FILE" "$MINISHELL" 2>&1 | grep "FILE DESCRIPTORS"
        FAILED=$((FAILED + 1))
    fi
    
    # Test 2: Pipe
    TOTAL=$((TOTAL + 1))
    echo -e "echo test | cat\nexit" | valgrind --track-fds=yes --error-exitcode=42 \
        --suppressions="$SUPP_FILE" "$MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Pipe - no FD leaks${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ Pipe - FD LEAK DETECTED${NC}"
        FAILED=$((FAILED + 1))
    fi
    
    # Test 3: Redirections
    TOTAL=$((TOTAL + 1))
    echo -e "echo test > /tmp/fd_test.txt\nexit" | valgrind --track-fds=yes --error-exitcode=42 \
        --suppressions="$SUPP_FILE" "$MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Redirections - no FD leaks${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ Redirections - FD LEAK DETECTED${NC}"
        FAILED=$((FAILED + 1))
    fi
    rm -f /tmp/fd_test.txt
    
    # Test 4: Heredoc
    TOTAL=$((TOTAL + 1))
    echo -e "cat << EOF\ntest\nEOF\nexit" | valgrind --track-fds=yes --error-exitcode=42 \
        --suppressions="$SUPP_FILE" "$MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Heredoc - no FD leaks${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ Heredoc - FD LEAK DETECTED${NC}"
        FAILED=$((FAILED + 1))
    fi
}

# ============================================================================ #
#                                  MAIN                                        #
# ============================================================================ #

main() {
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}Error: minishell not found at $MINISHELL${NC}"
        exit 1
    fi
    
    if ! command -v valgrind &> /dev/null; then
        echo -e "${RED}Error: valgrind is not installed${NC}"
        exit 1
    fi
    
    echo -e "${BOLD}${BLUE}"
    echo "╔════════════════════════════════════════════════════════════╗"
    echo "║                                                            ║"
    echo "║      MINISHELL - MEMORY LEAK STRESS TESTS                 ║"
    echo "║           42 School - Valgrind Coverage                   ║"
    echo "║                                                            ║"
    echo "╚════════════════════════════════════════════════════════════╝"
    echo -e "${NC}\n"
    
    # Run all test suites
    test_basic_leaks
    test_builtin_leaks
    test_pipe_leaks
    test_redir_leaks
    test_heredoc_leaks
    test_wildcard_leaks
    test_stress_leaks
    test_fd_leaks
    
    # Summary
    echo -e "\n${BOLD}${BLUE}═══════════════════════════════════════════${NC}"
    echo -e "${BOLD}${BLUE}         MEMORY TEST SUMMARY${NC}"
    echo -e "${BOLD}${BLUE}═══════════════════════════════════════════${NC}\n"
    
    echo -e "${BOLD}Total tests:${NC}  $TOTAL"
    echo -e "${GREEN}${BOLD}Passed:${NC}       $PASSED"
    echo -e "${RED}${BOLD}Failed:${NC}       $FAILED"
    
    if [ $TOTAL -gt 0 ]; then
        PERCENTAGE=$((PASSED * 100 / TOTAL))
        echo -e "${BOLD}Success rate:${NC} $PERCENTAGE%\n"
    fi
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}${BOLD}🎉 Perfect! No memory leaks detected!${NC}"
        echo -e "${GREEN}Your minishell is ready for evaluation.${NC}\n"
        return 0
    else
        echo -e "${RED}${BOLD}⚠ Memory leaks detected!${NC}"
        echo -e "${RED}Review the failed tests and fix the leaks before evaluation.${NC}\n"
        return 1
    fi
}

main
exit $?
