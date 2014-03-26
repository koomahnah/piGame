/******************************************************************************
*	kernel.ld
*	 by Alex Chadwick
*
*	A linker script for generation of raspberry pi kernel images.
******************************************************************************/

SECTIONS {
	/*
	* First and formost we need the .init section, containing the IVT.
	*/
	.init 0x8000 : {
		*(.init)
	}
	
	.text : {
		*(.text)
	}
	
	/* 
	* Next we put the data.
	*/
	.data : {
		*(.data)
	}
	_bss_start = .;
	.bss : {
		*(.bss)
	}
	_bss_end = .;
	.else : {
		*(*)
	}
	_else_end = .;
}
