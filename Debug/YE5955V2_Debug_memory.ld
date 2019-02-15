/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2019
 * Generated linker script file for MKL16Z64xxx4
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v10.1.0 [Build 589] [2017-11-14]  on Feb 11, 2019 8:41:11 AM
 */

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x10000 /* 64K bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x1ffff800, LENGTH = 0x2000 /* 8K bytes (alias RAM) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_PROGRAM_FLASH = 0x0 + 0x10000 ; /* 64K bytes */  
  __top_Flash = 0x0 + 0x10000 ; /* 64K bytes */  
  __base_SRAM = 0x1ffff800  ; /* SRAM */  
  __base_RAM = 0x1ffff800 ; /* RAM */  
  __top_SRAM = 0x1ffff800 + 0x2000 ; /* 8K bytes */  
  __top_RAM = 0x1ffff800 + 0x2000 ; /* 8K bytes */  
