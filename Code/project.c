#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

uint16_t back[] = {
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011,
	0b0000000000000011
};

uint16_t front[] = {
	0b1000000000000000,
	0b1000000000000000,
	0b1000000000000000,
	0b1000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};

uint16_t evil[] = {
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};

uint16_t evilgun[] = {
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};


uint16_t box[] = {
 	0b0000000000000000,
	0b0000000000000000,
	0b0000000000001100,
	0b0000000000000100,
	0b0000000000001100,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};

uint16_t gun[] = {
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};

uint16_t over[] = {
	0b0111110000000000,
	0b0100010000000000,
	0b0111110000000000,
	0b0000000000000000,
	0b0111110000000000,
	0b0101000000000000,
	0b0111000000000000,
	0b0000000000000000,
	0b0111110000000000,
	0b0101000000000000,
	0b0111000000000000,
	0b0000000000000000,
	0b0111010000000000,
	0b0101010000000000,
	0b0101110000000000,
	0b0000000000000000
};

uint16_t scor[] = {
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};

int digit[][3]={
    {
        0b11111,
        0b10001,
        0b11111,
    },
    {
        0b11111,
        0b10000,
        0b00000,
    },
    {
        0b11101,
        0b10101,
        0b10111,
    },
    {
        0b11111,
        0b10101,
        0b10101,
    },
    {
        0b11111,
        0b00100,
        0b11100,
    },
    {
        0b10111,
        0b10101,
        0b11101,
    },
    {
        0b10111,
        0b10101,
        0b11111,
    },
    {
        0b11111,
        0b10000,
        0b10000,
    },
    {
        0b11111,
        0b10101,
        0b11111,
    },
    {
        0b11111,
        0b10101,
        0b11101,
    }
};

int speed, cnt, gcnt, gap, jmp, evilrand , jcnt, jwait, lwait, xp, yp, xe, evilguncnt, evilgap, lifecnt, eviltime, score;

void moveleft(){
	if(box[0]) return;
	int i;
	for(i=1; i<16; i++){
		box[i-1]=box[i];
	}
	box[15] = 0;
	xp--;
}

void moveright(){
	if(box[15]) return;
	int i;
	for(i=14; i>=0; i--){
		box[i+1]=box[i];
	}
	box[0] = 0;
	xp++;
}

void moveup(){
	int i;
	for(i=0; i<16; i++){
		box[i]<<=1;
	}
	yp++;
}


void movedown(){
	int i;
	for(i=0; i<16; i++){
		box[i]>>=1;
	}
	yp--;
}

void shootgun(){
	gun[xp]|=(1<<yp);
	gun[xp+2]|=(1<<yp);
}

void gunmove(){
	int i;
	for(i=0; i<16; i++){
		gun[i]<<=1;
	}
}

void evilshoot(){
    if(xe<13){
        evilgun[xe]|=(1<<12);
        evilgun[xe+3]|=(1<<12);
    }
}

void evilgunmove(){
    if(xe<13){
        int i;
        for(i=0; i<16; i++){
            evilgun[i]>>=1;
        }
    }
}


void evilleft(){
    if(xe<=16){
        eviltime++;
        if(evil[0]) return;
        int i;
        for(i=1; i<16; i++){
            evil[i-1]=evil[i];
        }
        xe--;
        if(xe==15 || xe==12){
            evil[15]=0b0011000000000000;
        }
        else if(xe>12){
            evil[15]=0b0001000000000000;
        }
        else{
            evil[15]=0b0000000000000000;
        }
    }
    else{
        xe--;
    }
}

void evilright(){
     if(xe<16){
        eviltime++;
        if(evil[15]) return;
        int i;
        for(i=14; i>=0; i--){
            evil[i+1]=evil[i];
        }
        evil[0] = 0;
        xe++;
    }
    else{
        moveleft();
    }
}

void draw(){
	int cnt=6;
	while(cnt--){
		if(cnt%3==0){
            int i,j;
			for(i=0; i<16; i++){
				PORTC=0b10000001;
				PORTB=0x00;
				PORTA^=PORTA&(((1<<4)-1)<<4);
				PORTD^=PORTD&(((1<<4)-1)<<4);
				int bit=(back[i])>>8;
				PORTA=i|((bit&((1<<4)-1))<<4);
		    		PORTB=back[i];
		    		PORTD|=bit&(((1<<4)-1)<<4);
			}
		}
		else if(cnt%3==1){
			int i,j;
			for(i=0; i<16; i++){
				PORTC=0b10000010;
				PORTB=0x00;
				PORTA^=PORTA&(((1<<4)-1)<<4);
				PORTD^=PORTD&(((1<<4)-1)<<4);
				int bit=(front[i]|evil[i]|gun[i]|evilgun[i])>>8;
				PORTA=i|((bit&((1<<4)-1))<<4);
		    		PORTB=front[i]|evil[i]|gun[i]|evilgun[i];
		    		PORTD|=bit&(((1<<4)-1)<<4);
			}
		}
		else if(cnt%3==2){
            int i,j;
			for(i=0; i<16; i++){
				PORTC=0b00000011;
				PORTB=0x00;
				PORTA^=PORTA&(((1<<4)-1)<<4);
				PORTD^=PORTD&(((1<<4)-1)<<4);
				int bit=(box[i])>>8;
				PORTA=i|((bit&((1<<4)-1))<<4);
                PORTB=box[i];
                PORTD|=bit&(((1<<4)-1)<<4);
			}
        }
	}
}

void drawover(){
	int cnt=6;
	while(cnt--){
		if(cnt&1){
			int i,j;
			for(i=0; i<16; i++){
				PORTC=0b10000010;
				PORTB=0x00;
				PORTA^=PORTA&(((1<<4)-1)<<4);
				PORTD^=PORTD&(((1<<4)-1)<<4);
				int bit=(over[i])>>8;
				PORTA=i|((bit&((1<<4)-1))<<4);
                PORTB=over[i];
                PORTD|=bit&(((1<<4)-1)<<4);
			}
		}
		else{
            int i,j;
			for(i=0; i<16; i++){
				PORTC=0b10000000;
				PORTB=0x00;
				PORTA^=PORTA&(((1<<4)-1)<<4);
				PORTD^=PORTD&(((1<<4)-1)<<4);
				int bit=(scor[i])>>8;
				PORTA=i|((bit&((1<<4)-1))<<4);
                PORTB=scor[i];
                PORTD|=bit&(((1<<4)-1)<<4);
			}
        }
	}
}

void dec_life_cnt(){
	front[lifecnt-1]<<=1;
	front[lifecnt-1]>>=1;
	lifecnt--;
}


void playershothandle(){
    int i=0;
    for(i=0; i<16; i++){
        if(box[i]&evilgun[i]) dec_life_cnt(), evilgun[i]^=(box[i]&evilgun[i]);
    }
}

void playerhithandle(){
    int i=0;
    for(i=0; i<16; i++){
        if(box[i]&back[i]){
            dec_life_cnt();
            break;
        }
    }
}


void writescore(int scr){
    int i,j;
    for(i=0; i<3; i++){
        int d=scr%10;
        for(j=0; j<3; j++){
            int idx=13-i*4-j;
            scor[idx]&=(((1<<8)-1)<<8);
            scor[idx]|=digit[d][j]<<1;
        }
        scr/=10;
    }
}

void reset(){
    int i;
    for(i=0; i<16; i++){
        back[i]=0b0000000000000011;
        evil[i]=0b0000000000000000;
        gun[i]=0b0000000000000000;
        evilgun[i]=0b0000000000000000;
    }
    for(i=0; i<16; i++){
        if(i<4) front[i]=0b1000000000000011;
        else front[i]=0b0000000000000011;
    }
    for(i=0; i<16; i++){
        if(i==2 || i==4) box[i]=0b0000000000001100;
        else if(i==3) box[i]=0b000000000000100;
        else box[i]=0b1000000000000000;
    }
}

ISR(INT1_vect){
    if(lifecnt==0) jmp=1;
    else if(jwait==0){
    	jmp=3*speed;
        jcnt=jmp/8;
        jwait=3*speed+10;
    }
}

ISR(INT0_vect){
	if(lwait==0){
        moveleft();
        lwait=speed/3;
	}
}

int main(void){
    DDRA=0xFF;
    DDRB=0xFF;
    DDRC=0xFF;
    DDRD=0xF0;
    PORTC=0x00;
    PIND=0x0F;
    GICR |= (1<<INT0);
    GICR |= (1<<INT1);
    MCUCR |= 0b11111111;
    sei();
    while(1){
        speed=300, cnt=0;                       // Speed
        gcnt=2, gap=rand()%6+9;                 // Gunshoot
        jmp=0, jcnt=0, jwait=0;                 // Jump
        lwait = 0;                              // Left
        xp = 2, yp = 3, xe = 40;                // Position
        evilguncnt = 0, evilgap = rand()%3+3;   // Evil gunshoot
        lifecnt = 4, eviltime=0;                            // Life
        score = 0;
        while(1){
            draw();
            int i;
            if(cnt==speed){
                evilrand = rand()%16;
                if(evilrand<xe){
                    evilleft();
                }
                else if(evilrand>xe){
                    evilright();
                }
                gcnt++;
                for(i=1;i<16;i++){
                    back[i-1]=back[i];
                }
                if((gcnt%gap)==0) back[15]=0b0000000000001111, gcnt=0, gap=rand()%10+7;
                else if((gcnt%gap)==1) back[15]=0b0000000000000111;
                else back[15]=0b0000000000000011;
                cnt=0;
                playerhithandle();
                if(lifecnt==0) break;
                score++;
            }
            evilguncnt++;
            if(cnt%(speed/3)==0){
                gunmove();
                evilgunmove();
                playershothandle();
                if(lifecnt==0) break;
            }
            cnt++;
            if(jmp){
                if(jmp%jcnt==0){
                    int val=jmp/jcnt;
                    if(val>=7){
                         moveup();
                    }
                    else if(val==6) {
                        moveup(), moveright();
                    }
                    else if(val>=4) {
                        moveright();
                    }
                    else if(val==3){
                         movedown(), moveright();
                    }
                    else{
                         movedown();
                    }
                }
                jmp--;
            }
            if(evilguncnt==(evilgap*speed)){
                evilshoot();
                evilguncnt=0;
            }
            if(jwait) jwait--;
            if(lwait) lwait--;
        }
        writescore(score);
        while(1){
            drawover();
            if(jmp==1){
                reset();
                break;
            }
        }
    }
    return 0;
}
