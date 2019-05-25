#!/bin/bash

# Sean Szumlanski
# COP 3502, Spring 2019

# ==========================
# LonelyPartyArray: run18.sh
# ==========================
# You can run this script at the command line like so:
#
#   bash run18.sh
#
# For more details, see the assignment PDF.


################################################################################
# Shell check.
################################################################################

# Running this script with sh instead of bash can lead to false positives on the
# test cases. Yikes! These checks ensure the script is not being run through the
# Bourne shell (or any shell other than bash).

if [ "$BASH" != "/bin/bash" ]; then
  echo ""
  echo " Bloop! Please use bash to run this script, like so: bash test-all.sh"
  echo ""
  exit
fi

if [ -z "$BASH_VERSION" ]; then
  echo ""
  echo " Bloop! Please use bash to run this script, like so: bash test-all.sh"
  echo ""
  exit
fi


################################################################################
# Initialization.
################################################################################

PASS_CNT=0
NUM_TEST_CASES=15


################################################################################
# Magical incantations.
################################################################################

# Ensure that obnoxious glibc errors are piped to stderr.
export LIBC_FATAL_STDERR_=1

# Now redirect all local error messages to /dev/null (like "process aborted").
exec 2> /dev/null


################################################################################
# Check that all required files are present.
################################################################################

if [ ! -f LonelyPartyArray.c ]; then
	echo ""
	echo " Error: You must place LonelyPartyArray.c in this directory before"
	echo "        we can proceed. Aborting test script."
	echo ""
	exit
elif [ ! -f LonelyPartyArray.h ]; then
	echo ""
	echo " Error: You must place LonelyPartyArray.h in this directory before"
	echo "        we can proceed. Aborting test script."
	echo ""
	exit
elif [ ! -f sanity-check.c ]; then
	echo ""
	echo " Error: You must place sanity-check.c in this directory before"
	echo "        we can proceed. Aborting test script."
	echo ""
	exit
elif [ ! -d sample_output ]; then
	echo ""
	echo " Error: You must place the sample_output folder in this directory"
	echo "        before we can proceed. Aborting test script."
	echo ""
	exit
elif [ ! -f sample_output/sanity-check-output.txt ]; then
	echo ""
	echo " Error: You must place sanity-check-output.txt in the sample_output"
	echo "        directory before we can proceed. Aborting test script."
	echo ""
	exit
fi

check_test_case()
{
	local filename=$1

	if [ ! -f $filename ]; then
		echo ""
		echo " Error: You must place $filename in this directory"
		echo "        before we can proceed. Aborting test script."
		echo ""
		exit
	fi
}

check_sample_output()
{
	local filename=$1

	if [ ! -f sample_output/$filename ]; then
		echo ""
		echo " Error: You must place $filename in the"
		echo "        sample_output directory before we can proceed."
		echo "        Aborting test script."
		echo ""
		exit
	fi
}

for i in `seq -f "%02g" 1 $NUM_TEST_CASES`;
do
	check_test_case "testcase$i.c"
done

check_test_case "valgrind-test01.c"
check_test_case "valgrind-test02.c"
check_test_case "valgrind-test03-bonus.c"

for i in `seq -f "%02g" 1 $NUM_TEST_CASES`;
do
	check_sample_output "testcase$i-output.txt"
done

check_sample_output "valgrind-test01-output.txt"
check_sample_output "valgrind-test02-output.txt"
check_sample_output "valgrind-test03-bonus-output.txt"


################################################################################
# Run sanity check.
################################################################################

echo ""
echo "================================================================"
echo "Checking sizeof() values on this system..."
echo "================================================================"
echo ""

echo -n "  [Sanity Check] sanity-check.c ... "

function run_sanity_check ()
{
	# Attempt to compile.
	gcc sanity-check.c 2> /dev/null
	compile_val=$?
	if [[ $compile_val != 0 ]]; then
		echo "fail (failed to compile)"
		return
	fi

	# Run program. Capture return value to check whether it crashes.
	./a.out > myoutput.txt 2> /dev/null
	execution_val=$?
	if [[ $execution_val != 0 ]]; then
		echo "fail (program crashed)"
		return
	fi

	# Run diff and capture its return value.
	diff myoutput.txt sample_output/sanity-check-output.txt > /dev/null
	diff_val=$?

	# Output results based on diff's return value.
	if  [[ $diff_val != 0 ]]; then
		echo "fail (output mismatch)"
		echo ""
		echo "================================================================"
		echo "WARNING! WARNING! WARNING!"
		echo "================================================================"
		echo ""
		echo "  According to sanity-check.c, your system appears to be using"
		echo "  non-standard sizeof() values for the following data type(s):"
		echo ""
		cat myoutput.txt

	else
		echo "PASS!"
	fi
}

run_sanity_check


################################################################################
# Run test cases.
################################################################################

echo ""
echo "================================================================"
echo "Running test cases..."
echo "================================================================"
echo ""

# Function for running a single test case.

run_test_case()
{
	local testcase_file=$1
	local output_file=$2

	echo -n "  [Test Case] $testcase_file ... "

	# Attempt to compile.
	gcc LonelyPartyArray.c $testcase_file 2> /dev/null
	compile_val=$?
	if [[ $compile_val != 0 ]]; then
		echo "fail (failed to compile)"
		return
	fi

	# Run program. Capture return value to check whether it crashes.
	./a.out > myoutput.txt 2> /dev/null
	execution_val=$?
	if [[ $execution_val != 0 ]]; then
		echo "fail (program crashed)"
		return
	fi

	# Run diff and capture its return value.
	diff myoutput.txt sample_output/$output_file > /dev/null
	diff_val=$?
	
	# Output results based on diff's return value.
	if  [[ $diff_val != 0 ]]; then
		echo "fail (output mismatch)"
	else
		echo "PASS!"
		PASS_CNT=`expr $PASS_CNT + 1`
	fi
}

for i in `seq -f "%02g" 1 $NUM_TEST_CASES`;
do
	run_test_case "testcase$i.c" "testcase$i-output.txt" 2> /dev/null
done


################################################################################
# Check for memory leaks: run test cases through valgrind.
################################################################################

echo ""
echo "================================================================"
echo "Checking for memory leaks with valgrind..."
echo "================================================================"
echo ""

run_valgrind_test()
{
	local testcase_file=$1
	local output_file=$2

	echo -n "  [Valgrind] $testcase_file ... "

	# Attempt to compile.
	gcc LonelyPartyArray.c $testcase_file -g 2> /dev/null
	compile_val=$?
	if [[ $compile_val != 0 ]]; then
		echo "fail (failed to compile)"
		return
	fi

	# Run program through valgrind. Check whether program crashes.
	valgrind --leak-check=yes ./a.out > myoutput.txt 2> err.log
	execution_val=$?
	if [[ $execution_val != 0 ]]; then
		echo "fail (program crashed)"
		return
	fi

	# Check output for indication of memory leaks.
	grep --silent "no leaks are possible" err.log
	valgrindfail=$?
	if [[ $valgrindfail != 0 ]]; then
		echo "fail (memory leak detected)"
		return
	fi

	# Run diff and capture its return value.
	diff myoutput.txt sample_output/$output_file > /dev/null
	diff_val=$?
	
	# Output results based on diff's return value.
	if  [[ $diff_val != 0 ]]; then
		echo "fail (output mismatch)"
	else
		echo "PASS!"
		PASS_CNT=`expr $PASS_CNT + 1`
	fi
}

for i in `seq -f "%02g" 1 2`;
do
	run_valgrind_test "valgrind-test$i.c" "valgrind-test$i-output.txt" 2> /dev/null
done


################################################################################
# Run bonus test case.
################################################################################

echo ""
echo "================================================================"
echo "Running bonus test case for cloneLonelyPartyArray()..."
echo "================================================================"
echo ""

run_valgrind_test "valgrind-test03-bonus.c" "valgrind-test03-bonus-output.txt"


############################################################################
# Check for tabs vs. spaces.
############################################################################

echo ""
echo "================================================================"
echo "Checking for tabs vs. spaces..."
echo "================================================================"
echo ""

if ! [ -x "$(command -v grep)" ]; then
	echo "  Skipping tabs vs. spaces check; grep not installed. You"
	echo "  might see this message if you're running this script on a"
	echo "  Mac. Please be sure to test your final code on Eustis."
elif ! [ -x "$(command -v awk)" ]; then
	echo "  Skipping tabs vs. spaces check; awk not installed. You"
	echo "  might see this message if you're running this script on a"
	echo "  Mac. Please be sure to test your final code on Eustis."
else
	num_spc_lines=`grep "^ " LonelyPartyArray.c | wc -l | awk '{$1=$1};1'`
	num_tab_lines=`grep "$(printf '^\t')" LonelyPartyArray.c | wc -l | awk '{$1=$1};1'`
	num_und_lines=`grep "$(printf '^[^\t ]')" LonelyPartyArray.c | wc -l | awk '{$1=$1};1'`

	echo "  Number of lines beginning with spaces: $num_spc_lines"
	echo "  Number of lines beginning with tabs: $num_tab_lines"
	echo "  Number of lines with no indentation: $num_und_lines"

	if [ "$num_spc_lines" -gt 0 ] && [ "$num_tab_lines" -gt 0 ]; then
		echo ""
		echo "  Whoa, buddy! It looks like you're starting some lines of code with"
		echo "  tabs, and other lines of code with spaces. You'll need to choose"
		echo "  one or the other."
		echo ""
		echo "  Try running the following commands to see which of your lines begin"
		echo "  with spaces (the first command below) and which ones begin with tabs"
		echo "  (the second command below):"
		echo ""
		echo "     grep \"^ \" LonelyPartyArray.c -n"
		echo "     grep \"\$(printf '^[\t ]')\" LonelyPartyArray.c -n"
	elif [ "$num_spc_lines" -gt 0 ]; then
		echo ""
		echo "  Looks like you're using spaces for all your indentation! (Yay!)"
	elif [ "$num_tab_lines" -gt 0 ]; then
		echo ""
		echo "  Looks like you're using tabs for all your indentation! (Yay!)"
	else
		echo ""
		echo "  Whoa, buddy! It looks like none of your lines of code are indented!"
	fi
fi


################################################################################
# Cleanup phase.
################################################################################

# Clean up the executable file.
rm -f a.out
rm -f err.log
rm -f myoutput.txt


################################################################################
# Final thoughts.
################################################################################

echo ""
echo "================================================================"
echo "Final Thoughts"
echo "================================================================"

echo ""
echo "  Recall that these are not the same test cases released with"
echo "  the assignment PDF."
echo ""
echo "  Also, this script does not impose any runtime restrictions and"
echo "  therefore won't be able to detect any proccess timeouts that"
echo "  might be indicated in your official test case transcript."
echo ""
