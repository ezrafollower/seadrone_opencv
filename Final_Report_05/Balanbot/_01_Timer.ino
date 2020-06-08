//timer for reading (sending) data from (to) phone(matlab)
// max sampling time: 0.025s => max sampling rate: 40Hz (for serial)

//bool flag1 = false, flag2 = false, flag3 = false;
void SetupMsTimer(){
    MsTimer2::set(1000, TimerInterrupt);  // 10ms update
    MsTimer2::start();
}

void TimerInterrupt(){
  timer_count += 1;
/*------不要在這傳bt到手機*/
}


