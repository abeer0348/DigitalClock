/*
 * Digital Clock.c
 *
 * Created: 3/12/2022 11:59:58 AM
 * Author : Win10
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void ConfigTimer(){
	// INTERRUPT ENABLE
	sei();  // Global Interrupt
	TIMSK |= (1<<OCIE0); // Output Compare Interrupt
	
	// SET MODE OF TIMER
	TCCR0 |= (1<<WGM01); // CTC Mode

	
	// LOAD THE COMPARE VALUE
	OCR0 = 100; //
	
	//SET THE PRESCALER FOR THE TIMER
	TCCR0 |= (1<<CS00);
}

unsigned char h2=0,h1=0,m2=0,m1=0,s2=0,s1=0;
unsigned char ah2=0,ah1=0,am2=1,am1=0;

unsigned char r1onh2=0,r1onh1=0,r1onm2=1,r1onm1=0;
unsigned char r1offh2=0,r1offh1=0,r1offm2=2,r1offm1=0;


unsigned char r2onh2=0,r2onh1=0,r2onm2=2,r2onm1=0;
unsigned char r2offh2=0,r2offh1=0,r2offm2=3,r2offm1=0;

// Common Cathode
//unsigned char Num[11] = {0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,0b01111111,0b01100111,0b01110111};
//unsigned char Seg_pos[6] = {~1,~2,~4,~8,~16,~32};

// Common Anode
unsigned char Num[11] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000010,0b11111000,0b10000000,0b10011000,0b10001000};
unsigned char Seg_pos[6] = {1,2,4,8,16,32};
int r= 10,z=0;
int pos = 5;

void ChangeTime(){
	
	if (s1>9)	{
		s2++;
		s1=0;
	}
	if (s2>5)	{
		m1++;
		s2=0;
	}
	if (m1>9)	{
		m2++;
		m1=0;
	}
	if (m2>5)	{
		h1++;
		m2=0;
	}
	if (h1>9)	{
		h2++;
		h1=0;
	}
	if (h2>1 && h1>3)	{
		h2=0;
		h1=0;
		m2=0;
		m1=0;
		s2=0;
		s1=0;
	}
}	

void SetChangeTime(){
	
	if (s1>9)	{
		s1=0;
	}
	if (s2>5)	{
		s2=0;
	}
	if (m1>9)	{
		m1=0;
	}
	if (m2>5)	{
		m2=0;
	}
	if ((h1>9) || (h2>1 && h1>3)){
		h1=0;
	}
	if (h2>2)	{
		h2=0;
	}
}

void AlarmChangeTime(){
	
	if (am1>9)	{
		am1=0;
	}
	if (am2>5)	{
		am2=0;
	}
	if ((ah1>9) || (ah2>1 && ah1>3)) {
		ah1=0;
	}
	if (ah2>2)	{
		ah2=0;
	}
}
void R1_OnChangeTime(){
	
	if (r1onm1>9)	{
		r1onm1=0;
	}
	if (r1onm2>5)	{
		r1onm2=0;
	}
	if ((r1onh1>9) || (r1onh2>1 && r1onh1>3)) {
		r1onh1=0;
	}
	if (r1onh2>2)	{
		r1onh2=0;
	}
}
void R1_OffChangeTime(){
	
	if (r1offm1>9)	{
		r1offm1=0;
	}
	if (r1offm2>5)	{
		r1offm2=0;
	}
	if ((r1offh1>9) || (r1offh2>1 && r1offh1>3)) {
		r1offh1=0;
	}
	if (r1offh2>2)	{
		r1offh2=0;
	}
}
void R2_OnChangeTime(){
	
	if (r2onm1>9)	{
		r2onm1=0;
	}
	if (r2onm2>5)	{
		r2onm2=0;
	}
	if ((r2onh1>9) || (r2onh2>1 && r2onh1>3)) {
		r2onh1=0;
	}
	if (r2onh2>2)	{
		r2onh2=0;
	}
}
void R2_OffChangeTime(){
	
	if (r2offm1>9)	{
		r2offm1=0;
	}
	if (r2offm2>5)	{
		r2offm2=0;
	}
	if ((r2offh1>9) || (r2offh2>1 && r2offh1>3)) {
		r2offh1=0;
	}
	if (r2offh2>2)	{
		r2offh2=0;
	}
}
void R1_OnTimeShow(){
	unsigned char Relay1OnTime[6]= {r1onh2,r1onh1,r1onm2,r1onm1,r,z};
	Relay1OnTime[0] = r1onh2;
	Relay1OnTime[1] = r1onh1;
	Relay1OnTime[2] = r1onm2;
	Relay1OnTime[3] = r1onm1;
	Relay1OnTime[5] = z;
	
	if (pos==5) {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay1OnTime[pos]];
		_delay_ms(2);
		PORTC = Seg_pos[pos-1];
		PORTA = Num[Relay1OnTime[pos-1]];
		_delay_ms(2);
	}
	else {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay1OnTime[pos]];
		_delay_ms(2);
	}
	
	if (PIND & (1<<PD7)){
		pos--;
		if(pos==4) {pos = 3;}
		if (pos<0){ pos = 5;}
		_delay_ms(300);
	}
	
	if (PINB & (1<<PB7)){
		if (pos ==5){
			z++;
			if (z>3) {z=0;}
		}
		if (pos ==3){	r1onm1++;	}
		if (pos ==2){	r1onm2++;	}
		if (pos ==1){	r1onh1++;	}
		if (pos ==0){	r1onh2++;	}
		
		R1_OnChangeTime();
		_delay_ms(500);
	}
	
	
}
void R1_OffTimeShow(){
	unsigned char Relay1OffTime[6]= {r1offh2,r1offh1,r1offm2,r1offm1,r,z};
	Relay1OffTime[0] = r1offh2;
	Relay1OffTime[1] = r1offh1;
	Relay1OffTime[2] = r1offm2;
	Relay1OffTime[3] = r1offm1;
	Relay1OffTime[5] = z;
	
	if (pos==5) {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay1OffTime[pos]];
		_delay_ms(2);
		PORTC = Seg_pos[pos-1];
		PORTA = Num[Relay1OffTime[pos-1]];
		_delay_ms(2);
	}
	else {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay1OffTime[pos]];
		_delay_ms(2);
	}
	
	if (PIND & (1<<PD7)){
		pos--;
		if(pos==4) {pos = 3;}
		if (pos<0){ pos = 5;}
		_delay_ms(300);
	}
	
	if (PINB & (1<<PB7)){
		if (pos ==5){
			z++;
			if (z>3) {z=0;}
		}
		if (pos ==3){	r1offm1++;	}
		if (pos ==2){	r1offm2++;	}
		if (pos ==1){	r1offh1++;	}
		if (pos ==0){	r1offh2++;	}
		
		R1_OffChangeTime();
		_delay_ms(500);
	}
	
	
}
void R2_OnTimeShow(){
	unsigned char Relay2OnTime[6]= {r2onh2,r2onh1,r2onm2,r2onm1,r,z};
	Relay2OnTime[0] = r2onh2;
	Relay2OnTime[1] = r2onh1;
	Relay2OnTime[2] = r2onm2;
	Relay2OnTime[3] = r2onm1;
	Relay2OnTime[5] = z;
	
	if (pos==5) {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay2OnTime[pos]];
		_delay_ms(2);
		PORTC = Seg_pos[pos-1];
		PORTA = Num[Relay2OnTime[pos-1]];
		_delay_ms(2);
	}
	else {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay2OnTime[pos]];
		_delay_ms(2);
	}
	
	if (PIND & (1<<PD7)){
		pos--;
		if(pos==4) {pos = 3;}
		if (pos<0){ pos = 5;}
		_delay_ms(300);
	}
	
	if (PINB & (1<<PB7)){
		if (pos ==5){
			z++;
			if (z>3) {z=0;}
		}
		if (pos ==3){	r2onm1++;	}
		if (pos ==2){	r2onm2++;	}
		if (pos ==1){	r2onh1++;	}
		if (pos ==0){	r2onh2++;	}
		
		R2_OnChangeTime();
		_delay_ms(500);
	}
	
	
}	
void R2_OffTimeShow(){
	unsigned char Relay2OffTime[6]= {r2offh2,r2offh1,r2offm2,r2offm1,r,z};
	Relay2OffTime[0] = r2offh2;
	Relay2OffTime[1] = r2offh1;
	Relay2OffTime[2] = r2offm2;
	Relay2OffTime[3] = r2offm1;
	Relay2OffTime[5] = z;
	
	if (pos==5) {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay2OffTime[pos]];
		_delay_ms(2);
		PORTC = Seg_pos[pos-1];
		PORTA = Num[Relay2OffTime[pos-1]];
		_delay_ms(2);
	}
	else {
		PORTC = Seg_pos[pos];
		PORTA = Num[Relay2OffTime[pos]];
		_delay_ms(2);
	}
	
	if (PIND & (1<<PD7)){
		pos--;
		if(pos==4) {pos = 3;}
		if (pos<0){ pos = 5;}
		_delay_ms(300);
	}
	
	if (PINB & (1<<PB7)){
		if (pos ==5){
			z++;
			if (z>3) {z=0;}
		}
		if (pos ==3){	r2offm1++;	}
		if (pos ==2){	r2offm2++;	}
		if (pos ==1){	r2offh1++;	}
		if (pos ==0){	r2offh2++;	}
		
		R2_OffChangeTime();
		_delay_ms(500);
	}
	
	
}
ISR(TIMER0_COMP_vect){
	static int count;
	
	count +=1;
	
	if (count ==10000){
		s1++;
		ChangeTime();
		count = 0;	
	}
	
}
ISR(INT0_vect){
	TCCR0 &= ~(1<<CS00);
	unsigned char Time[6]= {h2,h1,m2,m1,s2,s1};
		
	while (1) {
		Time[0] = h2;
		Time[1] = h1;
		Time[2] = m2;
		Time[3] = m1;
		Time[4] = s2;
		Time[5] = s1;
	
		PORTC = Seg_pos[pos];
		PORTA = Num[Time[pos]];
		_delay_ms(2);		
	
	if (PIND & (1<<PD7)){
		pos--;
		if (pos<0){ pos = 5;}
		_delay_ms(500);
			
	}
	
	if (PINB & (1<<PB7)){
			if (pos ==5){	s1++;	}
			if (pos ==4){	s2++;	}
			if (pos ==3){	m1++;	}
			if (pos ==2){	m2++;	}
			if (pos ==1){	h1++;	}
			if (pos ==0){	h2++;	}
			
		SetChangeTime();
		_delay_ms(500);
		}
		
	if (PINB & (1<<PB6)){
		TCCR0 |= (1<<CS00);
		return;
		}	
	}
}
ISR(INT1_vect){
	unsigned char AlarmTime[4]= {ah2,ah1,am2,am1};
	
	pos = 3;
	while (1) {
		
		AlarmTime[0] = ah2;
		AlarmTime[1] = ah1;
		AlarmTime[2] = am2;
		AlarmTime[3] = am1;
		
		PORTC = Seg_pos[pos];
		PORTA = Num[AlarmTime[pos]];
		_delay_ms(2);
		
		if (PIND & (1<<PD7)){
			pos--;
			if (pos<0){ pos = 3;}
			_delay_ms(500);
		}
		
		if (PINB & (1<<PB7)){
			if (pos ==3){	am1++;	}
			if (pos ==2){	am2++;	}
			if (pos ==1){	ah1++;	}
			if (pos ==0){	ah2++;	}
			
			AlarmChangeTime();
			_delay_ms(500);
		}
		
		if (PINB & (1<<PB6)){
			return;
		}
	}
}
ISR(INT2_vect){
	pos = 5;
	while (1) {
		if (z==0){	R1_OnTimeShow();	}
		if (z==1){	R1_OffTimeShow();	}
		if (z==2){	R2_OnTimeShow();	}
		if (z==3){	R2_OffTimeShow();	}
		
		if (PINB & (1<<PB6)){
			return;
		}
	}
}
void ActuatorCheck(){
	
	if ( h2 == ah2 && h1 == ah1 && m2 == am2 && m1 == am1 )
	{
		PORTD |= (1<<PD0);
	}
	else {
		PORTD &= ~(1<<PD0);
	}
	int Time = (h2*1000)+(h1*100)+(m2*10)+(m1);
	int r1on = (r1onh2*1000)+(r1onh1*100)+(r1onm2*10)+(r1onm1);
	int r1off = (r1offh2*1000)+(r1offh1*100)+(r1offm2*10)+(r1offm1);
	int r2on = (r2onh2*1000)+(r2onh1*100)+(r2onm2*10)+(r2onm1);
	int r2off = (r2offh2*1000)+(r2offh1*100)+(r2offm2*10)+(r2offm1);
	
	if (r1on<r1off) {
		if (	(Time>=r1on) && (Time<r1off)	)
		{
			PORTD &= ~(1<<PD4);
		}
		else {
			PORTD |= (1<<PD4);
		}
	}
	if (r1on>r1off) {
		if (	(Time>=r1on) || (Time<r1off)	)
		{
			PORTD &= ~(1<<PD4);
		}
		else {
			PORTD |= (1<<PD4);
		}
	}
	if (r2on<r2off) {
		if (	(Time>=r2on) && (Time<r2off)	)
		{
			PORTD &= ~(1<<PD5);
		}
		else {
			PORTD |= (1<<PD5);
		}
	}
	if (r2on>r2off) {
		if (	(Time>=r2on) || (Time<r2off)	)
		{
			PORTD &= ~(1<<PD5);
		}
		else {
			PORTD |= (1<<PD5);
		}
	}
	

}
int main(void)
{
    /* Replace with your application code */
	
	DDRA = 0XFF;
	DDRC = 0XFF;
	DDRB = 0x00;
	DDRD = (1<<PD0) | (1<<PD4) | (1<<PD5);
	MCUCSR &= ~(1<<ISC2); // INT2 Falling Edge
	GICR |= (1<<INT0) | (1<<INT1) | (1<<INT2);
	MCUCR = (1<<ISC01) | (1<<ISC11); // INT0 and INT1 Falling Edge
	unsigned char Time[6]= {h2,h1,m2,m1,s2,s1};
		
	ConfigTimer();
	while (1)
			{
				Time[0] = h2;
				Time[1] = h1;
				Time[2] = m2;
				Time[3] = m1;
				Time[4] = s2;
				Time[5] = s1;
				
				for(int i=0;i<6;i++)
				{
					PORTC = Seg_pos[i];
					PORTA = Num[Time[i]];
					_delay_ms(1);
					
				}
				ActuatorCheck();
				
			}
}




