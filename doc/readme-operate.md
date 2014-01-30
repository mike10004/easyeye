# Operation

The two executable programs are `easyeye-extract` and 
`easyeye-compare`. If you run either one with the `--help` option, a
message explaining usage will be printed.

## easyeye-extract 

Extracts features from an iris image and writes the encoding to file.

    $ easyeye-extract hello.bmp hello.json

Add in a -v switch to see some analysis information printed on standard error.

## easyeye-compare

Once you have two or more of those .json encodings created with 
`easyeye-extract`, compute match scores like this:

    $ easyeye-compare probe.json target.json \[target2.json...\]

For each pairing of the probe encoding with a target encoding, the 
program prints four tab-separated tokens on standard output: a status
code, a hamming distance, the probe filename and the target filename.

