#!/bin/bash -
# arg1 is the build directory
# arg2 the input binary file
# arg3 output binary anme
projectdir="${1}"
infile="${2}"
outfile="${3}"
binary_type="${4}"
entry_point="${5}"

current_directory=`pwd`
echo ${current_directory}
cd "${projectdir}/../../../../../../../Utilities/ImageHeader"
basedir=`pwd`
cd ${current_directory}

case "$(uname -s)" in
   Linux)
      #line for python
      echo Postbuild with python script
      imgtool="${basedir}/Python27/Stm32ImageAddHeader.py"
      cmd="python"
      ;;
   *)
      #line for window executeable
      echo Postbuild with windows executable
      imgtool="${basedir}/exe.win-amd64-2.7/Stm32ImageAddHeader.exe"
      cmd=""
      ;;
esac

message="${infile}'s signature done. Output file: ${outfile}"
command="${cmd} ${imgtool} ${infile} ${outfile} ${binary_type} ${entry_point}"

${command}  > "${projectdir}/output_log.txt"
ret=$?

if [ ${ret} == 0 ]; then
echo ${message}
else
echo "postbuild.sh failed"
fi
