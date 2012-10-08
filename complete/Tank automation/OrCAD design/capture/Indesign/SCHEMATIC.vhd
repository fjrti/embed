LIBRARY IEEE;
USE IEEE.std_logic_1164.all;

ENTITY SCHEMATIC1 IS 

END SCHEMATIC1;



ARCHITECTURE STRUCTURE OF SCHEMATIC1 IS

-- COMPONENTS

COMPONENT LM7805
	PORT (
	VIN : IN std_logic;
	GND : IN std_logic;
	VOUT : OUT std_logic
	); END COMPONENT;

COMPONENT Relay
	PORT (
	\2\ : INOUT std_logic;
	\1\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\6\ : INOUT std_logic
	); END COMPONENT;

COMPONENT Cap
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT ATMega16
	PORT (
	\PB0/(XCK/T0)\ : INOUT std_logic;
	\PB1/(T1)\ : INOUT std_logic;
	\PB2/(INT2/AIN0)\ : INOUT std_logic;
	\PB3/(OC0/AIN1)\ : INOUT std_logic;
	\PB4/(S\\S\\)\ : INOUT std_logic;
	\PB5/(MOSI)\ : INOUT std_logic;
	\PB6/(MISO)\ : INOUT std_logic;
	\PB7/(SCK)\ : INOUT std_logic;
	\R\\E\\S\\E\\T\\\ : INOUT std_logic;
	XTAL2 : INOUT std_logic;
	XTAL1 : INOUT std_logic;
	\PD0/(RXD)\ : INOUT std_logic;
	\PD1/(TXD)\ : INOUT std_logic;
	\PD2/(INT0)\ : INOUT std_logic;
	\PD3/(INT1)\ : INOUT std_logic;
	\PD4/(OC1B)\ : INOUT std_logic;
	\PD5/(OC1A)\ : INOUT std_logic;
	\PD6/(ICP)\ : INOUT std_logic;
	\PD7/(OC2)\ : INOUT std_logic;
	VCC : INOUT std_logic;
	GND : INOUT std_logic;
	\PA7/(ADC7)\ : INOUT std_logic;
	\PA6/(ADC6)\ : INOUT std_logic;
	\PA5/(ADC5)\ : INOUT std_logic;
	\PA4/(ADC4)\ : INOUT std_logic;
	\PA2/(ADC2)\ : INOUT std_logic;
	\PA3/(ADC3)\ : INOUT std_logic;
	\PA1/(ADC1)\ : INOUT std_logic;
	\PA0/(ADC0)\ : INOUT std_logic;
	\PC0/(SCL)\ : INOUT std_logic;
	\PC1/(SDA)\ : INOUT std_logic;
	\PC2/(TCK)\ : INOUT std_logic;
	\PC3/(TMS)\ : INOUT std_logic;
	\PC4/(TDO)\ : INOUT std_logic;
	\PC5/(TDI)\ : INOUT std_logic;
	\PC6/(TOSC1)\ : INOUT std_logic;
	\PC7/(TOSC2)\ : INOUT std_logic;
	AVCC : INOUT std_logic;
	AREF : INOUT std_logic
	); END COMPONENT;

COMPONENT ULN2803
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\5\ : INOUT std_logic;
	\6\ : INOUT std_logic;
	\7\ : INOUT std_logic;
	\8\ : INOUT std_logic;
	\9\ : INOUT std_logic;
	\10\ : INOUT std_logic;
	\11\ : INOUT std_logic;
	\12\ : INOUT std_logic;
	\13\ : INOUT std_logic;
	\14\ : INOUT std_logic;
	\15\ : INOUT std_logic;
	\16\ : INOUT std_logic;
	\17\ : INOUT std_logic;
	\18\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \16x2 LCD\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\5\ : INOUT std_logic;
	\6\ : INOUT std_logic;
	\7\ : INOUT std_logic;
	\8\ : INOUT std_logic;
	\9\ : INOUT std_logic;
	\10\ : INOUT std_logic;
	\11\ : INOUT std_logic;
	\12\ : INOUT std_logic;
	\13\ : INOUT std_logic;
	\14\ : INOUT std_logic;
	\15\ : INOUT std_logic;
	\16\ : INOUT std_logic
	); END COMPONENT;

COMPONENT BRIDGE
	PORT (
	AC1 : INOUT std_logic;
	\+DC\ : INOUT std_logic;
	AC2 : INOUT std_logic;
	\-DC\ : INOUT std_logic
	); END COMPONENT;

-- SIGNALS

SIGNAL GND : std_logic;
SIGNAL N00481 : std_logic;
SIGNAL N00489 : std_logic;
SIGNAL N00497 : std_logic;
SIGNAL N00855 : std_logic;
SIGNAL N00874 : std_logic;
SIGNAL N01199 : std_logic;
SIGNAL N01448 : std_logic;
SIGNAL N01458 : std_logic;
SIGNAL N01465 : std_logic;
SIGNAL N02188 : std_logic;
SIGNAL N02196 : std_logic;
SIGNAL N02203 : std_logic;

-- INSTANCE ATTRIBUTES



-- GATE INSTANCES

BEGIN
U1 : LM7805	PORT MAP(
	VIN => N00855, 
	GND => GND, 
	VOUT => N01199
);
L1 : Relay	PORT MAP(
	\2\ => OPEN, 
	\1\ => N00855, 
	\3\ => N00497, 
	\4\ => OPEN, 
	\6\ => OPEN
);
C1 : Cap	PORT MAP(
	\1\ => N00855, 
	\2\ => N00874
);
C2 : Cap	PORT MAP(
	\1\ => N00855, 
	\2\ => N00874
);
C4 : Cap	PORT MAP(
	\1\ => N01199, 
	\2\ => N00874
);
IC1 : ATMega16	PORT MAP(
	\PB0/(XCK/T0)\ => OPEN, 
	\PB1/(T1)\ => OPEN, 
	\PB2/(INT2/AIN0)\ => OPEN, 
	\PB3/(OC0/AIN1)\ => OPEN, 
	\PB4/(S\\S\\)\ => OPEN, 
	\PB5/(MOSI)\ => OPEN, 
	\PB6/(MISO)\ => OPEN, 
	\PB7/(SCK)\ => OPEN, 
	\R\\E\\S\\E\\T\\\ => OPEN, 
	XTAL2 => OPEN, 
	XTAL1 => N02196, 
	\PD0/(RXD)\ => N02203, 
	\PD1/(TXD)\ => OPEN, 
	\PD2/(INT0)\ => OPEN, 
	\PD3/(INT1)\ => OPEN, 
	\PD4/(OC1B)\ => N01448, 
	\PD5/(OC1A)\ => N02188, 
	\PD6/(ICP)\ => N01465, 
	\PD7/(OC2)\ => N01458, 
	VCC => OPEN, 
	GND => OPEN, 
	\PA7/(ADC7)\ => OPEN, 
	\PA6/(ADC6)\ => OPEN, 
	\PA5/(ADC5)\ => OPEN, 
	\PA4/(ADC4)\ => OPEN, 
	\PA2/(ADC2)\ => OPEN, 
	\PA3/(ADC3)\ => OPEN, 
	\PA1/(ADC1)\ => N00489, 
	\PA0/(ADC0)\ => N00481, 
	\PC0/(SCL)\ => OPEN, 
	\PC1/(SDA)\ => OPEN, 
	\PC2/(TCK)\ => OPEN, 
	\PC3/(TMS)\ => OPEN, 
	\PC4/(TDO)\ => OPEN, 
	\PC5/(TDI)\ => OPEN, 
	\PC6/(TOSC1)\ => OPEN, 
	\PC7/(TOSC2)\ => OPEN, 
	AVCC => OPEN, 
	AREF => OPEN
);
J1 : ULN2803	PORT MAP(
	\1\ => N00481, 
	\2\ => N00497, 
	\3\ => N00489, 
	\4\ => OPEN, 
	\5\ => OPEN, 
	\6\ => OPEN, 
	\7\ => OPEN, 
	\8\ => OPEN, 
	\9\ => OPEN, 
	\10\ => OPEN, 
	\11\ => OPEN, 
	\12\ => OPEN, 
	\13\ => OPEN, 
	\14\ => OPEN, 
	\15\ => OPEN, 
	\16\ => OPEN, 
	\17\ => OPEN, 
	\18\ => OPEN
);
J3 : \16x2 LCD\	PORT MAP(
	\1\ => N00874, 
	\2\ => N01199, 
	\3\ => N00874, 
	\4\ => N02196, 
	\5\ => N00874, 
	\6\ => N02203, 
	\7\ => OPEN, 
	\8\ => OPEN, 
	\9\ => OPEN, 
	\10\ => OPEN, 
	\11\ => OPEN, 
	\12\ => OPEN, 
	\13\ => N01448, 
	\14\ => N02188, 
	\15\ => N01465, 
	\16\ => N01458
);
D1 : BRIDGE	PORT MAP(
	AC1 => N00855, 
	\+DC\ => N00874, 
	AC2 => OPEN, 
	\-DC\ => OPEN
);
END STRUCTURE;

