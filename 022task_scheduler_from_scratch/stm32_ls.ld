ENTRY(Reset_Handler)

MEMORY
{
	FLASH(rx):ORIGIN=0x08000000,LENGTH=1024K
	SRAM1(rwx):ORIGIN=0x200000000,LENGTH=128K
}

SECTIONS
{
	.text:
	{
		*(.isr_vector)
		*(.text)
		*(.rodata)
		_etext = .;
	}> FLASH

	.data:
	{
		_sdata = .;
		*(.data)
		_edata = .;
	}> SRAM AT> FLASH

	.bss
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
	}> SRAM
}
