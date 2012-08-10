#!/bin/sh

FILEPATH=$1;
SUFARR_LIB=$2;
ABSDIR=$(cd $(dirname $0) && pwd)

SN="[mk_apporo_index]"
PREFIX=/home/overlast/sakuragit/overlast_repos/Apporo/ # パッケージ化時に/usr/local/に変える

if [ ! ${FILEPATH} ] || [ ! -e ${FILEPATH} ] || [ ! -s ${FILEPATH} ] || [ ! -r ${FILEPATH} ];
then
    echo "${SN} there are not target file to index."
    echo "${SN} usage : mk_apporo_index.sh <target file path(TSV file)> <suffix array lib(sufary or tsubomi)>"
    echo "${SN} exit..."
    exit 1
fi

if [ ! ${SUFARR_LIB} ];
then
    echo "${SN} you must set suffix array library name."
    echo "${SN} usage : mk_apporo_index.sh <target file path(TSV file)> <suffix array lib(sufary or tsubomi)>"
    echo "${SN} exit..."
    exit 1
fi

if [ ! "${SUFARR_LIB}" = "sufary" ] && [ ! "${SUFARR_LIB}" = "tsubomi" ];
then
    echo "${SN} you must set suffix array library name."
    echo "${SN} usage : mk_apporo_index.sh <target file path(TSV file)> <suffix array lib(sufary or tsubomi)>"
    echo "${SN} exit..."
    exit 1
fi


echo "${SN} try to make suffix array index file..."
if [ -e ${FILEPATH}.arr ] && [ ! -w ${FILEPATH}.arr ];
then
    echo "${SN} can not write suffix array index file."
    echo "${SN} ${FILEPATH}.arr is not writeable."
    echo "${SN} exit..."
    exit 1
fi

${PREFIX}/bin/mk_apporo_char_index.pl ${FILEPATH} ${SUFARR_LIB} > ${FILEPATH}.ary

if [ ! -e ${FILEPATH} ] || [ ! -s ${FILEPATH} ] || [ ! -r ${FILEPATH} ];
then
    echo "${SN} fail to make suffix array index file."
    echo "${SN} exit..."
    exit 1
fi

echo "${SN} success to make suffix array index file."
echo "${SN} try to sort suffix array file"


if [ "${SUFARR_LIB}" = "sufary" ];
then
    if mkary -so ${FILEPATH} ;
    then
        echo "${SH} success to exec 'mkary -so ${FILEPATH}'"
    else
        echo "${SH} fail to exec 'mkary -so ${FILEPATH}'"
        exit 1
    fi
else
    if tsubomi_mkary -HU ${FILEPATH} ;
    then
        echo "${SH} success to exec 'tsubomi_mkary -s ${FILEPATH}'"
    else
        echo "${SH} fail to exec 'tsubomi_mkary -s ${FILEPATH}'"
        exit 1
    fi
fi

echo "${SN} try to make document id file..."
if [ -e ${FILEPATH}.did ] && [ ! -w ${FILEPATH}.did ];
then
    echo "${SN} can not write document id file."
    echo "${SN} ${FILEPATH}.arr is not writeable."
    echo "${SN} exit..."
    exit 1
fi

${PREFIX}/bin/mk_apporo_docid_index.pl ${FILEPATH} ${SUFARR_LIB} > ${FILEPATH}.did

if [ ! -e ${FILEPATH} ] || [ ! -s ${FILEPATH} ] || [ ! -r ${FILEPATH} ];
then
    echo "${SN} fail to make document id file."
    echo "${SN} exit..."
    exit 1
fi

echo "${SN} success to make all index file."
echo "${SN} finish..."

exit 0
