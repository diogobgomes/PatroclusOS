#!/bin/bash
# CONFIGURATION SCRIPT FOR PATROCLUS OS
#
# Run this before any attempt to build the OS, as that WILL fail!
# Gets some system information, and generates the final Makefiles
#
# Run config.sh --help to see how to use the script
#
#
# 2024 Diogo Gomes

# ------------------------------------------------------------------------------
# Versions
# ------------------------------------------------------------------------------
projectname="PatroclusOS"
version="0.2"
release="pre-alpha"
scriptversion="0.2"

# ------------------------------------------------------------------------------
# Set up error handling
# ------------------------------------------------------------------------------
set -e
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG # keep track of the last executed command
trap cleanup ERR # echo an error message before exiting
cleanup() {
    error_code=$?
    echo "[Internal] \"${last_command}\" command failed with exit code $?. Aborting!" | tee -a ${logfile}
    exit $((error_code))
}

# ------------------------------------------------------------------------------
# Color definitions
# ------------------------------------------------------------------------------
GREEN=$(tput setaf 2)
RED=$(tput setaf 1)
NC=$(tput sgr0)
YELLOW=$(tput setaf 3)

# ------------------------------------------------------------------------------
# Functions
# Some helper functions
# ------------------------------------------------------------------------------

function help {
    printf "Usage: config.sh [options]\n\n"
    printf "Important note: this script SHOULD be run outside the main source "
    printf "directory, as it will create the whole build environment in the directory "
    printf "it's run in, and in-source builds are a bad idea. If you want to force in-"
    printf "source builds, pass the --force-source option to config.sh\n\n"
    printf "Options:\n"
    printf "  -h,--help\t\tDisplay the help menu\n"
    printf "  -v,-V,--version\tDisplay version information\n"
    printf "  --toolchain=<file>\tUse <file> as the toolchain file, default is\n\t\t\t"
    printf "config/toolchain, check it out to see format of toolchain file\n"
    printf "  --diskimage=<name>\tUse <file> as the name for the final diskimage,\n\t\t\t"
    printf "default is ./diskimage.dd\n"
    printf "  --loopback=<loopback>\tUse <loopback> as the loopback device on\n\t\t\t"
    printf "which to mount the diskimage, default is /dev/loop0\n"
    printf "  --force-source\tForce in-source build, NOT RECOMMENDED!\n"
}

function version {
    printf "PatroclusOS config.sh configure script, version %s\n\n" ${scriptversion}
    printf "Written by Diogo Gomes (dbarrosgomes@gmail.com), 2024\n"
}

function log_header {
    stdbuf -oL printf "" >> ${logfile} # Make sure output is in order
    printf "\n\n" >> ${logfile}
    printf "#%.0s" {1..80} >> ${logfile}
    printf "\n" >> ${logfile}
    printf "#%.0s" {1..80} >> ${logfile}
    printf "\n" >> ${logfile}
    printf " %.0s" {1..35} >> ${logfile}
    printf "%s\n" "${1}" >> ${logfile}
    printf "#%.0s" {1..80} >> ${logfile}
    printf "\n" >> ${logfile}
    printf "#%.0s" {1..80} >> ${logfile}
    printf "\n" >> ${logfile}
}

function command_error {
    printf "${RED}%s\n" "$1"
    exit 1
}

function print_log {
    stdbuf -oL printf "" >> ${logfile} # Make sure output is in order
    printf "$1" | tee >(sed $'s/\033[[][^A-Za-z]*[A-Za-z]//g' >> ${logfile})
}

function check_prog {
    stdbuf -oL printf "" >> ${logfile} # Make sure output is in order
    printf "Checking whether $1 is present..."
    log_header "$1"
    var_name=${!2}
    local -n prog_var=$2
    printf "${2}=${var_name}\n" >> ${logfile}
    if $3 &>> ${logfile}; then
        printf "${GREEN} OK${NC}\n"
    else
        printf "${RED} FAIL${NC}\n"
        if $4; then
            print_log "${RED}Error: ${1} not found\n"
            exit 4
        else
            printf "Warning: ${1} not found, disabling targets\n" >> ${logfile}
            prog_var="FAIL"
            warnings=true
        fi
    fi
}

# ------------------------------------------------------------------------------
# Process user input
# ------------------------------------------------------------------------------

# Print header
printf "Starting config.sh for PatroclusOS!\n"

# Save invocation
invocation="$0 $@"

# Initialize option variables to make sure no environment contamination occurs
toolchain_file=
diskimage=
loopback=
force_outside_source=true

while :; do
    case $1 in
        -h | --help)
            help
            exit 0
            ;;
        -v | -V | --version)
            help
            exit 0
            ;;
        --toolchain)
            if [ "$2" ]; then
                toolchain_file=$2
                shift
            else
                command_error "Error: \"--toolchain\" requires a non-empty option argument"
            fi
            ;;
        --toolchain=?*)
            toolchain_file=`echo $1 | sed 's/--toolchain=//'`
            shift
            ;;
        --toolchain=)
            command_error "Error: \"--toolchain\" requires a non-empty option argument"
            ;;

        --diskimage)
            if [ "$2" ]; then
                diskimage=$2
                shift
            else
                command_error "Error: \"--diskimage\" requires a non-empty option argument"
            fi
            ;;
        --diskimage=?*)
            diskimage=`echo $1 | sed 's/--diskimage=//'`
            shift
            ;;
        --diskimage=)
            command_error "Error: \"--diskimage\" requires a non-empty option argument"
            ;;

        --loopback)
            if [ "$2" ]; then
                loopback=$2
                shift
            else
                command_error "Error: \"--loopback\" requires a non-empty option argument"
            fi
            ;;
        --loopback=?*)
            loopback=`echo $1 | sed 's/--loopback=//'`
            shift
            ;;
        --loopback=)
            command_error "Error: \"--loopback\" requires a non-empty option argument"
            ;;

        --force-source)
            force_outside_source=false
            shift
            ;;

        --) # End of all options
            shift
            break
            ;;
        -?*)
            command_error "Error: Unknown option \"${1}\""
            ;;
        *) # Default case: no more options
            break
    esac
done

# ------------------------------------------------------------------------------
# Default input values
# ------------------------------------------------------------------------------
rootsrcdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
toolchain_file=${toolchain_file:-${rootsrcdir}/config/toolchain.sh}
diskimage=${diskimage:-$(pwd)/diskimage.dd}
loopback=${loopback:-/dev/loop0}

# ------------------------------------------------------------------------------
# Start doing stuff
# ------------------------------------------------------------------------------

# Make sure we're out of source
if $force_outside_source; then
    if [ "${rootsrcdir}" = "$(pwd)" ]; then
        command_error "Error: Doing an in-source build is not supported, please run the command outside the source. If you really want to, you can use the flag --force-source, but it's not tested and not recommended"
    fi
fi

# Logfile
logfile="config.log"

if ! [ -w $logfile ] && [ -f $logfile ]; then
    command_error "Error: Could not open log file \"${logfile}\""
fi

printf "Writing logs to %s\n" "${logfile}"

# Write config.log header
printf "CONFIGURATION LOG\n\n" > ${logfile}
printf "Generated by configure version ${scriptversion}\n\n" >> ${logfile}
printf "INVOCATION = ${invocation}\n" >> ${logfile}
printf "DATE = $(date)\n\n" >> ${logfile}
printf "PLATFORM:\n" >> ${logfile}
printf "hostname = $(hostname)\n" >> ${logfile}
printf "Machine Hardware (uname -m) = $(uname -m)\n" >> ${logfile}
printf "Kernel Release (uname -r) = $(uname -r)\n" >> ${logfile}
printf "Kernel Name (uname -s) = $(uname -s)\n" >> ${logfile}
printf "Kernel Version (uname -v) = $(uname -v)\n\n" >> ${logfile}

# Include toolchain file, check contents
print_log "Including the toochain file \"${toolchain_file}\"..."
. ${toolchain_file}
print_log " ${GREEN}OK${NC}"

# ------------------------------------------------------------------------------
# Check expected programs
# ------------------------------------------------------------------------------
print_log "\nChecking for programs now\n"

warnings=false
check_prog "CC compiler" "cc_compiler" "${cc_compiler} -v" true
check_prog "CXX compiler" "cxx_compiler" "${cxx_compiler} -v" true
check_prog "AS" "as_prog" "${as_prog} --version" true
check_prog "CC 32-bit compiler" "cc32_compiler" "${cc32_compiler} -v" true
check_prog "CXX 32-bit compiler" "cxx32_compiler" "${cxx32_compiler} -v" true
check_prog "AS 32-bit" "as32_prog" "${as_prog} --version" true
check_prog "AR" "ar_prog" "${ar_prog} --version" true
check_prog "Python" "python_prog" "${python_prog} --version" false
check_prog "xorriso" "xorriso_prog" "${xorriso_prog} --version" false
check_prog "grub-mkrescue" "grub_prog" "${grub_prog} --version" false
check_prog "grub-file" "grub_prog_check" "${grub_prog_check} -h" false
check_prog "qemu" "qemu_prog" "${qemu_prog} --version" false
check_prog "doxygen" "doxygen_prog" "${doxygen_prog} --version" false

# Make sure it's GNU Make
make_prog=make
check_prog "make" "make_prog" "${make_prog} --version" true
printf "Checking that make is GNU Make..."
if ${make_prog} --version | grep "GNU Make" &> /dev/null ; then
    printf "${GREEN} OK${NC}\n"
else
    printf "${RED} FAIL\n"
    print_log "Error: Make is not GNU Make\n"
    exit 4
fi

# ------------------------------------------------------------------------------
# Generating the build files
# ------------------------------------------------------------------------------

# Useful variables
subdirs='bootloader kernel init lib32 lib64'

#destmakefile='Makefile bootloader/Makefile kernel/Makefile init/Makefile lib32/Makefile lib64/Makefile'
destmakefile='Makefile kernel/Makefile init/Makefile lib32/Makefile lib64/Makefile'
destmakefile=(${destmakefile})
destmaketools="progs.mk" # File make will include with tool definitions

srcmakefile="${rootsrcdir}/Makefile.in ${rootsrcdir}/kernel/Makefile.in ${rootsrcdir}/init/Makefile.in ${rootsrcdir}/lib/Makefile32.in ${rootsrcdir}/lib/Makefile64.in"
srcmakefile=(${srcmakefile})
srcmakeconfig=${rootsrcdir}/config/config.mk
srcmakeoutput=${rootsrcdir}/config/output.mk

# Generate the file structure
for folder in $subdirs; do
    mkdir -p $folder
done
mkdir -p isodir/boot/grub

# Generate final makefiles
printf "Generating final makefiles... "
for index in "${!destmakefile[@]}"; do
    if ! [ -w ${destmakefile[index]} ] && [ -f ${destmakefile[index]} ]; then
        printf "\n"
        command_error "Error: Could not open file \"${destmakefile[index]}\""
    fi

    printf "ROOTSRCDIR:=${rootsrcdir}\n" > ${destmakefile[index]}
    printf "MADE_CONFIG:=true\n\n" >> ${destmakefile[index]}
    cat ${srcmakefile[index]} >> ${destmakefile[index]}

    # Now edit the file
    sed -i '/#@START_HEADER/,/#@END_HEADER/d' ${destmakefile[index]}

    # Grub part
    if [ "${grub_prog}" == "FAIL" ] || [ "${xorriso_prog}" == "FAIL" ] || [ "${grub_prog_check}" == "FAIL" ]; then
        sed -i '/#@START_GRUB/,/#@END_GRUB/d' ${destmakefile[index]}
    else
        sed -i 's/#@START_GRUB//' ${destmakefile[index]}
        sed -i 's/#@END_GRUB//' ${destmakefile[index]}
    fi

    # Diskimage
    if [ "${python_prog}" == "FAIL" ]; then
        sed -i '/#@START_DISKIMAGE/,/#@END_DISKIMAGE/d' ${destmakefile[index]}
    else
        sed -i 's/#@START_DISKIMAGE//' ${destmakefile[index]}
        sed -i 's/#@END_DISKIMAGE//' ${destmakefile[index]}
    fi

    # Docs part
    if [ "${doxygen_prog}" == "FAIL" ]; then
        sed -i '/#@START_DOCS/,/#@END_DOCS/d' ${destmakefile[index]}
    else
        sed -i 's/#@START_DOCS//' ${destmakefile[index]}
        sed -i 's/#@END_DOCS//' ${destmakefile[index]}
    fi
done
printf "${GREEN}OK${NC}\n"

# Generate prog.mk
printf "Generating ${destmaketools}... "
if ! [ -w ${destmaketools} ] && [ -f ${destmaketools} ]; then
    printf "\n"
    command_error "Error: Could not open file \"${destmaketools}\""
fi
printf "# MAIN CONFIGURATION FILE FOR MAKEFILE\n#\n" > ${destmaketools}
printf "# Autogenerated by config.sh version ${scriptversion} on $(date)\n\n" >> ${destmaketools}
printf "unexport\n" >> ${destmaketools}
printf "CC64:=${cc_compiler}\n" >> ${destmaketools}
printf "CXX64:=${cxx_compiler}\n" >> ${destmaketools}
printf "LD64:=${cxx_compiler}\n" >> ${destmaketools}
printf "AS64:=${as_prog}\n" >> ${destmaketools}
printf "CC32:=${cc32_compiler}\n" >> ${destmaketools}
printf "CXX32:=${cxx32_compiler}\n" >> ${destmaketools}
printf "LD32:=${cxx32_compiler}\n" >> ${destmaketools}
printf "AS32:=${as32_prog}\n" >> ${destmaketools}
printf "AR:=${ar_prog}\n" >> ${destmaketools}
printf "PYTHON:=${python_prog}\n" >> ${destmaketools}
printf "GRUB_PROG:=${grub_prog}\n" >> ${destmaketools}
printf "GRUB_PROG_CHECK:=${grub_prog_check}\n\n" >> ${destmaketools}

printf "${GREEN}OK${NC}\n"

# TODO create diskimage

# ------------------------------------------------------------------------------
# Copy .gdbinit, create gdb_history
# ------------------------------------------------------------------------------
cp ${rootsrcdir}/config/gdbinit .gdbinit
touch .gdb_history

# ------------------------------------------------------------------------------
# Doxygen
# ------------------------------------------------------------------------------
if [ "${doxygen_prog}" != "FAIL" ]; then
    printf "Generating Doxyfile from config/Doxyfile.in... "
    mkdir -p docs
    cp -f ${rootsrcdir}/docs/Doxyfile.in Doxyfile
    sed -i 's,@OUTPUT_DIRECTORY@,'"$(pwd)/docs/"',' Doxyfile
    sed -i 's,@INPUT@,'"${rootsrcdir}"',' Doxyfile
    printf "${GREEN}OK${NC}\n"
fi

# ------------------------------------------------------------------------------
# Finish writing to log
# ------------------------------------------------------------------------------
log_header "Written files"
printf "Wrote files:\n" >> ${logfile}
for index in "${!destmakefile[@]}"; do printf "${destmakefile[index]}\n" >> ${logfile}; done
printf "${destmaketools}\n.gdbinit\n.gdb_history\nDoxyfile\n" >> ${logfile}

# ------------------------------------------------------------------------------
# Done!
# ------------------------------------------------------------------------------
if $warnings; then
    printf "${YELLOW}Warning:${NC} Some targets have been disabled for programs not found, check config.log\n"
fi

printf "${GREEN}Configure done, type make to build!\n${NC}"
