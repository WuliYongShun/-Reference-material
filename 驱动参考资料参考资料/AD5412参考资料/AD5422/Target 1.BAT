SET PATH=C:\Keil\ARM\BIN40\;C:\MentorGraphics\9.0PADS\SDD_HOME\common\win32\bin;C:\MentorGraphics\9.0PADS\SDD_HOME\common\win32\lib;C:/MentorGraphics/9.0PADS/MGC_HOME.ixn/bin;C:/MentorGraphics/9.0PADS/MGC_HOME.ixn/lib;C:\MentorGraphics\9.0PADS\MGC_HOME.ixn\bin;C:\MentorGraphics\9.0PADS\MGC_HOME.ixn\lib;C:\Program Files\Mentor Graphics\PADS\2005_1\Programs;C:\Program Files\Microsoft Visual Studio\Common\Tools\WinNT;C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin;C:\Program Files\Microsoft Visual Studio\Common\Tools;C:\Program Files\Microsoft Visual Studio\VC98\bin;C:\Program Files\StormII\Codec;C:\Program Files\StormII
SET RVCT40INC=C:\Keil\ARM\RV31\INC
SET RVCT40LIB=C:\Keil\ARM\RV31\LIB
SET CPU_TYPE=ADuC7026
SET CPU_VENDOR=Analog Devices
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00000000
"C:\Keil\ARM\BIN40\ArmAsm" --Via "aduc702x._ia"
"C:\Keil\ARM\BIN40\ArmCC" --Via "main.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via "aduc7026driver.__i"
"C:\Keil\ARM\BIN40\ArmLink" --Via "test.lnp"
C:\Keil\ARM\BIN40\fromelf.exe "test.axf" --i32combined --output "test.hex"
