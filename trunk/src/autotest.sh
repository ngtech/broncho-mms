#!/bin/bash

CEHCK_CMD="valgrind --tool=memcheck --leak-check=full"

function autotest()
{
	$CEHCK_CMD ./mms_header_test test_get_set
	$CEHCK_CMD ./wsp_header_codec_test 
	$CEHCK_CMD ./mms_input_buffer_test 
	$CEHCK_CMD ./mms_list_test
	$CEHCK_CMD ./mms_util_test 
	$CEHCK_CMD ./mms_content_type_test 
	$CEHCK_CMD ./mms_notify_test testdata/mms_notify.dat 
	$CEHCK_CMD ./mms_body_test test_get_set
	$CEHCK_CMD ./mms_body_part_test test_get_set 
	$CEHCK_CMD ./mms_body_part_test test_from_file testdata/Temp.txt
	$CEHCK_CMD ./mms_smil_test test_from_file testdata/smil.xml 
	$CEHCK_CMD ./mms_smil_test test_construct
	$CEHCK_CMD ./mms_content_type_test test_get_set
	$CEHCK_CMD ./mms_input_buffer_test
	$CEHCK_CMD ./mms_output_buffer_test

	for mms in testdata/*.mms 
	do
		$CEHCK_CMD ./mms_header_test $mms
		$CEHCK_CMD ./mms_body_part_test test_unpack $mms
		$CEHCK_CMD ./mms_body_test test_unpack_pack $mms
		$CEHCK_CMD ./mms_test test_unpack_pack $mms
		cmp lxj.mms $mms
		if [ ! "$?" = 0 ]
		then
			echo "error: not match"
		fi
	done
}

autotest >autotest.log


