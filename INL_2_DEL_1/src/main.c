#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN PD3 // Anslut LED till pinne PD3

volatile uint8_t ledState = 0; // Variabel för att hålla reda på LED-statusen

void setup_Timer();
void setup_led();

void setup() {
    // Konfigurera timer1
    setup_Timer();
    // Konfigurera LED
    setup_led();
    // Aktivera globala avbrott
    sei();
}

void loop() {
    // Inget behövs pga ISR för att hantera LED-toggling
}

// Avbrottservice rutin för Timer 1 Output Compare A Match
ISR(TIMER1_COMPA_vect) {
    // Toggle LED-pinnen baserat på den senaste LED-statusen
    ledState ^= 1;
    if (ledState) {
        PORTD |= (1 << LED_PIN); // Slå på LED
    } else {
        PORTD &= ~(1 << LED_PIN); // Slå av LED
    }
}

// Funktion för att konfigurera TIMER1
void setup_Timer() {
    // (TIMER/COUNTER CONTROL REGISTER) (1A & 1B)
    TCCR1A = 0; // Nollställa. Inga åtgärder behövs för TCCR1A. Detta för mer avancerad applikationer.
    TCCR1B = 0; // Nollställa.
    TCCR1B |= (1 << WGM12); // Sätt WGM12 för att aktivera CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // Beräkna rätt värde för compare match (OCR1A) för att generera en 200 ms period
    // Compare Match Reg = [(Clock Frequncy * Desired time period (sekunder) / Prescaler] - 1 = OCR1A
    // Compare Match Reg = [(16 MHz * 0.2s) / 64 ] - 1 = 49999 = 0.2 sekund = 200 ms
    OCR1A = 49999;

    // Aktivera avbrottsbegäran vid compare match A
    TIMSK1 |= (1 << OCIE1A);
}

void setup_led() {
    // Konfigurera LED-pinne
    DDRD |= (1 << LED_PIN); // Sätt bit 3 i DDRD för att göra pin 3 till output
}
