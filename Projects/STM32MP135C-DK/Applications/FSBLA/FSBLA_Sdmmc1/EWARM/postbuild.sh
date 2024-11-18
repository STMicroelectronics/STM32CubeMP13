#!/bin/bash
# arg1 is the build directory
# arg2 is the input binary file
# arg3 is the output binary name
# arg4 is the binary type
# arg5 is the entry point

projectdir="${1}"
infile="${2}"
outfile="${3}"
binary_type="${4}"
entry_point="${5}"

echo "Project Directory: ${projectdir}"

SOURCE="Projects"
slash=""
projectdir=$(echo "$projectdir" | tr '\\' '/')

# Check if projectdir contains the SOURCE directory
if echo "$projectdir" | grep -q "$SOURCE"; then
    # Determine the slash type
    if echo "${projectdir}" | grep -q '/'; then
        slash="/"
    elif echo "${projectdir}" | grep -q '\\'; then
        slash="\\"
    else
        echo "Invalid Project Directory"
        exit 1
    fi

    # Extract the firmware directory
    count=$(echo "${projectdir}" | tr "${slash}" '\n' | grep -n "${SOURCE}" | cut -d':' -f1)
    firmwaredir=$(echo "${projectdir}" | cut -d"${slash}" -f1-${count})
    echo "Projects Directory: ${firmwaredir}"
    basedir="${firmwaredir}/../Utilities/ImageHeader"
else
    echo "Firmware Directory not found"
    exit 1
fi

# Determine the operating system and set the appropriate command and tool
case "$(uname -s)" in
    Linux)
      #line for python
        echo "Postbuild with python script"
        imgtool="${basedir}/Python27/Stm32ImageAddHeader.py"
        cmd="python"
        ;;
    *)
      #line for window executeable
        echo "Postbuild with windows executable"
        imgtool="${basedir}/exe.win-amd64-2.7/Stm32ImageAddHeader.exe"
        cmd=""
        ;;
esac

# Construct the command
command="${cmd} ${imgtool} ${infile} ${outfile} ${binary_type} ${entry_point}"

# Execute the command and redirect output to a log file
${command} > "${projectdir}/output_log.txt"
ret=$?

# Check the return status and print the appropriate message
if [ ${ret} -eq 0 ]; then
    echo "${infile}'s signature done. Output file: ${outfile}"
else
    echo "postbuild.sh failed"
fi
