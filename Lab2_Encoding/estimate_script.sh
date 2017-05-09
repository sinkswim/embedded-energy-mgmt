#!/bin/bash

#  estimate_script.sh

# generate 19 streams
./ass_1/generate_traces
DATA_FILES_NO_ENC=./ass_1/traces/data/*.txt
ADDR_FILES_NO_ENC=./ass_1/traces/addr/*.txt

# for each non encoded stream perform energy estimation
rm ./no_encoding_results.txt
for f in $DATA_FILES_NO_ENC
do
    echo "Processing $f file..." >> ./no_encoding_results.txt
    ./ass_2/generate_estimates $f >> ./no_encoding_results.txt
    # cat $f
done

for f in $ADDR_FILES_NO_ENC
do
    echo "Processing $f file..." >> ./no_encoding_results.txt
    ./ass_2/generate_estimates $f >> ./no_encoding_results.txt
    # cat $f
done

# apply encodings and generate encoded streams

DATA_DIR_ENC=./ass_3/enc_traces/data/
ADDR_DIR_ENC=./ass_3/enc_traces/addr/

for f in $DATA_FILES_NO_ENC
do
    echo "Processing $f file..."
    NAME=${f##*/}  # retain the part after the last slash
    ./ass_3/gen_encoded_streams $f bus_inv > $DATA_DIR_ENC"businv_"$NAME
    ./ass_3/gen_encoded_streams $f t_part > $DATA_DIR_ENC"tpart_"$NAME
    # cat $f
done

for f in $ADDR_FILES_NO_ENC
do
    echo "Processing $f file..."
    NAME=${f##*/}  # retain the part after the last slash
    ./ass_3/gen_encoded_streams $f bus_inv > $ADDR_DIR_ENC"businv_"$NAME
    ./ass_3/gen_encoded_streams $f t_part > $ADDR_DIR_ENC"tpart_"$NAME
    ./ass_3/gen_encoded_streams $f gray > $ADDR_DIR_ENC"gray_"$NAME
    # cat $f
done

DATA_FILES_ENC=./ass_3/enc_traces/data/*.txt
ADDR_FILES_ENC=./ass_3/enc_traces/addr/*.txt

# for each encoded stream perform energy estimation
rm ./encoding_results.txt
for f in $DATA_FILES_ENC
do
    echo "Processing $f file..." >> ./encoding_results.txt
    ./ass_2/generate_estimates $f >> ./encoding_results.txt
# cat $f
done

for f in $ADDR_FILES_ENC
do
    echo "Processing $f file..." >> ./encoding_results.txt
    ./ass_2/generate_estimates $f >> ./encoding_results.txt
# cat $f
done
