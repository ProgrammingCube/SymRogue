'''
    Converts a .hex file built by cc65 to paper tape
    SYM-1 compatible (KIM-1 not implemented yet)
'''
import os, sys

def checksum(line):
    chkSum = 0
    for ch in line:
        chkSum += ch
    return chkSum

def convertToPtp(hex_f, ptp_f, size, chunkSize, address = 0x0200):
    print("Starting conversion...")
    opcode_str = b''
    for line in hex_f:
        opcode_str = opcode_str + line
    # opcode_str is now the entire file
    # now squeeze \r\n
    opcode_str = opcode_str.replace(b'\r\n', b'')
    #print(opcode_str)
    # now construct the file
    # will be in a loop
    prog_len = len(opcode_str)
    blocks = prog_len / chunkSize
    print("Length of bytecodes is ", prog_len)
    print("# of blocks: ", blocks)

    '''
    ptp_f.write(';')
    ptp_f.write(hex(16)[2:].upper())
    ptp_f.write(hex(address)[2:].upper())
    ptp_f.write(dataBytes.hex().upper())
    '''

    #line = bytearray()
    line_addr = address
    for x in range( int(blocks // 2 + (blocks % 2 > 0))):
        line_addr = line_addr + (chunkSize * bool(x))
        #print(";", end='')
        ptp_f.write(';')
        #print(str(hex(chunkSize)[2:].upper()), end='')
        ptp_f.write(str(hex(chunkSize)[2:].upper()))
        line = str(hex(chunkSize)[2:].upper())
        #print("{:04x}".format(line_addr).upper(), end = '')
        ptp_f.write("{:04x}".format(line_addr).upper())
        line = line + ("{:04x}".format(line_addr).upper())
        #print(str(opcode_str[(x * chunkSize) * 2:(x * chunkSize) * 2 + 32].upper())[2:-1], end='')
        ptp_f.write(str(opcode_str[(x * chunkSize) * 2:(x * chunkSize) * 2 + 32].upper())[2:-1])
        line = line + str(opcode_str[(x * chunkSize) * 2:(x * chunkSize) * 2 + 32].upper())[2:-1]
        #line = line + checksum(line)
        byte_line = bytearray.fromhex(line)
        #print('{0:0{1}X}'.format(checksum(byte_line),4))
        ptp_f.write('{0:0{1}X}'.format(checksum(byte_line),4))
        ptp_f.write('\n')
        address = address + chunkSize
    #print(";00\r")
    ptp_f.write(";00\r")
    print("Built", ptp_f.name)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Not enough arguments!")
        exit(-2)
    hexFile = sys.argv[1]
    ptpFile = sys.argv[2]
    chunkSize = 16
    startAddress = 0x0200

    try:
        hex_f = open(hexFile, "rb")
    except:
        print("File not found!")
        exit(-4)

    ptp_f = open(ptpFile, "w")
    hexFileSize = os.path.getsize(hexFile)
    convertToPtp(hex_f, ptp_f, hexFileSize, chunkSize, startAddress)
    ptp_f.close()
    hex_f.close()

