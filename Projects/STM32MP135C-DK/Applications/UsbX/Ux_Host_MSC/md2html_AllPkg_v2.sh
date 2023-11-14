#!/usr/bin/env bash

###############################################################################################################################
# DESCRIPTION
# How to use Pandoc
# To convert your existing HTML release notes into Markdown (useful to build your history), type the following command in your bash window:
# pandoc --from html --to markdown Release_Notes.html > Release_Notes.md
# To convert a .md file into an HTML one, type the following command in your bash window:
# pandoc -s -r markdown -t html5 -c "_htmresc/mini-st.css" Release_Notes.md > Release_Notes.html
# 
# Author: GAMMOUDI Chokri 
# Date:   02/2021
###############################################################################################################################

NOCOLOR='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'

BASEDIR=$(dirname $0)
ABSPATH=$(readlink -f $0)
ABSDIR=$(dirname $ABSPATH)
BASENAME=$(basename $ABSPATH)

export PATH_OF_PKG=$1
export Header="\n---\npagetitle: Readme\nlang: en\n---\n::: {.row}\n::: {.col-sm-12 .col-lg-8}\n"

error()       { echo -e "${RED}Usage Error: $* ${NOCOLOR}" >&2; exit 1; }
warning()     { echo "warning: $*" >&2; }

set -e
[ "$DEBUG" = "" ] || set -x

usage()
{
    cat <<EOF
Usage: ./$BASENAME <Path-of-package> 
     Examples:
       ./$BASENAME /c/git/cubeazure/Firmware
EOF
exit
}
if [ "$1" = "--help" ]; then
    usage
fi

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}
spinner()
{
    local pid=$!
    local delay=0.3
    local spinstr='|/-\'
    while [ "$(ps a | awk '{print $1}' | grep $pid)" ]; do
        local temp=${spinstr#?}
        printf " [%c]  " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}
# Main processing
echo -e "\n\n"
echo "					***********************************************"
echo "					* Convert ReadMe.md file into an html format *"
echo "					************************************************"

if [[ $# -eq 0 ]] ; then
	error "Missing or wrong option"
	usage
fi
if [ ! -d "$PATH_OF_PKG" ]; then 
	error "Wrong Path or Path does not exist!"
fi


/usr/bin/find $PATH_OF_PKG -type f -name README.md > list_readmemd.tmp

cat list_readmemd.tmp | while  read readmefile ; do

   export pathhtml=${readmefile%.*}.html
   export filehtml=$(basename $pathhtml)
   cp $readmefile $readmefile.orig
   { echo -e "$Header"; cat "$readmefile.orig"; } > "$readmefile"
   echo -e "\n:::\n:::\n" >> "$readmefile"
   echo "Convert : $readmefile"
   pandoc --self-contained  -s -r markdown -t html5 -c "$PATH_OF_PKG//../../../../../_htmresc/mini-st_2020.css" $readmefile > $pathhtml
   cp $readmefile.orig $readmefile
   \rm $readmefile.orig
   pushd `dirname $pathhtml`
   mv $filehtml ${filehtml,,} 2>/dev/null
   popd
done

\rm list_readmemd.tmp
