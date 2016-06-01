#!/usr/bin/env python2

from argparse import ArgumentParser
from Bio import SeqIO
import sys


import contextlib
@contextlib.contextmanager
def smart_open(filename, mode="r"):
    """
    From http://stackoverflow.com/questions/17602878/how-to-handle-both-with-open-and-sys-stdout-nicely
    """
    import gzip
    import re
    from sys import stdout, stdin

    if "w" in mode:
        MODE = "w"
    elif "a" in mode:
        MODE= "a"
    else:
        MODE = "r"

    if filename != '-':
        if re.match(r"^.*\.gz$", filename):
            assert(MODE != "a")
            fh = gzip.open(filename, mode=MODE)
        else:
            fh = open(filename, mode=mode)
    else:
        assert(MODE != "a")
        fh = stdout if MODE == "w" else stdin
    try:
        yield fh
    finally:
        if fh is not stdout and fh is not stdin:
            fh.close()


def parse_size(s):
    import re

    m = re.match(r"^.*___size___(\d+)$", s)

    if m:
        g = m.groups()
        return int(g[0])
    else:
        return 1

assert(parse_size("dsdsfsd___size___10") == 10)


if __name__ == "__main__":
    parser = ArgumentParser(description="Report supernodes")
    parser.add_argument("input",
                        type=str,
                        help="input FASTA file with abundances in ids")
    parser.add_argument("output",
                        type=str,
                        help="output FASTA file")
    parser.add_argument("--limit", "-l",
                        type=int,
                        default=5,
                        help="size limit (default: %(default)s)")

    args = parser.parse_args()

    print "Supernode reporter started..."
    print "Command line: %s" % " ".join(sys.argv)

    result = []
    with smart_open(args.input, "r") as fin:
        for record in SeqIO.parse(fin, "fasta"):
            id = str(record.description)
            size = parse_size(id)
            if size >= args.limit:
                if not "___size___" in id:
                    id = "%s___size___%d" % (id, size)
                record.id = record.name = record.description = id
                result.append(record)

    with smart_open(args.output, "w") as fout:
        SeqIO.write(result, fout, "fasta")

    print "Supernode reporter done"
