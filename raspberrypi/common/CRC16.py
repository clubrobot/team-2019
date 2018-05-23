#!/usr/bin/env python3

def CRCprocessBuffer(Buffer):
   #~~ Generierung der CCITT-CRC16 Checksumme
   bitrange = range(8) # 8 Bits
   crcsum   = 0
   polynom  = 0x1021 #CCITT Polynom

   for byte in Buffer:
      crcsum ^= byte << 8
      for bit in bitrange: # Schleife für 8 Bits
         crcsum <<= 1
         if crcsum & 0x7FFF0000:
            #~~ Es gab einen Uebertrag ins Bit-16
            crcsum = (crcsum & 0x0000FFFF) ^ polynom
   return crcsum

def CRCcheck(buffer,crc):
    
    new_crc = CRCprocessBuffer(buffer)

    if crc == new_crc:
        return True
    else:
        return False