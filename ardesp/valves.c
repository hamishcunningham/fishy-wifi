
const int V1 = 1; // growbed
const int V2 = 2; //     valve
const int V3 = 3; //         ids
const int L1 = 1; // growbed
const int L2 = 2; //     water
const int L3 = 3; //         levels

// valve and level state machine
int IN_THE_BEGINNING = -1;

typedef struct {
  long readingTime = -1;
  long prevReadingTime = -1;
  int state = IN_THE_BEGINNING;
  int fillingValve = -1;

  bool v1on = false;
  bool v2on = false;
  bool v3on = false;

  bool b1filling = false;
  bool b2filling = false;
  bool b3filling = false;
} flowState_t;
flowState_t flowState;

void doValveLogic(monitor_t *m, flowState_t *s) {
  s->prevReadingTime = s->readingTime;
  s->readingTime = millis();

  if(s->state == IN_THE_BEGINNING) { // assume we just booted
    choose a valve at random and set it to filling; turn the valve on
    set the other valves to draining and turn them off; set their drain cycle start timestamps to now
  } else {
    if fillingValve is full; set it to draining and turn off
      set its drain cycle start timestamp to now
      set fillingValve to next valve
    
  }


  choose a pump
    if we're filling
      if we're full, swap state to draining
      move on to next pump
    if we're draining
      if we've waited DRAIN_CYCLE_LENGTH swap state to filling

  if(s->state == IN_THE_BEGINNING) { // assume we just booted
    s->v1

    handle_pump1() { handle_pump(1, PUMP1_MCP_PIN); }
    swapPumpState(pumpNum, newState, mcpPin);

  } else { // check sanity of fill vectors, generate warning re. water pump?
  }

  return s->state;
}
