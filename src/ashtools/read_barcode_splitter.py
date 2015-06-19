#!/usr/bin/env python2

from Bio import SeqIO
import os
import os.path
import argparse
import sys
from collections import defaultdict

def extract_barcode(s):
    """
    @M01691:10:000000000-A7F7L:1:2107:21387:4106_2:N:0:1_UMI:CTGACGTTACTCGG:GGGGGGGGGGGGGG_517009
    ->
    CTGACGTTACTCGG
    """
    import re

    m = re.match(r".*:([ACTG]{14}):.{14}_\d+", s)
    return m.groups()[0]


assert(extract_barcode("@M01691:10:000000000-A7F7L:1:2107:21387:4106_2:N:0:1_UMI:CTGACGTTACTCGG:GGGGGGGGGGGGGG_517009") == "CTGACGTTACTCGG")
assert(extract_barcode("@M01691:10:000000000-A7F7L:1:2109:5441:12945_1:N:0:1_UMI:AAGTATTTAGTAAC:FFEFFGGGGGGFG9_565461") == "AAGTATTTAGTAAC")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Split source FASTQ file due to barcode")
    parser.add_argument("-s", "--reads",
                        type=str,
                        required=True,
                        help="Reads file in FASTQ format")
    parser.add_argument("-o", "--output-dir",
                        type=str,
                        required=True,
                        help="Output directory")
    parser.add_argument("-v", "--verbose",
                        action="store_true",
                        help="Be verbose (default no)")
    parser.add_argument("-f", "--force-remove-existing-output-dir",
                        action="store_true",
                        help="Write to existing output dir (default no)")

    args = parser.parse_args()

    # Create output dir if needed
    try:
        os.mkdir(args.output_dir)
    except Exception as ex:
        if not args.force_remove_existing_output_dir:
            print(ex)
            sys.exit(1)
        else:
            print("Writing to existing output dir...")

    barcode2reads_list = defaultdict(list)
    with open(args.reads, "rU") as fh:
        for read in SeqIO.parse(fh, "fastq"):
            barcode = extract_barcode(str(read.id))
            if args.verbose:
                print(barcode)
            barcode2reads_list[barcode].append(read)


    for barcode, reads in barcode2reads_list.iteritems():
        fname = "%s/%s.fastq" % (args.output_dir, barcode)
        with open(fname, "w") as fh:
            SeqIO.write(reads, fh, "fastq")