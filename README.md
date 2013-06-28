flysums
=======

Utilities for calculating checksums on the fly.

These are useful for piping data through the utility between other programs,
calculating the hash of the data as it passes through. This is useful as it
can avoid a second read of the data to calculate the hash afterwards.

STDIN is read, added to the hash, and output to STDOUT.
When STDIN reaches end-of-file the hexdigest of the hash is written to the filename given on the command line.

Usage
=====

See flysums -h for a list of options.

flysums -a MD5 -f FROM_FILE MD5SUMS < FROM_FILE > TO_FILE

