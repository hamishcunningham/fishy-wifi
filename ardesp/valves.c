const int  V_IN_THE_BEGINNING           = -1; // flow...
const int  V_ON_THE_GO                  = 0;  // ...state
const int  V_ARITY                      = 3;  // how many valves?
const long V_FILL_MILLIS                = 15000; // cycle...
const long V_DRAIN_MILLIS               = 45000; // ...timings
const long V_NEXT_CYCLE_DELAY_MILLIS    = 20000; // cycle staggering
const long V_CYCLING                    = -2;    // we're now cycling
const bool V_FILLING                    = true;  // valve...
const bool V_DRAINING                   = false; // ...state

typedef struct { // the valves
  long nextCycle = -1;          // staggered start offset (set on first loop)
  boolean state = V_DRAINING;   // filling or draining
  long lastStateChange = -1;    // when we last flipped
} valve_t;
typedef struct { // flow state
  long readingTime = -1;        // last reading millis
  long prevReadingTime = -1;    // previous reading millis
  int state = V_IN_THE_BEGINNING; // just booted?
  valve_t valves[V_ARITY];      // the valves
} flowState_t;
flowState_t flowState;

void valveLogic(monitor_t *m, flowState_t *s) {
  t = millis();
  s->prevReadingTime = s->readingTime;
  s->readingTime = t;

  if(s->state == V_IN_THE_BEGINNING) { // assume we just booted
    long nextCycleStart = t;
    for(int i = 0; i < V_ARITY; i++) {
      s->valves[i].nextCycle = nextCycleStart;
      nextCycleStart += V_NEXT_CYCLE_DELAY_MILLIS;
    }
    s->state = V_ON_THE_GO;
  } else {
    for(int i = 0; i < V_ARITY; i++) {
      valve_t *v = &( s->valves[i] );
      if(v->nextCycle != V_CYCLING && v->nextCycle < t) // not time to start
        continue;
      else if(v->nextCycle != V_CYCLING) { // time to start: begin filling
        valveStateChange(v, i, V_FILLING);
        v->nextCycle = V_CYCLING; // ignore nextCycle from now on
      }

      if(v->state == V_FILLING) { // we're filling; if full swap to drain
        if(/* TODO */)
          valveStateChange(v, i, V_DRAINING);
      } else { // we're draining; if we waited long enough, swap to fill
        if(t - V_DRAIN_MILLIS > v->lastStateChange)
          valveStateChange(v, i, V_FILLING);
      }
    }
  }
  // TODO check sanity of fill vectors, generate warning re. water pump?
}
void valveStateChange(valve_t *v, int valveNum, boolean newState) {
  int mcpPin = valvePinMap(i);
  Serial.print("Growbed Valve Air Pump ");
  Serial.print(valveNum);

  if(newState == V_FILLING){
    mcp.digitalWrite(mcpPin, HIGH);
    Serial.println(" on");
  } else {
    mcp.digitalWrite(mcpPin, LOW);
    Serial.println(" off");
  }
  
  v->state = newState;
  v->lastStateChange = millis();
}
// TODO change webserver valve handlers to use on/off funcs, or vPM at least
const int V1_MCP_PIN = 0;
const int V2_MCP_PIN = 3;
const int V3_MCP_PIN = 7;
int valvePinMap(int v) {
  switch(v) {
    case 0:  return V1_MCP_PIN; break;
    case 1:  return V2_MCP_PIN; break;
    case 2:  return V3_MCP_PIN; break;
    default: Serial.println("you pillock!");
  }
  return -1;
}
