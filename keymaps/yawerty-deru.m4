!
! This is an `xmodmap' input file for a German PC 102 key keyboard
! and use with the xruskb program (version 1.9.3 plus patches
! for swapping groups of more than two columns)
! It establishes a combined German / Russian (yawerty) keyboard layout.

keycode 0x09 =	Escape
keycode 0x43 =	F1
keycode 0x44 =	F2
keycode 0x45 =	F3
keycode 0x46 =	F4
keycode 0x47 =	F5
keycode 0x48 =	F6
keycode 0x49 =	F7
keycode 0x4A =	F8
keycode 0x4B =	F9
keycode 0x4C =	F10
keycode 0x5F =	F11
keycode 0x60 =	F12
keycode 0x6F =	Print		Execute
keycode 0x4E =	Scroll_Lock
keycode 0x6E =	Pause		Break
keycode 0x31 =	asciicircum	degree						Cyrillic_yu	Cyrillic_YU
keycode 0x0A =	1		exclam						1		exclam
keycode 0x0B =	2		quotedbl	twosuperior 	oneeighth   	2		at		2		quotedbl
keycode 0x0C =	3		section		threesuperior	sterling	3		Cyrillic_hardsign 3		section
keycode 0x0D =	4		dollar						4		Cyrillic_HARDSIGN
keycode 0x0E =	5		percent						5		percent
keycode 0x0F =	6		ampersand					6		asciicircum
keycode 0x10 =	7		slash		braceleft	seveneighths	7		ampersand 	7		slash
keycode 0x11 =	8		parenleft 	bracketleft	trademark	8		asterisk	8 		parenleft
keycode 0x12 =	9		parenright	bracketright	plusminus	9		parenleft	9		parenright
keycode 0x13 =	0		equal		braceright	degree		0		parenright	0		equal
keycode 0x14 =	ssharp		question	backslash	questiondown	minus		underscore	ssharp		question
keycode 0x15 =	apostrophe	grave						Cyrillic_che	Cyrillic_CHE
keycode 0x16 =	BackSpace
keycode 0x6A =	Insert
keycode 0x61 =	Home
keycode 0x63 =	Prior
keycode 0x4D =	Num_Lock	Pointer_EnableKeys
keycode 0x70 =	KP_Divide
keycode 0x3F =	KP_Multiply
keycode 0x52 =	KP_Subtract
keycode 0x17 =	Tab		ISO_Left_Tab
keycode 0x18 =	q		Q		at		NoSymbol	Cyrillic_ya	Cyrillic_YA	q		Q
keycode 0x19 =	w		W						Cyrillic_ve	Cyrillic_VE
keycode 0x1A =	e		E		0xa4 		0xa4 		Cyrillic_ie	Cyrillic_IE	e		E
keycode 0x1B =	r		R						Cyrillic_er	Cyrillic_ER
keycode 0x1C =	t		T						Cyrillic_te	Cyrillic_TE
keycode 0x1D =	z		Z						Cyrillic_yeru	Cyrillic_YERU
keycode 0x1E =	u		U						Cyrillic_u	Cyrillic_U
keycode 0x1F =	i		I						Cyrillic_i	Cyrillic_I
keycode 0x20 =	o		O						Cyrillic_o	Cyrillic_O
keycode 0x21 =	p		P						Cyrillic_pe	Cyrillic_PE
keycode 0x22 =	udiaeresis	Udiaeresis					Cyrillic_sha	Cyrillic_SHA
keycode 0x23 =	plus		asterisk	 asciitilde 	macron		Cyrillic_shcha	Cyrillic_shcha	plus		asterisk
keycode 0x24 =	Return
keycode 0x6B =	Delete
keycode 0x67 =	End
keycode 0x69 =	Next
keycode 0x4F =	KP_Home		KP_7
keycode 0x50 =	KP_Up		KP_8
keycode 0x51 =	KP_Prior	KP_9
keycode 0x56 =	KP_Add
keycode 0x42 =	Caps_Lock
keycode 0x26 =	a		A						Cyrillic_a	Cyrillic_A
keycode 0x27 =	s		S						Cyrillic_es	Cyrillic_ES
keycode 0x28 =	d		D						Cyrillic_de	Cyrillic_DE
keycode 0x29 =	f		F						Cyrillic_ef	Cyrillic_EF
keycode 0x2A =	g		G						Cyrillic_ghe	Cyrillic_GHE
keycode 0x2B =	h		H						Cyrillic_ha	Cyrillic_HA
keycode 0x2C =	j		J						Cyrillic_shorti	Cyrillic_SHORTI
keycode 0x2D =	k		K						Cyrillic_ka	Cyrillic_KA
keycode 0x2E =	l		L						Cyrillic_el	Cyrillic_EL
keycode 0x2F =	odiaeresis	Odiaeresis					semicolon	colon
keycode 0x30 =	adiaeresis	Adiaeresis					apostrophe	quotedbl
keycode 0x33 =	numbersign	apostrophe					Cyrillic_e	Cyrillic_E
keycode 0x53 =	KP_Left		KP_4
keycode 0x54 =	KP_Begin	KP_5
keycode 0x55 =	KP_Right	KP_6
keycode 0x32 =	Shift_L

! This key is only present on German keyboards, not on English ones. I took the liberty to place the IO here.
keycode 0x5E =	less		greater 	bar		brokenbar	Cyrillic_io	Cyrillic_IO 	less		 greater

keycode 0x34 =	y		Y						Cyrillic_ze	Cyrillic_ZE
keycode 0x35 =	x		X						Cyrillic_softsign Cyrillic_SOFTSIGN
keycode 0x36 =	c		C						Cyrillic_tse	Cyrillic_TSE
keycode 0x37 =	v		V						Cyrillic_zhe    Cyrillic_ZHE
keycode 0x38 =	b		B						Cyrillic_be	Cyrillic_BE
keycode 0x39 =	n		N						Cyrillic_en	Cyrillic_EN
keycode 0x3A =	m		M		mu		 masculine 	Cyrillic_em	Cyrillic_EM	m		M
keycode 0x3B =	comma		semicolon					comma 		less
keycode 0x3C =	period		colon						period		greater
keycode 0x3D =	minus		underscore					slash		question
keycode 0x3E =	Shift_R
keycode 0x62 =	Up
keycode 0x57 =	KP_End		KP_1
keycode 0x58 =	KP_Down		KP_2
keycode 0x59 =	KP_Next		KP_3
keycode 0x6C =	KP_Enter
keycode 0x25 =	Control_L
keycode 0x40 =	Alt_L Meta_L
keycode 0x41 =	space
keycode 0x71 =	Mode_switch	Multi_key
keycode 0x6D =	Control_R
keycode 0x64 =	Left
keycode 0x68 =	Down
keycode 0x66 =	Right
keycode 0x5A =	KP_Insert	KP_0
keycode 0x5B =	KP_Delete	KP_Decimal

clear Shift
clear Lock
clear Control
clear Mod1
clear Mod2
clear Mod3
clear Mod4
clear Mod5

add    Shift   = Shift_L Shift_R
add    Lock    = Caps_Lock
add    Control = Control_L Control_R
add    Mod1    = Alt_L
add    Mod2    = Num_Lock
add    Mod3    = Mode_switch
add    Mod5    = Scroll_Lock
