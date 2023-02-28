.global _ZN3CCB13contextSwitchEPNS_7ContextES1_
.type _ZN3CCB13contextSwitchEPNS_7ContextES1_, @function
_ZN3CCB13contextSwitchEPNS_7ContextES1_:
    sd ra, 0 * 8(a0) #old.context.ra
    sd sp, 1 * 8(a0) #old.context.sp

#u registru ra se nalazi adresa povratka iz funkcije contextSwich
#stara nit (koja je pozvala dispatch) odatle treba da nastavi svoje izvrsavanje nekad kasnije, zato u svoje polje ra upisuje tu vrednost


#sada dolazi nova nit koja pocinje da se izvrsava od adrese koja se nalazi u running.context.ra
#zato u registar ra upisujemo njenu vrednost i kad izadjemo iz funkcije odatle krecemo da radimo (NOVA NIT ODATLE RADI)
    ld ra, 0 * 8(a1) #running.context.ra
    ld sp, 1 * 8(a1) #running.context.sp

    ret