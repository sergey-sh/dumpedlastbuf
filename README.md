# dumpedlastbuf
Console app for reading from stdin and writing to stdout via a ring buffer with the output of the last data to the dump file if there is a problem when reading or writing.

# Download
git clone https://github.com/sergey-sh/dumpedlastbuf.git

# Compile
cd dumpedlastbuf
make

# Example of use

seq -w 1 1 10000000|./dumpedlastbuf 6016 9998592|wc -c 

In this example, 6016 bytes are read from the standard input once and sent to standard output. If reading or writing did not complete 6106 bytes, the last 9998592 is copied to the dump file under the name YYYYMMDDHHmmSS.dump in the current directory.
