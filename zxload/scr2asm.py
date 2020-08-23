#! /usr/bin/python3

import sys

FORMAT ="c" # C, Z80AMS

if "-z80asm" in sys.argv:
    FORMAT="z80asm"

# {filename} will be replaced by file name in FILE_BEGIN and in FILE_END
# {name} will be replaced by file name without extension in FILE_BEGIN and in FILE_END

if FORMAT == "c":
    # C FORMAT
    FILE_BEGIN="\n// {filename} imported with scr2asm\nconst uint8_t {name}_data[] = {\n"
    FILE_END="\n}; // end of {filename} import\n"
    HEXA_PREFIX = "0x" # or "$"
    LINE_PREFIX = "\t" # or "db"
    LINE_SUFIX = "\n" # 
    VALUES_SEPARATOR = ","
    LAST_VALUE_IN_LINE_SEPARATOR = ","
    SEPARATE_3RD_BLOCKS = "\n"
    IMAGE_SECTION_START = "\n// image data\n\n"
    ATTRIB_SECTION_START = "\n// attrib data\n\n"

if FORMAT == "z80asm":
    # Z80 ASM FORMAT
    FILE_BEGIN="\n{name}_data:\t\t; {filename} imported with scr2asm\n"
    FILE_END="\n; end of {filename} import\n"
    HEXA_PREFIX = "#" # or "$"
    LINE_PREFIX = "\tdefb\t" # or "db"
    LINE_SUFIX = "\n" # 
    VALUES_SEPARATOR = ","
    LAST_VALUE_IN_LINE_SEPARATOR = ""
    SEPARATE_3RD_BLOCKS = "\n"
    IMAGE_SECTION_START = "\n; image data\n\n"
    ATTRIB_SECTION_START = "\n; attrib data\n\n"


def replaceFields(text):
    return text.replace( "{filename}", fileName).replace( "{name}", fileName.rsplit(".",1 )[0].rsplit("/",-1)[-1])

if __name__ == "__main__":

    if len( sys.argv ) < 2 :
        print( """
    ZX Speccy screen dump to asm
    by Carles Oriol 2020

    Usage: scr2asm [-format] screenname.scr
    Available formats:

                        -c       C
                        -z80asm  Z80 ASM" 
""")
        sys.exit( -1 )
    
    fileName = sys.argv[-1]

    with open(fileName, mode='rb') as file:        
        fileContent = file.read()

        print( replaceFields(FILE_BEGIN), end="" )   

        for y in range(0,192+24): 

            if IMAGE_SECTION_START != "" and y==0:
                print (IMAGE_SECTION_START, end="")
            if ATTRIB_SECTION_START != "" and y==192:
                print (ATTRIB_SECTION_START, end="")

            print( LINE_PREFIX, end="")
            for x in range(0,32):
                print( HEXA_PREFIX+'{:02X}'.format( fileContent[x+y*32]), end="")                
                print(VALUES_SEPARATOR if x < 31 else LAST_VALUE_IN_LINE_SEPARATOR, end = "")

            print( LINE_SUFIX, end="")

            if y in [64,128] and SEPARATE_3RD_BLOCKS != "": # separate 3rds blocks
                print(SEPARATE_3RD_BLOCKS, end="")

        print( replaceFields(FILE_END)  )
